#include <stm32g474xx.h>
#include <stm32g4xx_hal.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <./uart_buffer.h>
#include <./timer.h>
#include <./adc.h>

void GPIO_Init(void);
void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef Led = {0};

    Led.Pin = GPIO_PIN_5;
    Led.Mode = GPIO_MODE_OUTPUT_PP;
    Led.Pull = GPIO_NOPULL;
    Led.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &Led);
}

void SysTick_Handler(void);
void SysTick_Handler(void) {
    HAL_IncTick(); //Esssentially increments a global volatile variable called uwTick.
}


int main(void)
{
    HAL_Init();

    GPIO_Init();
    UART_init();
    Timer_Init();

    HAL_UART_Transmit(&hlpuart1, (uint8_t*)"\r\n--- Booting Up ---\r\n", 22, 100);

    ADC_Init();

    HAL_UART_Transmit(&hlpuart1, (uint8_t*)"ADC Initialized!\r\n", 18, 100);


    char msg[50];
    while (1){
        HAL_ADC_Start(&adc1);
        HAL_ADC_PollForConversion(&adc1, 10);
        uint16_t adc_reading = HAL_ADC_GetValue(&adc1);

        snprintf(msg, sizeof(msg), "Sensor: %u\r\n", adc_reading);

        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

        HAL_Delay(1000);
        
    }

    return 0;
}

