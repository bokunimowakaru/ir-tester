/*********************************************************************
本ソースリストおよびソフトウェアは、ライセンスフリーです。
利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

#define	NAME	"IR-TESTER"
#define	VERSION	"1.06"

/*
コマンド・リファレンス

	■シリアル・コマンド
	
	ＰＣからハイパーターミナルやTeraTermを使用して制御します。
	通信設定は「19200 8-N-1」としてください。
	「read」と入力すると赤外線検出が始まり、検出すると受信データ等が
	表示されます。「send」と入力すると受信した信号を送信します。

	read [1-9]		リモコン信号の読取り(読取り時間 約５秒×引数)
	read loop		連続読取り(無限ループ、復帰はリセット)
	read simple		シンプルモードで受信(予めmode=AEHA/NEC/SIRCを設定する必要がある)
	send [1-9]		リモコン信号の送信(送信回数)
	send simple		シンプルモードで送信(タイマーを使用せずにデータ生成)
	put [1-12]		読み取った信号をレジスタ（ＲＡＭ）に登録（学習）
	[get] [1-12]	レジスタに登録された信号を復元して送信「getは省略可能」
	display			信号の表示
	input={0xNN...}	データ入力 input={0xAA,0x5A,...} のように入力する
	mode=[mode]		モード設定(AEHA/NEC/SIRC)／モード名の省略で設定確認
	freq=[10-89] 	赤外線38kHz(AEHA)搬送波を10～80kHzに設定／数値省略で設定確認
	tau=[10-89]		赤外線38kHz(AEHA)搬送波のデューティー(tau)を10～80％に設定
	width=[100-890]	変調単位を100～800usに設定(10us単位)／数値省略で設定確認
	length=[10-99]	コード長(ビット数)を変更／数値省略で設定確認
	cal [0-9]		キャリブレーション(0で補正なし)／引数なしで校正値の確認
	edit			データ編集用エディタの起動(未実装)
	quit			シリアル入出力しない単独テスターモードに移行(復帰はリセット)

	■単独テスター・コマンド

	ボタンを押下した状態(SW1=H)で起動(もしくはリセット)すると単独テスターとして
	使用できます。
	Panasonic製BDレコーダーのリモコン(1)の４色ボタンでコマンドを入力します。
	４色ボタンを押して、若干、遅れてから処理が開始されます。
	
	青色ボタン	send 送信
	赤色ボタン	read 読取り
	緑色ボタン	disp 表示(液晶のみ)
	黄色ボタン	loop 連続読取り(送信なし)
	数字ボタン	get  レジスタの信号を送信
	
	※常時read待ちになっていますので、赤ボタンを押さなくても読み取れます。

	■キャリブレーションについて

	※起動時にcalを実施しているので通常は再実施する必要はありません。
	　しかし、初回calの丸め誤差の関係で動作しない機器があります。
	　引数を0にすると設定初期値を元に初回calと同じ動作を行います。
	　引数を1にすると現在の値を元に補正します。
	　2～9の引数にてcal値を遅い方向に補正できます。

	■既知の不具合または制限事項
	・受信データ長の時間表示の精度が4ms前後の誤差を持っています。
	・SIRCをread loopで連続読み取りを行うとAEHAと判断してしまいます。
	・単独テスターモードで送信すると、信号が少し遅れる。
	　(多少の改善は行えますが原理的にゼロには出来ません)
	・SYNC DurationとSYNC CountsのON:OFF時間比が同じにならない
	　(SYNC Durationは検出遅延を補正していますが、Countsは補正していない)

	■開発環境に関する注意点
	・株式会社ベストテクノロジーのGCC Developer Liteを使用します。
		http://www.besttechnology.co.jp/
	・該社ホームページの「ナレッジベース」よりダウンロードできます。
		http://www.besttechnology.co.jp/modules/knowledge/?GCC%20Developer%20Lite
	・この開発環境に含まれるH8マイコン用のライブラリを使用しています。
	・複数のファイルが同時に扱えませんので以下のように工夫しています。
		・全てのソースをincludeで包括
		・main.cに初期設定パラメータを統括
		・一部のソースにはincludeにフラグを付加して重複inculdeを防止
	・推奨の利用方法
		・main.cを開発環境で開く
		・その他のソースはテキストエディタで開く
		・コンパイルの都度、全ソースがコンパイルされます

	■Ｑ＆Ａ
	・このプログラムを使った商品は販売できますか？
	　→当方に著作権のある部分の使用には問題ありません。
	・ソースコードを編集して配布してもかまいませんか？
	　→当方に著作権のある部分の再配布には問題ありません。
	・機能追加を御願いしたい
	　→場合によっては対応しますが、全てには対応できません。
	・リモコンコードを提供するので追加して欲しい
	　→場合によっては対応しますが、全てには対応できません。
	・readの反対はwriteで、sendの反対はreceiveです。
	　→sendとreceiveの組み合わせで入力していただいても結構です。

	■参考文献
	・Ｈ８マイコン関連
		3664F,3694Fで始めるH8
			http://www9.plala.or.jp/fsson/NewHP_elc/Link_nH8.html
		株式会社ベストテクノロジー
			http://www.besttechnology.co.jp/
		株式会社　秋月電子通商
			http://akizukidenshi.com/
		株式会社ルネサステクノロジー
			http://japan.renesas.com/
	・赤外線リモコン方式
		Remote Control with PIC
			http://einst.hp.infoseek.co.jp/Remocon2/Remocon2.html
		赤外線学習リモコンの信号定義データの合成
			http://www.geocities.jp/shrkn65/remocon/
		NECフォーマットの赤外線リモコン・フォーマット
			http://www.necel.com/ja/faq/mi_com/__com_remo.html
	
	■謝辞
	・当方サイトを御利用いただいている方々を始め、
	　多くの方々に支えられて開発することが出来ました。
	　アフィリエイトで研究費に協力をいただいた方々、
	　参考文献の資料を公開されている方々、
	　キャラクタ液晶の表示部分のソースを提供いただいたnakさん、
	　ご協力をいただきました方々に、心より感謝の意を表します。

	■変更履歴(概略)
	・V1.01：受信データ長が長くなるのを修正
	・V1.06：CAL中にノイズを受信時のハングアップ防止
*/

