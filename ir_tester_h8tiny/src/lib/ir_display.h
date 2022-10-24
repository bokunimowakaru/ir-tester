/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

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
