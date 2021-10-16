/*
 * distorsion.h
 *
 *  Created on: 26 lug 2021
 *      Author: Brian
 */

#ifndef INC_DISTORTION_H_
#define INC_DISTORTION_H_

typedef struct {
	float InputGain;
	float OutputGain;
	float Crossover;
	float Clipping;
	float out;
} Distortion;
void Distortion_Init(Distortion *dist, float inputgain, float crossover, float clipping, float outputgain);
float Distortion_Update(Distortion *dist, float inp);

#endif /* INC_DISTORTION_H_ */
