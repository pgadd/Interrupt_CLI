#ifndef ADC
#define ADC

#include <stm32g474xx.h>
#include <stm32g4xx_hal.h>
#include <stdint.h>
#include <stdio.h>

ADC_HandleTypeDef adc1;
extern ADC_ChannelConfTypeDef sConfig;

void ADC_Init(void);

#endif
