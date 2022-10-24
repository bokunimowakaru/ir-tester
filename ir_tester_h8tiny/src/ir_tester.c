/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

#define	NAME	"IR-TESTER"
#define	VERSION	"1.06"

/*
�R�}���h�E���t�@�����X

	���V���A���E�R�}���h
	
	�o�b����n�C�p�[�^�[�~�i����TeraTerm���g�p���Đ��䂵�܂��B
	�ʐM�ݒ�́u19200 8-N-1�v�Ƃ��Ă��������B
	�uread�v�Ɠ��͂���ƐԊO�����o���n�܂�A���o����Ǝ�M�f�[�^����
	�\������܂��B�usend�v�Ɠ��͂���Ǝ�M�����M���𑗐M���܂��B

	read [1-9]		�����R���M���̓ǎ��(�ǎ�莞�� ��T�b�~����)
	read loop		�A���ǎ��(�������[�v�A���A�̓��Z�b�g)
	read simple		�V���v�����[�h�Ŏ�M(�\��mode=AEHA/NEC/SIRC��ݒ肷��K�v������)
	send [1-9]		�����R���M���̑��M(���M��)
	send simple		�V���v�����[�h�ő��M(�^�C�}�[���g�p�����Ƀf�[�^����)
	put [1-12]		�ǂݎ�����M�������W�X�^�i�q�`�l�j�ɓo�^�i�w�K�j
	[get] [1-12]	���W�X�^�ɓo�^���ꂽ�M���𕜌����đ��M�uget�͏ȗ��\�v
	display			�M���̕\��
	input={0xNN...}	�f�[�^���� input={0xAA,0x5A,...} �̂悤�ɓ��͂���
	mode=[mode]		���[�h�ݒ�(AEHA/NEC/SIRC)�^���[�h���̏ȗ��Őݒ�m�F
	freq=[10-89] 	�ԊO��38kHz(AEHA)�����g��10�`80kHz�ɐݒ�^���l�ȗ��Őݒ�m�F
	tau=[10-89]		�ԊO��38kHz(AEHA)�����g�̃f���[�e�B�[(tau)��10�`80���ɐݒ�
	width=[100-890]	�ϒ��P�ʂ�100�`800us�ɐݒ�(10us�P��)�^���l�ȗ��Őݒ�m�F
	length=[10-99]	�R�[�h��(�r�b�g��)��ύX�^���l�ȗ��Őݒ�m�F
	cal [0-9]		�L�����u���[�V����(0�ŕ␳�Ȃ�)�^�����Ȃ��ōZ���l�̊m�F
	edit			�f�[�^�ҏW�p�G�f�B�^�̋N��(������)
	quit			�V���A�����o�͂��Ȃ��P�ƃe�X�^�[���[�h�Ɉڍs(���A�̓��Z�b�g)

	���P�ƃe�X�^�[�E�R�}���h

	�{�^���������������(SW1=H)�ŋN��(�������̓��Z�b�g)����ƒP�ƃe�X�^�[�Ƃ���
	�g�p�ł��܂��B
	Panasonic��BD���R�[�_�[�̃����R��(1)�̂S�F�{�^���ŃR�}���h����͂��܂��B
	�S�F�{�^���������āA�኱�A�x��Ă��珈�����J�n����܂��B
	
	�F�{�^��	send ���M
	�ԐF�{�^��	read �ǎ��
	�ΐF�{�^��	disp �\��(�t���̂�)
	���F�{�^��	loop �A���ǎ��(���M�Ȃ�)
	�����{�^��	get  ���W�X�^�̐M���𑗐M
	
	���펞read�҂��ɂȂ��Ă��܂��̂ŁA�ԃ{�^���������Ȃ��Ă��ǂݎ��܂��B

	���L�����u���[�V�����ɂ���

	���N������cal�����{���Ă���̂Œʏ�͍Ď��{����K�v�͂���܂���B
	�@�������A����cal�̊ۂߌ덷�̊֌W�œ��삵�Ȃ��@�킪����܂��B
	�@������0�ɂ���Ɛݒ菉���l�����ɏ���cal�Ɠ���������s���܂��B
	�@������1�ɂ���ƌ��݂̒l�����ɕ␳���܂��B
	�@2�`9�̈����ɂ�cal�l��x�������ɕ␳�ł��܂��B

	�����m�̕s��܂��͐�������
	�E��M�f�[�^���̎��ԕ\���̐��x��4ms�O��̌덷�������Ă��܂��B
	�ESIRC��read loop�ŘA���ǂݎ����s����AEHA�Ɣ��f���Ă��܂��܂��B
	�E�P�ƃe�X�^�[���[�h�ő��M����ƁA�M���������x���B
	�@(�����̉��P�͍s���܂��������I�Ƀ[���ɂ͏o���܂���)
	�ESYNC Duration��SYNC Counts��ON:OFF���Ԕ䂪�����ɂȂ�Ȃ�
	�@(SYNC Duration�͌��o�x����␳���Ă��܂����ACounts�͕␳���Ă��Ȃ�)

	���J�����Ɋւ��钍�ӓ_
	�E������Ѓx�X�g�e�N�m���W�[��GCC Developer Lite���g�p���܂��B
		http://www.besttechnology.co.jp/
	�E�Y�Ѓz�[���y�[�W�́u�i���b�W�x�[�X�v���_�E�����[�h�ł��܂��B
		http://www.besttechnology.co.jp/modules/knowledge/?GCC%20Developer%20Lite
	�E���̊J�����Ɋ܂܂��H8�}�C�R���p�̃��C�u�������g�p���Ă��܂��B
	�E�����̃t�@�C���������Ɉ����܂���̂ňȉ��̂悤�ɍH�v���Ă��܂��B
		�E�S�Ẵ\�[�X��include�ŕ
		�Emain.c�ɏ����ݒ�p�����[�^�𓝊�
		�E�ꕔ�̃\�[�X�ɂ�include�Ƀt���O��t�����ďd��inculde��h�~
	�E�����̗��p���@
		�Emain.c���J�����ŊJ��
		�E���̑��̃\�[�X�̓e�L�X�g�G�f�B�^�ŊJ��
		�E�R���p�C���̓s�x�A�S�\�[�X���R���p�C������܂�

	���p���`
	�E���̃v���O�������g�������i�͔̔��ł��܂����H
	�@�������ɒ��쌠�̂��镔���̎g�p�ɂ͖�肠��܂���B
	�E�\�[�X�R�[�h��ҏW���Ĕz�z���Ă����܂��܂��񂩁H
	�@�������ɒ��쌠�̂��镔���̍Ĕz�z�ɂ͖�肠��܂���B
	�E�@�\�ǉ�����肢������
	�@���ꍇ�ɂ���Ă͑Ή����܂����A�S�Ăɂ͑Ή��ł��܂���B
	�E�����R���R�[�h��񋟂���̂Œǉ����ė~����
	�@���ꍇ�ɂ���Ă͑Ή����܂����A�S�Ăɂ͑Ή��ł��܂���B
	�Eread�̔��΂�write�ŁAsend�̔��΂�receive�ł��B
	�@��send��receive�̑g�ݍ��킹�œ��͂��Ă��������Ă����\�ł��B

	���Q�l����
	�E�g�W�}�C�R���֘A
		3664F,3694F�Ŏn�߂�H8
			http://www9.plala.or.jp/fsson/NewHP_elc/Link_nH8.html
		������Ѓx�X�g�e�N�m���W�[
			http://www.besttechnology.co.jp/
		������Ё@�H���d�q�ʏ�
			http://akizukidenshi.com/
		������Ѓ��l�T�X�e�N�m���W�[
			http://japan.renesas.com/
	�E�ԊO�������R������
		Remote Control with PIC
			http://einst.hp.infoseek.co.jp/Remocon2/Remocon2.html
		�ԊO���w�K�����R���̐M����`�f�[�^�̍���
			http://www.geocities.jp/shrkn65/remocon/
		NEC�t�H�[�}�b�g�̐ԊO�������R���E�t�H�[�}�b�g
			http://www.necel.com/ja/faq/mi_com/__com_remo.html
	
	���ӎ�
	�E�����T�C�g���䗘�p���������Ă�����X���n�߁A
	�@�����̕��X�Ɏx�����ĊJ�����邱�Ƃ��o���܂����B
	�@�A�t�B���G�C�g�Ō�����ɋ��͂��������������X�A
	�@�Q�l�����̎��������J����Ă�����X�A
	�@�L�����N�^�t���̕\�������̃\�[�X��񋟂���������nak����A
	�@�����͂����������܂������X�ɁA�S��芴�ӂ̈ӂ�\���܂��B

	���ύX����(�T��)
	�EV1.01�F��M�f�[�^���������Ȃ�̂��C��
	�EV1.06�FCAL���Ƀm�C�Y����M���̃n���O�A�b�v�h�~
*/

