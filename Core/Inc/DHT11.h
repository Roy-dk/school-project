//
// Created by dk on 2021/10/26.
//

#ifndef DHT11_DHT11_H
#define DHT11_DHT11_H

#include "stm32f4xx_hal.h"
#define u8 uint8_t


#define	DHT11_DQ_OUT(v) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,v)
#define	DHT11_DQ_IN  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)



u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
int DHT11_update_Data();
void DHT11_Rst(void);//��λDHT11

#endif //DHT11_DHT11_H
