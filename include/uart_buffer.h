#ifndef UART_BUFFER
#define UART_BUFFER

#include <stm32g474xx.h>
#include <stm32g4xx_hal.h>
#include <stdint.h>
#include <stdio.h>

UART_HandleTypeDef hlpuart1;

uint8_t rx_data;

void UART_init(void);
void LPUART1_IRQHandler(void);

#endif