#ifndef ADC
#define ADC

#include <stm32g474xx.h>
#include <stm32g4xx_hal.h>
#include <stdint.h>
#include <stdio.h>

ADC_HandleTypeDef adc1;
DMA_HandleTypeDef hdma_adc1;
uint32_t adc_buffer[1];

void ADC_Init(void);

#endif
