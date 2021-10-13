/*
 * reverb.h
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */

#ifndef INC_REVERB_H_
#define INC_REVERB_H_

#include "allpass_filter.h"
#include "comb_filter.h"

typedef struct {
	float out;
	float dry;
	float wet;
	ALLPASSFilter apf_rvb_1,apf_rvb_2;
	COMBFilter cbf_rvb_1,cbf_rvb_2,cbf_rvb_3,cbf_rvb_4;
} Reverb;
void Reverb_Init(Reverb *rvb, uint16_t size, float decay, float dry, float wet);
float Reverb_Update(Reverb *rvb, float inp);

#endif /* INC_REVERB_H_ */
