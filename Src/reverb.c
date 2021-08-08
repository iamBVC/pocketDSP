/*
 * reverb.c
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */


#include "allpass_filter.h"
#include "comb_filter.h"
#include "reverb.h"

ALLPASSFilter apf_rvb_1,apf_rvb_2;
COMBFilter cbf_rvb_1,cbf_rvb_2,cbf_rvb_3,cbf_rvb_4;

void Reverb_Init(Reverb *rvb, uint16_t D1, uint16_t D2, uint16_t D3, uint16_t D4, uint16_t D5, uint16_t D6, float G1, float G2, float G3, float G4, float G5, float G6){

	COMBFilter_Init(&cbf_rvb_1, D1, G1);
	COMBFilter_Init(&cbf_rvb_2, D2, G2);
	COMBFilter_Init(&cbf_rvb_3, D3, G3);
	COMBFilter_Init(&cbf_rvb_4, D4, G4);
	ALLPASSFilter_Init(&apf_rvb_1, D5, G5);
	ALLPASSFilter_Init(&apf_rvb_2, D6, G6);
	rvb->out = 0.0f;
}
float Reverb_Update(Reverb *rvb, float inp){
	COMBFilter_Update(&cbf_rvb_1, inp);
	COMBFilter_Update(&cbf_rvb_2, inp);
	COMBFilter_Update(&cbf_rvb_3, inp);
	COMBFilter_Update(&cbf_rvb_4, inp);
	ALLPASSFilter_Update(&apf_rvb_1, (cbf_rvb_1.out + cbf_rvb_2.out + cbf_rvb_3.out + cbf_rvb_4.out) / 4.0);
	ALLPASSFilter_Update(&apf_rvb_2, apf_rvb_1.out);
	rvb->out = apf_rvb_2.out;
	return rvb->out;
}
