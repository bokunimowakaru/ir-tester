/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/
/*
������̊T�v

	SYNC_H	L->H��Ɏ��g����͂̂��߂ɖ�1ms�̃f�[�^���擾
			�擾��ɁA���ϒl�A�s�[�N�_����́i0.5ms���x!?�j
	SYNC_L	H->L�̑ҋ@
	DATA_H	�����Ń��[�h�̔�����s���Ă���B0.5ms���x(1T�M���M��)
	DATA_L	H->L�̑ҋ@
	
	Ver 0.15

��������
	�����x���[�h

�����e�X�g
	IR�Z���T�̃o�����ɂ�銴�x����

�������̕s�

	��SIRC�̓ǂݎ�肪�s���� (Ver 0.21)
	
*/

/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/
#ifndef NAME
	#define	NAME	"IR-ADC-ir_adc_read"
	#define	VERSION	"0.27-?"
#endif
#ifndef TYPE
	#define	TYPE "�ߋ���LED�ݻ�p"
#endif
//#define   TEST		// �e�X�g����define����(���O�\��)
//#define   TEST2		// �e�X�g����define����(���g���e�X�g�p)

/* ���Ő錾����Ă��邩������Ȃ����� */
#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>
#endif
#ifndef SYS_CLOCK
	#define		SYS_CLOCK	16				// 3664F=16MHz 3694F=20MHz
	float sys_clock = SYS_CLOCK*1000000;	// �����p[Hz] �L�����u���[�V�����Ή��̂��߂�float�Ő錾
#endif
#ifndef BYTE_TYPE
	typedef unsigned char byte;
#endif
#ifndef DATA_SIZE
  #define		DATA_SIZE	16				// �f�[�^��(byte),4�̔{��
#endif
#ifndef IR_READ_TIME_OUT
  #define		IR_READ_TIME_OUT	5		// IR���̓^�C���A�E�g[�b]
#endif
#ifndef TIMER_A_DEF                /* TIMER_A ���荞�ݏ��� */
  #define TIMER_A_DEF
  unsigned int TIMER_SEC		=	0	;	// �q�s�b�J�E���g�p�O���[�o���ϐ�
  void int_timera(void){ 
    IRR1.BIT.IRRTA=0;				      /* �^�C�}�[A�����t���O */
    TIMER_SEC++;
  }
#endif
#ifndef LED1_OUT
	#define		LED1_OUT	IO.PDR1.BIT.B6	// �e�X�g�pLED�̐ڑ��|�[�g
	#define		LED2_OUT	IO.PDR1.BIT.B7	// �e�X�g�pLED�̐ڑ��|�[�g
	#define		LED_OUT_OFF	0				// �e�X�gLED�񔭌����̏o�͒l
	#define		LED_OUT_ON	1				// �e�X�gLED�������̏o�͒l
	#define		IR_OUT		IO.PDR1.BIT.B4	// �ԊO��LED�̐ڑ��|�[�g
	#define		IR_OUT_OFF	0				// �ԊO��LED�񔭌����̏o�͒l
	#define		IR_OUT_ON	1				// �ԊO��LED�������̏o�͒l
	#define		IR_IN		IO.PDR1.BIT.B5	// �ԊO���Z���T�̐ڑ��|�[�g
	#define		IR_IN_OFF	0				// �ԊO���Z���T�������̓��͒l
	#define		IR_IN_ON	1				// �ԊO���Z���T������̓��͒l
#endif
#ifndef IR_PORT_INIT
void ir_port_init(){
	/*�|�[�g1         76543210*/
	IO.PMR1.BYTE  = 0b00000000;     // ���[�h(P1,5)     ���o��=0  ���̑�=1
	IO.PCR1       = 0b11010111;     // ���o�͐ݒ�       ����  =0  �o��  =1  B3=���U�[�u
	IO.PUCR1.BYTE = 0b00000000;     // �v���A�b�v(P1,5) ���Ȃ�=0  ����  =1
	IO.PDR1.BYTE  = 0b00000000;     // �A�N�Z�X         �k�o��=0  �g�o��=1
	
	IR_OUT=IR_OUT_OFF;				// �ԊO���̔������I�t
}
#endif
#ifndef DIAG
	#define DIAG	DIAG
	byte		DIAG	=	1;				// �f�f���[�h
