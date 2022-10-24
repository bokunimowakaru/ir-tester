/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/
#define IR_CALC_H	// ヘッダファイルなし

unsigned long pow_10( int i ){
	unsigned long j=1;
	
	if( i<0 || i> 8 ){
		return(-1);
	}else {
		while( i ){
			j *= 10;
			i--;
		}
		return( j );
	}
}

void SCI3_PRINTF_float( float in, int place){
	
	int i;
	
	if( place > 0 && place <= 8){
		for( i=(place-1) ; i >= 0 ; i-- ){
			SCI3_PRINTF("%d",(int)( in / pow_10(i) ) - (int)( in / pow_10(i+1) )*10 );
			if( i==3 || i==6 ) SCI3_OUT_STRING(",");
		}
	}else if (place < -1 && place > -8 ){
		SCI3_OUT_STRING(".");
		for( i=-1 ; i >= place ; i-- ){
			SCI3_PRINTF("%d",(int)( in * pow_10(-i) ) - (int)(in * pow_10(-i-1) )*10 );
		}
	}else{
		SCI3_OUT_STRING("ERROR");
	}
}

void sys_cal( void ){
	unsigned short timer[16];					// Wタイマー用
	float target = 2929.73*(float)SYS_CLOCK;	// 基準TIME_A 6カウント(6/256 sec)時にWカウンタがカウントすべき値
	int ppm[16];
	int ppm_avr=0;
	int timer_avr=0;
	unsigned long clock_disp;
	int i;
	int wait1,wait2;
	
	DI;		//割り込み禁止
	TA.TMA.BYTE     =0b00001000;  /* タイマーＡ設定 */
	/*                 |  |||-|__ 分周比 000～111(高速時) 000～011(低即時)
	    固定値=0000 ___|--||             000:0.5120 sec.  000 1000 msec.
	                       |             001:0.2560 sec.  001  500 msec.
	 高速=0, 低速=1 _______|             100:0.0160 sec.  010  250 msec.
	                                     111:0.0005 sec.  011  31.25msec*/
	IRR1.BIT.IRRTA  =0;           /* タイマーA割込フラグ */
	IENR1.BIT.IENTA =1;           /* タイマーA割込設定 */
	
	TW.TCRW.BYTE    =0b00110000;  /* タイマーＷ設定 クロックΦ／８ (2MHz=0.5us/ 2.5MHz=0.4us) */
	TW.TMRW.BIT.CTS =0;           /* タイマーＷ非動作 */
	
	EI;		//割り込み開始
	if( DIAG ) SCI3_PRINTF("SYS Cal. ");
	
	while( TA.TCA != 200 );					// 次回に確実に0で待ち受けるために
	/* 測定開始 */
	for( i=0 ; i<16 ; i++){
		wait1 = i*13;
		wait2 = i*13+6;
		TW.TCNT = 0;						/* タイマーＷリセット */
		while( TA.TCA != wait1 );			/* カウンタＡリセット待ち */

		TW.TMRW.BIT.CTS=1;					/* タイマーＷカウント開始 */
		while( TA.TCA != wait2 );
		TW.TMRW.BIT.CTS=0;					/* タイマーＷカウント停止 */
		timer[i] = TW.TCNT;					/* タイマーＷの値を保存 */
	}
	for( i=0 ; i<16 ; i++){
		ppm[i] = (int)( ((float)timer[i]/target-1.)*1000000. + 0.5);	//基準TIME_Aに対する偏差
		ppm_avr +=ppm[i];
		timer_avr += (timer[i]-(unsigned short)target);					//基準TIME_Aよりも早いと＋
		// SCI3_PRINTF(" %d(%dppm)",(timer[i]-(unsigned short)target),ppm[i]);
	}
	sys_clock = (float)SYS_CLOCK*(float)ppm_avr/16. + (float)SYS_CLOCK*1000000.;
	clock_disp = (unsigned long)( (float)SYS_CLOCK*(float)ppm_avr/16. ) + (unsigned long)SYS_CLOCK*1000000ul;
	
	ppm_avr /= 16;
	timer_avr /= 16;
	/* 測定完了 */
	DI;		//割り込み禁止
	IENR1.BIT.IENTA =0;           /* タイマーＡ割込み解除 */
	TW.TMRW.BIT.CTS =0;           /* タイマーＷ割込み解除 */
	
	EI;		//割り込み開始
	if( DIAG ){
		SCI3_PRINTF("->%d SysCK=",timer_avr);
		SCI3_PRINTF_float( clock_disp,8 );
		/*
		for( i=7 ; i >= 0 ; i-- ){
			SCI3_PRINTF("%d",(int)( clock_disp / pow_10(i) ) - (int)(clock_disp / pow_10(i+1) )*10 );
		} */
		SCI3_PRINTF("Hz(%dppm) ->Done \n",ppm_avr);
	}
	/* ie_cal4adc.cの時のみコメントアウト
	TIMER_V_B_AEHA	=	(int)(sys_clock/1000./4./(float)FREQ_AEHA+0.5);	// 132
	TIMER_V_B_NEC	=	(int)(sys_clock/1000./4./(float)FREQ_NEC +0.5);	// 132
	TIMER_V_B_SIRC	=	(int)(sys_clock/1000./4./(float)FREQ_SIRC+0.5);	// 125
	TIMER_V_A_AEHA	=	(int)((float)TIMER_V_B_AEHA*(float)FREQ_TAU/100.);		// 66
	TIMER_V_A_NEC	=	(int)((float)TIMER_V_B_NEC *(float)FREQ_TAU/100.);		// 66
	TIMER_V_A_SIRC	=	(int)((float)TIMER_V_B_SIRC*(float)FREQ_TAU/100.);		// 62
	TIMER_V_TS_AEHA	=	(int)((float)SYMBOL_AEHA*sys_clock/1000000/(float)TIMER_V_B_AEHA/4.+0.5);	// 18(17.8)
	TIMER_V_TS_NEC 	=	(int)((float)SYMBOL_NEC *sys_clock/1000000/(float)TIMER_V_B_NEC /4.+0.5);	// 21(21.3)
	TIMER_V_TS_SIRC	=	(int)((float)SYMBOL_SIRC*sys_clock/1000000/(float)TIMER_V_B_SIRC/4.+0.5);	// 24(24.0)
	*/
	DI;		//割り込み禁止
}

