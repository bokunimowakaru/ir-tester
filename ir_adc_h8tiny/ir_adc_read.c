/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/
/*
■動作の概要

	SYNC_H	L->H後に周波数解析のために約1msのデータを取得
			取得後に、平均値、ピーク点を解析（0.5ms程度!?）
	SYNC_L	H->Lの待機
	DATA_H	ここでモードの判定を行っている。0.5ms程度(1Tギリギリ)
	DATA_L	H->Lの待機
	
	Ver 0.15

■未実装
	高感度モード

■未テスト
	IRセンサのバラつきによる感度特性

■既存の不具合

	●SIRCの読み取りが不安定 (Ver 0.21)
	
*/

/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/
#ifndef NAME
	#define	NAME	"IR-ADC-ir_adc_read"
	#define	VERSION	"0.27-?"
#endif
#ifndef TYPE
	#define	TYPE "近距離LEDｾﾝｻ用"
#endif
//#define   TEST		// テスト時にdefineする(ログ表示)
//#define   TEST2		// テスト時にdefineする(周波数テスト用)

/* 他で宣言されているかもしれないもの */
#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>
#endif
#ifndef SYS_CLOCK
	#define		SYS_CLOCK	16				// 3664F=16MHz 3694F=20MHz
	float sys_clock = SYS_CLOCK*1000000;	// 調整用[Hz] キャリブレーション対応のためにfloatで宣言
#endif
#ifndef BYTE_TYPE
	typedef unsigned char byte;
#endif
#ifndef DATA_SIZE
  #define		DATA_SIZE	16				// データ長(byte),4の倍数
#endif
#ifndef IR_READ_TIME_OUT
  #define		IR_READ_TIME_OUT	5		// IR入力タイムアウト[秒]
#endif
#ifndef TIMER_A_DEF                /* TIMER_A 割り込み処理 */
  #define TIMER_A_DEF
  unsigned int TIMER_SEC		=	0	;	// ＲＴＣカウント用グローバル変数
  void int_timera(void){ 
    IRR1.BIT.IRRTA=0;				      /* タイマーA割込フラグ */
    TIMER_SEC++;
  }
#endif
#ifndef LED1_OUT
	#define		LED1_OUT	IO.PDR1.BIT.B6	// テスト用LEDの接続ポート
	#define		LED2_OUT	IO.PDR1.BIT.B7	// テスト用LEDの接続ポート
	#define		LED_OUT_OFF	0				// テストLED非発光時の出力値
	#define		LED_OUT_ON	1				// テストLED発光時の出力値
	#define		IR_OUT		IO.PDR1.BIT.B4	// 赤外線LEDの接続ポート
	#define		IR_OUT_OFF	0				// 赤外線LED非発光時の出力値
	#define		IR_OUT_ON	1				// 赤外線LED発光時の出力値
	#define		IR_IN		IO.PDR1.BIT.B5	// 赤外線センサの接続ポート
	#define		IR_IN_OFF	0				// 赤外線センサ非受光時の入力値
	#define		IR_IN_ON	1				// 赤外線センサ受光時の入力値
#endif
#ifndef IR_PORT_INIT
void ir_port_init(){
	/*ポート1         76543210*/
	IO.PMR1.BYTE  = 0b00000000;     // モード(P1,5)     入出力=0  その他=1
	IO.PCR1       = 0b11010111;     // 入出力設定       入力  =0  出力  =1  B3=リザーブ
	IO.PUCR1.BYTE = 0b00000000;     // プルアップ(P1,5) しない=0  する  =1
	IO.PDR1.BYTE  = 0b00000000;     // アクセス         Ｌ出力=0  Ｈ出力=1
	
	IR_OUT=IR_OUT_OFF;				// 赤外線の発光をオフ
}
#endif
#ifndef DIAG
	#define DIAG	DIAG
	byte		DIAG	=	1;				// 診断モード
#endif
#ifndef SCI_SIZE
	#define		SCI_SIZE	64				// シリアルデータ長
#endif
#ifndef MILLISEC_WAIT
	#define MILLISEC_WAIT	MILLISEC_WAIT
	unsigned int MILLISEC_WAIT	=	564	;	// 1ms分の空ループ初期値
#endif
#ifndef IR_MODE_ENUM
	#define IR_MODE_ENUM
	enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// 赤外線モードの型
