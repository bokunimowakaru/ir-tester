/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

#ifndef IR_DATA_H
	#define IR_DATA_H
	#include "ir_data.h"
#endif

int ir_search(byte *data, int data_len, enum Ir_Mode mode){
	int i,id,len_b;
	int left;
	byte F=1;
	
	left=IR_DATA_SIZE;
	id=-1;
	
	len_b = (int)(data_len/8);
	if( (len_b * 8) != data_len ) len_b++;
	
	while( left>0 ){
		id++;
		left -= sizeof(IR_DATA[id]);
		F=1;
		if( IR_DATA[id].mode == mode && IR_DATA[id].len == data_len ){
			F=0;											// 不一致フラグ
			for(i=0;i<len_b;i++){
				if( *(IR_DATA[id].data+i) != *(data+i) ){	// 不一致検出
					F=1;
					i=len_b;									// forループ離脱
				}
			}
			if( F==0 ){										// 完全に一致した
				left=0;										// whileループ離脱
			}
		}
	}
	if( F==0 ) {
		return( id );
	} else {
		return( -1 );
	}
}