void millsec_cal( void ){
	int wait;
	unsigned char timer_0;
	
	DI;		//割り込み禁止
	TA.TMA.BYTE     =0b00001000;  /* タイマーＡ設定 */
	/*                 |  |||-|__ 分周比 000～111(高速時) 000～011(低即時)
	    固定値=0000 ___|--||             000:0.5120 sec.  000 1000 msec.
	                       |             001:0.2560 sec.  001  500 msec.
	 高速=0, 低速=1 _______|             100:0.0160 sec.  010  250 msec.
	                                     111:0.0005 sec.  011  31.25msec*/
	IRR1.BIT.IRRTA  =0;           /* タイマーA割込フラグ */
	IENR1.BIT.IENTA =1;           /* タイマーA割込設定 */
	EI;		//割り込み開始
	if( DIAG ) SCI3_PRINTF("Milli.Sec. Cal. Started ");
	while( TA.TCA );			/* カウンタリセット待ち*/
	/* 測定開始 */
	wait_millisec(781);			// 781ms
	timer_0 = TA.TCA;				// timer_0 * 1000ms / 256 -> 781 ms
	wait = (int)( ((float)MILLISEC_WAIT*256*781)/((float)timer_0*1000)+0.5 );
	/* 測定完了 */
	DI;		//割り込み禁止
	IENR1.BIT.IENTA =0;           /* タイマーＡ割込み解除 */
	EI;		//割り込み開始
	if( DIAG ){
		SCI3_PRINTF("->%d",timer_0);
		SCI3_PRINTF("->Done MSWAIT=%d",MILLISEC_WAIT);
		SCI3_PRINTF("->%d\n",wait);
	}
	/* キャリブレーション結果が不一致 */
	/*
	if( wait != MILLISEC_WAIT ){
		MILLISEC_WAIT = wait;
		if( DIAG ) SCI3_PRINTF("Re");
		millsec_cal(timer_0);
	}
	*/
	DI;		//割り込み禁止
}
