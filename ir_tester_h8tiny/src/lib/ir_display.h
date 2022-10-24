/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

void ir_sci_init(char *sci_tx, char *sci_rx);
void wait_millisec(int millisec);

#ifdef LCD
void ir_lcd_init(void);
void ir_lcd_disp(byte *data, int data_len);
void ir_lcd_disp_id(int id);
#endif

void ir_sci_disp(byte *data, int data_len);
void ir_sci_disp_mode(enum Ir_Mode mode);

/*
#ifndef IR_DISPLAY_H
	#define IR_DISPLAY_H
	#include "ir_display.c"
#endif
*/
#define IR_DISPLAY_H
