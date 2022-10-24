/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/
#define IR_CALC_H	// �w�b�_�t�@�C���Ȃ�

unsigned long pow_10( int i ){
	unsigned long j=1;
	
	if( i<0 || i> 8 ){
		return(-1);
	}else {
		while( i ){
			j *= 10;
			i--;
		}
		return( j );
	}
}

void SCI3_PRINTF_float( float in, int place){
	
	int i;
	
	if( place > 0 && place <= 8){
		for( i=(place-1) ; i >= 0 ; i-- ){
			SCI3_PRINTF("%d",(int)( in / pow_10(i) ) - (int)( in / pow_10(i+1) )*10 );
			if( i==3 || i==6 ) SCI3_OUT_STRING(",");
		}
	}else if (place < -1 && place > -8 ){
		SCI3_OUT_STRING(".");
		for( i=-1 ; i >= place ; i-- ){
			SCI3_PRINTF("%d",(int)( in * pow_10(-i) ) - (int)(in * pow_10(-i-1) )*10 );
		}
	}else{
		SCI3_OUT_STRING("ERROR");
	}
}

void sys_cal( void ){
	unsigned short timer[16];					// W�^�C�}�[�p
	float target = 2929.73*(float)SYS_CLOCK;	// �TIME_A 6�J�E���g(6/256 sec)����W�J�E���^���J�E���g���ׂ��l
	int ppm[16];
	int ppm_avr=0;
	int timer_avr=0;
	unsigned long clock_disp;
	int i;
	int wait1,wait2;
	
	DI;		//���荞�݋֎~
	TA.TMA.BYTE     =0b00001000;  /* �^�C�}�[�`�ݒ� */
	/*                 |  |||-|__ ������ 000�`111(������) 000�`011(�ᑦ��)
	    �Œ�l=0000 ___|--||             000:0.5120 sec.  000 1000 msec.
	                       |             001:0.2560 sec.  001  500 msec.
	 ����=0, �ᑬ=1 _______|             100:0.0160 sec.  010  250 msec.
	                                     111:0.0005 sec.  011  31.25msec*/
	IRR1.BIT.IRRTA  =0;           /* �^�C�}�[A�����t���O */
	IENR1.BIT.IENTA =1;           /* �^�C�}�[A�����ݒ� */
	
	TW.TCRW.BYTE    =0b00110000;  /* �^�C�}�[�v�ݒ� �N���b�N���^�W (2MHz=0.5us/ 2.5MHz=0.4us) */
	TW.TMRW.BIT.CTS =0;           /* �^�C�}�[�v�񓮍� */
	
	EI;		//���荞�݊J�n
	if( DIAG ) SCI3_PRINTF("SYS Cal. ");
	
	while( TA.TCA != 200 );					// ����Ɋm����0�ő҂��󂯂邽�߂�
	/* ����J�n */
	for( i=0 ; i<16 ; i++){
		wait1 = i*13;
		wait2 = i*13+6;
		TW.TCNT = 0;						/* �^�C�}�[�v���Z�b�g */
		while( TA.TCA != wait1 );			/* �J�E���^�`���Z�b�g�҂� */

		TW.TMRW.BIT.CTS=1;					/* �^�C�}�[�v�J�E���g�J�n */
		while( TA.TCA != wait2 );
		TW.TMRW.BIT.CTS=0;					/* �^�C�}�[�v�J�E���g��~ */
		timer[i] = TW.TCNT;					/* �^�C�}�[�v�̒l��ۑ� */
	}
	for( i=0 ; i<16 ; i++){
		ppm[i] = (int)( ((float)timer[i]/target-1.)*1000000. + 0.5);	//�TIME_A�ɑ΂���΍�
		ppm_avr +=ppm[i];
		timer_avr += (timer[i]-(unsigned short)target);					//�TIME_A���������Ɓ{
		// SCI3_PRINTF(" %d(%dppm)",(timer[i]-(unsigned short)target),ppm[i]);
	}
	sys_clock = (float)SYS_CLOCK*(float)ppm_avr/16. + (float)SYS_CLOCK*1000000.;
	clock_disp = (unsigned long)( (float)SYS_CLOCK*(float)ppm_avr/16. ) + (unsigned long)SYS_CLOCK*1000000ul;
	
	ppm_avr /= 16;
	timer_avr /= 16;
	/* ���芮�� */
	DI;		//���荞�݋֎~
	IENR1.BIT.IENTA =0;           /* �^�C�}�[�`�����݉��� */
	TW.TMRW.BIT.CTS =0;           /* �^�C�}�[�v�����݉��� */
	
	EI;		//���荞�݊J�n
	if( DIAG ){
		SCI3_PRINTF("->%d SysCK=",timer_avr);
		SCI3_PRINTF_float( clock_disp,8 );
		/*
		for( i=7 ; i >= 0 ; i-- ){
			SCI3_PRINTF("%d",(int)( clock_disp / pow_10(i) ) - (int)(clock_disp / pow_10(i+1) )*10 );
		} */
		SCI3_PRINTF("Hz(%dppm) ->Done \n",ppm_avr);
	}
	/* ie_cal4adc.c�̎��̂݃R�����g�A�E�g
	TIMER_V_B_AEHA	=	(int)(sys_clock/1000./4./(float)FREQ_AEHA+0.5);	// 132
	TIMER_V_B_NEC	=	(int)(sys_clock/1000./4./(float)FREQ_NEC +0.5);	// 132
	TIMER_V_B_SIRC	=	(int)(sys_clock/1000./4./(float)FREQ_SIRC+0.5);	// 125
	TIMER_V_A_AEHA	=	(int)((float)TIMER_V_B_AEHA*(float)FREQ_TAU/100.);		// 66
	TIMER_V_A_NEC	=	(int)((float)TIMER_V_B_NEC *(float)FREQ_TAU/100.);		// 66
	TIMER_V_A_SIRC	=	(int)((float)TIMER_V_B_SIRC*(float)FREQ_TAU/100.);		// 62
	TIMER_V_TS_AEHA	=	(int)((float)SYMBOL_AEHA*sys_clock/1000000/(float)TIMER_V_B_AEHA/4.+0.5);	// 18(17.8)
	TIMER_V_TS_NEC 	=	(int)((float)SYMBOL_NEC *sys_clock/1000000/(float)TIMER_V_B_NEC /4.+0.5);	// 21(21.3)
	TIMER_V_TS_SIRC	=	(int)((float)SYMBOL_SIRC*sys_clock/1000000/(float)TIMER_V_B_SIRC/4.+0.5);	// 24(24.0)
	*/
	DI;		//���荞�݋֎~
}

