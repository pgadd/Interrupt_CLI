#include <stm32g474xx.h>
#include <stm32g4xx_hal.h>
#include <stdint.h>
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


int main(void)
{
    HAL_Init();

    GPIO_Init();
    UART_init();
    Timer_Init();
    ADC_Init();

    
    while (1){
        HAL_ADC_Start(&adc1);
        HAL_ADC_PollForConversion(&adc1, 10);
        uint16_t adc_reading = HAL_ADC_GetValue(&adc1);
    }

    return 0;
}

