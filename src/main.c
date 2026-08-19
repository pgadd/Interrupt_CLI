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

    HAL_ADC_Start_DMA(&adc1, (uint32_t*)adc_buffer, 1);

    while (1){

        // --- Read Left Eye (PA0 / Channel 1) ---
        uint32_t left_light = adc_buffer[0];

        if (previous_light > left_light + 15) {
            step_dir = step_dir * -1;
        }

        previous_light = left_light;
        current_pwm += step_dir;

        if (current_pwm < 500 ){
            current_pwm = 500;
            step_dir = step_dir * -1; 
        } else if (current_pwm > 2500) {
            current_pwm = 2500;
            step_dir = step_dir * -1;
        }

        //The pwm channel data register to send to the motor.
        TIM3->CCR2 = current_pwm;

        snprintf(msg, sizeof(msg), "light: %lu | current_pwm: %d\r\n", left_light, current_pwm);
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), 100);

        HAL_Delay(20);
        
    }

    return 0;
}

