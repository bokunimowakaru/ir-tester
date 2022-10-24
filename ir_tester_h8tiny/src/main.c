/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/
/*
家電製品協会(AEHA)方式

	搬送波		約940nm
	副搬送波	min. 33kHz	30.3us H(15.1us)+L(15.2us)
				typ. 38kHz	26.3us H(13.1us)+L(13.2us)
				max. 40kHz	25.0us H(12.5us)+L(12.5us)
	変調単位Ｔ	min. 350us	
				typ. 470us	( H(13.0us)+L(13.1us) ) ×18
				max. 500us	

	SYNC	H(8T) + L(4T)
	BIT0	H(1T) + L(1T)
	SYMBOL	H(1T) + L(2T)	//判定長
	BIT1	H(1T) + L(3T)
	STOP	H(1T)
	REPEAT	L(>8000us※) + SYNC～STOP(同一コード送信)
											※実装は70～80ms以下

	FORMAT:
	SYNC + Vendor(16) + Parity(4) + Sys(4) + data(8～48) + STOP

	data() = ID(8) + code(8) + parity(8)

		(LSB First) C0 C1 C2 C3 C4 C5 C6 C7 (MSB Last)
		
NEC方式(東芝)

	搬送波		約950nm
	副搬送波	typ. 38kHz	26.3us H(8.8us)+L(17.5us)
	変調単位Ｔ	562.5us		( H(8.8us)+L(17.5us) ) ×21 + H(8.8us)

	SYNC	H(16T) + L(8T)
	BIT0	H(1T) + L(1T)
	SYMBOL	H(1T) + L(2T)	//判定長
	BIT1	H(1T) + L(3T)
	STOP	H(1T)
	REPEAT	L(4T) + H(1T)の繰り返し（コードは送らない）

	FORMAT:
	SYNC + Custom Code (16bits) 
		+ Data Code (8bits) + Data Code' (8bits) 
		
		(LSB First) C0 C1 C2 C3 C4 C5 C6 C7 (MSB Last)

SONY(SIRC)方式

	副搬送波	40kHz
	変調単位Ｔ	600us

	SYNC	H(4T) + L(1T)
	BIT0	H(1T) + L(1T)
	SYMBOL	H(1.5T) + L(1T)	//判定長
	BIT1	H(2T) + L(1T)
	STOP	L(75T-sum)		// 75T=45ms
	REPEAT	45ms毎に同一コードを送信(単一ボタンでも3回リピート)

参考文献
	http://einst.hp.infoseek.co.jp/Remocon2/Remocon2.html
	http://www.geocities.jp/shrkn65/remocon/
	http://www.necel.com/ja/faq/mi_com/__com_remo.html

コード確認

				Vende PS
	SHARP	TV	AA:5A:8F:12:15:E1
	                  70 ED EA
	                        11:A1
	                        EE
	                        16:D1
	                        83:22
	
	                           sum
	Pana	DVD	02:20:B0:00:86:36
	
	SONY	DVD	UP	79:9D:4
	
	東芝	TV	40:BF:1A:E5
				c     d  d'(反転)
*/

#define BYTE_TYPE
typedef unsigned char byte;

#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>             // 3664Fか3694Fかを選択する
#endif
#ifndef SYS_CLOCK
	#define		SYS_CLOCK	16				// 3664F=16MHz 3694F=20MHz
	float sys_clock = SYS_CLOCK*1000000;	// 調整用[Hz]
#endif

#ifndef DIAG
	#define DIAG	DIAG
	byte		DIAG	=	1;			// 診断モード
#endif

#define		LCD							// 液晶が無い場合は消す

//#define   IR_ADC_MODE         // LED近距離用の場合にdefineする

#ifdef LCD
	#define LCD_H8_H
	#include "lib/lcd_h8.c"
#endif

#ifdef IR_ADC_MODE
  #define TYPE  "近距離LEDｾﾝｻ IR-ADC 版"
#else
  #define TYPE  "IR Receiver Module 版"
#endif

#define		IR_OUT		IO.PDR1.BIT.B4	// 赤外線LEDの接続ポート
#define		IR_OUT_OFF	0				// 赤外線LED非発光時の出力値
#define		IR_OUT_ON	1				// 赤外線LED発光時の出力値
#define		IR_IN		IO.PDR1.BIT.B5	// 赤外線センサの接続ポート
#define		IR_IN_OFF	0				// 赤外線センサ非受光時の入力値
#define		IR_IN_ON	1				// 赤外線センサ受光時の入力値
#define		IR_DELAY_LH	200				// 赤外線センサ感光時の遅延時間[us]
#define		IR_DELAY_HL	320				// 赤外線センサ消灯時の遅延時間[us]
#define		LED1_OUT	IO.PDR8.BIT.B0	// LED赤の接続ポート
#define		LED2_OUT	IO.PDR8.BIT.B1	// LED緑の接続ポート
#define		LED_OUT_OFF	0				// LED非発光時の出力値
#define		LED_OUT_ON	1				// LED発光時の出力値
#define		BUTN		IO.PDR8.BIT.B2	// ボタンの接続ポート
#define		BUTN_OFF	0				// ボタンを離している
#define		BUTN_ON		1				// ボタンを押している
#define		SYNC_WAIT	4095			// 同期長の２倍以上 最大32235
#define		DATA_SIZE	16				// データ長(byte),4の倍数
#define		SCI_SIZE	64				// シリアルデータ長
#define		IR_READ_TIME_OUT	5		// IR入力タイムアウト[秒]
#define		IR_READ_NOISE_DET	4		// 1/4シンボル基本単位以下をノイズとする

