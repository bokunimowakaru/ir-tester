/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>
#endif

#ifndef IR_IN
	#define		IR_IN		IO.PDR1.BIT.B5	// 赤外線センサの接続ポート
	#define		IR_IN_OFF	0				// 赤外線センサ非受光時の入力値
	#define		IR_IN_ON	1				// 赤外線センサ受光時の入力値
#endif
#ifndef SYNC_WAIT
	#define		SYNC_WAIT	4095			// 同期長の２倍以上 最大32235
#endif
#ifndef DATA_SIZE
	#define		DATA_SIZE	16				// データ長(byte),4の倍数
#endif
#ifndef IR_MODE_ENUM
	#define IR_MODE_ENUM
	enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// 赤外線モードの型
#endif

/* 関数宣言 */
int ir_sens(int det);
int ir_read_simple(unsigned char *data, enum Ir_Mode mode);

// ヘッダファイル内Ｃソース ※注意
/*
#ifndef IR_READ_S_H
	#define IR_READ_S_H
	#include "ir_read_simple.c"
#endif
*/
#define IR_READ_S_H