// ヘッダファイル
#include "lib/ir_data.h"		// リモコンデータ検索
#include "lib/ir_read_simple.h"	// タイマー不使用の読取り
#include "lib/ir_send_simple.h"	// タイマー不使用の送信
#include "lib/ir_display.h"		// 表示関連、＃＃＃ウェイト＃＃＃

/* make */

// ヘッダありソース
#include "lib/ir_data.c"		// リモコンデータ検索
#include "lib/ir_read_simple.c"	// タイマー不使用の読取り
#include "lib/ir_send_simple.c"	// タイマー不使用の送信
#include "lib/ir_display.c"		// 表示関連、＃＃＃ウェイト＃＃＃

// ヘッダなしソース(順変更注意)
#ifndef IR_ADC_MODE
	#include "lib/ir_read.c"		// 通常の読取り
#endif
#include "lib/ir_send.c"		// タイマー使用の送信
#include "lib/ir_cal.c"			// キャリブレーション
#define MAIN
#ifdef IR_ADC_MODE
	#include "lib/ir_adc_read.c"	// 赤外線ＬＥＤ読取り
#endif

/* メイン */
void ir_tester(void){
	byte data[13][DATA_SIZE],backup[DATA_SIZE];
	// 各命令内で自由に使える変数
	int i, adj, len, timeout;
	int data_len[13];
	// 各命令間で共用する変数
	int backup_len = 0;
	int id = 0;
	byte loop=0;
	char sci_tx[SCI_SIZE], sci_rx[SCI_SIZE];
	enum Ir_Mode mode[13];	mode[0]=NA;
	enum Ir_Mode backup_mode=NA;

	ir_port_init();						// 汎用入出力ポートの初期化
	if( BUTN == BUTN_ON ) DIAG = 0;		// ボタンを押して起動すると単独テスターモード
	if (DIAG) {
		ir_sci_init(sci_tx,sci_rx);		// シリアル初期化
	}
	ir_led_init();						// LED初期化
	LED1_OUT=LED_OUT_ON;
	LED2_OUT=LED_OUT_ON;
	
	#ifdef LCD
	lcd_init();							// 液晶制御ＩＣのプレ初期化
	wait_millisec(15);
	lcd_init();							// ２回目で初期化される
	lcd_control(1,0,0);					// 液晶制御 表示ON,カーソルOFF,点滅OFF
	lcd_cls();							// 液晶消去
	ir_lcd_init();						// 液晶初期化(ir_display.c)
	#endif

	DI;	/*割込み禁止*/
	sys_cal();
	millsec_cal();						// ミリ秒のキャリブレーション
	ir_aeha_cal(0);						// シンボル速度のキャリブレーション
	ir_led_init();						// LED消灯
	EI;	/*割込み許可*/
	
	/*プリセットデータをレジスタへ読み込み*/
	for( i=0; i<13 ; i++){
		for( len=0 ; len < (int)((IR_DATA[i].len)/8)+1 ; len++){
			data[i][len]	= *(IR_DATA[i].data+len);
			data_len[i]		= IR_DATA[i].len;
			mode[i]			= IR_DATA[i].mode;
		}
	}
	
	// 処理
	while(1){
		#ifdef LCD
		lcd_goto(LCD_ROW_1);
		lcd_putstr("Ready   ");
		#endif
		sci_rx[0]='r';
		if( DIAG && BUTN == BUTN_OFF) {	// 単独モードやボタン押下状態ではシリアル入力しない
			SCI3_PRINTF("\nReady\n> ");
			SCI3_IN_STRING (sci_rx, sizeof(sci_rx) - 1);	//シリアル入力
		}
		for( i=1 ; i<sizeof(sci_rx)-2 ; i++){			// スペースかイコールで区切られた２文字を抽出
			if( sci_rx[i]==' ' || sci_rx[i]=='=' ){
				sci_rx[1]=sci_rx[i+1];
				sci_rx[2]=sci_rx[i+2];
				i=sizeof(sci_rx)-2;
			}else{
				sci_rx[1]=' ';
			}
		}
		if( DIAG == 0 ){				// 単独モード
			if( id >= 13 && id <= 20){
				switch( id ){
					case 13:	case 17:	//青ボタン
						sci_rx[0]='s';	sci_rx[1]=' ';						// 送信命令発行
						for(i=0;i<DATA_SIZE;i++) data[0][i]=backup[i];		// バックアップで上書きする
						data_len[0]=backup_len;	mode[0]=backup_mode;
						break;
					case 14:	case 18:	//赤ボタン
						sci_rx[0]='r';	sci_rx[1]='3';	sci_rx[2]=' ';	// 受信命令発行
						break;
					case 15:	case 19:	//緑ボタン
						sci_rx[0]='d';	sci_rx[1]=' ';						// 表示命令発行
						for(i=0;i<DATA_SIZE;i++) data[0][i]=backup[i];		// バックアップで上書きする
						data_len[0]=backup_len;	mode[0]=backup_mode;
						break;
					case 16:	case 20:	//黄ボタン
						sci_rx[0]='r';  sci_rx[1]='l';	sci_rx[2]=' ';
						break;
				}
				id = -1;
			}else if( id >= 21 && id <= 32){
				sci_rx[0]='g';
				if( id < 30 ){
					sci_rx[1]= '0' + (id-20);
					sci_rx[2]= ' ';
				}else{
					sci_rx[1]= '1';
					sci_rx[2]= '0' + (id-30);
				}
				id = -1;
			}else{
				/*
				while( IR_IN == IR_IN_OFF );	//赤外線入力待ち(SIRCは無理)
				*/
				sci_rx[0]='r';	sci_rx[1]='0';	sci_rx[2]=' ';
			}
		}
		/* コマンド解析 */
		switch( sci_rx[0] ){							// コマンドは先頭の１文字のみで判断する
			/* PUT レジスタ登録コマンド */
			case 'p':	case 'P':
				if( DIAG ) SCI3_PRINTF("put\n");
				if( sci_rx[1] >= '1' &&  sci_rx[1] <= '9'){
					adj = (unsigned int)sci_rx[1]-(unsigned int)'0';
					if( sci_rx[1] == '1' && sci_rx[2] >= '0' &&  sci_rx[2] <= '2'){
						adj = 10 + (unsigned int)sci_rx[2]-(unsigned int)'0';
					}
					if( DIAG ) SCI3_PRINTF("register=%d\n",adj);
					for(i=0;i<DATA_SIZE;i++) data[adj][i]=data[0][i];
					data_len[adj]=data_len[0];
					mode[adj]=mode[0];
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: put [1～12(Register)]\n");
					}
				}
				break;
			/* GET レジスタ復帰＆送信コマンド */
			case '1':	case '2':case '3':	case '4':	case '5':
			case '6':	case '7':	case '8':	case '9':		// 10～12は対応しない
				sci_rx[1]=sci_rx[0];
				sci_rx[2]=' ';
				sci_rx[0]='g';
				// goto case 'g':
				/* FALLTHROUGH */
			case 'g':	case 'G':
				if( DIAG ) SCI3_PRINTF("get\n");
				if( sci_rx[1] >= '1' &&  sci_rx[1] <= '9'){
					adj = (unsigned int)sci_rx[1]-(unsigned int)'0';
					if( sci_rx[1] == '1' && sci_rx[2] >= '0' &&  sci_rx[2] <= '2'){
						adj = 10 + (unsigned int)sci_rx[2]-(unsigned int)'0';
					}
					if( DIAG ) SCI3_PRINTF("register=%d\n",adj);
					for(i=0;i<DATA_SIZE;i++) data[0][i]=data[adj][i];
					data_len[0]=data_len[adj];
					mode[0]=mode[adj];
					/*送信*/
					LED1_OUT=LED_OUT_ON;
					DI;	/*割込み禁止*/
					ir_send(data[0],data_len[0],mode[0]);     // 赤外線出力（タイマー使用）
					EI;	/*割込み許可*/
					LED1_OUT=LED_OUT_OFF;
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: get [1～12(Register)]\n");
					}
				}
				break;
			/* READ 読取りコマンド */
			case 'r':	case 'R':
				if( DIAG ) SCI3_PRINTF("read\n");
				timeout=1;	loop=0;
				if( sci_rx[1] >= '1' &&  sci_rx[1] <= '9'){				//引数は１桁の数字のみ扱う
					timeout=(unsigned int)sci_rx[1]-(unsigned int)'0';
					if( DIAG ) SCI3_PRINTF("timeout=%d\n",timeout);
				}
				if( sci_rx[1] == 'l' || sci_rx[1] == 'L'){
					if( DIAG ) SCI3_PRINTF("loop mode\n");
					loop=1;											// 無限ループ設定
				}
				if( sci_rx[1] == '0' ) timeout=32767;
				#ifdef LCD
				lcd_goto(LCD_ROW_1);
				lcd_putstr("IR wait ");
				#endif
				for(i=0;i<DATA_SIZE;i++){							// タイムアウトやエラーに備えてデータ保存
					backup[i]=data[0][i];
					data[0][i]=0;
				}
				backup_len=data_len[0];
				backup_mode=mode[0];
				while( timeout > 0 || loop){
					LED2_OUT=LED_OUT_ON;
					len = -1;
					while( len == -1){											// Len=-1はエラー。瞬時にread再開
						if( sci_rx[1] == 's' || sci_rx[1] == 'S' ){
							// #ifdef IR_READ_S_H
							len = ir_read_simple(data[0],mode[0]);				// lenはデータ長。0はreadタイムアウト
							// #endif
						}else{
							#ifdef IR_ADC_MODE
							ir_adc_init(0);
							len = ir_adc_read(data[0], IR_READ_TIME_OUT,&mode[0]);
							#else
							len = ir_read(data[0], IR_READ_TIME_OUT,&mode[0]);	// lenはデータ長。0はreadタイムアウト
							#endif
						}
					}															// timeoutをデクリメントして回数を減らす
					LED2_OUT=LED_OUT_OFF;
					if( len == 0){
						if( DIAG ) SCI3_PRINTF("no signal\n");
						timeout--;
						#ifdef LCD
						lcd_goto(LCD_ROW_2);
						lcd_putstr("NoSignal");
						#endif
					} else {										// lenが0でない(タイムアウトやエラーでない)
						data_len[0]=len;							// data_lenはデータ長の確定
						id = ir_search(data[0], data_len[0], mode[0]);
						if( DIAG ) {
							ir_sci_disp(data[0], data_len[0]);  			// シリアルへ表示
							if( id >= 0 ) SCI3_PRINTF("ID=%d %s\n",id,IR_DATA[id].name );
						}
						#ifdef LCD
						if( id >= 0 ){								// IDがあればＩＤを表示
							ir_lcd_disp_id(id);
						}
						if( id < 0 || id > 32 ) {
							ir_lcd_disp(data[0],data_len[0]);		// IDが無い場合や簡易コマンドでない場合はコードを表示
						}
						#endif
						timeout=0;
					}
				}
				if(len==0){
					for(i=0;i<DATA_SIZE;i++) data[0][i]=backup[i];	// タイムアウト時にバックアップを戻す
					data_len[0]=backup_len;
					mode[0]=backup_mode;
				}
				break;
				/*
				if( DIAG ) SCI3_PRINTF("loop read\n");
				#ifdef LCD
				lcd_goto(LCD_ROW_1);
				lcd_putstr("IR wait ");
				#endif
				while(1){
					for(i=0;i<DATA_SIZE;i++) data[i]=0;
					LED2_OUT=LED_OUT_ON;
					data_len = ir_read(data, IR_READ_TIME_OUT,&mode[0]);
					LED2_OUT=LED_OUT_OFF;
					if( data_len == 0){
						if( DIAG ) SCI3_PRINTF("no signal\n");
						#ifdef LCD
						lcd_goto(LCD_ROW_2);
						lcd_putstr("NoSignal");
						#endif
					} else {
						data_len=len;								// data_lenはデータ長の確定
						id = ir_search(data, data_len, mode[0]);
						if( DIAG ) {
							ir_sci_disp(data, data_len);  // シリアルへ表示
							if( id >= 0 ) SCI3_PRINTF("ID=%d %s\n",id,IR_DATA[id].name );
							SCI3_PRINTF("\n");
						}
						#ifdef LCD
						ir_lcd_disp(data,data_len); 			// 液晶へ表示
						if( id >= 0 ) ir_lcd_disp_id(id);
						#endif
					}
				}
				break;
			*/
			/* SEND 送信コマンド */
			case 's':	case 'S':
				if( DIAG ) SCI3_PRINTF("send\n");
				timeout=1;
				if( sci_rx[1] >= '1' &&  sci_rx[1] <= '9'){
					timeout=(unsigned int)sci_rx[1]-(unsigned int)'0';
					if( DIAG ) SCI3_PRINTF("times=%d\n",timeout);
				}
				if( sci_rx[1] == '0' ) timeout=9999;
				while (timeout>0){
					LED1_OUT=LED_OUT_ON;
					#ifdef LCD
					lcd_goto(LCD_ROW_1);
					lcd_putstr("IR send ");
					#endif
					if( sci_rx[1] == 's' || sci_rx[1] == 'S' ){
						#ifdef IR_SEND_S_H
						ir_send_simple(data[0],data_len[0],mode[0]);     // 赤外線出力(シンプルモード)
						#endif
					}else{
						DI;	/*割込み禁止*/
						ir_send(data[0],data_len[0],mode[0]);     // 赤外線出力（タイマー使用）
						EI;	/*割込み許可*/
					}
					LED1_OUT=LED_OUT_OFF;
					#ifdef LCD
					ir_lcd_disp(data[0],data_len[0]); // 液晶へ表示
					#endif
					#ifndef LCD
					wait_millisec(200);
					#endif
					timeout--;
				}
				break;
			/* DISPLAY 表示コマンド */
			case 'd':	case 'D':
				if( DIAG ) SCI3_PRINTF("display\n");
				if( DIAG ) {
					ir_sci_disp(data[0], data_len[0]);
					ir_sci_disp_mode( mode[0] );
				}
				#ifdef LCD
					ir_lcd_disp(data[0],data_len[0]); 			// 液晶へ表示
				#endif
				break;
			/* データ入力コマンド */
			case 'i':	case 'I':
				if( DIAG ) SCI3_PRINTF("input\n");
				
				if( sci_rx[1] == '{' && sci_rx[2] == '0' ){
					for( i=4; i<(SCI_SIZE-2) ; i++){
						if ( sci_rx[i] >= 'A' &&  sci_rx[i] <= 'F' ) sci_rx[i] -= ('A'-'0'-10);
						if ( sci_rx[i] >= 'a' &&  sci_rx[i] <= 'f' ) sci_rx[i] -= ('a'-'0'-10);
					}
					len=0;
					for( i=3; i<(SCI_SIZE-3) ; i++){
						if ( sci_rx[i] == 'x' || sci_rx[i] == 'X' ){
							i++;
							if( sci_rx[i] >= '0' && sci_rx[i] <= '0'+15 ){
								adj = sci_rx[i] -'0';
								if( sci_rx[i+1] >= '0' && sci_rx[i+1] <= '0'+15 ){
									adj *= 16;
									adj += sci_rx[i+1] -'0';
									i++;
								}
								data[0][len]=(byte)adj;
								if( len < DATA_SIZE-1 ) len++;
							}
						}
						if ( sci_rx[i] == '}' ) i=SCI_SIZE-1;
					}
					if( len>0 ){
						data_len[0] = len*8;
						if( DIAG ) ir_sci_disp(data[0], data_len[0]);
					}
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: input={0xNN,0xNN...}\nCurrent=",data_len[0]);
						ir_sci_disp(data[0], data_len[0]);
					}
				}
				break;
			/* MODE IRモードの指定コマンド */
			case 'm':	case 'M':
				if( DIAG ) SCI3_PRINTF("mode\n");
				switch( sci_rx[1] ){
					case 'a':	case 'A':
						if( DIAG ) SCI3_PRINTF("mode=AEHA\n");
						mode[0]=AEHA;
						break;
					case 'n':	case 'N':
						if( DIAG ) SCI3_PRINTF("mode=NEC\n");
						mode[0]=NEC;
						break;
					case 's':	case 'S':
						if( DIAG ) SCI3_PRINTF("mode=SIRC\n");
						mode[0]=SIRC;
						break;
					default:
						if( DIAG ){
							SCI3_PRINTF("Usage: mode=[AEHA/NEC/SIRC]\nCurrent ");
							ir_sci_disp_mode( mode[0] );
						}
						break;
				}
				break;
			/* データー長(ビット数の変更) */
			case 'l':	case 'L':
				if( DIAG ) SCI3_PRINTF("length\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '9' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0');
					if( DIAG ) SCI3_PRINTF("length = %d -> %d [%%]\n",data_len[0],len);
					data_len[0]=len;
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: length=[10～99]\nCurrent length =%d",data_len[0]);
					}
				}
				break;
			/* 赤外線38kHzのデューティー変更 */
			case 't':	case 'T':
				if( DIAG ) SCI3_PRINTF("tau (modulation duty cycle)\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '8' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0');
					if( DIAG ) SCI3_PRINTF("tau = %d -> %d [%%]\n",100-FREQ_TAU,len);
					FREQ_TAU=100-len;
					DI;	/*割込み禁止*/
					sys_cal();
					EI;	/*割込み許可*/
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: tau=[10～80]\nCurrent tau =%d",100-FREQ_TAU);
					}
				}
				break;
			/* 赤外線38kHzの周波数変更 */
			case 'f':	case 'F':
				if( DIAG ) SCI3_PRINTF("frequency\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '8' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0');
					switch( mode[0] ){
						case AEHA:
							if( DIAG ) SCI3_PRINTF("FREQ_AEHA = %d -> %d [kHz]\n",FREQ_AEHA,len);
							FREQ_AEHA = len;
							DI;	/*割込み禁止*/
							sys_cal();
							EI;	/*割込み許可*/
							break;
						case NEC:
							if( DIAG ) SCI3_PRINTF("FREQ_NEC  = %d -> %d [kHz]\n",FREQ_NEC,len);
							FREQ_NEC = len;
							DI;	/*割込み禁止*/
							sys_cal();
							EI;	/*割込み許可*/
							break;
						case SIRC:
							if( DIAG ) SCI3_PRINTF("FREQ_SIRC = %d -> %d [kHz]\n",FREQ_SIRC,len);
							FREQ_SIRC = len;
							DI;	/*割込み禁止*/
							sys_cal();
							EI;	/*割込み許可*/
							break;
						default:
							if( DIAG ){
								SCI3_PRINTF("IR Mode ERROR\nCurrent ");
								ir_sci_disp_mode( mode[0] );
							}
							break;
					}
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: frequency=[10～80]\n");
						ir_cal_disp_freq();
						ir_sci_disp_mode( mode[0] );
					}
				}
				break;
			/* 変調基本単位長の変更 */
			case 'w':	case 'W':
				if( DIAG ) SCI3_PRINTF("width\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '8' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0')*10;
					switch( mode[0] ){
						case AEHA:
							if( DIAG ) SCI3_PRINTF("SYMBOL_AEHA = %d -> %d [us]\n",SYMBOL_AEHA,len);
							SYMBOL_AEHA = len;
							DI;	/*割込み禁止*/
							sys_cal();
							EI;	/*割込み許可*/
							break;
						case NEC:
							if( DIAG ) SCI3_PRINTF("SYMBOL_NEC  = %d -> %d [us]\n",SYMBOL_NEC,len);
							SYMBOL_NEC = len;
							DI;	/*割込み禁止*/
							sys_cal();
							EI;	/*割込み許可*/
							break;
						case SIRC:
							if( DIAG ) SCI3_PRINTF("SYMBOL_SIRC = %d -> %d [us]\n",SYMBOL_SIRC,len);
							SYMBOL_SIRC = len;
							DI;	/*割込み禁止*/
							sys_cal();
							EI;	/*割込み許可*/
							break;
						default:
							if( DIAG ){
								SCI3_PRINTF("IR Mode ERROR\nCurrent ");
								ir_sci_disp_mode( mode[0] );
							}
							break;
					}
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: width=[100～800(10)]\n");
						ir_cal_disp_symbol();
						ir_sci_disp_mode( mode[0] );
					}
				}
				break;
			/* CAL キャリブレーション・コマンド */
			case 'c':	case 'C':
				if( DIAG ) SCI3_PRINTF("calibration\n");
				adj=0;
				if( sci_rx[1] >= '0' &&  sci_rx[1] <= '9'){
					adj=(unsigned int)sci_rx[1]-(unsigned int)'0';
					if( adj && DIAG ) SCI3_PRINTF("cal adj=%d\n",adj);
					DI;	/*割込み禁止*/
					sys_cal();
					ir_aeha_cal(adj);	
					EI;	/*割込み許可*/
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: calibration=[0～9(0=no adjust)]\n");
						ir_cal_disp();
					}
				}
				break;
			/* QUIT 単独テスターモードへ移行するコマンド */
			case 'q':	case 'Q':
				if( DIAG ) SCI3_PRINTF("quit\n");
				DIAG = 0;
				break;
			default:
				if( DIAG ) SCI3_PRINTF("Error\n input 'read' or 'send'\n");
				break;
		}
	}
}
