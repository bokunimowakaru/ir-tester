/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/

// 各モード(AEHA/NEC/SIRC)毎に関数を用意している(mode設定の遅延を低減するために)
// 方式が固定の場合は不要な関数を削除する

#ifndef IR_SEND_S_H
	#define IR_SEND_S_H
	#include "ir_send_simple.h"
#endif

/* AEHA方式の赤外ＬＥＤ点滅用 */
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

/* NEC方式の赤外ＬＥＤ点滅用 */
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

/* SIRC方式の赤外ＬＥＤ点滅用 */
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

/* 信号送出（モード別に点滅関数を呼び出す） */
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
			/*以降はAEHAと同じ(上位フォーマットは違う)*/
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
				t=5;						// 送信済シンボル基本単位(SYNCで送信)
				len=data_len;				// 送信済ビット数のカウント用
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
