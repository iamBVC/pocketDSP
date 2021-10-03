/*
 * dac.c
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#include "dac.h"

void DAC_Init(){
	HAL_I2S_Init(&hi2s1);
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
	HAL_Delay(100);
}