#define MILLISEC_WAIT	MILLISEC_WAIT
unsigned int MILLISEC_WAIT	=	564	;	// 1ms分の空ループ初期値

#define FLASH_DEF
unsigned int FLASH_AEHA_WAIT=	5	;	// 13us(38kHz)分の空ループ初期値
unsigned int FLASH_AEHA_TIMES=	18	;	// シンボル区間の点滅回数
unsigned int FLASH_NEC_WAIT=	5	;	// 13us(38kHz)分の空ループ初期値
unsigned int FLASH_NEC_TIMES=	22	;	// シンボル区間の点滅回数
unsigned int FLASH_SIRC_WAIT=	5	;	// 12.5us(40kHz)分の空ループ初期値
unsigned int FLASH_SIRC_TIMES=	24	;	// シンボル区間の点滅回数

#define TIMER_A_DEF
unsigned int TIMER_SEC		=	0	;	// ＲＴＣカウント用グローバル変数

unsigned int FREQ_AEHA		=	38	;	// AEHA 38kHz
unsigned int FREQ_NEC		=	38	;	// NEC  38kHz
unsigned int FREQ_SIRC		=	40	;	// SIRC 40kHz
unsigned int FREQ_TAU		=	50	;	// 搬送波デューティー
unsigned int SYMBOL_AEHA	=	470	;	// AEHA 470us
unsigned int SYMBOL_NEC		=	563	;	// NEC  562.5us
unsigned int SYMBOL_SIRC	=	600	;	// SIRC 600us

unsigned int TIMER_V_B_AEHA	=	132;	// TIMER_Vの周期(20MHz時)
unsigned int TIMER_V_B_NEC	=	132;	// ir_cal.c で再計算する
unsigned int TIMER_V_B_SIRC	=	125;
unsigned int TIMER_V_A_AEHA	=	66;		// TIMER_Vのデューティー(TIMER_V_B/2)
unsigned int TIMER_V_A_NEC	=	66;
unsigned int TIMER_V_A_SIRC	=	62;
unsigned int TIMER_V_TS_AEHA=	18;		// １シンボル分のTIMER_Vのカウント数
unsigned int TIMER_V_TS_NEC =	21;
unsigned int TIMER_V_TS_SIRC=	24;
unsigned int TIMER_A_EOF[2]	=	{10,10};	// TIMER_Aによる終了判定時間補正

byte		 TIMER_V_FLASH	=	(0b00000110 ^ (IR_OUT_OFF*0b00001111));
										// 赤外線LEDの論理 0110=Ｈ出力で点灯
byte		 TIMER_V_WAIT	=	(0b00000101 ^ (IR_OUT_OFF*0b00001111));
										// 赤外線LEDの論理 0101=Ｌ出力で消灯

#define IR_MODE_ENUM
enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// 赤外線モードの型

#define IR_PORT_INIT
void ir_port_init(){
	/*ポート1         76543210*/
	IO.PMR1.BYTE  = 0b00000000;     // モード(P1,5)     入出力=0  その他=1
	IO.PCR1       = 0b11010111;     // 入出力設定       入力  =0  出力  =1  B3=リザーブ
	IO.PUCR1.BYTE = 0b00000000;     // プルアップ(P1,5) しない=0  する  =1
	IO.PDR1.BYTE  = 0b00000000;     // アクセス         Ｌ出力=0  Ｈ出力=1

	/*ポート8         76543210*/	// 秋月 H8 Tiny I/O BOARD TERA2 [K-00207]用
	IO.PCR8       = 0b11110011;     // 入出力設定       入力  =0  出力  =1
	IO.PDR8.BYTE  = 0b00000000;     // アクセス         Ｌ出力=0  Ｈ出力=1

	/*TIMER_V         76543210*/
	TV.TCRV0.BYTE = 0b00010000;
	/*      B割込 ____||||||_|___ Clock 設定でカウント開始
	        A割込 _____||||______ カウンタクリア 00:しない 10:Bクリア
	       OF割込 ______|
	*/	
	TV.TCSRV.BYTE = TIMER_V_WAIT;
	//              0b00000101;
	/*    Bフラグ ____||| ||||___ Aマッチ出力 10:H出力  00:変化しない
	      Aフラグ _____|| ||_____ Bマッチ出力 01:L出力  11:トグル
	     OFフラグ ______|
	*/
}

void ir_led_init(){
	IR_OUT=IR_OUT_OFF;
	LED1_OUT=LED_OUT_OFF;
	LED2_OUT=LED_OUT_OFF;
}

/* TIMER_A 割り込み処理 */
void int_timera(void){ 
  IRR1.BIT.IRRTA=0;				/* タイマーA割込フラグ */
  TIMER_SEC++;
}

#ifndef IR_TESTER_H
	#define IR_TESTER_H
	#include "ir_tester.c"
#endif

/* IR_TESTER呼び出し */
#define MAIN
void main( void ){
	ir_tester();
}
