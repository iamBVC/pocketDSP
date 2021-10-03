/*
 * distorsion.h
 *
 *  Created on: 26 lug 2021
 *      Author: Brian
 */

#ifndef INC_DISTORSION_H_
#define INC_DISTORSION_H_

typedef struct {
	float Threshold;
	float Ratio;
	float out;
} Distorsion;
void Distorsion_Init(Distorsion *dist, float T, float R);
float Distorsion_Update(Distorsion *dist, float inp);

#endif /* INC_DISTORSION_H_ */
