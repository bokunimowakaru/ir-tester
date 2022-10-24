/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

#ifndef IR_DISPLAY_H
	#define IR_DISPLAY_H
	#include "ir_display.h"
#endif

void ir_sci_init(char *sci_tx, char *sci_rx){
	SCI3_INIT(br19200, sci_tx, sizeof(sci_tx), sci_tx, sizeof(sci_tx));
	EI;									// 割り込み許可
    SCI3_OUT_STRING("\n");
    SCI3_OUT_STRING( NAME );
    SCI3_OUT_STRING(" Ver ");
    SCI3_OUT_STRING( VERSION );
    SCI3_OUT_STRING(" (");
    SCI3_OUT_STRING( TYPE );
    SCI3_OUT_STRING(") by ボクにもわかる地上デジタル ");
    SCI3_OUT_STRING("\n");
}

void wait_millisec(int millisec)
{
    int i,j;
    
    LED1_OUT = LED_OUT_ON;
    LED2_OUT = LED_OUT_ON;
    for (i=0;i<millisec;i++) {
        for (j=0;j<MILLISEC_WAIT;j++);
    }
    LED1_OUT = LED_OUT_OFF;
    LED2_OUT = LED_OUT_OFF;
}

#ifdef LCD
void ir_lcd_init(void){
	lcd_goto(LCD_ROW_1);
	lcd_putstr("Init...");
	lcd_goto(LCD_ROW_2);
	lcd_putstr(VERSION);
}
void ir_lcd_disp(byte *data, int data_len){
	int i,j;
	byte sum=0;
	
	lcd_cls();
	for(i=0 ; i < (data_len / 16) ; i+=16){
		lcd_goto(LCD_ROW_1);
		for(j=0; j<8 ; j++){
			lcd_disp_hex(data[i+j]);		sum += data[i+j];
		}
		lcd_goto(LCD_ROW_2);
		for(j=0; j<8 ; j++){
			lcd_disp_hex(data[i+j+8]);		sum += data[i+j];
		}
		wait_millisec(1000 - DIAG * 900);
	}
	lcd_cls();
	lcd_goto(LCD_ROW_1);	lcd_putstr("LEN=");	lcd_disp_3(data_len);
	lcd_goto(LCD_ROW_2);	lcd_putstr("SUM=");	lcd_disp_hex(sum);
	wait_millisec(1000 - DIAG * 1000);
	lcd_goto(LCD_ROW_2);
	for(j=0; j<8 ; j++){
		lcd_disp_hex(data[j]);
	}
}
void ir_lcd_disp_id(int id){
	if( id >= 0 ){
		lcd_cls();
		lcd_goto(LCD_ROW_1);
		lcd_putstr("ID=");
		#ifdef IR_DATA_H
		lcd_disp_3(id);
		lcd_goto(LCD_ROW_2);
		lcd_putstr(IR_DATA[id].name);
		#endif
		wait_millisec(1000 - DIAG * 1000);
	}
}
#endif

void ir_sci_disp(byte *data, int data_len){
	int i, len;
	byte sum;
	
	
	len = (int)(data_len/8);
	if( (len * 8) != data_len ) len++;
	SCI3_PRINTF("Data[%d]={",len); 
	for(i=0 ; i < len ; i++){
		SCI3_PRINTF("0x%X",data[i]);
		sum += data[i];
		if( i < len-1 ){
			SCI3_OUT_STRING(",");
		}
	}
	SCI3_PRINTF("}; len=%d;\n",data_len);
}

void ir_sci_disp_mode(enum Ir_Mode mode){
	
		SCI3_PRINTF("IR Mode = ");
		switch( mode ){
			case NA:	SCI3_PRINTF("NA\n");break;
			case AEHA:	SCI3_PRINTF("AEHA\n");break;
			case NEC:	SCI3_PRINTF("NEC\n");break;
			case SIRC:	SCI3_PRINTF("SIRC\n");break;
			case ERROR:	SCI3_PRINTF("ERROR\n");break;
		}
}
