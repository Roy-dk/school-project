/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <smg.h>
#include <DHT11.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define u8 uint8_t
#define u16 uint16_t
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
u8 smg_wei=0;
u8 num=0;
u8 time=0;
u16 led_t=0;
u16 dht11_t=0;
u8 temperature;
u8 humidity;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
extern RTC_HandleTypeDef hrtc;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles ADC1 global interrupt.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break interrupt and TIM9 global interrupt.
  */
void TIM1_BRK_TIM9_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 0 */

  /* USER CODE END TIM1_BRK_TIM9_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 1 */

  /* USER CODE END TIM1_BRK_TIM9_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */
    //ADC_Vol = Get_Adc_Average(100);
    //ADC_Vol =(float) ADC_ConvertedValue/4096.0*3.3; // 读取转换的AD倿
    float* ADC_all = get_ADC_all();
    float* ADC_count = get_ADC_count();

    int ADC_ConvertedValue = ADC_GetValue();
    (*ADC_all) += ADC_ConvertedValue;
    (*ADC_count)++;

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
  * @brief This function handles TIM1 trigger and commutation interrupts and TIM11 global interrupt.
  */
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 0 */

  /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 1 */

  /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
    u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
    int key_event = get_key_event();

    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&hrtc,&RTC_TimeStruct,RTC_FORMAT_BIN);
    DHT11_Read_Data(&temperature,&humidity);
        int* alarm_humidity = get_alarm_humidity();
        int* alarm_temperature = get_alarm_temperature();
        if((*alarm_humidity > 0 && *alarm_humidity < humidity) ||
                (*alarm_temperature > 0 && *alarm_temperature < temperature)){

            int LED0 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0);
            int BEEP0 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8);
            led_t++;
            if(led_t == 250){
                HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,!LED0);
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,!BEEP0);
                led_t = 0;
            }
        } else{
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,SET);
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,SET);
        }

    if(key_event==1)
    {

            DHT11_Read_Data(&temperature,&humidity);
            switch(smg_wei)
            {
                case 2:  num = smg_num[temperature/10]; break;
                case 3:  num =smg_num[temperature%10];break;
                case 6:  num = smg_num[humidity/10]; break;
                case 7:  num =smg_num[humidity%10];break;
            }

            LED_Write_Data(num,smg_wei);
            LED_Refresh();

            smg_wei++;
            if(smg_wei==4)  smg_wei=6;
            if(smg_wei==8)  smg_wei=2;
        }
    if(key_event==2)
    {

        RTC_DateTypeDef RTC_DateStruct  = get_date();
        switch(smg_wei)
        {
            case 0:  num = RTC_DateStruct.Year == 100 ? 0x02 : smg_num[RTC_DateStruct.Year/10]; break;//ʱ
            case 1:  num = RTC_DateStruct.Year == 100 ? 0x02 : smg_num[RTC_DateStruct.Year%10]; break;
            case 2:
            case 5:  num = 0x02; break;
            case 3:  num = RTC_DateStruct.Month == 100 ? 0x02 : smg_num[RTC_DateStruct.Month/10]; break; //��
            case 4:  num = RTC_DateStruct.Month == 100 ? 0x02 : smg_num[RTC_DateStruct.Month%10]; break;
            case 6:  num = RTC_DateStruct.Date == 100 ? 0x02 : smg_num[RTC_DateStruct.Date/10]; break; //��
            case 7:  num = RTC_DateStruct.Date == 100 ? 0x02 : smg_num[RTC_DateStruct.Date%10]; break;
        }
        LED_Write_Data(num,smg_wei);
        LED_Refresh();
        smg_wei++;
        if(smg_wei==8) smg_wei=0;
    }

    if(key_event==3)
        {

            RTC_TimeTypeDef RTC_TimeStruct;
            RTC_TimeStruct = get_time();
            switch(smg_wei)
            {
                case 0:  num = RTC_TimeStruct.Hours == 100 ? 0x02 : smg_num[RTC_TimeStruct.Hours/10]; break;//ʱ
                case 1:  num = RTC_TimeStruct.Hours == 100 ? 0x02 : smg_num[RTC_TimeStruct.Hours%10]; break;
                case 2:
                case 5:  num = 0x02; break;
                case 3:  num = RTC_TimeStruct.Minutes == 100 ? 0x02 : smg_num[RTC_TimeStruct.Minutes/10]; break; //��
                case 4:  num = RTC_TimeStruct.Minutes == 100 ? 0x02 : smg_num[RTC_TimeStruct.Minutes%10]; break;
                case 6:  num = RTC_TimeStruct.Seconds == 100 ? 0x02 : smg_num[RTC_TimeStruct.Seconds/10]; break; //��
                case 7:  num = RTC_TimeStruct.Seconds == 100 ? 0x02 : smg_num[RTC_TimeStruct.Seconds%10]; break;
            }
            LED_Write_Data(num,smg_wei);
            LED_Refresh();
            smg_wei++;
            if(smg_wei==8) smg_wei=0;
        }


  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
    HAL_UART_Receive_IT(&huart1, &TempChar, 1);
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
