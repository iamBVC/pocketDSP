#ifndef USER_SETTING_H_
#define USER_SETTING_H_

#define RD_PORT GPIOA
#define RD_PIN  GPIO_PIN_3
#define WR_PORT GPIOA
#define WR_PIN  GPIO_PIN_2
#define CD_PORT GPIOA          // RS PORT
#define CD_PIN  GPIO_PIN_1     // RS PIN
#define CS_PORT GPIOA
#define CS_PIN  GPIO_PIN_0
#define RESET_PORT GPIOC
#define RESET_PIN  GPIO_PIN_3

#define D0_PORT GPIOE
#define D0_PIN GPIO_PIN_9
#define D1_PORT GPIOE
#define D1_PIN GPIO_PIN_10
#define D2_PORT GPIOC
#define D2_PIN GPIO_PIN_5
#define D3_PORT GPIOB
#define D3_PIN GPIO_PIN_0
#define D4_PORT GPIOB
#define D4_PIN GPIO_PIN_1
#define D5_PORT GPIOB
#define D5_PIN GPIO_PIN_2
#define D6_PORT GPIOE
#define D6_PIN GPIO_PIN_7
#define D7_PORT GPIOE
#define D7_PIN GPIO_PIN_8

#define XP_PORT D6_PORT
#define XP_PIN D6_PIN
#define YP_PORT WR_PORT
#define YP_PIN WR_PIN
#define XM_PORT CD_PORT
#define XM_PIN CD_PIN
#define YM_PORT D7_PORT
#define YM_PIN D7_PIN
#define FCS_PORT GPIOC
#define FCS_PIN  GPIO_PIN_2

extern ADC_HandleTypeDef hadc1;
#define YP_ADC_CHANNEL ADC_CHANNEL_14
#define XM_ADC_CHANNEL ADC_CHANNEL_17

#define  WIDTH    ((uint16_t)320)
#define  HEIGHT   ((uint16_t)480)

/****************** delay in microseconds ***********************/
extern TIM_HandleTypeDef htim16;
void delay(uint32_t time) {
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim16, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim16)) < time);
}

#define write_8(d) { \
   GPIOB->BSRR = 0b0000000000000111 << 16; \
   GPIOC->BSRR = 0b0000000000100000 << 16; \
   GPIOE->BSRR = 0b0000011110000000 << 16; \
   GPIOB->BSRR = (((d) & (1<<3)) >> 3) \
               | (((d) & (1<<4)) >> 3) \
			   | (((d) & (1<<5)) >> 3); \
   GPIOC->BSRR = (((d) & (1<<2)) << 3); \
   GPIOE->BSRR = (((d) & (1<<0)) << 9) \
               | (((d) & (1<<1)) << 9) \
			   | (((d) & (1<<6)) << 1) \
			   | (((d) & (1<<7)) << 1); \
    }

#define read_8() (          (((GPIOB->IDR & (1<<0)) << 3) \
                           | ((GPIOB->IDR & (1<<1)) << 3) \
                           | ((GPIOB->IDR & (1<<2)) << 3) \
                           | ((GPIOC->IDR & (1<<5)) >> 3) \
                           | ((GPIOE->IDR & (1<<7)) >> 1) \
                           | ((GPIOE->IDR & (1<<8)) >> 1) \
                           | ((GPIOE->IDR & (1<<9)) >> 9) \
                           | ((GPIOE->IDR & (1<<10)) >> 9)))

#define WRITE_DELAY { }
#define IDLE_DELAY  { }
#define READ_DELAY  { }
#define SUPPORT_9488_555          //costs +230 bytes, 0.03s / 0.19s

#endif /* USER_SETTING_H_ */
