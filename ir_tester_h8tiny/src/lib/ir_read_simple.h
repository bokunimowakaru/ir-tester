/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>
#endif

#ifndef IR_IN
	#define		IR_IN		IO.PDR1.BIT.B5	// �ԊO���Z���T�̐ڑ��|�[�g
	#define		IR_IN_OFF	0				// �ԊO���Z���T�������̓��͒l
	#define		IR_IN_ON	1				// �ԊO���Z���T������̓��͒l
#endif
#ifndef SYNC_WAIT
	#define		SYNC_WAIT	4095			// �������̂Q�{�ȏ� �ő�32235
#endif
#ifndef DATA_SIZE
	#define		DATA_SIZE	16				// �f�[�^��(byte),4�̔{��
#endif
#ifndef IR_MODE_ENUM
	#define IR_MODE_ENUM
	enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// �ԊO�����[�h�̌^
#endif

/* �֐��錾 */
int ir_sens(int det);
int ir_read_simple(unsigned char *data, enum Ir_Mode mode);

// �w�b�_�t�@�C�����b�\�[�X ������
/*
#ifndef IR_READ_S_H
	#define IR_READ_S_H
	#include "ir_read_simple.c"
#endif
*/
#define IR_READ_S_H
