/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/
/*
�Ɠd���i����(AEHA)����

	�����g		��940nm
	�������g	min. 33kHz	30.3us H(15.1us)+L(15.2us)
				typ. 38kHz	26.3us H(13.1us)+L(13.2us)
				max. 40kHz	25.0us H(12.5us)+L(12.5us)
	�ϒ��P�ʂs	min. 350us	
				typ. 470us	( H(13.0us)+L(13.1us) ) �~18
				max. 500us	

	SYNC	H(8T) + L(4T)
	BIT0	H(1T) + L(1T)
	SYMBOL	H(1T) + L(2T)	//���蒷
	BIT1	H(1T) + L(3T)
	STOP	H(1T)
	REPEAT	L(>8000us��) + SYNC�`STOP(����R�[�h���M)
											��������70�`80ms�ȉ�

	FORMAT:
	SYNC + Vendor(16) + Parity(4) + Sys(4) + data(8�`48) + STOP

	data() = ID(8) + code(8) + parity(8)

		(LSB First) C0 C1 C2 C3 C4 C5 C6 C7 (MSB Last)
		
NEC����(����)

	�����g		��950nm
	�������g	typ. 38kHz	26.3us H(8.8us)+L(17.5us)
	�ϒ��P�ʂs	562.5us		( H(8.8us)+L(17.5us) ) �~21 + H(8.8us)

	SYNC	H(16T) + L(8T)
	BIT0	H(1T) + L(1T)
	SYMBOL	H(1T) + L(2T)	//���蒷
	BIT1	H(1T) + L(3T)
	STOP	H(1T)
	REPEAT	L(4T) + H(1T)�̌J��Ԃ��i�R�[�h�͑���Ȃ��j

	FORMAT:
	SYNC + Custom Code (16bits) 
		+ Data Code (8bits) + Data Code' (8bits) 
		
		(LSB First) C0 C1 C2 C3 C4 C5 C6 C7 (MSB Last)

SONY(SIRC)����

	�������g	40kHz
	�ϒ��P�ʂs	600us

	SYNC	H(4T) + L(1T)
	BIT0	H(1T) + L(1T)
	SYMBOL	H(1.5T) + L(1T)	//���蒷
	BIT1	H(2T) + L(1T)
	STOP	L(75T-sum)		// 75T=45ms
	REPEAT	45ms���ɓ���R�[�h�𑗐M(�P��{�^���ł�3�񃊃s�[�g)

�Q�l����
	http://einst.hp.infoseek.co.jp/Remocon2/Remocon2.html
	http://www.geocities.jp/shrkn65/remocon/
	http://www.necel.com/ja/faq/mi_com/__com_remo.html

�R�[�h�m�F

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
	
	����	TV	40:BF:1A:E5
				c     d  d'(���])
*/

#define BYTE_TYPE
typedef unsigned char byte;

#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>             // 3664F��3694F����I������
#endif
#ifndef SYS_CLOCK
	#define		SYS_CLOCK	16				// 3664F=16MHz 3694F=20MHz
	float sys_clock = SYS_CLOCK*1000000;	// �����p[Hz]
#endif

#ifndef DIAG
	#define DIAG	DIAG
	byte		DIAG	=	1;			// �f�f���[�h
#endif

#define		LCD							// �t���������ꍇ�͏���

//#define   IR_ADC_MODE         // LED�ߋ����p�̏ꍇ��define����

#ifdef LCD
	#define LCD_H8_H
	#include "lib/lcd_h8.c"
#endif

#ifdef IR_ADC_MODE
  #define TYPE  "�ߋ���LED�ݻ IR-ADC ��"
#else
  #define TYPE  "IR Receiver Module ��"
#endif

#define		IR_OUT		IO.PDR1.BIT.B4	// �ԊO��LED�̐ڑ��|�[�g
#define		IR_OUT_OFF	0				// �ԊO��LED�񔭌����̏o�͒l
#define		IR_OUT_ON	1				// �ԊO��LED�������̏o�͒l
#define		IR_IN		IO.PDR1.BIT.B5	// �ԊO���Z���T�̐ڑ��|�[�g
#define		IR_IN_OFF	0				// �ԊO���Z���T�������̓��͒l
#define		IR_IN_ON	1				// �ԊO���Z���T������̓��͒l
#define		IR_DELAY_LH	200				// �ԊO���Z���T�������̒x������[us]
#define		IR_DELAY_HL	320				// �ԊO���Z���T�������̒x������[us]
#define		LED1_OUT	IO.PDR8.BIT.B0	// LED�Ԃ̐ڑ��|�[�g
#define		LED2_OUT	IO.PDR8.BIT.B1	// LED�΂̐ڑ��|�[�g
#define		LED_OUT_OFF	0				// LED�񔭌����̏o�͒l
#define		LED_OUT_ON	1				// LED�������̏o�͒l
#define		BUTN		IO.PDR8.BIT.B2	// �{�^���̐ڑ��|�[�g
#define		BUTN_OFF	0				// �{�^���𗣂��Ă���
#define		BUTN_ON		1				// �{�^���������Ă���
#define		SYNC_WAIT	4095			// �������̂Q�{�ȏ� �ő�32235
#define		DATA_SIZE	16				// �f�[�^��(byte),4�̔{��
#define		SCI_SIZE	64				// �V���A���f�[�^��
#define		IR_READ_TIME_OUT	5		// IR���̓^�C���A�E�g[�b]
#define		IR_READ_NOISE_DET	4		// 1/4�V���{����{�P�ʈȉ����m�C�Y�Ƃ���

