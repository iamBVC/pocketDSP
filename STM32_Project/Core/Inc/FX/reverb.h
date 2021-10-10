/*
 * reverb.h
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */

#ifndef INC_REVERB_H_
#define INC_REVERB_H_

typedef struct {
	float out;
	float wet;
	float dry;
} Reverb;
void Reverb_Init(Reverb *rvb, uint16_t size, float decay, float wet, float dry);
float Reverb_Update(Reverb *rvb, float inp);

#endif /* INC_REVERB_H_ */
