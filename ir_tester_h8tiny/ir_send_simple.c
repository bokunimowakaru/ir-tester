/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/

// �e���[�h(AEHA/NEC/SIRC)���Ɋ֐���p�ӂ��Ă���(mode�ݒ�̒x����ጸ���邽�߂�)
// �������Œ�̏ꍇ�͕s�v�Ȋ֐����폜����

#ifndef IR_SEND_S_H
	#define IR_SEND_S_H
	#include "ir_send_simple.h"
#endif

/* AEHA�����̐ԊO�k�d�c�_�ŗp */
void ir_flash(int times){
	int j,k;
	
	for(j=0 ; j < times * FLASH_AEHA_TIMES ; j++){
		IR_OUT = IR_OUT_ON;
		for(k=0 ; k < FLASH_AEHA_WAIT ; k++);
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_AEHA_WAIT ; k++);
	}
}
void ir_wait(int times){
	int j,k;
	
	for(j=0 ; j < times * FLASH_AEHA_TIMES ; j++){
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_AEHA_WAIT ; k++);
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_AEHA_WAIT ; k++);
	}
}

/* NEC�����̐ԊO�k�d�c�_�ŗp */
void ir_flash_nec(int times){
	int j,k;
	
	for(j=0 ; j < times * FLASH_NEC_TIMES ; j++){
		IR_OUT = IR_OUT_ON;
		for(k=0 ; k < FLASH_NEC_WAIT ; k++);
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_NEC_WAIT ; k++);
	}
}
void ir_wait_nec(int times){
	int j,k;
	
	for(j=0 ; j < times * FLASH_NEC_TIMES ; j++){
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_NEC_WAIT ; k++);
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_NEC_WAIT ; k++);
	}
}

/* SIRC�����̐ԊO�k�d�c�_�ŗp */
void ir_flash_sirc(int times){
	int j,k;
	
	for(j=0 ; j < times * FLASH_SIRC_TIMES ; j++){
		IR_OUT = IR_OUT_ON;
		for(k=0 ; k < FLASH_SIRC_WAIT ; k++);
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_SIRC_WAIT ; k++);
	}
}
void ir_wait_sirc(int times){
	int j,k;
	
	for(j=0 ; j < times * FLASH_SIRC_TIMES ; j++){
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_SIRC_WAIT ; k++);
		IR_OUT = IR_OUT_OFF;
		for(k=0 ; k < FLASH_SIRC_WAIT ; k++);
	}
}

/* �M�����o�i���[�h�ʂɓ_�Ŋ֐����Ăяo���j */
void ir_send_simple(byte *data, int data_len, enum Ir_Mode mode){
	int i,j,t,len,bit;
	byte out;

	switch( mode ){
		case AEHA:
			ir_flash(8);						// send 'SYNC'
			ir_wait(4);							// send 'SYNC'
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					ir_flash(1);
					if( ((out)&(1)) == 0){
						ir_wait((int)1);
					}else{
						ir_wait((int)3);
					}
					data_len--;
					if( data_len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			ir_flash(1);						// send 'stop'
			break;
		case NEC:
			ir_flash_nec(16);						// send 'SYNC'
			ir_wait_nec(8);							// send 'SYNC'
			/*�ȍ~��AEHA�Ɠ���(��ʃt�H�[�}�b�g�͈Ⴄ)*/
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					ir_flash_nec(1);
					if( ((out)&(1)) == 0){
						ir_wait_nec((int)1);
					}else{
						ir_wait_nec((int)3);
					}
					data_len--;
					if( data_len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			ir_flash_nec(1);						// send 'stop'
			break;
		case SIRC:
			for(j=0;j<3;j++){
				t=5;						// ���M�σV���{����{�P��(SYNC�ő��M)
				len=data_len;				// ���M�σr�b�g���̃J�E���g�p
				ir_flash_sirc(4);			// send 'SYNC'
				ir_wait_sirc(1);			// send 'SYNC'
				for(i=0;i<DATA_SIZE;i++){
					out = data[i];
					for(bit=0;bit<8;bit++){
						if( ((out)&(1)) == 0){
							ir_flash_sirc((int)1);
							t +=2 ;
						}else{
							ir_flash_sirc((int)2);
							t +=3 ;
						}
						ir_wait_sirc(1);
						len--;
						if( len == 0){
							i = DATA_SIZE -1;
							bit=7;
						}
						out >>= 1;
					}
				}
				if( t <= 75 ) ir_wait_sirc( 75-t );
			}
			break;
		case NA:
			break;
		case ERROR:
			break;
	}
}
