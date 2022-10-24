/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

/* �M���ǂݎ�� */
int ir_read(byte *data, int time_out, enum Ir_Mode *mode){
	int i,bit;						// ���[�v�p
	unsigned char timer_0,timer_1,timer_2,timer_3;	// �M����(TIMER_A�J�E���g)
	unsigned short timerW_1,timerW_2;		// �M����(TIMER_W�J�E���g)
	int tmp;						// �^�C�}���Z�p�i9�r�b�g�K�v�j
	int	len_on=0,len_off=0;			// �M����(���[�v�J�E���g)
	int symbol_len,noise;			// ����p�V���{����
	int symbol_rate=0;				// ���ʕ\���p
	int len, data_wait;
	int data_len;					/* Ir�f�[�^�̃r�b�g�� */
	unsigned char det=IR_IN_OFF;	// ���莞�̓��͐M�����x��(SIRC�Ή�)
	unsigned char in;

	if( DIAG ) SCI3_OUT_STRING("IR Sensing...");

	DI;		//���荞�݋֎~
	TA.TMA.BYTE     =0b00001000;  /* �^�C�}�[�`�ݒ� */
	IRR1.BIT.IRRTA  =0;           /* �^�C�}�[A�����t���O */
	IENR1.BIT.IENTA =1;           /* �^�C�}�[A�����ݒ� */
	TW.TCRW.BYTE    =0b00110000;  /* �^�C�}�[�v�ݒ� �N���b�N���^�W (2MHz=0.5us/ 2.5MHz=0.4us) */
	TW.TMRW.BIT.CTS =0;           /* �^�C�}�[�v�񓮍� */
	EI;		//���荞�݊J�n
	
	TIMER_SEC = 0;
	TW.TCNT = 0;                  /* �^�C�}�[�v���Z�b�g*/

	len_on=SYNC_WAIT;											// 
	len_off=SYNC_WAIT;											//
	while( TIMER_SEC <= time_out && IR_IN == IR_IN_OFF);
	TW.TMRW.BIT.CTS=1;					/* �^�C�}�[�v�J�E���g�J�n*/
	timer_0 = TA.TCA;											// 
	/* SYNC_ON���o */											// 
	while( TIMER_SEC <= time_out && len_on == SYNC_WAIT){		//
		len_on = ir_sens(IR_IN_ON);								//
	}															//
	timerW_1 = TW.TCNT;					/* �^�C�}�[�v�̒l��ۑ� */
	timer_1 = TA.TCA;					// ���[�h����p��SYNC_ON���ԑ���
																//
	/* SYNC_OFF���o */											//
	while( TIMER_SEC <= time_out && len_off == SYNC_WAIT){		//
		len_off = ir_sens(IR_IN_OFF);							//
	}															//
	TW.TMRW.BIT.CTS=0;					/* �^�C�}�[�v�J�E���g��~*/
	if( TIMER_SEC >= time_out ) return(0);	/* �^�C���A�E�g�� */

	/*�v���J�n */
	timer_2 = TA.TCA;
	timerW_2 = TW.TCNT;					/* �^�C�}�[�v�̒l��ۑ� */
	
	/* ���[�h���� */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5�~16 �� 9ms�ȏ�
	           488us�~16 �� 7.808ms  (Clock 2�� = 7.813ms)�Ŕ���*/
	tmp = timer_1 - timer_0;			// SYNC_ON�̎�������
	if( tmp < 0 ) tmp += 256;			// �����ƃI�[�o�t���[���܂����ł���
	if( tmp >= 2 && len_on > len_off ){	// 7.8ms�ȏ��SYNC_OFF���Z��
		*mode = NEC;
		symbol_len = len_off/4;
		//det=IR_IN_OFF;
	}else if( (long)len_on < 3*(long)len_off ){		// AEHA 2:1 2<3
		*mode = AEHA;
		symbol_len = len_off/2;
		//det=IR_IN_OFF;
	}else{											// SIRC 4:1 4>3
		*mode = SIRC;
		det=IR_IN_ON;
		symbol_len = (3*len_off)/2;
	}
	
	/* �f�[�^�[�ǎ��*/
	noise = symbol_len / IR_READ_NOISE_DET;		/* �m�C�Y���� */
	data_wait = 2*symbol_len;					/* �I�����o���鎝���� */

	for(i=0;i<DATA_SIZE;i++){
		in = 0;
		for(bit=0;bit<8;bit++){
			len = ir_sens( det );
			if( len > noise && len < data_wait){
				if( len < symbol_len ){
					in = in>>1;
					in += 0;
				}else{
					in = in>>1;
					in += 128;
				}
			}else{
				if( len >= data_wait ){
				/* �I�������o�����ꍇ�̏���(�ʏ�̃��[�v���珜�O���邽�߂ɍēxif�������Ă���)  */
					in = in>>(8 - bit);
					data[i]=in;
					data_len = i * 8 + bit;
					i = DATA_SIZE -1;
					bit=7;
				} else {
				/* �m�C�Y�����o�����ꍇ�̏���(�ʏ�̃��[�v���珜�O���邽�߂ɍēxif�������Ă���) */
					DI;		//���荞�݋֎~
					IENR1.BIT.IENTA =0;           /* �^�C�}�[�`�����݉��� */
					EI;		//���荞�݊J�n
					*mode = ERROR;
					return(-1);						// �G���[�Ȃ̂ŏu����read�ĊJ�ł���悤�ɂ���
				}
			}
		}
		data[i]=in;
	}
	timer_3 = TA.TCA - TIMER_A_EOF[det];
	
	/* �f�[�^�ǎ�芮�� */
	tmp = timer_3 - timer_2;
	if( tmp <= 0 ) tmp += 256;
	timerW_2 -= timerW_1;
	timerW_1 = (unsigned short)( 8000000./sys_clock*(float)timerW_1+0.5) + IR_DELAY_LH - IR_DELAY_HL;
	timerW_2 = (unsigned short)( 8000000./sys_clock*(float)timerW_2+0.5) + IR_DELAY_HL - IR_DELAY_LH;
	
	DI;		//���荞�݋֎~
	IENR1.BIT.IENTA =0;           /* �^�C�}�[�`�����݉��� */
	TW.TMRW.BIT.CTS =0;           /* �^�C�}�[�v�񓮍� */
	EI;		//���荞�݊J�n
	
	/* �f�[�^�m�F (SYNC���̃m�C�Y�G���[�����o) */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5�~16 �� 9ms�ȏ�
	           488us�~16 �� 7.808ms  (Clock 2�� = 7.813ms)�Ŕ���*/
	switch( *mode ){
		case AEHA:
			symbol_rate = 4000000/(int)(timerW_1 + timerW_2);
			if( len_on < len_off || len_on > len_off * 4 ){	// SYNC_ON��SYNC_OFF���ُ�ɒZ��
				*mode = ERROR;
				return(-1); 
			}
			break;
		case NEC:
			symbol_rate = 8000000/(int)(timerW_1 + timerW_2);
			if( len_on > len_off * 4 ){				 // SYNC_OFF���ɒ[�ɒZ��
				*mode = ERROR;
				return(-1);
			}
			break;
		case SIRC:
			symbol_rate = 2000000/(int)(timerW_1 + timerW_2);
			if( len_on < len_off ){				 	// SYNC_ON���ɒ[�ɒZ��
				*mode = ERROR;
				return(-1);
			}
			break;
		case ERROR:
			return(-1);
			break;
		case NA:									// NA�̂Ƃ��̓^�C���A�E�g�����iread�ĊJ���Ȃ��j
			return(0);
			break;
	}
	
	if( DIAG ){
		SCI3_OUT_STRING("done\n\n");
		ir_sci_disp_mode( *mode );
		SCI3_PRINTF("Data Length  = %d bits\n",data_len); 
		SCI3_PRINTF("Symbol Counts= %d samples\n",symbol_len); 
		timer_3 -= timer_2; /* ������ char �Ȃ̂� ���ɂȂ�Ȃ� */
		timer_2 -= timer_1;
		timer_1 -= timer_0;
		SCI3_PRINTF("SYNC Duration= %d us (ON: %d us)/(OFF: %d us)\n",timerW_1+timerW_2,timerW_1,timerW_2);
		SCI3_PRINTF("SYNC Counts  = %d samples (ON: %d samples)/(OFF: %d samples)\n",len_on+len_off,len_on,len_off);
		SCI3_PRINTF("DATA Duration= %d msec.\n",(1000 * timer_3)/256);
		SCI3_PRINTF("TOTAL Time   = %d msec.\n",(1000 * (timer_1+timer_2+timer_3))/256);
		SCI3_PRINTF("SYMBOL Rate  = %d Baud (bps)\n",symbol_rate);
		SCI3_PRINTF("DATA Rate    = %d Baud (bps)\n\n",((256 * data_len)/timer_3));
	}
	return(data_len);
}
