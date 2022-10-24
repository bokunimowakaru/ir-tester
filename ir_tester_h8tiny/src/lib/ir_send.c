/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

/*
	Clock = 20MHz
						/4
	AEHA,NEC	38kHz	131.6	132(37.9kHz) 66
	SIRC		40kHz	125		125(38.1kHz) 63

*/

void ir_flash_timer(int times){
	int j;
	
	TV.TCSRV.BYTE = TIMER_V_FLASH;
	EI;	/*割込み許可*/
	for(j=0 ; j < times ; j++){
		while( (TV.TCSRV.BYTE & 0b10000000) == 0 );
		TV.TCSRV.BYTE &= 0b01111111;
	}
	DI;	/*割込み禁止*/
	TV.TCSRV.BYTE = TIMER_V_WAIT;
}
void ir_wait_timer(int times){
	int j;
	
	TV.TCSRV.BYTE = TIMER_V_WAIT;
	EI;	/*割込み許可*/
	for(j=0 ; j < times ; j++){
		while( (TV.TCSRV.BYTE & 0b10000000) == 0b00000000 );
		TV.TCSRV.BYTE &= 0b01111111;
	}
	DI;	/*割込み禁止*/
}

/* 信号送出 */
void ir_send(byte *data, int data_len, enum Ir_Mode mode){
	int i,j,bit,t,len;
	byte out;
	
	DI;	/*割込み禁止*/
	TV.TCRV0.BYTE = 0b00010001;
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
	switch( mode ){
		case NA:
			if( DIAG ) SCI3_PRINTF("mode=NA\n");
			break;
		case AEHA:
			TV.TCORB=TIMER_V_B_AEHA;	// 周期
			TV.TCORA=TIMER_V_A_AEHA;	// デューティー
			ir_flash_timer(8*TIMER_V_TS_AEHA);						// send 'SYNC'
			ir_wait_timer(4*TIMER_V_TS_AEHA);						// send 'SYNC'
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					ir_flash_timer(1*TIMER_V_TS_AEHA);
					if( ((out)&(1)) == 0){
						ir_wait_timer(1*TIMER_V_TS_AEHA);
					}else{
						ir_wait_timer(3*TIMER_V_TS_AEHA);
					}
					data_len--;
					if( data_len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			ir_flash_timer(1*TIMER_V_TS_AEHA);						// send 'stop'
			break;
		case NEC:
			TV.TCORB=TIMER_V_B_NEC;	// 周期
			TV.TCORA=TIMER_V_A_NEC;	// デューティー
			ir_flash_timer(16*TIMER_V_TS_NEC);						// send 'SYNC'
			ir_wait_timer(8*TIMER_V_TS_NEC);							// send 'SYNC'
			/*以降はAEHAと同じ(上位フォーマットは違う)*/
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					ir_flash_timer(1*TIMER_V_TS_NEC);
					if( ((out)&(1)) == 0){
						ir_wait_timer(1*TIMER_V_TS_NEC);
					}else{
						ir_wait_timer(3*TIMER_V_TS_NEC);
					}
					data_len--;
					if( data_len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			ir_flash_timer(1*TIMER_V_TS_NEC);						// send 'stop'
			break;
		case SIRC:
			TV.TCORB=TIMER_V_B_SIRC;	// 周期
			TV.TCORA=TIMER_V_A_SIRC;	// デューティー
			
			/* シンボル基本単位の総数tをカウント */
			t=5;						// 送信済シンボル基本単位の初期化(SYNCで送信)
			len=data_len;
			for(i=0;i<DATA_SIZE;i++){
				out = data[i];
				for(bit=0;bit<8;bit++){
					if( ((out)&(1)) == 0){
						t +=2 ;
					}else{
						t +=3 ;
					}
					len--;
					if( len == 0){
						i = DATA_SIZE -1;
						bit=7;
					}
					out >>= 1;
				}
			}
			if( t > 75 ) t=0;			// データが長すぎる場合
			/* SIRC送信×３回 */
			for(j=0;j<3;j++){
				len=data_len;			// 送信済ビット数のカウント用(AEHA,NECとの違い)
				ir_flash_timer(4*TIMER_V_TS_SIRC);						// send 'SYNC'
				ir_wait_timer(1*TIMER_V_TS_SIRC);						// send 'SYNC'
				for(i=0;i<DATA_SIZE;i++){
					out = data[i];
					for(bit=0;bit<8;bit++){
						if( ((out)&(1)) == 0){
							ir_flash_timer(1*TIMER_V_TS_SIRC);
						}else{
							ir_flash_timer(2*TIMER_V_TS_SIRC);
						}
						ir_wait_timer(1*TIMER_V_TS_SIRC);
						len--;
						if( len == 0){
							i = DATA_SIZE -1;
							bit=7;
						}
						out >>= 1;
					}
				}
				ir_wait_timer( (75-t)*TIMER_V_TS_SIRC );
			}
			break;
		case ERROR:
			if( DIAG ) SCI3_PRINTF("ERROR\n");
			break;
	}
}
