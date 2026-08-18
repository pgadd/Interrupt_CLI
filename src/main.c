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
    ADC_Init();

    HAL_UART_Transmit(&hlpuart1, (uint8_t*)"\r\n--- Booting Up ---\r\n", 22, 100);


    char msg[50];

    int step_dir = 10;
    uint32_t previous_light = 0;
    uint16_t current_pwm = 1500;

    ADC_ChannelConfTypeDef sConfig1 = {0};

    sConfig1.Channel = ADC_CHANNEL_1;
    sConfig1.Rank = ADC_REGULAR_RANK_1;
    sConfig1.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;


    while (1){

        // --- Read Left Eye (PA0 / Channel 1) ---
        HAL_ADC_ConfigChannel(&adc1, &sConfig1);
        HAL_ADC_Start(&adc1);
        HAL_ADC_PollForConversion(&adc1, 10);
        uint32_t left_light = HAL_ADC_GetValue(&adc1);
        HAL_ADC_Stop(&adc1);

        if (previous_light > left_light) {
            step_dir = step_dir * -1;
        }

        previous_light = left_light;
        current_pwm += step_dir;

        if (current_pwm < 500 ){
            current_pwm = 500;
        } else if (current_pwm > 2500) {
            current_pwm = 2500;
        }


        TIM3->CCR2 = current_pwm;

        snprintf(msg, sizeof(msg), "light: %lu | current_pwm: %d\r\n", left_light, current_pwm);
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), 100);

        HAL_Delay(100);
        
    }

    return 0;
}

