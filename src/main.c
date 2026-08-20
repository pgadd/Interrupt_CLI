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


    typedef struct {
        uint16_t pan;
        uint16_t tilt;
    } TurretPosition;

    TurretPosition center = {1500, 1500}; // Start motors in the middle
    
    char msg[100];

    HAL_ADC_Start_DMA(&adc1, (uint32_t*)adc_buffer, 1);

    int pan_dir = 30;
    int tilt_dir = 30;
    uint32_t previous_light = adc_buffer[0];
    uint32_t last_move_time = HAL_GetTick();
    int active_axis = 0; // 0 = Pan, 1 = Tilt

    while (1){
        // Wait 50ms for the servo to finish a smooth, single step
        if (HAL_GetTick() - last_move_time >= 40) { 
            
            uint32_t current_light = adc_buffer[0];

            uint32_t diff = (current_light > previous_light) ? 
                            (current_light - previous_light) : 
                            (previous_light - current_light);

            
            if (diff > 1) {
                if (current_light + 10 < previous_light) { 
                    if (active_axis == 0) pan_dir *= -1; 
                    else tilt_dir *= -1; 
                }

                // Actually move the motors
                if (active_axis == 0) {
                    center.pan += pan_dir;
                    if (center.pan < 500)  { center.pan = 500;  pan_dir *= -1; }
                    if (center.pan > 2500) { center.pan = 2500; pan_dir *= -1; }
                    TIM3->CCR1 = center.pan;
                    active_axis = 1;
                } else {
                    center.tilt += tilt_dir;
                    if (center.tilt < 500)  { center.tilt = 500;  tilt_dir *= -1; }
                    if (center.tilt > 2500) { center.tilt = 2500; tilt_dir *= -1; }
                    TIM3->CCR2 = center.tilt;
                    active_axis = 0; 
                }
            }

            previous_light = current_light;

            snprintf(msg, sizeof(msg), "X: %u | Y: %u | Light: %lu\r\n", center.pan, center.tilt, current_light);
            HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), 100);
            
            last_move_time = HAL_GetTick();
        }
    }

    return 0;
}

