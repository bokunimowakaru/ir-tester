/*********************************************************************
�{�\�[�X���X�g����у\�t�g�E�F�A�́A���C�Z���X�t���[�ł��B
���p�A�ҏW�A�Ĕz�z�������R�ɍs���܂����A���쌠�\���̉��ς͋֎~���܂��B

                               Copyright (c) 2009 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
*********************************************************************/

#ifndef IR_DATA_H
	#define IR_DATA_H
	#include "ir_data.h"
#endif

int ir_search(byte *data, int data_len, enum Ir_Mode mode){
	int i,id,len_b;
	int left;
	byte F=1;
	
	left=IR_DATA_SIZE;
	id=-1;
	
	len_b = (int)(data_len/8);
	if( (len_b * 8) != data_len ) len_b++;
	
	while( left>0 ){
		id++;
		left -= sizeof(IR_DATA[id]);
		F=1;
		if( IR_DATA[id].mode == mode && IR_DATA[id].len == data_len ){
			F=0;											// �s��v�t���O
			for(i=0;i<len_b;i++){
				if( *(IR_DATA[id].data+i) != *(data+i) ){	// �s��v���o
					F=1;
					i=len_b;									// for���[�v���E
				}
			}
			if( F==0 ){										// ���S�Ɉ�v����
				left=0;										// while���[�v���E
			}
		}
	}
	if( F==0 ) {
		return( id );
	} else {
		return( -1 );
	}
}
