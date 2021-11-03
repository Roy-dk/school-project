//
// Created by dk on 2021/10/26.
//

#ifndef DHT11_SMG_H
#define DHT11_SMG_H
#include "stm32f4xx_hal.h"
#define u8 int8_t

#define LED_A0(v) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,v)
#define LED_A1(v) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,v)
#define LED_A2(v) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,v)


#define LED_DS(v)		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,v)
#define LED_LCLK(v)	    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,v)
#define LED_SCK(v)		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,v)

void LED_Refresh(void);
void LED_Write_Data(u8 duan,u8 wei);

#endif //DHT11_SMG_H
