/*
 * reverb.c
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */



#include "reverb.h"



void Reverb_Init(Reverb *rvb, uint16_t size, float decay, float dry, float wet){
	COMBFilter_Init(&rvb->cbf_rvb_1, size, decay);
	COMBFilter_Init(&rvb->cbf_rvb_2, size*0.967, decay);
	COMBFilter_Init(&rvb->cbf_rvb_3, size*0.923, decay);
	COMBFilter_Init(&rvb->cbf_rvb_4, size*0.871, decay);
	ALLPASSFilter_Init(&rvb->apf_rvb_1, size*0.171, decay);
	ALLPASSFilter_Init(&rvb->apf_rvb_2, size*0.057, decay);
	rvb->dry = dry;
	rvb->wet = wet;
	rvb->out = 0.0f;
}
float Reverb_Update(Reverb *rvb, float inp){
	COMBFilter_Update(&rvb->cbf_rvb_1, inp);
	COMBFilter_Update(&rvb->cbf_rvb_2, inp);
	COMBFilter_Update(&rvb->cbf_rvb_3, inp);
	COMBFilter_Update(&rvb->cbf_rvb_4, inp);
	ALLPASSFilter_Update(&rvb->apf_rvb_1, (rvb->cbf_rvb_1.out + rvb->cbf_rvb_2.out + rvb->cbf_rvb_3.out + rvb->cbf_rvb_4.out) / 4.0);
	ALLPASSFilter_Update(&rvb->apf_rvb_2, rvb->apf_rvb_1.out);
	rvb->out = ((rvb->apf_rvb_2.out - inp) * rvb->wet) + (inp * rvb->dry);
	return rvb->out;
}
