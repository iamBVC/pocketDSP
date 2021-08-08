/*
 * phaser.c
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */


#include "allpass_filter.h"
#include "phaser.h"

ALLPASSFilter apf_phs_1,apf_phs_2,apf_phs_3,apf_phs_4,apf_phs_5,apf_phs_6,apf_phs_7,apf_phs_8;

void Phaser_Init(Phaser *phs, uint16_t D, float M, float G){
	ALLPASSFilter_Init(&apf_phs_1, D, G);
	ALLPASSFilter_Init(&apf_phs_2, D, G);
	ALLPASSFilter_Init(&apf_phs_3, D, G);
	ALLPASSFilter_Init(&apf_phs_4, D, G);
	ALLPASSFilter_Init(&apf_phs_5, D, G);
	ALLPASSFilter_Init(&apf_phs_6, D, G);
	ALLPASSFilter_Init(&apf_phs_7, D, G);
	ALLPASSFilter_Init(&apf_phs_8, D, G);
	phs->DryWet = M;
	phs->out = 0.0f;
}
float Phaser_Update(Phaser *phs, float inp){
	ALLPASSFilter_Update(&apf_phs_1, inp);
	ALLPASSFilter_Update(&apf_phs_2, apf_phs_1.out);
	ALLPASSFilter_Update(&apf_phs_3, apf_phs_2.out);
	ALLPASSFilter_Update(&apf_phs_4, apf_phs_3.out);
	ALLPASSFilter_Update(&apf_phs_5, apf_phs_4.out);
	ALLPASSFilter_Update(&apf_phs_6, apf_phs_5.out);
	ALLPASSFilter_Update(&apf_phs_7, apf_phs_6.out);
	ALLPASSFilter_Update(&apf_phs_8, apf_phs_7.out);
	phs->out = (apf_phs_8.out * phs->DryWet) + (inp * (1.0 - phs->DryWet));
	return phs->out;
}
