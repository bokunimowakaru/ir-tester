/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

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
    for (i=0;i<millisec;i++) {
        for (j=0;j<MILLISEC_WAIT;j++);
    }
    LED1_OUT = LED_OUT_OFF;
}
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
