/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

/* 信号読み取り */
int ir_read(byte *data, int time_out, enum Ir_Mode *mode){
	int i,bit;						// ループ用
	unsigned char timer_0,timer_1,timer_2,timer_3;	// 信号長(TIMER_Aカウント)
	unsigned short timerW_1,timerW_2;		// 信号長(TIMER_Wカウント)
	int tmp;						// タイマ演算用（9ビット必要）
	int	len_on=0,len_off=0;			// 信号長(ループカウント)
	int symbol_len,noise;			// 判定用シンボル長
	int symbol_rate=0;				// 結果表示用
	int len, data_wait;
	int data_len;					/* Irデータのビット数 */
	unsigned char det=IR_IN_OFF;	// 判定時の入力信号レベル(SIRC対応)
	unsigned char in;

	if( DIAG ) SCI3_OUT_STRING("IR Sensing...");

	DI;		//割り込み禁止
	TA.TMA.BYTE     =0b00001000;  /* タイマーＡ設定 */
	IRR1.BIT.IRRTA  =0;           /* タイマーA割込フラグ */
	IENR1.BIT.IENTA =1;           /* タイマーA割込設定 */
	TW.TCRW.BYTE    =0b00110000;  /* タイマーＷ設定 クロックΦ／８ (2MHz=0.5us/ 2.5MHz=0.4us) */
	TW.TMRW.BIT.CTS =0;           /* タイマーＷ非動作 */
	EI;		//割り込み開始
	
	TIMER_SEC = 0;
	TW.TCNT = 0;                  /* タイマーＷリセット*/

	len_on=SYNC_WAIT;											// 
	len_off=SYNC_WAIT;											//
	while( TIMER_SEC <= time_out && IR_IN == IR_IN_OFF);
	TW.TMRW.BIT.CTS=1;					/* タイマーＷカウント開始*/
	timer_0 = TA.TCA;											// 
	/* SYNC_ON検出 */											// 
	while( TIMER_SEC <= time_out && len_on == SYNC_WAIT){		//
		len_on = ir_sens(IR_IN_ON);								//
	}															//
	timerW_1 = TW.TCNT;					/* タイマーＷの値を保存 */
	timer_1 = TA.TCA;					// モード判定用にSYNC_ON時間測定
																//
	/* SYNC_OFF検出 */											//
	while( TIMER_SEC <= time_out && len_off == SYNC_WAIT){		//
		len_off = ir_sens(IR_IN_OFF);							//
	}															//
	TW.TMRW.BIT.CTS=0;					/* タイマーＷカウント停止*/
	if( TIMER_SEC >= time_out ) return(0);	/* タイムアウト時 */

	/*計測開始 */
	timer_2 = TA.TCA;
	timerW_2 = TW.TCNT;					/* タイマーＷの値を保存 */
	
	/* モード判定 */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5×16 ＝ 9ms以上
	           488us×16 ＝ 7.808ms  (Clock 2個 = 7.813ms)で判定*/
	tmp = timer_1 - timer_0;			// SYNC_ONの持続時間
	if( tmp < 0 ) tmp += 256;			// 負だとオーバフローをまたいでいる
	if( tmp >= 2 && len_on > len_off ){	// 7.8ms以上でSYNC_OFFが短い
		*mode = NEC;
		symbol_len = len_off/4;
		//det=IR_IN_OFF;
	}else if( (long)len_on < 3*(long)len_off ){		// AEHA 2:1 2<3
		*mode = AEHA;
		symbol_len = len_off/2;
		//det=IR_IN_OFF;
	}else{											// SIRC 4:1 4>3
		*mode = SIRC;
		det=IR_IN_ON;
		symbol_len = (3*len_off)/2;
	}
	
	/* データー読取り*/
	noise = symbol_len / IR_READ_NOISE_DET;		/* ノイズ判定 */
	data_wait = 2*symbol_len;					/* 終了検出する持続長 */

	for(i=0;i<DATA_SIZE;i++){
		in = 0;
		for(bit=0;bit<8;bit++){
			len = ir_sens( det );
			if( len > noise && len < data_wait){
				if( len < symbol_len ){
					in = in>>1;
					in += 0;
				}else{
					in = in>>1;
					in += 128;
				}
			}else{
				if( len >= data_wait ){
				/* 終了を検出した場合の処理(通常のループから除外するために再度ifをかけている)  */
					in = in>>(8 - bit);
					data[i]=in;
					data_len = i * 8 + bit;
					i = DATA_SIZE -1;
					bit=7;
				} else {
				/* ノイズを検出した場合の処理(通常のループから除外するために再度ifをかけている) */
					DI;		//割り込み禁止
					IENR1.BIT.IENTA =0;           /* タイマーＡ割込み解除 */
					EI;		//割り込み開始
					*mode = ERROR;
					return(-1);						// エラーなので瞬時にread再開できるようにする
				}
			}
		}
		data[i]=in;
	}
	timer_3 = TA.TCA - TIMER_A_EOF[det];
	
	/* データ読取り完了 */
	tmp = timer_3 - timer_2;
	if( tmp <= 0 ) tmp += 256;
	timerW_2 -= timerW_1;
	timerW_1 = (unsigned short)( 8000000./sys_clock*(float)timerW_1+0.5) + IR_DELAY_LH - IR_DELAY_HL;
	timerW_2 = (unsigned short)( 8000000./sys_clock*(float)timerW_2+0.5) + IR_DELAY_HL - IR_DELAY_LH;
	
	DI;		//割り込み禁止
	IENR1.BIT.IENTA =0;           /* タイマーＡ割込み解除 */
	TW.TMRW.BIT.CTS =0;           /* タイマーＷ非動作 */
	EI;		//割り込み開始
	
	/* データ確認 (SYNC部のノイズエラーを検出) */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5×16 ＝ 9ms以上
	           488us×16 ＝ 7.808ms  (Clock 2個 = 7.813ms)で判定*/
	switch( *mode ){
		case AEHA:
			symbol_rate = 4000000/(int)(timerW_1 + timerW_2);
			if( len_on < len_off || len_on > len_off * 4 ){	// SYNC_ONかSYNC_OFFが異常に短い
				*mode = ERROR;
				return(-1); 
			}
			break;
		case NEC:
			symbol_rate = 8000000/(int)(timerW_1 + timerW_2);
			if( len_on > len_off * 4 ){				 // SYNC_OFFが極端に短い
				*mode = ERROR;
				return(-1);
			}
			break;
		case SIRC:
			symbol_rate = 2000000/(int)(timerW_1 + timerW_2);
			if( len_on < len_off ){				 	// SYNC_ONが極端に短い
				*mode = ERROR;
				return(-1);
			}
			break;
		case ERROR:
			return(-1);
			break;
		case NA:									// NAのときはタイムアウト扱い（read再開しない）
			return(0);
			break;
	}
	
	if( DIAG ){
		SCI3_OUT_STRING("done\n\n");
		ir_sci_disp_mode( *mode );
		SCI3_PRINTF("Data Length  = %d bits\n",data_len); 
		SCI3_PRINTF("Symbol Counts= %d samples\n",symbol_len); 
		timer_3 -= timer_2; /* これらは char なので 負にならない */
		timer_2 -= timer_1;
		timer_1 -= timer_0;
		SCI3_PRINTF("SYNC Duration= %d us (ON: %d us)/(OFF: %d us)\n",timerW_1+timerW_2,timerW_1,timerW_2);
		SCI3_PRINTF("SYNC Counts  = %d samples (ON: %d samples)/(OFF: %d samples)\n",len_on+len_off,len_on,len_off);
		SCI3_PRINTF("DATA Duration= %d msec.\n",(1000 * timer_3)/256);
		SCI3_PRINTF("TOTAL Time   = %d msec.\n",(1000 * (timer_1+timer_2+timer_3))/256);
		SCI3_PRINTF("SYMBOL Rate  = %d Baud (bps)\n",symbol_rate);
		SCI3_PRINTF("DATA Rate    = %d Baud (bps)\n\n",((256 * data_len)/timer_3));
	}
	return(data_len);
}
