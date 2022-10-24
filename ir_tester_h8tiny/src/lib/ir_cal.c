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
	TIMER_V_B_AEHA	=	(int)(sys_clock/1000./4./(float)FREQ_AEHA+0.5);	// 132
	TIMER_V_B_NEC	=	(int)(sys_clock/1000./4./(float)FREQ_NEC +0.5);	// 132
	TIMER_V_B_SIRC	=	(int)(sys_clock/1000./4./(float)FREQ_SIRC+0.5);	// 125
	TIMER_V_A_AEHA	=	(int)((float)TIMER_V_B_AEHA*(float)FREQ_TAU/100.);		// 66
	TIMER_V_A_NEC	=	(int)((float)TIMER_V_B_NEC *(float)FREQ_TAU/100.);		// 66
	TIMER_V_A_SIRC	=	(int)((float)TIMER_V_B_SIRC*(float)FREQ_TAU/100.);		// 62
	TIMER_V_TS_AEHA	=	(int)((float)SYMBOL_AEHA*sys_clock/1000000/(float)TIMER_V_B_AEHA/4.+0.5);	// 18(17.8)
	TIMER_V_TS_NEC 	=	(int)((float)SYMBOL_NEC *sys_clock/1000000/(float)TIMER_V_B_NEC /4.+0.5);	// 21(21.3)
	TIMER_V_TS_SIRC	=	(int)((float)SYMBOL_SIRC*sys_clock/1000000/(float)TIMER_V_B_SIRC/4.+0.5);	// 24(24.0)
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
	if( DIAG ) SCI3_PRINTF("Milli.Sec. Cal. ");
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

void ir_aeha_cal(int adj){			// adj=0 回数を初期化 adj=1 初期化せず adj>1 遅い方向に補正
	int i;
	unsigned char timer_0,timer_1,timer_2;
	float wait;
	int wait_i;
	
	if( adj==0 ){
		FLASH_AEHA_TIMES=	(int)((SYMBOL_AEHA*FREQ_AEHA+500)/1000)	;
		FLASH_NEC_TIMES=	(int)((SYMBOL_NEC*FREQ_NEC+500)/1000)	;
		FLASH_SIRC_TIMES=	(int)((SYMBOL_SIRC*FREQ_SIRC+500)/1000)	;
	}else{
		adj -= 1;
	}
	
	DI;		//割り込み禁止
	TA.TMA.BYTE     =0b00001000;  /* タイマーＡ設定 */
	IRR1.BIT.IRRTA  =0;           /* タイマーA割込フラグ */
	IENR1.BIT.IENTA =1;           /* タイマーA割込設定 */
	EI;		//割り込み開始
	if( DIAG ) SCI3_PRINTF("MOD. Cal. ");
	while( TA.TCA );			/* カウンタリセット待ち*/
	/* 測定開始 */
	for(i=0 ; i < 416 ; i++){
		ir_wait(4);				// 4*470us	total=782.08ms
	}
	timer_0 = TA.TCA;				// timer_0 * 1000ms / 256 -> 782.08ms (目標TCA値200)
	ir_sens(0);
	timer_1 = TA.TCA;
	ir_sens(1);
	timer_2 = TA.TCA;
	/* 測定完了 */
	DI;		//割り込み禁止
	IENR1.BIT.IENTA =0;           /* タイマーＡ割込み解除 */
	EI;		//割り込み開始
	wait = ((float)FLASH_AEHA_WAIT*256.*782.08)/((float)timer_0*1000.);
	wait_i = (int)(wait+0.5);
	while( timer_1-timer_0 < 3 || timer_2-timer_1 < 3){
		if( DIAG ) SCI3_PRINTF("\nNoise Detected.\nPlease keep the device away from a noise\n");
		wait_millisec(1000);
		while( TA.TCA == timer_0 );
		ir_sens(0);
		timer_1 = TA.TCA;
		ir_sens(1);
		timer_2 = TA.TCA;
	}
	
	if( DIAG ) {
		SCI3_PRINTF("-> %d WAIT=%d",timer_0,FLASH_AEHA_WAIT);
		SCI3_PRINTF("->%d(%d.%d) ",wait_i,(int)wait,(int)( wait*100 - (100*(int)wait) +.5) );
		SCI3_PRINTF("EOF=%d/%d",timer_1-timer_0,timer_2-timer_1);
	}
	/* キャリブレーション値が不一致 */
	if( wait_i != FLASH_AEHA_WAIT || TIMER_A_EOF[0] != timer_1-timer_0 || TIMER_A_EOF[1] != timer_2-timer_1){
		FLASH_AEHA_WAIT = wait_i;
		FLASH_NEC_WAIT  = (int)(wait*(float)FREQ_NEC/(float)FREQ_AEHA+0.5);
		FLASH_SIRC_WAIT = (int)(wait*(float)FREQ_SIRC/(float)FREQ_AEHA+0.5);
		/* センサーをグローバル変数に代入する */
		TIMER_A_EOF[0] = timer_1-timer_0;
		TIMER_A_EOF[1] = timer_2-timer_1;
		if( DIAG ) SCI3_PRINTF("\nRe");
		ir_aeha_cal(0);
	}else{
		/* キャリブレーション値が一致している。だけどタイマー値と合っていないので点滅回数であわせる */
		if( DIAG ) SCI3_PRINTF(" TIMES=%d",FLASH_AEHA_TIMES);
		FLASH_AEHA_TIMES=(int)( (float)SYMBOL_AEHA*(float)FREQ_AEHA/1000.*256.*782.08/((float)timer_0*1000.)+.5 )+adj;
		FLASH_NEC_TIMES= (int)( (float)SYMBOL_NEC *(float)FREQ_NEC /1000.*256.*782.08/((float)timer_0*1000.)+.5 )+adj;
		FLASH_SIRC_TIMES=(int)( (float)SYMBOL_SIRC*(float)FREQ_SIRC/1000.*256.*782.08/((float)timer_0*1000.)+.5 )+adj;
		if( DIAG ) SCI3_PRINTF("->%d\n",FLASH_AEHA_TIMES);
	}
	DI;		//割り込み禁止
}