// �w�b�_�t�@�C��
#include "lib/ir_data.h"		// �����R���f�[�^����
#include "lib/ir_read_simple.h"	// �^�C�}�[�s�g�p�̓ǎ��
#include "lib/ir_send_simple.h"	// �^�C�}�[�s�g�p�̑��M
#include "lib/ir_display.h"		// �\���֘A�A�������E�F�C�g������

/* make */

// �w�b�_����\�[�X
#include "lib/ir_data.c"		// �����R���f�[�^����
#include "lib/ir_read_simple.c"	// �^�C�}�[�s�g�p�̓ǎ��
#include "lib/ir_send_simple.c"	// �^�C�}�[�s�g�p�̑��M
#include "lib/ir_display.c"		// �\���֘A�A�������E�F�C�g������

// �w�b�_�Ȃ��\�[�X(���ύX����)
#ifndef IR_ADC_MODE
	#include "lib/ir_read.c"		// �ʏ�̓ǎ��
#endif
#include "lib/ir_send.c"		// �^�C�}�[�g�p�̑��M
#include "lib/ir_cal.c"			// �L�����u���[�V����
#define MAIN
#ifdef IR_ADC_MODE
	#include "lib/ir_adc_read.c"	// �ԊO���k�d�c�ǎ��
#endif

