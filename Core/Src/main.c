/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "kEY.h"
#include "smg.h"
#include "DHT11.h"
#include "stdio.h"
#include <stdlib.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RX_Length  1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint16_t adc_value[1000];
char Usart_ReceiveBuffer[RX_Length] = {0};
char USART1MESSAGE[RX_Length] = {0};
int USART_COUNTER = 0;
char END_STA = 0;
char TempChar = 0;
int key_event = 0;
uint32_t ADC_ConvertedValue = 0;
float ADC_count = 0;
float ADC_all = 0;
float ADC_Vol = 0;
int alarm_temperature = 0;//温度报警
int alarm_humidity = 0;//湿度报警
RTC_DateTypeDef RTC_DateStruct;
RTC_DateTypeDef old_RTC_DateStruct;

RTC_TimeTypeDef RTC_TimeStruct;
RTC_TimeTypeDef old_RTC_TimeStruct;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


float *get_ADC_all(){
    return &ADC_all;
}

float *get_ADC_count(){
    return &ADC_count;
}
RTC_DateTypeDef get_date(){
    return RTC_DateStruct;
}

RTC_TimeTypeDef get_time(){
    return RTC_TimeStruct;
}

RTC_DateTypeDef* get_datelp(){
    return &RTC_DateStruct;
}
int* get_alarm_temperature(){
    return &alarm_temperature;
}

int* get_alarm_humidity(){
    return &alarm_humidity;
}

int str_len(char *s)
{
    int i = 0;
    while ( s[i++] != '\0');
    return i;

}

int cmd_is(char* cmd_recv,char *cmd){
    for(int i=0;;i++){
        if(cmd[i]!=0){
            if(cmd_recv[i] != cmd[i]){
                return 0;
            }
        }
        else{
            return 1;
        }
    }

}

int get_value_int(char* cmd_recv){
    for(int i=0;;i++){
        if(cmd_recv[i] == '='){
            return(atoi(cmd_recv + i + 1));
        }
    }

}

int get_value_date(char* cmd_recv,uint8_t* year,uint8_t* mounth, uint8_t* date,uint8_t* week){
    for(int i=0;;i++){
        if(cmd_recv[i] == '='){
            char *datelp = cmd_recv + i + 1;
            char buf[10] = {0};
            int index = 0;
            int date_list_index = 0;
            for(int j=0;datelp[j];j++){
                buf[index++] = datelp[j];
                if(datelp[j]==','||datelp[j]==';'){
                    if(date_list_index==0)*year= (uint8_t)atoi(buf);
                    if(date_list_index==1)*mounth= (uint8_t)atoi(buf);
                    if(date_list_index==2)*date= (uint8_t)atoi(buf);
                    if(date_list_index==3)*week= (uint8_t)atoi(buf);
                    date_list_index++;
                    index = 0;
                    for (int k = 0; k < 8; k++) {
                        buf[k]=0;
                    }
                    if(datelp[j]==';'){
                        return 1;
                    }
                }

            }
            return 0;


        }
    }

}

int get_value_time(char* cmd_recv,uint8_t* hour,uint8_t* minute, uint8_t* second){
    for(int i=0;;i++){
        if(cmd_recv[i] == '='){
            char *datelp = cmd_recv + i + 1;
            char buf[10] = {0};
            int index = 0;
            int date_list_index = 0;
            for(int j=0;datelp[j];j++){
                buf[index++] = datelp[j];
                if(datelp[j]==','||datelp[j]==';'){
                    if(date_list_index==0)*hour= (uint8_t)atoi(buf);
                    if(date_list_index==1)*minute= (uint8_t)atoi(buf);
                    if(date_list_index==2)*second= (uint8_t)atoi(buf);
                    date_list_index++;
                    index = 0;
                    for (int k = 0; k < 8; k++) {
                        buf[k]=0;
                    }
                    if(datelp[j]==';'){
                        return 1;
                    }
                }

            }
            return 0;


        }
    }

}

HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
    RTC_DateTypeDef RTC_DateStructure;
    HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
    RTC_DateStructure.Date=date;
    RTC_DateStructure.Month=month;
    //RTC_DateStructure.WeekDay=week;
    RTC_DateStructure.Year=year;
    RTC_DateStruct = RTC_DateStructure;
    old_RTC_DateStruct = RTC_DateStructure;
    return HAL_RTC_SetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
}

HAL_StatusTypeDef RTC_Set_Time(u8 h,u8 m,u8 s)
{
    RTC_TimeTypeDef RTC_TimeStructure;
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
    RTC_TimeStructure.Hours=h+1;
    RTC_TimeStructure.Minutes=m;
    RTC_TimeStructure.Seconds=s;
    RTC_TimeStruct = RTC_TimeStructure;
    old_RTC_TimeStruct = RTC_TimeStructure;
    return HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    ADC_ConvertedValue = HAL_ADC_GetValue(hadc);
    //ADC_all += ADC_ConvertedValue;
    //ADC_count++;
}