void ir_cal_disp_freq(void){
	SCI3_PRINTF("     FREQ_AEHA   %d -> %d [kHz]\n",38,FREQ_AEHA);
	SCI3_PRINTF("     FREQ_NEC    %d -> %d [kHz]\n",38,FREQ_NEC);
	SCI3_PRINTF("     FREQ_SIRC   %d -> %d [kHz]\n",40,FREQ_SIRC);
}

void ir_cal_disp_symbol(void){
	SCI3_PRINTF("     SYMBOL_AEHA %d -> %d [us]\n",470,SYMBOL_AEHA);
	SCI3_PRINTF("     SYMBOL_NEC  %d -> %d [us]\n",563,SYMBOL_NEC);
	SCI3_PRINTF("     SYMBOL_SIRC %d -> %d [us]\n",600,SYMBOL_SIRC);
}

void ir_cal_disp(void){

	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("FREQUENCY SETTINGS\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	ir_cal_disp_freq();
	SCI3_PRINTF("\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("SYMBOL LENGTH SETTINGS\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	ir_cal_disp_symbol();
	SCI3_PRINTF("\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("TIMER CAL DATA for ir_send\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("TIMER_V_B_AEHA   %d -> %d \n",132,TIMER_V_B_AEHA);
	SCI3_PRINTF("TIMER_V_B_NEC    %d -> %d \n",132,TIMER_V_B_NEC);
	SCI3_PRINTF("TIMER_V_B_SIRC   %d -> %d \n",125,TIMER_V_B_SIRC);
	SCI3_PRINTF("TIMER_V_A_AEHA   %d -> %d \n",66,TIMER_V_A_AEHA);
	SCI3_PRINTF("TIMER_V_A_NEC    %d -> %d \n",66,TIMER_V_A_NEC);
	SCI3_PRINTF("TIMER_V_A_SIRC   %d -> %d \n",62,TIMER_V_A_SIRC);
	SCI3_PRINTF("TIMER_V_TS_AEHA  %d -> %d \n",18,TIMER_V_TS_AEHA);
	SCI3_PRINTF("TIMER_V_TS_NEC   %d -> %d \n",21,TIMER_V_TS_NEC);
	SCI3_PRINTF("TIMER_V_TS_SIRC  %d -> %d \n",24,TIMER_V_TS_SIRC);
	SCI3_PRINTF("\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("TIMER CAL DATA for ir_read\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("TIMER_A_EOF[0]   %d -> %d \n",10,TIMER_A_EOF[0]);
	SCI3_PRINTF("TIMER_A_EOF[1]   %d -> %d \n",10,TIMER_A_EOF[1]);
	SCI3_PRINTF("\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("LOOP COUNTER CAL DATA for ir_send_simple\n");
	SCI3_PRINTF("+-------+-------+-------+-------+-------+-------\n");
	SCI3_PRINTF("MILLISEC_WAIT    %d -> %d \n",64,TIMER_V_B_AEHA);
	SCI3_PRINTF("FLASH_AEHA_WAIT  %d -> %d \n",5,FLASH_AEHA_WAIT);
	SCI3_PRINTF("FLASH_AEHA_TIMES %d -> %d \n",18,FLASH_AEHA_TIMES);
	SCI3_PRINTF("FLASH_NEC_WAIT   %d -> %d \n",5,FLASH_NEC_WAIT);
	SCI3_PRINTF("FLASH_NEC_TIMES  %d -> %d \n",22,FLASH_NEC_TIMES);
	SCI3_PRINTF("FLASH_SIRC_WAIT  %d -> %d \n",5,FLASH_SIRC_WAIT);
	SCI3_PRINTF("FLASH_SIRC_TIMES %d -> %d \n",24,FLASH_SIRC_TIMES);
}