/* ���C�� */
void ir_tester(void){
	byte data[13][DATA_SIZE],backup[DATA_SIZE];
	// �e���ߓ��Ŏ��R�Ɏg����ϐ�
	int i, adj, len, timeout;
	int data_len[13];
	// �e���ߊԂŋ��p����ϐ�
	int backup_len = 0;
	int id = 0;
	byte loop=0;
	char sci_tx[SCI_SIZE], sci_rx[SCI_SIZE];
	enum Ir_Mode mode[13];	mode[0]=NA;
	enum Ir_Mode backup_mode=NA;

	ir_port_init();						// �ėp���o�̓|�[�g�̏�����
	if( BUTN == BUTN_ON ) DIAG = 0;		// �{�^���������ċN������ƒP�ƃe�X�^�[���[�h
	if (DIAG) {
		ir_sci_init(sci_tx,sci_rx);		// �V���A��������
	}
	ir_led_init();						// LED������
	LED1_OUT=LED_OUT_ON;
	LED2_OUT=LED_OUT_ON;
	
	#ifdef LCD
	lcd_init();							// �t������h�b�̃v��������
	wait_millisec(15);
	lcd_init();							// �Q��ڂŏ����������
	lcd_control(1,0,0);					// �t������ �\��ON,�J�[�\��OFF,�_��OFF
	lcd_cls();							// �t������
	ir_lcd_init();						// �t��������(ir_display.c)
	#endif

	DI;	/*�����݋֎~*/
	sys_cal();
	millsec_cal();						// �~���b�̃L�����u���[�V����
	ir_aeha_cal(0);						// �V���{�����x�̃L�����u���[�V����
	ir_led_init();						// LED����
	EI;	/*�����݋���*/
	
	/*�v���Z�b�g�f�[�^�����W�X�^�֓ǂݍ���*/
	for( i=0; i<13 ; i++){
		for( len=0 ; len < (int)((IR_DATA[i].len)/8)+1 ; len++){
			data[i][len]	= *(IR_DATA[i].data+len);
			data_len[i]		= IR_DATA[i].len;
			mode[i]			= IR_DATA[i].mode;
		}
	}
	
	// ����
	while(1){
		#ifdef LCD
		lcd_goto(LCD_ROW_1);
		lcd_putstr("Ready   ");
		#endif
		sci_rx[0]='r';
		if( DIAG && BUTN == BUTN_OFF) {	// �P�ƃ��[�h��{�^��������Ԃł̓V���A�����͂��Ȃ�
			SCI3_PRINTF("\nReady\n> ");
			SCI3_IN_STRING (sci_rx, sizeof(sci_rx) - 1);	//�V���A������
		}
		for( i=1 ; i<sizeof(sci_rx)-2 ; i++){			// �X�y�[�X���C�R�[���ŋ�؂�ꂽ�Q�����𒊏o
			if( sci_rx[i]==' ' || sci_rx[i]=='=' ){
				sci_rx[1]=sci_rx[i+1];
				sci_rx[2]=sci_rx[i+2];
				i=sizeof(sci_rx)-2;
			}else{
				sci_rx[1]=' ';
			}
		}
		if( DIAG == 0 ){				// �P�ƃ��[�h
			if( id >= 13 && id <= 20){
				switch( id ){
					case 13:	case 17:	//�{�^��
						sci_rx[0]='s';	sci_rx[1]=' ';						// ���M���ߔ��s
						for(i=0;i<DATA_SIZE;i++) data[0][i]=backup[i];		// �o�b�N�A�b�v�ŏ㏑������
						data_len[0]=backup_len;	mode[0]=backup_mode;
						break;
					case 14:	case 18:	//�ԃ{�^��
						sci_rx[0]='r';	sci_rx[1]='3';	sci_rx[2]=' ';	// ��M���ߔ��s
						break;
					case 15:	case 19:	//�΃{�^��
						sci_rx[0]='d';	sci_rx[1]=' ';						// �\�����ߔ��s
						for(i=0;i<DATA_SIZE;i++) data[0][i]=backup[i];		// �o�b�N�A�b�v�ŏ㏑������
						data_len[0]=backup_len;	mode[0]=backup_mode;
						break;
					case 16:	case 20:	//���{�^��
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
				while( IR_IN == IR_IN_OFF );	//�ԊO�����͑҂�(SIRC�͖���)
				*/
				sci_rx[0]='r';	sci_rx[1]='0';	sci_rx[2]=' ';
			}
		}
		/* �R�}���h��� */
		switch( sci_rx[0] ){							// �R�}���h�͐擪�̂P�����݂̂Ŕ��f����
			/* PUT ���W�X�^�o�^�R�}���h */
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
						SCI3_PRINTF("Usage: put [1�`12(Register)]\n");
					}
				}
				break;
			/* GET ���W�X�^���A�����M�R�}���h */
			case '1':	case '2':case '3':	case '4':	case '5':
			case '6':	case '7':	case '8':	case '9':		// 10�`12�͑Ή����Ȃ�
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
					/*���M*/
					LED1_OUT=LED_OUT_ON;
					DI;	/*�����݋֎~*/
					ir_send(data[0],data_len[0],mode[0]);     // �ԊO���o�́i�^�C�}�[�g�p�j
					EI;	/*�����݋���*/
					LED1_OUT=LED_OUT_OFF;
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: get [1�`12(Register)]\n");
					}
				}
				break;
			/* READ �ǎ��R�}���h */
			case 'r':	case 'R':
				if( DIAG ) SCI3_PRINTF("read\n");
				timeout=1;	loop=0;
				if( sci_rx[1] >= '1' &&  sci_rx[1] <= '9'){				//�����͂P���̐����݈̂���
					timeout=(unsigned int)sci_rx[1]-(unsigned int)'0';
					if( DIAG ) SCI3_PRINTF("timeout=%d\n",timeout);
				}
				if( sci_rx[1] == 'l' || sci_rx[1] == 'L'){
					if( DIAG ) SCI3_PRINTF("loop mode\n");
					loop=1;											// �������[�v�ݒ�
				}
				if( sci_rx[1] == '0' ) timeout=32767;
				#ifdef LCD
				lcd_goto(LCD_ROW_1);
				lcd_putstr("IR wait ");
				#endif
				for(i=0;i<DATA_SIZE;i++){							// �^�C���A�E�g��G���[�ɔ����ăf�[�^�ۑ�
					backup[i]=data[0][i];
					data[0][i]=0;
				}
				backup_len=data_len[0];
				backup_mode=mode[0];
				while( timeout > 0 || loop){
					LED2_OUT=LED_OUT_ON;
					len = -1;
					while( len == -1){											// Len=-1�̓G���[�B�u����read�ĊJ
						if( sci_rx[1] == 's' || sci_rx[1] == 'S' ){
							// #ifdef IR_READ_S_H
							len = ir_read_simple(data[0],mode[0]);				// len�̓f�[�^���B0��read�^�C���A�E�g
							// #endif
						}else{
							#ifdef IR_ADC_MODE
							ir_adc_init(0);
							len = ir_adc_read(data[0], IR_READ_TIME_OUT,&mode[0]);
							#else
							len = ir_read(data[0], IR_READ_TIME_OUT,&mode[0]);	// len�̓f�[�^���B0��read�^�C���A�E�g
							#endif
						}
					}															// timeout���f�N�������g���ĉ񐔂����炷
					LED2_OUT=LED_OUT_OFF;
					if( len == 0){
						if( DIAG ) SCI3_PRINTF("no signal\n");
						timeout--;
						#ifdef LCD
						lcd_goto(LCD_ROW_2);
						lcd_putstr("NoSignal");
						#endif
					} else {										// len��0�łȂ�(�^�C���A�E�g��G���[�łȂ�)
						data_len[0]=len;							// data_len�̓f�[�^���̊m��
						id = ir_search(data[0], data_len[0], mode[0]);
						if( DIAG ) {
							ir_sci_disp(data[0], data_len[0]);  			// �V���A���֕\��
							if( id >= 0 ) SCI3_PRINTF("ID=%d %s\n",id,IR_DATA[id].name );
						}
						#ifdef LCD
						if( id >= 0 ){								// ID������΂h�c��\��
							ir_lcd_disp_id(id);
						}
						if( id < 0 || id > 32 ) {
							ir_lcd_disp(data[0],data_len[0]);		// ID�������ꍇ��ȈՃR�}���h�łȂ��ꍇ�̓R�[�h��\��
						}
						#endif
						timeout=0;
					}
				}
				if(len==0){
					for(i=0;i<DATA_SIZE;i++) data[0][i]=backup[i];	// �^�C���A�E�g���Ƀo�b�N�A�b�v��߂�
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
						data_len=len;								// data_len�̓f�[�^���̊m��
						id = ir_search(data, data_len, mode[0]);
						if( DIAG ) {
							ir_sci_disp(data, data_len);  // �V���A���֕\��
							if( id >= 0 ) SCI3_PRINTF("ID=%d %s\n",id,IR_DATA[id].name );
							SCI3_PRINTF("\n");
						}
						#ifdef LCD
						ir_lcd_disp(data,data_len); 			// �t���֕\��
						if( id >= 0 ) ir_lcd_disp_id(id);
						#endif
					}
				}
				break;
			*/
			/* SEND ���M�R�}���h */
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
						ir_send_simple(data[0],data_len[0],mode[0]);     // �ԊO���o��(�V���v�����[�h)
						#endif
					}else{
						DI;	/*�����݋֎~*/
						ir_send(data[0],data_len[0],mode[0]);     // �ԊO���o�́i�^�C�}�[�g�p�j
						EI;	/*�����݋���*/
					}
					LED1_OUT=LED_OUT_OFF;
					#ifdef LCD
					ir_lcd_disp(data[0],data_len[0]); // �t���֕\��
					#endif
					#ifndef LCD
					wait_millisec(200);
					#endif
					timeout--;
				}
				break;
			/* DISPLAY �\���R�}���h */
			case 'd':	case 'D':
				if( DIAG ) SCI3_PRINTF("display\n");
				if( DIAG ) {
					ir_sci_disp(data[0], data_len[0]);
					ir_sci_disp_mode( mode[0] );
				}
				#ifdef LCD
					ir_lcd_disp(data[0],data_len[0]); 			// �t���֕\��
				#endif
				break;
			/* �f�[�^���̓R�}���h */
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
			/* MODE IR���[�h�̎w��R�}���h */
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
			/* �f�[�^�[��(�r�b�g���̕ύX) */
			case 'l':	case 'L':
				if( DIAG ) SCI3_PRINTF("length\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '9' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0');
					if( DIAG ) SCI3_PRINTF("length = %d -> %d [%%]\n",data_len[0],len);
					data_len[0]=len;
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: length=[10�`99]\nCurrent length =%d",data_len[0]);
					}
				}
				break;
			/* �ԊO��38kHz�̃f���[�e�B�[�ύX */
			case 't':	case 'T':
				if( DIAG ) SCI3_PRINTF("tau (modulation duty cycle)\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '8' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0');
					if( DIAG ) SCI3_PRINTF("tau = %d -> %d [%%]\n",100-FREQ_TAU,len);
					FREQ_TAU=100-len;
					DI;	/*�����݋֎~*/
					sys_cal();
					EI;	/*�����݋���*/
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: tau=[10�`80]\nCurrent tau =%d",100-FREQ_TAU);
					}
				}
				break;
			/* �ԊO��38kHz�̎��g���ύX */
			case 'f':	case 'F':
				if( DIAG ) SCI3_PRINTF("frequency\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '8' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0');
					switch( mode[0] ){
						case AEHA:
							if( DIAG ) SCI3_PRINTF("FREQ_AEHA = %d -> %d [kHz]\n",FREQ_AEHA,len);
							FREQ_AEHA = len;
							DI;	/*�����݋֎~*/
							sys_cal();
							EI;	/*�����݋���*/
							break;
						case NEC:
							if( DIAG ) SCI3_PRINTF("FREQ_NEC  = %d -> %d [kHz]\n",FREQ_NEC,len);
							FREQ_NEC = len;
							DI;	/*�����݋֎~*/
							sys_cal();
							EI;	/*�����݋���*/
							break;
						case SIRC:
							if( DIAG ) SCI3_PRINTF("FREQ_SIRC = %d -> %d [kHz]\n",FREQ_SIRC,len);
							FREQ_SIRC = len;
							DI;	/*�����݋֎~*/
							sys_cal();
							EI;	/*�����݋���*/
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
						SCI3_PRINTF("Usage: frequency=[10�`80]\n");
						ir_cal_disp_freq();
						ir_sci_disp_mode( mode[0] );
					}
				}
				break;
			/* �ϒ���{�P�ʒ��̕ύX */
			case 'w':	case 'W':
				if( DIAG ) SCI3_PRINTF("width\n");
				if( sci_rx[1] >= '1' && sci_rx[1] <= '8' && sci_rx[2] >= '0' &&  sci_rx[2] <= '9'){
					len=((sci_rx[1]-(unsigned int)'0')*10+sci_rx[2]-(unsigned int)'0')*10;
					switch( mode[0] ){
						case AEHA:
							if( DIAG ) SCI3_PRINTF("SYMBOL_AEHA = %d -> %d [us]\n",SYMBOL_AEHA,len);
							SYMBOL_AEHA = len;
							DI;	/*�����݋֎~*/
							sys_cal();
							EI;	/*�����݋���*/
							break;
						case NEC:
							if( DIAG ) SCI3_PRINTF("SYMBOL_NEC  = %d -> %d [us]\n",SYMBOL_NEC,len);
							SYMBOL_NEC = len;
							DI;	/*�����݋֎~*/
							sys_cal();
							EI;	/*�����݋���*/
							break;
						case SIRC:
							if( DIAG ) SCI3_PRINTF("SYMBOL_SIRC = %d -> %d [us]\n",SYMBOL_SIRC,len);
							SYMBOL_SIRC = len;
							DI;	/*�����݋֎~*/
							sys_cal();
							EI;	/*�����݋���*/
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
						SCI3_PRINTF("Usage: width=[100�`800(10)]\n");
						ir_cal_disp_symbol();
						ir_sci_disp_mode( mode[0] );
					}
				}
				break;
			/* CAL �L�����u���[�V�����E�R�}���h */
			case 'c':	case 'C':
				if( DIAG ) SCI3_PRINTF("calibration\n");
				adj=0;
				if( sci_rx[1] >= '0' &&  sci_rx[1] <= '9'){
					adj=(unsigned int)sci_rx[1]-(unsigned int)'0';
					if( adj && DIAG ) SCI3_PRINTF("cal adj=%d\n",adj);
					DI;	/*�����݋֎~*/
					sys_cal();
					ir_aeha_cal(adj);	
					EI;	/*�����݋���*/
				}else{
					if( DIAG ){
						SCI3_PRINTF("Usage: calibration=[0�`9(0=no adjust)]\n");
						ir_cal_disp();
					}
				}
				break;
			/* QUIT �P�ƃe�X�^�[���[�h�ֈڍs����R�}���h */
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
