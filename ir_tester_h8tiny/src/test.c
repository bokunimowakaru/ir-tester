#ifndef NAME
	#define	NAME	"IR-ADC"
	#define	VERSION	"0.27"
#endif
#ifndef TYPE
	#define	TYPE "�ߋ���LED�ݻ�p"
#endif
#ifndef TARGET_H8_H
	#define TARGET_H8_H
	#include <3664.h>
#endif
#ifndef SYS_CLOCK
	#define		SYS_CLOCK	16				// 3664F=16MHz 3694F=20MHz
	float sys_clock = SYS_CLOCK*1000000;	// �����p[Hz] �L�����u���[�V�����Ή��̂��߂�float�Ő錾
#endif
#include "lib/ir_adc_read.c"