#endif
#ifndef SCI_SIZE
	#define		SCI_SIZE	64				// �V���A���f�[�^��
#endif
#ifndef MILLISEC_WAIT
	#define MILLISEC_WAIT	MILLISEC_WAIT
	unsigned int MILLISEC_WAIT	=	564	;	// 1ms���̋󃋁[�v�����l
#endif
#ifndef IR_MODE_ENUM
	#define IR_MODE_ENUM
	enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// �ԊO�����[�h�̌^
#endif
#ifndef IR_READ_NOISE_DET
	#define	IR_READ_NOISE_DET	4			// 1/4�V���{����{�P�ʁi�s�j�ȉ����m�C�Y�Ƃ���
#endif
#ifndef IR_DISPLAY_H
	#include "ir_display4adc.h"
	#include "ir_display4adc.c"
#endif
#ifndef IR_CALC_H
	#include "ir_cal4adc.c"
#endif

/* IR_ADC��p ir_adc_read.h */

/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               https://bokunimo.net/bokunimowakaru/
*********************************************************************/
#define   IR_ADC_DATA_SIZE	128
#define   IR_ADC_LH			51		// L -> H ���背�x�� 1.0V (255/5*1.0)
#define   IR_ADC_HL			10		// H -> L ���背�x�� 0.2V (255/5*0.2)
#define   IR_ADC_NOISE		3
#define   IR_ADC_LPF_N		3		// ADC�m�C�Y����(���σt�B���^)

int ir_adc_init(int adc_port);
byte ir_adc_sens( void );
//byte ir_adc_sens_lpf( void );
void ir_adc_detect_timer_off(void);
int ir_adc_read(byte *data, int time_out, enum Ir_Mode *mode);

/* IR_ADC��p ir_adc_read.c */

/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

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
	    ADF �I�� ___|||||      
	   ADIE ���� ____||||_____ CKS  0:�ᑬ 1:����
	   ADST �J�n _____||______ SCAN 0:�P�� 1:�A��
	*/
	return( adc_port );
}

byte ir_adc_sens( void ){
	AD.ADCSR.BIT.ADST = 1;            //�ǂݎ��
	while(AD.ADCSR.BIT.ADF==0);       //�ǂݎ�芮���҂�
	return( AD.ADDRA >> 8 );
}

/* ���M���Ή��̂��߂̂k�o�e */
/*
byte ir_adc_sens_lpf( void ){
	IR_ADC_lpf -= (int)IR_ADC[IR_ADC_i];
	AD.ADCSR.BIT.ADST = 1;            //�ǂݎ��
	while(AD.ADCSR.BIT.ADF==0);       //�ǂݎ�芮���҂�
	IR_ADC[IR_ADC_i] = ( AD.ADDRA >> 8 );
	IR_ADC_lpf += (int)IR_ADC[IR_ADC_i];
	IR_ADC_i++;
	if( IR_ADC_i >= IR_ADC_LPF_N ) IR_ADC_i=0;
	return( (byte)(IR_ADC_lpf/(int)IR_ADC_LPF_N) );
}
byte IR_ADC[IR_ADC_LPF_N];			// �k�o�e�p�e�h�e�n���W�X�^
byte IR_ADC_i;						// �k�o�e�p�J�E���^
unsigned int IR_ADC_lpf;			// �k�o�e�p�e�h�e�n�̓������v
*/

void ir_adc_detect_timer_off(void){
	DI;		//���荞�݋֎~
	TW.TMRW.BIT.CTS =0; 					/* �^�C�}�[�v�񓮍� */
	IENR1.BIT.IENTA =0; 					/* �^�C�}�[�`�����݉��� */
	EI;		//���荞�݊J�n
}

