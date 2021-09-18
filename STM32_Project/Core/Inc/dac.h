/*
 * dac.h
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "stm32h7xx_hal.h"

extern I2S_HandleTypeDef hi2s1;
extern TIM_HandleTypeDef htim13;

void DAC_Start();
void DAC_Stop();

#endif /* INC_DAC_H_ */
