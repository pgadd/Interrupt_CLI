#ifndef TIMER
#define TIMER

#include <stm32g474xx.h>
#include <stm32g4xx_hal.h>
#include <stdint.h>
#include <stdio.h>

TIM_HandleTypeDef htim3;

void Timer_Init(void);


#endif