int ir_adc_read(byte *data, int time_out, enum Ir_Mode *mode){
	int i;					// for�p
	int len=0;				// �V���{����
	byte freq_meas=0;		// ���g������̃T���v����(�N���b�N�ɂ���ĕς��)
	byte bit;				// �����J�E���g�p
	byte prev=0;			// �O��̃T���v���f�[�^
//	byte dc_level=0;		// �������x��(���M���̎�M�p)
//	unsigned short sum=0;	// ��͗p �T���v���̍��v�l 256*255 = int��OK
//	byte avr=0;				// ���g����͗p �T���v���̕��ϒl
	byte max_c=0;			// ���g����͗p�@�ő�s�[�N���̃J�E���g�p
//	byte min_c=0;			// ���g����͗p�@�ŏ��s�[�N���̃J�E���g�p
//	byte max=0;				// ���g����͗p�@�ő�s�[�N�l�̕ێ��p
//	byte min=255;			// ���g����͗p�@�ŏ��s�[�N�l�̕ێ��p
	byte first=0;			// ���g����͗p�@�s�[�N�̍ŏ��̃T���v���ԍ�
	byte last=0;			// ���g����͗p�@�s�[�N�̍Ō�̃T���v���ԍ�
	byte rise=1;			// ���g����͗p�@�M���̏㏸�����~�����̃t���O
	byte adc_data[IR_ADC_DATA_SIZE];	// ���g������̃T���v�����O�p
	byte det;				// ���o�ɐ�(H�����o=SIRC��L�����o=AEHA/NEC)
	unsigned short	in;		// ���͒�
	int symbol_rate=0;		// ���ʕ\���p
	
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
	/* �e�X�g�p�^�C�}�[ */
	#ifdef TEST
	unsigned short	timer1;
	unsigned short	timer2;
	unsigned short	timer3;
	#endif
	unsigned short	sync_H;			// SYNC�V���{���g��
	unsigned short	sync_L;			// SYNC�V���{���k��
	unsigned short	symbol_len;		// �V���{����{�P��
	unsigned short	noise_len;		// �m�C�Y���蒷
	unsigned short	data_wait;		// �I�����蒷
	unsigned short	in_len;
	unsigned short	in_H;
	unsigned short	in_L;
	
	float wave_us;
	float freq_hz;
	float data_ms;
	float total_ms;

	/* LPF������ */
	/*
	for( i=0 ; i < IR_ADC_LPF_N ; i++ ) IR_ADC[i]=0;
	IR_ADC_i=0;
	IR_ADC_lpf=0;
	for( i=0 ; i < IR_ADC_LPF_N ; i++ )	dc_level = ir_adc_sens();
	*/

	i = (int)( sys_clock / 1000000 * (1000/5.7)/20 +.5);	// 16MHz:7.1us(140)  20MHz 5.7us(175)
	//                                ~~~~                     ���[us]
	if( i > IR_ADC_DATA_SIZE-1 ){
		freq_meas =IR_ADC_DATA_SIZE-1;
	}else{
		freq_meas = i;
	}
	bit=freq_meas;
	
	if( DIAG ) SCI3_PRINTF("listening...(%dsec.)\n",time_out);

	wait_millisec(50);

	DI;		//���荞�݋֎~
	TA.TMA.BYTE 	=0b00001000;	/* �^�C�}�[�`�ݒ� */
	IRR1.BIT.IRRTA	=0;				/* �^�C�}�[A�����t���O */
	IENR1.BIT.IENTA =1; 			/* �^�C�}�[A�����ݒ� */
	IO.PDR8.BYTE    =0b00000000;	// �A�N�Z�X         �k�o��=0  �g�o��=1
	TW.TCRW.BYTE	=0b00110000;	/* �^�C�}�[�v�ݒ� �N���b�N���^�W (2.5MHz=0.4us) */
	TW.TMRW.BIT.CTS =0; 			/* �^�C�}�[�v�񓮍� */
	EI;		//���荞�݊J�n

	TIMER_SEC = 0;
	TW.TCNT = 0;						/* �^�C�}�[�v���Z�b�g*/
	TW.TMRW.BIT.CTS=1;					/* �^�C�}�[�v�J�E���g�J�n*/

	/* �f�[�^�҂�*/
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
			return(0);					// �^�C���A�E�g�ɂ�郊�^�[���́u�O�v
		}
	}
	timer_start = TW.TCNT;			//���ԋL�^
	timer_a_start = TA.TCA;
	#ifdef TEST
	LED1_OUT = LED_OUT_OFF;
	LED2_OUT = LED_OUT_ON;
	#endif
	
	/* ���g������p(�Ȃ�ׂ������ɓ���) �f�[�^�͔��΂���(data[freq_meas-1]�`data[0])*/
	// ib=freq_meas;
	while(bit){				// 			3639/256 5.7us 
		AD.ADCSR.BIT.ADST = 1;            //�ǂݎ��			29114(20MHz) 5.7us
		bit--;
		while(AD.ADCSR.BIT.ADF==0);       //�ǂݎ�芮���҂�
		adc_data[bit] = AD.ADDRA>>8;	  //�ǂݎ���8�s�b�g
	}
	timer_freq = TW.TCNT;
	timer_freq -= timer_start;	// short�Ȃ̂ŒP�ƌv�Z����Ό����l�����Ȃ��ėǂ�

	/* �f�[�^�v�Z�i�����A�x���Ă��ǂ��E�o�ߎ��Ԃ̓^�C�}�[����j */
	// max=0, min=1024, max_c=0,min_c=0
	/* ���ϒl�̌v�Z 
	for( bit = freq_meas-1 ; bit > 0 ; bit--){
		sum += adc_data[bit];			// sum��long
	}
	avr = (byte)(sum/(unsigned short)(freq_meas));
	*/

	/* �s�[�N�_�̃J�E���g */
	
	if( adc_data[freq_meas-2] < adc_data[freq_meas-1] ){
		rise = 0;
	}else{
		rise = 1;
	}
	for( bit = freq_meas-1 ; bit > 0 ; bit--){
		if( rise ){ 							// ��蒆
			if(  adc_data[bit-1] < adc_data[bit] ){
			//				��			����		// ���͉���ɂȂ�
				rise =0;
				max_c++;
				if( first == 0 ) first = bit;
				last = bit;
			}
		} else {								// (rise == 0 ) ���蒆
			if( adc_data[bit-1] > adc_data[bit] ){
												// ���͏��ɂȂ�
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
		return(-1);					// �M���G���[�ɂ�郊�^�[���́u-1�v
	}

	/* H->L �҂� */
	timer_wait = (int)(13.5*256./1000.)+2;	// =5 13500us�̃^�C���A�E�g���Ԑݒ�
	prev=255;
	timer_a=TA.TCA;	
	#ifdef TEST
	timer1 = TW.TCNT;				// H->L�҂����ԑ���p
	LED1_OUT = LED_OUT_ON;
	#endif
	while( prev > IR_ADC_HL ){
		prev=ir_adc_sens();
		if( ( TA.TCA-timer_a ) > timer_wait ){
			ir_adc_detect_timer_off();	// ���ɐM�������o���Ă���̂Ń^�C���A�E�g�ł��G���[�Ɣ���
			#ifdef TEST
			SCI3_PRINTF("sync H->L timeout\n");
			// SCI3_PRINTF("timer=%d first=%d last=%d avr=%d sample=%d max_count=%d\n",timer_freq,first,last,avr,freq_meas,max_c);
			#endif
			return(-1);					// �M���G���[�ɂ�郊�^�[���́u-1�v
		}
	}
	timer_sync_H = TW.TCNT;
	#ifdef TEST
	timer1 = timer_sync_H - timer1;	// H->L�҂����Ԃ̌���(���Ȃ��Ɗ�Ȃ�)
	timer2 = timer_sync_H;
	LED1_OUT = LED_OUT_OFF;
	#endif
//	timer_wait = (unsigned short)(26*sys_clock/8000000);//  26us = 38kHz	
//	while( TW.TCNT-timer_sync_H < timer_wait);		//	26us�̑҂����ԁi L �m��҂��j
	timer_sync_H -= timer_start;	// short�Ȃ̂ŒP�ƌv�Z����Ό����l�����Ȃ��ėǂ�
//	timer_sync_H -= timer_freq;
	sync_H = (int)((8000000./sys_clock)*(float)(timer_sync_H)+0.5);

	/* L->H �҂� */
	timer_wait = (int)(6.75*256./1000.)+2;	//=3 6750us�̃^�C���A�E�g���Ԑݒ�
	prev=0;
	#ifdef TEST
	LED1_OUT = LED_OUT_ON;
	#endif
	while( prev < IR_ADC_LH ){
		prev=ir_adc_sens();
		if( ( TA.TCA-timer_a ) > timer_wait ){
			ir_adc_detect_timer_off();	// ���ɐM�������o���Ă���̂Ń^�C���A�E�g�ł��G���[�Ɣ���
			#ifdef TEST
			SCI3_PRINTF("sync L->H timeout\n");
			// SCI3_PRINTF("timer=%d first=%d last=%d avr=%d sample=%d max_count=%d\n",timer_freq,first,last,avr,freq_meas,max_c);
			#endif
			return(-1);					// �M���G���[�ɂ�郊�^�[���́u-1�v
		}
	}
	timer_sync_L = TW.TCNT;
	#ifdef TEST
	timer3 = timer_sync_L;
	timer2 = timer_sync_L - timer2;				// L->H �҂�����
	LED1_OUT = LED_OUT_OFF;
	#endif
	timer_symbol   = timer_sync_L;
	timer_sync_L -= timer_start;	// short�Ȃ̂ŒP�ƌv�Z����Ό����l�����Ȃ��ėǂ�
//	timer_sync_L -= timer_freq;
	timer_sync_L -= timer_sync_H;
	sync_L = (unsigned short)((8000000./sys_clock)*(float)(timer_sync_L)+0.5);

	/* ���[�h���� */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5�~16 �� 9000us	6750us�ȏ�łm�d�b�Ɣ���*/
	/* �f�[�^�m�F (SYNC���̃m�C�Y�G���[�����o) */
	/*	AEHA	H(8T) + L(4T)	2:1
		NEC		H(16T) + L(8T)	2:1
		SIRC	H(4T) + L(1T)	4:1	*/
	/* NEC typ 562.5�~16 �� 9ms�ȏ�
	           488us�~16 �� 7.808ms  (Clock 2�� = 7.813ms)�Ŕ���*/

	if( sync_H>6750 && sync_H > sync_L ){	// 7.8ms�ȏ��SYNC_OFF���Z��
		det=0;					// NEC
		symbol_len = timer_sync_L/4;			// NEC symbol_len 8T/4= 2T
		symbol_rate = 8000000/(unsigned short)(sync_H+sync_L);
		if( sync_H > sync_L * 4 ){				 // SYNC_OFF���ɒ[�ɒZ��
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
		if( sync_H < sync_L || sync_H > sync_L * 4 ){	// SYNC_ON��SYNC_OFF���ُ�ɒZ��
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
		if( sync_H < sync_L ){				 	// SYNC_ON���ɒ[�ɒZ��
			ir_adc_detect_timer_off();
			#ifdef TEST
			SCI3_PRINTF("SIRC sync_H<sync_L\n");
			SCI3_PRINTF("SYNC Duration= %d count (ON: %d count)/(OFF: %d count)[TEST]\n",timer_sync_H+timer_sync_L,timer_sync_H,timer_sync_L);
			#endif

			return(-1);
		}
		*mode = SIRC;
	}

	noise_len = symbol_len / IR_READ_NOISE_DET / 2;		// T��IR_READ_NOISE_DET�܂�
	data_wait = 5*symbol_len/2;							// 5T�܂ő҂��I�����o

	#ifdef TEST
	timer3 = TW.TCNT - timer3;			// H��̔��莞��
	LED2_OUT = LED_OUT_OFF;
	#endif

	/* �M���ǎ�� */
	/*  
	
		BIT0	H(1T) + L(1T)
		SYMBOL	H(1T) + L(2T)	//���蒷
		BIT1	H(1T) + L(3T)
	
		BIT0	H(1T) + L(1T)
		SYMBOL	H(1T) + L(2T)	//���蒷
		BIT1	H(1T) + L(3T)
	
		BIT0	H(1T) + L(1T)
		SYMBOL	H(1.5T) + L(1T)	//���蒷
		BIT1	H(2T) + L(1T)
	*/
	timer_wait = 2;				// 3.9�`7.8ms�̃^�C���A�E�g���Ԑݒ�
	for(i=0;i<DATA_SIZE;i++){
		in = 0;
		for(bit=0;bit<8;bit++){
			
			/* H->L �҂� */
			timer_a=TA.TCA;	
			while( prev > IR_ADC_HL ){
				prev=ir_adc_sens();
				if( ( TA.TCA-timer_a ) > timer_wait ){
					ir_adc_detect_timer_off();	// �g�ɂȂ����܂܂͍l�����Ȃ��̂ňُ�
					#ifdef TEST
					SCI3_PRINTF("data read H->L timeout\n");
					#endif
					*mode = ERROR;
					return(-1);					// �M���G���[�ɂ�郊�^�[���́u-1�v
				}
			}
			in_len = TW.TCNT;
			timer_a_end=TA.TCA;					// �f�[�^�I���ɔ����ă^�C�}�[�l��ۑ�
			in_H   = in_len - timer_symbol;
			timer_total += in_H;
			timer_symbol = in_len;
			#ifdef TEST
			LED1_OUT = LED_OUT_OFF;
			#endif
							
			/* L->H �҂� */
			timer_a=TA.TCA;	
			while( prev < IR_ADC_LH ){
				prev=ir_adc_sens();
				if( ( TA.TCA-timer_a ) > timer_wait ){
						prev=255;				// �g�ɂȂ�Ȃ�(�M���̏I���)
				}
			}
			in_len = TW.TCNT;
			in_L   = in_len - timer_symbol;
			timer_symbol = in_len;
			
			if( *mode == SIRC ){				// SIRC�̂Ƃ��͂g�̒����Ŕ��f
				in_len = in_H;
				#ifdef TEST
				LED1_OUT = LED_OUT_ON;
				#endif
			}else{								// AEHA��NEC�͂k�̒����Ŕ��f
				in_len = in_L;
			}
			if( in_len > noise_len && in_len < data_wait){	// �L���Ȃ�
				if( in_len < symbol_len ){				// �k��Ԃ��Z�� BIT0
					in = in>>1;
					in += 0;
				}else{									// �k��Ԃ����� BIT1
					in = in>>1;
					in += 128;
				}
				timer_total += in_L;
			}else{
				if( in_L >= data_wait ){		// OFF���Ŕ��f ��in_len����SIRC�̏ꍇ�ɔ��f�ł��Ȃ��Ȃ�B
				/* �I�������o�����ꍇ�̏���(�ʏ�̃��[�v���珜�O���邽�߂ɍēxif�������Ă���)  */
					in = in>>(8 - bit); 		// ��r�b�g�̃V�t�g
					data[i]=in;					// �f�[�^�ۑ�
					len = i * 8 + bit;			// �����ۑ�
					i = DATA_SIZE -1;			// ���[�v����(break)
					bit=7;						// ���[�v����(break)
					#ifdef TEST
					SCI3_PRINTF("symbol_len=%d, noise_len=%d EOF=(in_len=%d > data_wait=%d)\n",symbol_len,noise_len,in_len,data_wait);
					#endif
				} else {
				/* �m�C�Y�����o�����ꍇ�̏���(�ʏ�̃��[�v���珜�O���邽�߂ɍēxif�������Ă���) */
					ir_adc_detect_timer_off();
					#ifdef TEST
					SCI3_PRINTF("data data_wait timeout\n");
					SCI3_PRINTF("symbol_len=%d, noise_len=%d in_len=%d data_wait=%d\n",symbol_len,noise_len,in_len,data_wait);
					SCI3_PRINTF("SYNC Duration= %d count (ON: %d count)/(OFF: %d count)[TEST]\n",timer_sync_H+timer_sync_L,timer_sync_H,timer_sync_L);
					#endif
					*mode = ERROR;
					return(-1);						// �G���[�Ȃ̂ŏu����read�ĊJ�ł���悤�ɂ���
				}
			}
		}
		data[i]=in;
	}

	/*�^�C�}�[�I�t*/
	ir_adc_detect_timer_off();
	
	wave_us = (8000000./sys_clock)*(float)timer_freq*(float)(first-last-1)/(float)freq_meas/(float)(max_c-1);
	freq_hz = 1000000./wave_us;
	data_ms  = (8000./sys_clock)*(float)(timer_total);
	total_ms = (8000./sys_clock)*(float)(timer_sync_H+timer_sync_L+timer_total);
	timer_a = timer_a_end - timer_a_start;

	/* ���ʕ\�� */
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

/* �P�̃e�X�g�p�\�[�X */
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
		ir_sci_init(sci_tx,sci_rx);		// �V���A��������
	}
	DI;	/*�����݋֎~*/
	sys_cal();
	millsec_cal();						// �~���b�̃L�����u���[�V����
	EI;	/*�����݋���*/

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