void millsec_cal( void ){
	int wait;
	unsigned char timer_0;
	
	DI;		//���荞�݋֎~
	TA.TMA.BYTE     =0b00001000;  /* �^�C�}�[�`�ݒ� */
	/*                 |  |||-|__ ������ 000�`111(������) 000�`011(�ᑦ��)
	    �Œ�l=0000 ___|--||             000:0.5120 sec.  000 1000 msec.
	                       |             001:0.2560 sec.  001  500 msec.
	 ����=0, �ᑬ=1 _______|             100:0.0160 sec.  010  250 msec.
	                                     111:0.0005 sec.  011  31.25msec*/
	IRR1.BIT.IRRTA  =0;           /* �^�C�}�[A�����t���O */
	IENR1.BIT.IENTA =1;           /* �^�C�}�[A�����ݒ� */
	EI;		//���荞�݊J�n
	if( DIAG ) SCI3_PRINTF("Milli.Sec. Cal. Started ");
	while( TA.TCA );			/* �J�E���^���Z�b�g�҂�*/
	/* ����J�n */
	wait_millisec(781);			// 781ms
	timer_0 = TA.TCA;				// timer_0 * 1000ms / 256 -> 781 ms
	wait = (int)( ((float)MILLISEC_WAIT*256*781)/((float)timer_0*1000)+0.5 );
	/* ���芮�� */
	DI;		//���荞�݋֎~
	IENR1.BIT.IENTA =0;           /* �^�C�}�[�`�����݉��� */
	EI;		//���荞�݊J�n
	if( DIAG ){
		SCI3_PRINTF("->%d",timer_0);
		SCI3_PRINTF("->Done MSWAIT=%d",MILLISEC_WAIT);
		SCI3_PRINTF("->%d\n",wait);
	}
	/* �L�����u���[�V�������ʂ��s��v */
	/*
	if( wait != MILLISEC_WAIT ){
		MILLISEC_WAIT = wait;
		if( DIAG ) SCI3_PRINTF("Re");
		millsec_cal(timer_0);
	}
	*/
	DI;		//���荞�݋֎~
}