#define MILLISEC_WAIT	MILLISEC_WAIT
unsigned int MILLISEC_WAIT	=	564	;	// 1ms���̋󃋁[�v�����l

#define FLASH_DEF
unsigned int FLASH_AEHA_WAIT=	5	;	// 13us(38kHz)���̋󃋁[�v�����l
unsigned int FLASH_AEHA_TIMES=	18	;	// �V���{����Ԃ̓_�ŉ�
unsigned int FLASH_NEC_WAIT=	5	;	// 13us(38kHz)���̋󃋁[�v�����l
unsigned int FLASH_NEC_TIMES=	22	;	// �V���{����Ԃ̓_�ŉ�
unsigned int FLASH_SIRC_WAIT=	5	;	// 12.5us(40kHz)���̋󃋁[�v�����l
unsigned int FLASH_SIRC_TIMES=	24	;	// �V���{����Ԃ̓_�ŉ�

#define TIMER_A_DEF
unsigned int TIMER_SEC		=	0	;	// �q�s�b�J�E���g�p�O���[�o���ϐ�

unsigned int FREQ_AEHA		=	38	;	// AEHA 38kHz
unsigned int FREQ_NEC		=	38	;	// NEC  38kHz
unsigned int FREQ_SIRC		=	40	;	// SIRC 40kHz
unsigned int FREQ_TAU		=	50	;	// �����g�f���[�e�B�[
unsigned int SYMBOL_AEHA	=	470	;	// AEHA 470us
unsigned int SYMBOL_NEC		=	563	;	// NEC  562.5us
unsigned int SYMBOL_SIRC	=	600	;	// SIRC 600us

unsigned int TIMER_V_B_AEHA	=	132;	// TIMER_V�̎���(20MHz��)
unsigned int TIMER_V_B_NEC	=	132;	// ir_cal.c �ōČv�Z����
unsigned int TIMER_V_B_SIRC	=	125;
unsigned int TIMER_V_A_AEHA	=	66;		// TIMER_V�̃f���[�e�B�[(TIMER_V_B/2)
unsigned int TIMER_V_A_NEC	=	66;
unsigned int TIMER_V_A_SIRC	=	62;
unsigned int TIMER_V_TS_AEHA=	18;		// �P�V���{������TIMER_V�̃J�E���g��
unsigned int TIMER_V_TS_NEC =	21;
unsigned int TIMER_V_TS_SIRC=	24;
unsigned int TIMER_A_EOF[2]	=	{10,10};	// TIMER_A�ɂ��I�����莞�ԕ␳

byte		 TIMER_V_FLASH	=	(0b00000110 ^ (IR_OUT_OFF*0b00001111));
										// �ԊO��LED�̘_�� 0110=�g�o�͂œ_��
byte		 TIMER_V_WAIT	=	(0b00000101 ^ (IR_OUT_OFF*0b00001111));
										// �ԊO��LED�̘_�� 0101=�k�o�͂ŏ���

#define IR_MODE_ENUM
enum Ir_Mode {NA,AEHA,NEC,SIRC,ERROR};	// �ԊO�����[�h�̌^

#define IR_PORT_INIT
void ir_port_init(){
	/*�|�[�g1         76543210*/
	IO.PMR1.BYTE  = 0b00000000;     // ���[�h(P1,5)     ���o��=0  ���̑�=1
	IO.PCR1       = 0b11010111;     // ���o�͐ݒ�       ����  =0  �o��  =1  B3=���U�[�u
	IO.PUCR1.BYTE = 0b00000000;     // �v���A�b�v(P1,5) ���Ȃ�=0  ����  =1
	IO.PDR1.BYTE  = 0b00000000;     // �A�N�Z�X         �k�o��=0  �g�o��=1

	/*�|�[�g8         76543210*/	// �H�� H8 Tiny I/O BOARD TERA2 [K-00207]�p
	IO.PCR8       = 0b11110011;     // ���o�͐ݒ�       ����  =0  �o��  =1
	IO.PDR8.BYTE  = 0b00000000;     // �A�N�Z�X         �k�o��=0  �g�o��=1

	/*TIMER_V         76543210*/
	TV.TCRV0.BYTE = 0b00010000;
	/*      B���� ____||||||_|___ Clock �ݒ�ŃJ�E���g�J�n
	        A���� _____||||______ �J�E���^�N���A 00:���Ȃ� 10:B�N���A
	       OF���� ______|
	*/	
	TV.TCSRV.BYTE = TIMER_V_WAIT;
	//              0b00000101;
	/*    B�t���O ____||| ||||___ A�}�b�`�o�� 10:H�o��  00:�ω����Ȃ�
	      A�t���O _____|| ||_____ B�}�b�`�o�� 01:L�o��  11:�g�O��
	     OF�t���O ______|
	*/
}

void ir_led_init(){
	IR_OUT=IR_OUT_OFF;
	LED1_OUT=LED_OUT_OFF;
	LED2_OUT=LED_OUT_OFF;
}

/* TIMER_A ���荞�ݏ��� */
void int_timera(void){ 
  IRR1.BIT.IRRTA=0;				/* �^�C�}�[A�����t���O */
  TIMER_SEC++;
}

#ifndef IR_TESTER_H
	#define IR_TESTER_H
	#include "ir_tester.c"
#endif

/* IR_TESTER�Ăяo�� */
#define MAIN
void main( void ){
	ir_tester();
}
