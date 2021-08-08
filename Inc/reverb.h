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
} Reverb;
void Reverb_Init(Reverb *rvb, uint16_t D1, uint16_t D2, uint16_t D3, uint16_t D4, uint16_t D5, uint16_t D6, float G1, float G2, float G3, float G4, float G5, float G6);
float Reverb_Update(Reverb *rvb, float inp);

#endif /* INC_REVERB_H_ */
