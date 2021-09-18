/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VBAT_Pin GPIO_PIN_0
#define VBAT_GPIO_Port GPIOC
#define F_CS_Pin GPIO_PIN_2
#define F_CS_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOA
#define LCD_CD_Pin GPIO_PIN_1
#define LCD_CD_GPIO_Port GPIOA
#define LCD_WR_Pin GPIO_PIN_2
#define LCD_WR_GPIO_Port GPIOA
#define LCD_RD_Pin GPIO_PIN_3
#define LCD_RD_GPIO_Port GPIOA
#define DAC_LRCK_Pin GPIO_PIN_4
#define DAC_LRCK_GPIO_Port GPIOA
#define DAC_SCLK_Pin GPIO_PIN_5
#define DAC_SCLK_GPIO_Port GPIOA
#define DAC_MCLK_Pin GPIO_PIN_6
#define DAC_MCLK_GPIO_Port GPIOA
#define DAC_SDIN_Pin GPIO_PIN_7
#define DAC_SDIN_GPIO_Port GPIOA
#define LCD_D2_Pin GPIO_PIN_5
#define LCD_D2_GPIO_Port GPIOC
#define LCD_D3_Pin GPIO_PIN_0
#define LCD_D3_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_1
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_2
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_7
#define LCD_D6_GPIO_Port GPIOE
#define LCD_D7_Pin GPIO_PIN_8
#define LCD_D7_GPIO_Port GPIOE
#define LCD_D0_Pin GPIO_PIN_9
#define LCD_D0_GPIO_Port GPIOE
#define LCD_D1_Pin GPIO_PIN_10
#define LCD_D1_GPIO_Port GPIOE
#define SD_CS_Pin GPIO_PIN_11
#define SD_CS_GPIO_Port GPIOE
#define SD_SCK_Pin GPIO_PIN_12
#define SD_SCK_GPIO_Port GPIOE
#define SD_MISO_Pin GPIO_PIN_13
#define SD_MISO_GPIO_Port GPIOE
#define SD_MOSI_Pin GPIO_PIN_14
#define SD_MOSI_GPIO_Port GPIOE
#define ADC_LRCK_Pin GPIO_PIN_12
#define ADC_LRCK_GPIO_Port GPIOB
#define ADC_SCLK_Pin GPIO_PIN_13
#define ADC_SCLK_GPIO_Port GPIOB
#define ADC_SDOUT_Pin GPIO_PIN_14
#define ADC_SDOUT_GPIO_Port GPIOB
#define ADC_MCLK_Pin GPIO_PIN_15
#define ADC_MCLK_GPIO_Port GPIOB
#define ADC_M0_Pin GPIO_PIN_8
#define ADC_M0_GPIO_Port GPIOD
#define ADC_M1_Pin GPIO_PIN_9
#define ADC_M1_GPIO_Port GPIOD
#define ADC_RST_Pin GPIO_PIN_10
#define ADC_RST_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