#endif
#ifndef IR_READ_NOISE_DET
	#define	IR_READ_NOISE_DET	4			// 1/4シンボル基本単位（Ｔ）以下をノイズとする
#endif
#ifndef IR_DISPLAY_H
	#include "ir_display4adc.h"
	#include "ir_display4adc.c"
#endif
#ifndef IR_CALC_H
	#include "ir_cal4adc.c"
#endif

/* IR_ADC専用 ir_adc_read.h */

/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/
#define   IR_ADC_DATA_SIZE	128
#define   IR_ADC_LH			51		// L -> H 判定レベル 1.0V (255/5*1.0)
#define   IR_ADC_HL			10		// H -> L 判定レベル 0.2V (255/5*0.2)
#define   IR_ADC_NOISE		3
#define   IR_ADC_LPF_N		3		// ADCノイズ除去(平均フィルタ)

int ir_adc_init(int adc_port);
byte ir_adc_sens( void );
//byte ir_adc_sens_lpf( void );
void ir_adc_detect_timer_off(void);
int ir_adc_read(byte *data, int time_out, enum Ir_Mode *mode);

/* IR_ADC専用 ir_adc_read.c */

/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/
/*
#ifndef IR_ADC_H
	#define IR_ADC_H
	#include "ir_adc_read.h"
#endif
*/

int ir_adc_init(int adc_port){
	if( adc_port<0 || adc_port>3 ) return(-1);
	AD.ADCSR.BYTE=0b00001000|adc_port;       
	/*              ||||||_|__ Port 0=000 1=001 2=010 3=011
	    ADF 終了 ___|||||      
	   ADIE 割込 ____||||_____ CKS  0:低速 1:高速
	   ADST 開始 _____||______ SCAN 0:単発 1:連続
	*/
	return( adc_port );
}

byte ir_adc_sens( void ){
	AD.ADCSR.BIT.ADST = 1;            //読み取り
	while(AD.ADCSR.BIT.ADF==0);       //読み取り完了待ち
	return( AD.ADDRA >> 8 );
}

/* 小信号対応のためのＬＰＦ */
/*
byte ir_adc_sens_lpf( void ){
	IR_ADC_lpf -= (int)IR_ADC[IR_ADC_i];
	AD.ADCSR.BIT.ADST = 1;            //読み取り
	while(AD.ADCSR.BIT.ADF==0);       //読み取り完了待ち
	IR_ADC[IR_ADC_i] = ( AD.ADDRA >> 8 );
	IR_ADC_lpf += (int)IR_ADC[IR_ADC_i];
	IR_ADC_i++;
	if( IR_ADC_i >= IR_ADC_LPF_N ) IR_ADC_i=0;
	return( (byte)(IR_ADC_lpf/(int)IR_ADC_LPF_N) );
}
byte IR_ADC[IR_ADC_LPF_N];			// ＬＰＦ用ＦＩＦＯレジスタ
byte IR_ADC_i;						// ＬＰＦ用カウンタ
unsigned int IR_ADC_lpf;			// ＬＰＦ用ＦＩＦＯの内部合計
*/

void ir_adc_detect_timer_off(void){
	DI;		//割り込み禁止
	TW.TMRW.BIT.CTS =0; 					/* タイマーＷ非動作 */
	IENR1.BIT.IENTA =0; 					/* タイマーＡ割込み解除 */
	EI;		//割り込み開始
}

