/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

/*
	Clock = 20MHz
						/4
	AEHA,NEC	38kHz	131.6	132(37.9kHz) 66
	SIRC		40kHz	125		125(38.1kHz) 63

*/

void ir_flash_timer(int times){
	int j;
	
	TV.TCSRV.BYTE = TIMER_V_FLASH;
	EI;	/*�����݋���*/
	for(j=0 ; j < times ; j++){
		while( (TV.TCSRV.BYTE & 0b10000000) == 0 );
		TV.TCSRV.BYTE &= 0b01111111;
	}
	DI;	/*�����݋֎~*/
	TV.TCSRV.BYTE = TIMER_V_WAIT;
}
void ir_wait_timer(int times){
	int j;
	
	TV.TCSRV.BYTE = TIMER_V_WAIT;
	EI;	/*�����݋���*/
	for(j=0 ; j < times ; j++){
		while( (TV.TCSRV.BYTE & 0b10000000) == 0b00000000 );
		TV.TCSRV.BYTE &= 0b01111111;
	}
	DI;	/*�����݋֎~*/
}

/* �M�����o */
void ir_send(byte *data, int data_len, enum Ir_Mode mode){
	int i,j,bit,t,len;
	byte out;
	
	DI;	/*�����݋֎~*/
	TV.TCRV0.BYTE = 0b00010001;
	/*      B���� ____||||||_|___ Clock �ݒ�ŃJ�E���g�J�n
	        A���� _____||||______ �J�E���^�N���A 00:���Ȃ� 10:B�N���A
	       OF���� ______|
	*/	
	TV.TCSRV.BYTE = TIMER_V_WAIT;
	//              0b00000101;
	/*    B�t���O ____||| ||||___ A�}�b�`�o�� 10:H�o��  00:�ω����Ȃ�
	      A�t���O _____|| ||_____ B�}�b�`�o�� 01:L�o��  11:�g�O��
	     OF�t���O ______|
	*/
	switch( mode ){
		case NA:
			if( DIAG ) SCI3_PRINTF("mode=NA\n");
			break;
		case AEHA:
			TV.TCORB=TIMER_V_B_AEHA;	// ����
			TV.TCORA=TIMER_V_A_AEHA;	// �f���[�e�B�[
			ir_flash_timer(8*TIMER_V_TS_AEHA);						// send 'SYNC'
			ir_wait_timer(4*TIMER_V_TS_AEHA);						// send 'SYNC'
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					ir_flash_timer(1*TIMER_V_TS_AEHA);
					if( ((out)&(1)) == 0){
						ir_wait_timer(1*TIMER_V_TS_AEHA);
					}else{
						ir_wait_timer(3*TIMER_V_TS_AEHA);
					}
					data_len--;
					if( data_len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			ir_flash_timer(1*TIMER_V_TS_AEHA);						// send 'stop'
			break;
		case NEC:
			TV.TCORB=TIMER_V_B_NEC;	// ����
			TV.TCORA=TIMER_V_A_NEC;	// �f���[�e�B�[
			ir_flash_timer(16*TIMER_V_TS_NEC);						// send 'SYNC'
			ir_wait_timer(8*TIMER_V_TS_NEC);							// send 'SYNC'
			/*�ȍ~��AEHA�Ɠ���(��ʃt�H�[�}�b�g�͈Ⴄ)*/
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					ir_flash_timer(1*TIMER_V_TS_NEC);
					if( ((out)&(1)) == 0){
						ir_wait_timer(1*TIMER_V_TS_NEC);
					}else{
						ir_wait_timer(3*TIMER_V_TS_NEC);
					}
					data_len--;
					if( data_len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			ir_flash_timer(1*TIMER_V_TS_NEC);						// send 'stop'
			break;
		case SIRC:
			TV.TCORB=TIMER_V_B_SIRC;	// ����
			TV.TCORA=TIMER_V_A_SIRC;	// �f���[�e�B�[
			
			/* �V���{����{�P�ʂ̑���t���J�E���g */
			t=5;						// ���M�σV���{����{�P�ʂ̏�����(SYNC�ő��M)
			len=data_len;
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					if( ((out)&(1)) == 0){
						t +=2 ;
					}else{
						t +=3 ;
					}
					len--;
					if( len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			if( t > 75 ) t=0;			// �f�[�^����������ꍇ
			/* SIRC���M�~�R�� */
			for(j=0;j<3;j++){
				len=data_len;			// ���M�σr�b�g���̃J�E���g�p(AEHA,NEC�Ƃ̈Ⴂ)
				ir_flash_timer(4*TIMER_V_TS_SIRC);						// send 'SYNC'
				ir_wait_timer(1*TIMER_V_TS_SIRC);						// send 'SYNC'
				for(i=0;i<DATA_SIZE;i++){
					out = data[i];
					for(bit=0;bit<8;bit++){
						if( ((out)&(1)) == 0){
							ir_flash_timer(1*TIMER_V_TS_SIRC);
						}else{
							ir_flash_timer(2*TIMER_V_TS_SIRC);
						}
						ir_wait_timer(1*TIMER_V_TS_SIRC);
						len--;
						if( len == 0){
							i = DATA_SIZE -1;
							bit=7;
						}
						out >>= 1;
					}
				}
				ir_wait_timer( (75-t)*TIMER_V_TS_SIRC );
			}
			break;
		case ERROR:
			if( DIAG ) SCI3_PRINTF("ERROR\n");
			break;
	}
}