int ADC_GetValue(){
    return HAL_ADC_GetValue(&hadc1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if(!END_STA) {
            if (TempChar!=';') {
                Usart_ReceiveBuffer[USART_COUNTER++] = TempChar;
            }
            else {//0xxd 0x00 结束
                Usart_ReceiveBuffer[USART_COUNTER++] = TempChar;
                if (Usart_ReceiveBuffer[USART_COUNTER + 1] == 0){
                    END_STA = 1;
                }

            }
        }
        if(END_STA) {//0x00 进入
            int temp = USART_COUNTER;
            Usart_ReceiveBuffer[USART_COUNTER++] = TempChar;
            END_STA = 0;
            USART_COUNTER = 0;
            for(int i=0;i<temp;i++)
                USART1MESSAGE[i] = Usart_ReceiveBuffer[i];
            for (int i = 0; i < RX_Length; i++)
                Usart_ReceiveBuffer[i] = 0;
            char* cmd_recv = USART1MESSAGE;

            if(cmd_is(cmd_recv,"alarm_temperature")){
                alarm_temperature = get_value_int(cmd_recv);
            }else if(cmd_is(cmd_recv,"alarm_humidity")){
                alarm_humidity = get_value_int(cmd_recv);
            }else if(cmd_is(cmd_recv,"date")){
                int8_t y,m,d,w;
                if(get_value_date(cmd_recv,&y,&m,&d,&w)){
                    RTC_Set_Date(y,m,d,w);
                }
            }else if(cmd_is(cmd_recv,"time")){
                int8_t y,m,d,w;
                if(get_value_time(cmd_recv,&y,&m,&d)){
                    RTC_Set_Time(y,m,d);
                }
            }


            HAL_UART_Transmit_IT(&huart1,USART1MESSAGE,temp);
        }
    }
}

int get_key_event() {
    return key_event;
}

