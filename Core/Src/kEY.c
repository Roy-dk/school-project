//
// Created by dk on 2021/10/26.
//


#include "kEY.h"
#define u8 uint8_t
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;
    if(mode)key_up=1;
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
    {
        HAL_Delay(10);
        key_up=0;
        if(KEY0==0)return KEY0_PRES;
        else if(KEY1==0)return KEY1_PRES;
        else if(KEY2==0)return KEY2_PRES;
        else if(WK_UP==1)return WKUP_PRES;
    }else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1;
    return 0;
}