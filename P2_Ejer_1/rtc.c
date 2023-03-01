#include "rtc.h"
#include "lcd.h"
#include "cmsis_os2.h"   
#include <stdio.h>
#include "Timer.h"
#include "ThreadFlags.h" 
/*********************************************************************
		Inspiración: https://controllerstech.com/internal-rtc-in-stm32/
*********************************************************************/
TIM_HandleTypeDef htim2;

RTC_HandleTypeDef hrtc;
RTC_AlarmTypeDef sAlarm;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
extern osTimerId_t tim_id1;
extern osThreadId_t tid_ThreadFlags;
char time[30];
char date[30];
/***********************************************************
							Inicialización del RTC
***********************************************************/
void RTC_Init(void)
{
	//__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);					// RCC_RTCCLKSOURCE_HSE_DIV10			RCC_RTCCLKSOURCE_LSE
	__HAL_RCC_RTC_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_PWR_CLK_ENABLE();		// Para poder leer los registros

	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;					// 32,768kHz/((127+1)(255+1)) = 1 Hz			// Para LSE 127 y 255, para HSE de 8 MHz es 127 y 6249
	hrtc.Init.AsynchPrediv = 127;			//127;		//0x7f;
	hrtc.Init.SynchPrediv = 255;			//255;		//0x17ff;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	
	// Ajustamos el tiempo
	sTime.Hours = 12;
	sTime.TimeFormat=RTC_HOURFORMAT_24;
	sTime.Minutes = 24;
	sTime.Seconds = 56;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
	// Ajustamos la fecha
	sDate.Year = 23;
	sDate.Month = RTC_MONTH_FEBRUARY;
	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Date = 21;
	
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);		// Habilitamos las interrupciones de las alarma del RTC
	HAL_RTC_Init(&hrtc);
}

/***********************************************************
						Obtención del tiempo en binario
***********************************************************/
void Get_Time_RTC_Binary(void)
{
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

/***********************************************************
						Ajusta un nuevo tiempo
***********************************************************/
void Set_Time_RTC(uint8_t hour, uint8_t minute, uint8_t second)
{
	sTime.Hours = hour;
	sTime.Minutes = minute;
	sTime.Seconds = second;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}
/***********************************************************
						Obtención de la fecha en binario
***********************************************************/
void Get_Date_RTC()
{
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

/***********************************************************
						Ajusta una nueva fecha
***********************************************************/
void Set_RTC_Date(uint8_t year, uint8_t month, uint8_t week, uint8_t date)
{
	sDate.Year = year;
	sDate.Month = month;
	sDate.WeekDay = week;
	sDate.Date = date;
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}
/***********************************************************
						Coloca una alarma
***********************************************************/
void Set_Alarm(uint8_t hour, uint8_t minute, uint8_t second)
{

	sAlarm.AlarmTime.Hours = hour;
	sAlarm.AlarmTime.Minutes = minute;
	sAlarm.AlarmTime.Seconds = second;
	
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 21;
	
	sAlarm.AlarmMask = RTC_ALARMMASK_MINUTES;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE; 
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.Alarm = RTC_ALARM_A;
	
	
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);

}

/***********************************************************
						Handler de las alarmas
***********************************************************/
void RTC_Alarm_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&hrtc);
	
}

/***********************************************************
						Callback de la alarma
***********************************************************/
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) 
{ 
	if(hrtc->Instance == RTC){
	  osThreadFlagsSet(tid_ThreadFlags,0x00000001U);
  
		Get_Time_RTC_Binary();
		sTime.Minutes = sTime.Minutes + 1;
		HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, RTC_FORMAT_BIN);
	}
}

/***********************************************************
			Muestra la fecha y el tiempo en el display
***********************************************************/
void Display_Date_Time(void)
	
{
	
	RTC_TimeTypeDef gTime;
	RTC_DateTypeDef gDate;
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);//Primero el time y luego el date siempre
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	sprintf(time, "Time: %.2d:%.2d:%.2d", gTime.Hours, gTime.Minutes, gTime.Seconds);
	
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	sprintf(date, "Date: %.2d-%.2d-%.4d", gDate.Date, gDate.Month, gDate.Year+2000);

	LCD_symbolToLocalBuffer_L1(time, strlen(time));
	LCD_symbolToLocalBuffer_L2(date, strlen(date));
	LCD_update();
}

void parpadeo_led(void){
	/*
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 47999; 
	// Frecuencia de interrupcion de 3,5 Hz Apartado 1A
	//htim7.Init.Period = 499;
	// Frecuencia de interrupcion 2/3 Hz Apartado 1B
	htim2.Init.Period = 349;
	
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	HAL_TIM_Base_Init(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);
*/

}
/*
void TIM2_IRQHandler(void){
	HAL_TIM_IRQHandler(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
	if(htim -> Instance == TIM2)
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
	  
}
*/