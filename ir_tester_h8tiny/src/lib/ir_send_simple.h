/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

// �e���[�h(AEHA/NEC/SIRC)���Ɋ֐���p�ӂ��Ă���(mode�ݒ�̒x����ጸ���邽�߂�)
// �������Œ�̏ꍇ�͕s�v�Ȋ֐����폜����

#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>
#endif

#ifndef IR_OUT
	#define		IR_OUT		IO.PDR1.BIT.B4	// �ԊO��LED�̐ڑ��|�[�g
	#define		IR_OUT_OFF	0				// �ԊO��LED�񔭌����̏o�͒l
	#define		IR_OUT_ON	1				// �ԊO��LED�������̏o�͒l
#endif
#ifndef SYNC_WAIT
	#define		SYNC_WAIT	4095			// �������̂Q�{�ȏ� �ő�32235
#endif
#ifndef DATA_SIZE
	#define		DATA_SIZE	16				// �f�[�^��(byte),4�̔{��
#endif

#ifndef FLASH_DEF
	#define FLASH_DEF
	unsigned int FLASH_AEHA_WAIT=	5	;	// 13us(38kHz)���̋󃋁[�v�����l
	unsigned int FLASH_AEHA_TIMES=	18	;	// �V���{����Ԃ̓_�ŉ�
	unsigned int FLASH_NEC_WAIT=	5	;	// 13us(38kHz)���̋󃋁[�v�����l
	unsigned int FLASH_NEC_TIMES=	22	;	// �V���{����Ԃ̓_�ŉ�
	unsigned int FLASH_SIRC_WAIT=	5	;	// 12.5us(40kHz)���̋󃋁[�v�����l
	unsigned int FLASH_SIRC_TIMES=	24	;	// �V���{����Ԃ̓_�ŉ�
#endif
#ifndef IR_MODE_ENUM
	#define IR_MODE_ENUM
	enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// �ԊO�����[�h�̌^
#endif

void ir_flash(int times);
void ir_wait(int times);
void ir_flash_nec(int times);
void ir_wait_nec(int times);
void ir_flash_sirc(int times);
void ir_wait_sirc(int times);
void ir_send_simple(byte *data, int data_len, enum Ir_Mode mode);

// �w�b�_�t�@�C�����b�\�[�X ������
/*
#ifndef IR_SEND_S_H
	#define IR_SEND_S_H
	#include "ir_send_simple.c"
#endif
*/

#define IR_SEND_S_H
