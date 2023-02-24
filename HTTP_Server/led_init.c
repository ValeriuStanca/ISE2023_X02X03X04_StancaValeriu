#include "led_init.h"

static GPIO_InitTypeDef GPIO_InitStruct;


	void LED_Init(void){
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP; /*Elegimos modo Pull Up*/
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /*Elegimos la velocidad*/
		
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}     
	void RGB_init(void){
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN; /*Elegimos modo Pull Up*/
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /*Elegimos la velocidad*/
		
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}

		