int ir_adc_read(byte *data, int time_out, enum Ir_Mode *mode){
	int i;					// for用
	int len=0;				// シンボル数
	byte freq_meas=0;		// 周波数測定のサンプル回数(クロックによって変わる)
	byte bit;				// 高速カウント用
	byte prev=0;			// 前回のサンプルデータ
//	byte dc_level=0;		// 直流レベル(小信号の受信用)
//	unsigned short sum=0;	// 解析用 サンプルの合計値 256*255 = intでOK
//	byte avr=0;				// 周波数解析用 サンプルの平均値
	byte max_c=0;			// 周波数解析用　最大ピーク数のカウント用
//	byte min_c=0;			// 周波数解析用　最小ピーク数のカウント用
//	byte max=0;				// 周波数解析用　最大ピーク値の保持用
//	byte min=255;			// 周波数解析用　最小ピーク値の保持用
	byte first=0;			// 周波数解析用　ピークの最初のサンプル番号
	byte last=0;			// 周波数解析用　ピークの最後のサンプル番号
	byte rise=1;			// 周波数解析用　信号の上昇か下降中かのフラグ
	byte adc_data[IR_ADC_DATA_SIZE];	// 周波数測定のサンプリング用
	byte det;				// 検出極性(H長検出=SIRCとL長検出=AEHA/NEC)
	unsigned short	in;		// 入力長
	int symbol_rate=0;		// 結果表示用
	
	unsigned short	timer_start;
	unsigned short	timer_freq;
	unsigned short	timer_sync_H;
	unsigned short	timer_sync_L;
	unsigned short	timer_symbol;
	unsigned long	timer_total=0ul;
	byte			timer_a;
	byte			timer_a_start;
	byte			timer_a_end;
	byte			timer_wait;
	/* テスト用タイマー */
	#ifdef TEST
	unsigned short	timer1;
	unsigned short	timer2;
	unsigned short	timer3;
	#endif
	unsigned short	sync_H;			// SYNCシンボルＨ長
	unsigned short	sync_L;			// SYNCシンボルＬ長
	unsigned short	symbol_len;		// シンボル基本単位
	unsigned short	noise_len;		// ノイズ判定長
	unsigned short	data_wait;		// 終了判定長
	unsigned short	in_len;
	unsigned short	in_H;
	unsigned short	in_L;
	
	float wave_us;
	float freq_hz;
	float data_ms;
	float total_ms;

	/* LPF初期化 */
	/*
	for( i=0 ; i < IR_ADC_LPF_N ; i++ ) IR_ADC[i]=0;
	IR_ADC_i=0;
	IR_ADC_lpf=0;
	for( i=0 ; i < IR_ADC_LPF_N ; i++ )	dc_level = ir_adc_sens();
	*/

	i = (int)( sys_clock / 1000000 * (1000/5.7)/20 +.5);	// 16MHz:7.1us(140)  20MHz 5.7us(175)
	//                                ~~~~                     区間[us]
	if( i > IR_ADC_DATA_SIZE-1 ){
		freq_meas =IR_ADC_DATA_SIZE-1;
	}else{
		freq_meas = i;
	}
	bit=freq_meas;
	
	if( DIAG ) SCI3_PRINTF("listening...(%dsec.)\n",time_out);

	wait_millisec(50);

	DI;		//割り込み禁止
	TA.TMA.BYTE 	=0b00001000;	/* タイマーＡ設定 */
	IRR1.BIT.IRRTA	=0;				/* タイマーA割込フラグ */
	IENR1.BIT.IENTA =1; 			/* タイマーA割込設定 */
	IO.PDR8.BYTE    =0b00000000;	// アクセス         Ｌ出力=0  Ｈ出力=1
	TW.TCRW.BYTE	=0b00110000;	/* タイマーＷ設定 クロックΦ／８ (2.5MHz=0.4us) */
	TW.TMRW.BIT.CTS =0; 			/* タイマーＷ非動作 */
	EI;		//割り込み開始

	TIMER_SEC = 0;
	TW.TCNT = 0;						/* タイマーＷリセット*/
	TW.TMRW.BIT.CTS=1;					/* タイマーＷカウント開始*/

	/* データ待ち*/
	prev = 0;
	#ifdef TEST
	LED1_OUT = LED_OUT_ON;
	#endif
	while( prev < IR_ADC_LH ){
		prev = ir_adc_sens();
		if( TIMER_SEC >= time_out ){
			#ifdef TEST
			if( DIAG ) SCI3_PRINTF("time out\n");
			#endif
			ir_adc_detect_timer_off();
			return(0);					// タイムアウトによるリターンは「０」
		}
	}
	timer_start = TW.TCNT;			//時間記録
	timer_a_start = TA.TCA;
	#ifdef TEST
	LED1_OUT = LED_OUT_OFF;
	LED2_OUT = LED_OUT_ON;
	#endif
	
	/* 周波数測定用(なるべく高速に動作) データは反対から(data[freq_meas-1]～data[0])*/
	// ib=freq_meas;
	while(bit){				// 			3639/256 5.7us 
		AD.ADCSR.BIT.ADST = 1;            //読み取り			29114(20MHz) 5.7us
		bit--;
		while(AD.ADCSR.BIT.ADF==0);       //読み取り完了待ち
		adc_data[bit] = AD.ADDRA>>8;	  //読み取りは8ピット
	}
	timer_freq = TW.TCNT;
	timer_freq -= timer_start;	// shortなので単独計算すれば桁を考慮しなくて良い

	/* データ計算（多少、遅くても良い・経過時間はタイマー測定） */
	// max=0, min=1024, max_c=0,min_c=0
	/* 平均値の計算 
	for( bit = freq_meas-1 ; bit > 0 ; bit--){
		sum += adc_data[bit];			// sumはlong
	}
	avr = (byte)(sum/(unsigned short)(freq_meas));
	*/

	/* ピーク点のカウント */
	
	if( adc_data[freq_meas-2] < adc_data[freq_meas-1] ){
		rise = 0;
	}else{
		rise = 1;
	}
	for( bit = freq_meas-1 ; bit > 0 ; bit--){
		if( rise ){ 							// 上り中
			if(  adc_data[bit-1] < adc_data[bit] ){
			//				次			現在		// 次は下りになる
				rise =0;
				max_c++;
				if( first == 0 ) first = bit;
				last = bit;
			}
		} else {								// (rise == 0 ) 下り中
			if( adc_data[bit-1] > adc_data[bit] ){
												// 次は上りになる
				rise = 1;
			}
		}
	}

//	if( avr < IR_ADC_HL ){
	if( ir_adc_sens() < IR_ADC_HL ){
		ir_adc_detect_timer_off();
		#ifdef TEST
		// SCI3_PRINTF("avr(%d)<IR_ADC_HL(%d) timer=%d first=%d last=%d avr=%d sample=%d max_count=%d\n",avr,IR_ADC_HL,timer_freq,first,last,avr,freq_meas,max_c);
		SCI3_PRINTF("ir_adc_sens(%d)<IR_ADC_HL(%d) timer=%d first=%d last=%d sample=%d max_count=%d\n",ir_adc_sens(),IR_ADC_HL,timer_freq,first,last,freq_meas,max_c);
		#endif
		return(-1);					// 信号エラーによるリターンは「-1」
	}

	/* H->L 待ち */
	timer_wait = (int)(13.5*256./1000.)+2;	// =5 13500usのタイムアウト時間設定
	prev=255;
	timer_a=TA.TCA;	
	#ifdef TEST
	timer1 = TW.TCNT;				// H->L待ち時間測定用
	LED1_OUT = LED_OUT_ON;
	#endif
	while( prev > IR_ADC_HL ){
		prev=ir_adc_sens();
		if( ( TA.TCA-timer_a ) > timer_wait ){
			ir_adc_detect_timer_off();	// 既に信号を検出しているのでタイムアウトでもエラーと判定
			#ifdef TEST
			SCI3_PRINTF("sync H->L timeout\n");
			// SCI3_PRINTF("timer=%d first=%d last=%d avr=%d sample=%d max_count=%d\n",timer_freq,first,last,avr,freq_meas,max_c);
			#endif
			return(-1);					// 信号エラーによるリターンは「-1」
		}
	}
	timer_sync_H = TW.TCNT;
	#ifdef TEST
	timer1 = timer_sync_H - timer1;	// H->L待ち時間の決定(少ないと危ない)
	timer2 = timer_sync_H;
	LED1_OUT = LED_OUT_OFF;
	#endif
//	timer_wait = (unsigned short)(26*sys_clock/8000000);//  26us = 38kHz	
//	while( TW.TCNT-timer_sync_H < timer_wait);		//	26usの待ち時間（ L 確定待ち）
	timer_sync_H -= timer_start;	// shortなので単独計算すれば桁を考慮しなくて良い
//	timer_sync_H -= timer_freq;
	sync_H = (int)((8000000./sys_clock)*(float)(timer_sync_H)+0.5);

	/* L->H 待ち */
	timer_wait = (int)(6.75*256./1000.)+2;	//=3 6750usのタイムアウト時間設定
	prev=0;
	#ifdef TEST
	LED1_OUT = LED_OUT_ON;
	#endif
	while( prev < IR_ADC_LH ){
		prev=ir_adc_sens();
		if( ( TA.TCA-timer_a ) > timer_wait ){
			ir_adc_detect_timer_off();	// 既に信号を検出しているのでタイムアウトでもエラーと判定
			#ifdef TEST
			SCI3_PRINTF("sync L->H timeout\n");
			// SCI3_PRINTF("timer=%d first=%d last=%d avr=%d sample=%d max_count=%d\n",timer_freq,first,last,avr,freq_meas,max_c);
			#endif
			return(-1);					// 信号エラーによるリターンは「-1」
		}
	}
	timer_sync_L = TW.TCNT;
	#ifdef TEST
	timer3 = timer_sync_L;
	timer2 = timer_sync_L - timer2;				// L->H 待ち時間
	LED1_OUT = LED_OUT_OFF;
	#endif
	timer_symbol   = timer_sync_L;
	timer_sync_L -= timer_start;	// shortなので単独計算すれば桁を考慮しなくて良い
//	timer_sync_L -= timer_freq;
	timer_sync_L -= timer_sync_H;
	sync_L = (unsigned short)((8000000./sys_clock)*(float)(timer_sync_L)+0.5);

	/* モード判定 */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5×16 ＝ 9000us	6750us以上でＮＥＣと判定*/
	/* データ確認 (SYNC部のノイズエラーを検出) */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5×16 ＝ 9ms以上
	           488us×16 ＝ 7.808ms  (Clock 2個 = 7.813ms)で判定*/

	if( sync_H>6750 && sync_H > sync_L ){	// 7.8ms以上でSYNC_OFFが短い
		det=0;					// NEC
		symbol_len = timer_sync_L/4;			// NEC symbol_len 8T/4= 2T
		symbol_rate = 8000000/(unsigned short)(sync_H+sync_L);
		if( sync_H > sync_L * 4 ){				 // SYNC_OFFが極端に短い
			ir_adc_detect_timer_off();
			#ifdef TEST
			SCI3_PRINTF("NEC sync_H>sync_L*4\n");
			SCI3_PRINTF("SYNC Duration= %d count (ON: %d count)/(OFF: %d count)[TEST]\n",timer_sync_H+timer_sync_L,timer_sync_H,timer_sync_L);
			#endif
			return(-1);
		}
		*mode = NEC;
	}else if( sync_H < 3*sync_L ){	// AEHA 2:1 2<3
		det=0;
		symbol_len = timer_sync_L/2;			// AEHA symbol_len 4T/2 = 2T
		symbol_rate = 4000000/(int)(sync_H+sync_L);
		if( sync_H < sync_L || sync_H > sync_L * 4 ){	// SYNC_ONかSYNC_OFFが異常に短い
			ir_adc_detect_timer_off();
			#ifdef TEST
			SCI3_PRINTF("AEHA sync_H<sync_L or sync_H>sync_L*4\n");
			SCI3_PRINTF("SYNC Duration= %d count (ON: %d count)/(OFF: %d count)[TEST]\n",timer_sync_H+timer_sync_L,timer_sync_H,timer_sync_L);
			#endif
			return(-1); 
		}
		*mode = AEHA;
	}else{								// SIRC 4:1 4>3
		det=1;
		symbol_len = (3*timer_sync_L)/2;		// SIRC symbol_len 4T/2 = 2T
		symbol_rate = 2000000/(int)(sync_H+sync_L);
		if( sync_H < sync_L ){				 	// SYNC_ONが極端に短い
			ir_adc_detect_timer_off();
			#ifdef TEST
			SCI3_PRINTF("SIRC sync_H<sync_L\n");
			SCI3_PRINTF("SYNC Duration= %d count (ON: %d count)/(OFF: %d count)[TEST]\n",timer_sync_H+timer_sync_L,timer_sync_H,timer_sync_L);
			#endif

			return(-1);
		}
		*mode = SIRC;
	}

	noise_len = symbol_len / IR_READ_NOISE_DET / 2;		// T÷IR_READ_NOISE_DETまで
	data_wait = 5*symbol_len/2;							// 5Tまで待つ＝終了検出

	#ifdef TEST
	timer3 = TW.TCNT - timer3;			// H後の判定時間
	LED2_OUT = LED_OUT_OFF;
	#endif

	/* 信号読取り */
	/*  
	
		BIT0	H(1T) + L(1T)
		SYMBOL	H(1T) + L(2T)	//判定長
		BIT1	H(1T) + L(3T)
	
		BIT0	H(1T) + L(1T)
		SYMBOL	H(1T) + L(2T)	//判定長
		BIT1	H(1T) + L(3T)
	
		BIT0	H(1T) + L(1T)
		SYMBOL	H(1.5T) + L(1T)	//判定長
		BIT1	H(2T) + L(1T)
	*/
	timer_wait = 2;				// 3.9～7.8msのタイムアウト時間設定
	for(i=0;i<DATA_SIZE;i++){
		in = 0;
		for(bit=0;bit<8;bit++){
			
			/* H->L 待ち */
			timer_a=TA.TCA;	
			while( prev > IR_ADC_HL ){
				prev=ir_adc_sens();
				if( ( TA.TCA-timer_a ) > timer_wait ){
					ir_adc_detect_timer_off();	// Ｈになったままは考えられないので異常
					#ifdef TEST
					SCI3_PRINTF("data read H->L timeout\n");
					#endif
					*mode = ERROR;
					return(-1);					// 信号エラーによるリターンは「-1」
				}
			}
			in_len = TW.TCNT;
			timer_a_end=TA.TCA;					// データ終了に備えてタイマー値を保存
			in_H   = in_len - timer_symbol;
			timer_total += in_H;
			timer_symbol = in_len;
			#ifdef TEST
			LED1_OUT = LED_OUT_OFF;
			#endif
							
			/* L->H 待ち */
			timer_a=TA.TCA;	
			while( prev < IR_ADC_LH ){
				prev=ir_adc_sens();
				if( ( TA.TCA-timer_a ) > timer_wait ){
						prev=255;				// Ｈにならない(信号の終わり)
				}
			}
			in_len = TW.TCNT;
			in_L   = in_len - timer_symbol;
			timer_symbol = in_len;
			
			if( *mode == SIRC ){				// SIRCのときはＨの長さで判断
				in_len = in_H;
				#ifdef TEST
				LED1_OUT = LED_OUT_ON;
				#endif
			}else{								// AEHAとNECはＬの長さで判断
				in_len = in_L;
			}
			if( in_len > noise_len && in_len < data_wait){	// 有効なら
				if( in_len < symbol_len ){				// Ｌ区間が短い BIT0
					in = in>>1;
					in += 0;
				}else{									// Ｌ区間が長い BIT1
					in = in>>1;
					in += 128;
				}
				timer_total += in_L;
			}else{
				if( in_L >= data_wait ){		// OFF長で判断 ※in_lenだとSIRCの場合に判断できなくなる。
				/* 終了を検出した場合の処理(通常のループから除外するために再度ifをかけている)  */
					in = in>>(8 - bit); 		// 空ビットのシフト
					data[i]=in;					// データ保存
					len = i * 8 + bit;			// 長さ保存
					i = DATA_SIZE -1;			// ループ抜け(break)
					bit=7;						// ループ抜け(break)
					#ifdef TEST
					SCI3_PRINTF("symbol_len=%d, noise_len=%d EOF=(in_len=%d > data_wait=%d)\n",symbol_len,noise_len,in_len,data_wait);
					#endif
				} else {
				/* ノイズを検出した場合の処理(通常のループから除外するために再度ifをかけている) */
					ir_adc_detect_timer_off();
					#ifdef TEST
					SCI3_PRINTF("data data_wait timeout\n");
					SCI3_PRINTF("symbol_len=%d, noise_len=%d in_len=%d data_wait=%d\n",symbol_len,noise_len,in_len,data_wait);
					SCI3_PRINTF("SYNC Duration= %d count (ON: %d count)/(OFF: %d count)[TEST]\n",timer_sync_H+timer_sync_L,timer_sync_H,timer_sync_L);
					#endif
					*mode = ERROR;
					return(-1);						// エラーなので瞬時にread再開できるようにする
				}
			}
		}
		data[i]=in;
	}

	/*タイマーオフ*/
	ir_adc_detect_timer_off();
	
	wave_us = (8000000./sys_clock)*(float)timer_freq*(float)(first-last-1)/(float)freq_meas/(float)(max_c-1);
	freq_hz = 1000000./wave_us;
	data_ms  = (8000./sys_clock)*(float)(timer_total);
	total_ms = (8000./sys_clock)*(float)(timer_sync_H+timer_sync_L+timer_total);
	timer_a = timer_a_end - timer_a_start;

	/* 結果表示 */
	if( DIAG ){
		SCI3_OUT_STRING("done\n\n");
		#ifdef TEST2
		SCI3_OUT_STRING("System Clock = ");
		SCI3_PRINTF_float( sys_clock ,8 );	SCI3_OUT_STRING(" Hz\n");
		SCI3_PRINTF("timer_freq=%d first=%d last=%d meas=%d mac_count=%d\n",timer_freq,first,last,freq_meas,max_c);
		for( bit = 0 ; bit < freq_meas ; bit++){
			SCI3_PRINTF("%x:",adc_data[bit]);
		}
		SCI3_OUT_STRING("\n");
		#endif
		#ifdef TEST
		SCI3_OUT_STRING("TEST\n");
		SCI3_PRINTF("t1=%d[us](>100) t2=%d(>100)[us] t3=%d(<100)[us] freq=%d[us]\n",(int)((8000000./sys_clock)*(float)(timer1)+0.5),(int)((8000000./sys_clock)*(float)(timer2)+0.5),(int)((8000000./sys_clock)*(float)(timer3)+0.5),(int)((8000000./sys_clock)*(float)(timer_freq)+0.5));
		// SCI3_PRINTF("timer=%d first=%d last=%d avr=%d sample=%d max_count=%d[TEST]\n",timer_freq,first,last,avr,freq_meas,max_c);
		SCI3_PRINTF("%d %d0 [TEST]\n",(int)wave_us,(int)(freq_hz/10));
		SCI3_PRINTF("SYNC Duration= %d count (ON: %d count)/(OFF: %d count)[TEST]\n",timer_sync_H+timer_sync_L,timer_sync_H,timer_sync_L);
		#endif
		SCI3_PRINTF("Wave Length  = %d",(int)wave_us ); SCI3_PRINTF_float( wave_us , -3);
		SCI3_OUT_STRING("[us]\n");
		SCI3_OUT_STRING("Carrier freq = "); SCI3_PRINTF_float( freq_hz , 5);
		SCI3_OUT_STRING("[Hz]\n");
		ir_sci_disp_mode( *mode );
		SCI3_PRINTF("Data Length  = %d bits\n",len); 
		SCI3_PRINTF("SYNC Duration= %d us (ON: %d us)/(OFF: %d us)\n",sync_H+sync_L,sync_H,sync_L);
		// SCI3_PRINTF("DATA Duration= %d msec.\n",(int)(1000.*(float)(timer_a)/256.-(float)(sync_H+sync_L)/1000.+0.5) );
		SCI3_PRINTF("DATA Duration= %d",(int)data_ms ); SCI3_PRINTF_float( data_ms , -3);
		SCI3_OUT_STRING(" msec.\n");
		// SCI3_PRINTF("TOTAL Time   = %d msec.\n",(int)(1000.*(float)(timer_a)/256.+0.5) );
		SCI3_PRINTF("TOTAL Time   = %d",(int)total_ms ); SCI3_PRINTF_float( total_ms , -3);
		SCI3_OUT_STRING(" msec.\n");
		SCI3_PRINTF("SYMBOL Rate  = %d Baud (bps)\n",symbol_rate);
		// SCI3_PRINTF("DATA Rate    = %d Baud (bps)\n\n",(int)((1000.*(float)len)/(1000.*(float)(timer_a)/256.-(float)(sync_H+sync_L)/1000.+0.5)));
		SCI3_PRINTF("DATA Rate    = %d Baud (bps)\n\n",(int)((1000.*(float)len)/data_ms) );
	}
	return ( len );
}

/* 単体テスト用ソース */
#ifndef MAIN
int main(void){
	int len;
	byte data[DATA_SIZE];
	int time_out=IR_READ_TIME_OUT;
	char sci_tx[SCI_SIZE], sci_rx[SCI_SIZE];
	enum Ir_Mode mode;

	ir_port_init();
	ir_adc_init(0);
	if (DIAG) {
		ir_sci_init(sci_tx,sci_rx);		// シリアル初期化
	}
	DI;	/*割込み禁止*/
	sys_cal();
	millsec_cal();						// ミリ秒のキャリブレーション
	EI;	/*割込み許可*/

	while(1){
		len=0;
		while( len < 1 ){
			len = ir_adc_read( &data[0], time_out, &mode);
			if( len == -1 && DIAG ) SCI3_PRINTF("noise detected\n");
		}
		if( DIAG ){
			ir_sci_disp(&data[0],len);
		}
	}
}
#endif
