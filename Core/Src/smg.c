//
// Created by dk on 2021/10/26.
//

#include "smg.h"

void LED_Wei(char num)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,num&0x01);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,(num&0x02)>>1);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,(num&0x04)>>2);

}

void LED_Refresh(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
}
void LED_Write_Data(u8 duan,u8 wei)
{
    int i;
    for( i=0;i<8;i++)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,(duan>>i)&0x01);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
    }
    LED_Wei(wei);
}