float Get_Adc_Average(int* ch,int times)
{
    float temp_val=0;
    u8 t;
    for(t=0;t<times;t++)
    {
        temp_val+=HAL_ADC_GetValue(ch);
    }
    return temp_val/times;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    //HAL_ADC_Start_DMA(&hadc1,&adc_value,1000);
    HAL_ADC_Start_IT(&hadc1);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);//使能
    HAL_UART_Receive_IT(&huart1, &TempChar, 1);//开启接收中断
    DHT11_update_Data();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    int dht11_t = 0;
    char  buffer[200] = {0};
    key_event=1;
    int change_mode = 0;
    int mode_index = 0;
    int led6pwmval = 0,dir=1;
    float fi = 0;
    
  while (1)
  {
      float ff = 0;
      HAL_ADC_Start_IT(&hadc1);
      if(dir)led6pwmval++;
      else led6pwmval--;
      int compare = 50 + 50*sin(fi);
      fi += 0.1;
      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,compare);//10ms



      if(change_mode == 0) {
          HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN);
          old_RTC_DateStruct = RTC_DateStruct;
          HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
          old_RTC_TimeStruct = RTC_TimeStruct;
      }
      int key = KEY_Scan(0);
      if(key){
          sprintf(buffer,"%d key down;\n",key);
          HAL_UART_Transmit(&huart1,buffer,str_len(buffer),1000);

          if(key==WKUP_PRES){
              if(change_mode == 1){
                  RTC_DateStruct = old_RTC_DateStruct;
                  RTC_Set_Date(RTC_DateStruct.Year, RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_DateStruct.WeekDay);
                  RTC_TimeStruct = old_RTC_TimeStruct;
                  RTC_Set_Time(RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);
              }
              change_mode = !change_mode;
          }
          if(change_mode == 0){
              switch(key)
              {
                  case KEY0_PRES:
                  {
                      key_event=1;
                      break;
                  }
                  case KEY1_PRES:
                  {
                      key_event=2;
                      break;
                  }
                  case KEY2_PRES:
                      key_event=3;
                      break;
              }

          }
          else{//修改模式
              switch(key_event)
              {
                  case 1:
                  {

                      switch(key)
                      {
                          case KEY0_PRES:
                          {

                              break;
                          }
                          case KEY1_PRES:
                          {

                              break;
                          }
                          case KEY2_PRES:

                              break;
                      }
                      break;
                  }
                  case 2:
                  {
                      switch(key)
                      {
                          case KEY0_PRES:
                          {
                              mode_index--;
                              if (mode_index == -1){
                                  mode_index = 2;
                              }
                              break;
                          }
                          case KEY1_PRES:
                          {
                              switch (mode_index) {
                                  case 0:
                                      old_RTC_DateStruct.Year++;
                                      break;
                                  case 1:
                                      RTC_DateStruct.Month = old_RTC_DateStruct.Month++;
                                      if ( old_RTC_DateStruct.Month == 13){
                                          old_RTC_DateStruct.Month = 1;
                                      }
                                      break;
                                  case 2:
                                      RTC_DateStruct.Date = old_RTC_DateStruct.Date++;
                                      if ( old_RTC_DateStruct.Date == 31){
                                          old_RTC_DateStruct.Date = 1;
                                      }
                                      break;
                              }

                              break;
                          }
                          case KEY2_PRES:
                              mode_index++;
                              if (mode_index == 3){
                                  mode_index = 0;
                              }
                              break;
                      }
                      switch (mode_index) {
                          case 0:
                              RTC_DateStruct.Year = old_RTC_DateStruct.Year;
                              RTC_DateStruct.Month = 100;
                              RTC_DateStruct.Date = 100;
                              break;
                          case 1:
                              RTC_DateStruct.Year = 100;
                              RTC_DateStruct.Month = old_RTC_DateStruct.Month;
                              RTC_DateStruct.Date = 100;
                              break;
                          case 2:
                              RTC_DateStruct.Year = 100;
                              RTC_DateStruct.Month = 100;
                              RTC_DateStruct.Date = old_RTC_DateStruct.Date;
                              break;
                      }

                      break;
                  }
                  case 3:
                  {
                      switch(key)
                      {
                          case KEY0_PRES:
                          {
                              mode_index--;
                              if (mode_index == -1){
                                  mode_index = 2;
                              }
                              break;
                          }
                          case KEY1_PRES:
                          {
                              switch (mode_index) {
                                  case 0:
                                      old_RTC_TimeStruct.Hours++;
                                      if ( old_RTC_TimeStruct.Hours == 24){
                                          old_RTC_TimeStruct.Hours = 0;
                                      }
                                      break;
                                  case 1:
                                      RTC_TimeStruct.Minutes = old_RTC_TimeStruct.Minutes++;
                                      if ( old_RTC_TimeStruct.Minutes == 60){
                                          old_RTC_TimeStruct.Minutes = 0;
                                      }
                                      break;
                                  case 2:
                                      RTC_TimeStruct.Seconds = old_RTC_TimeStruct.Seconds++;
                                      if ( old_RTC_TimeStruct.Seconds == 31){
                                          old_RTC_TimeStruct.Seconds = 1;
                                      }
                                      break;
                              }

                              break;
                          }
                          case KEY2_PRES:
                              mode_index++;
                              if (mode_index == 3){
                                  mode_index = 0;
                              }
                              break;
                      }
                      switch (mode_index) {
                          case 0:
                              RTC_TimeStruct.Hours = old_RTC_TimeStruct.Hours;
                              RTC_TimeStruct.Minutes = 100;
                              RTC_TimeStruct.Seconds = 100;
                              break;
                          case 1:
                              RTC_TimeStruct.Hours = 100;
                              RTC_TimeStruct.Minutes = old_RTC_TimeStruct.Minutes;
                              RTC_TimeStruct.Seconds = 100;
                              break;
                          case 2:
                              RTC_TimeStruct.Hours = 100;
                              RTC_TimeStruct.Minutes = 100;
                              RTC_TimeStruct.Seconds = old_RTC_TimeStruct.Seconds;
                              break;
                      }

                      break;
                  }
              }

          }
  }
      dht11_t++;
      if(dht11_t==80)
      {
          dht11_t=0;
          DHT11_update_Data();


      }
      uint8_t temperature,humidity;
      DHT11_Read_Data(&temperature,&humidity);
      sprintf(buffer,"temperature=%d;\n",temperature);
      HAL_UART_Transmit(&huart1,buffer,str_len(buffer),1000);

      sprintf(buffer,"humidity=%d;\n",humidity);
      HAL_UART_Transmit(&huart1,buffer,str_len(buffer),1000);

      //ADC_Vol = Get_Adc_Average(&hadc1,100);
      HAL_Delay(20);
      ADC_Vol = ADC_all/ADC_count;
      ADC_Vol =(float) ADC_Vol/4096.0*3.3; // 读取转换的AD倿
      ADC_all = ADC_count = 0;

      sprintf(buffer,"adc_v=%f;\n",ADC_Vol);
      HAL_UART_Transmit(&huart1,buffer,str_len(buffer),1000);

      RTC_DateTypeDef RTC_DateStruct;
      HAL_RTC_GetDate(&hrtc,&RTC_DateStruct,RTC_FORMAT_BIN);

      sprintf(buffer,"date=%d,%d,%d,%d;\n",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_DateStruct.WeekDay);
      HAL_UART_Transmit(&huart1,buffer,str_len(buffer),1000);

      RTC_TimeTypeDef RTC_TimeStruct;
      HAL_RTC_GetTime(&hrtc,&RTC_TimeStruct,RTC_FORMAT_BIN);

      sprintf(buffer,"time=%d,%d,%d;\n",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);
      HAL_UART_Transmit(&huart1,buffer,str_len(buffer),1000);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2) {

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_OCTOBER;
  sDate.Date = 0x26;
  sDate.Year = 0x21;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1,0x32F2);
    }
  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 720-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 100;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 9600-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_5|PIN_DHT11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_8, GPIO_PIN_SET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3
                           PC4 PC5 PC6 PC7
                           PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PIN_DHT11_Pin */
  GPIO_InitStruct.Pin = PIN_DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(PIN_DHT11_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
