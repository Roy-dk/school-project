//
// Created by dk on 2021/10/26.
//

#ifndef DHT11_KEY_H
#define DHT11_KEY_H
#include <stm32f4xx_hal.h>

#define KEY0        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)  //KEY0����PC8
#define KEY1        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)  //KEY1����PC9
#define KEY2        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)  //KEY2����PD2
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP����PA0

#define KEY0_PRES	1	//KEY0
#define KEY1_PRES	2	//KEY1
#define KEY2_PRES	3	//KEY2
#define WKUP_PRES	4	//WK_UP

uint8_t KEY_Scan(uint8_t mode);

#endif //DHT11_KEY_H
