/*
 * phaser.h
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */

#ifndef INC_PHASER_H_
#define INC_PHASER_H_

typedef struct {
	float DryWet;
	float out;
} Phaser;
void Phaser_Init(Phaser *phs, uint16_t D, float M, float G);
float Phaser_Update(Phaser *phs, float inp);

#endif /* INC_PHASER_H_ */
