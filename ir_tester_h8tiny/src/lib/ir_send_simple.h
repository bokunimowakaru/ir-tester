/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

// 各モード(AEHA/NEC/SIRC)毎に関数を用意している(mode設定の遅延を低減するために)
// 方式が固定の場合は不要な関数を削除する

#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>
#endif

#ifndef IR_OUT
	#define		IR_OUT		IO.PDR1.BIT.B4	// 赤外線LEDの接続ポート
	#define		IR_OUT_OFF	0				// 赤外線LED非発光時の出力値
	#define		IR_OUT_ON	1				// 赤外線LED発光時の出力値
#endif
#ifndef SYNC_WAIT
	#define		SYNC_WAIT	4095			// 同期長の２倍以上 最大32235
#endif
#ifndef DATA_SIZE
	#define		DATA_SIZE	16				// データ長(byte),4の倍数
#endif

#ifndef FLASH_DEF
	#define FLASH_DEF
	unsigned int FLASH_AEHA_WAIT=	5	;	// 13us(38kHz)分の空ループ初期値
	unsigned int FLASH_AEHA_TIMES=	18	;	// シンボル区間の点滅回数
	unsigned int FLASH_NEC_WAIT=	5	;	// 13us(38kHz)分の空ループ初期値
	unsigned int FLASH_NEC_TIMES=	22	;	// シンボル区間の点滅回数
	unsigned int FLASH_SIRC_WAIT=	5	;	// 12.5us(40kHz)分の空ループ初期値
	unsigned int FLASH_SIRC_TIMES=	24	;	// シンボル区間の点滅回数
#endif
#ifndef IR_MODE_ENUM
	#define IR_MODE_ENUM
	enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// 赤外線モードの型
#endif

void ir_flash(int times);
void ir_wait(int times);
void ir_flash_nec(int times);
void ir_wait_nec(int times);
void ir_flash_sirc(int times);
void ir_wait_sirc(int times);
void ir_send_simple(byte *data, int data_len, enum Ir_Mode mode);

// ヘッダファイル内Ｃソース ※注意
/*
#ifndef IR_SEND_S_H
	#define IR_SEND_S_H
	#include "ir_send_simple.c"
#endif
*/

#define IR_SEND_S_H
