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


    typedef enum {
    SCAN_LEFT,
    WAIT_LEFT,
    SCAN_RIGHT,
    WAIT_RIGHT,
    SCAN_UP,
    WAIT_UP,
    SCAN_DOWN,
    WAIT_DOWN,
    CALCULATE_CENTER
    } TurretState;

    TurretState current_state = SCAN_LEFT;

    // Your excellent struct ideas:
    typedef struct {
        uint32_t left;
        uint32_t right;
        uint32_t up;
        uint32_t down;
    } LightReadings;

    typedef struct {
        uint16_t pan;
        uint16_t tilt;
    } TurretPosition;

    LightReadings readings = {0};
    TurretPosition center = {1500, 1500}; // Start motors in the middle
    uint32_t move_start_time = 0;
    const uint32_t MOTOR_SETTLE_TIME = 30;
    
    char msg[100];

    HAL_ADC_Start_DMA(&adc1, (uint32_t*)adc_buffer, 1);

    while (1){
        switch(current_state) {
            case SCAN_LEFT:
                // 1. Command the motor
                TIM3->CCR3 = center.pan - 20;
                
                // 2. Start the stopwatch
                move_start_time = HAL_GetTick();
                
                // 3. Move to the waiting state
                current_state = WAIT_LEFT;
                break;

            case WAIT_LEFT:
                if ((HAL_GetTick() - move_start_time) >= MOTOR_SETTLE_TIME) {
                    readings.left = adc_buffer[0];
                    
                    current_state = SCAN_RIGHT;
                }
                break;
            
            case SCAN_RIGHT:
                TIM3->CCR3 = center.pan + 20;

                move_start_time = HAL_GetTick();

                current_state = WAIT_RIGHT;
                break;

            case WAIT_RIGHT:
                if ((HAL_GetTick() - move_start_time) >= MOTOR_SETTLE_TIME) {
                    readings.right = adc_buffer[0];
                    
                    current_state = SCAN_UP;
                }
                break;
            
            case SCAN_UP:
                TIM3->CCR2 = center.pan + 20;

                move_start_time = HAL_GetTick();

                current_state = WAIT_UP;
                break;
            
            case WAIT_UP:
                if ((HAL_GetTick() - move_start_time) >= MOTOR_SETTLE_TIME) {
                    readings.up = adc_buffer[0];
                    
                    current_state = SCAN_DOWN;
                }
                break;
            
            case SCAN_DOWN:
                TIM3->CCR2 = center.pan - 20;

                move_start_time = HAL_GetTick();

                current_state = WAIT_DOWN;
                break;
            
            case WAIT_DOWN:
                if ((HAL_GetTick() - move_start_time) >= MOTOR_SETTLE_TIME) {
                    readings.down = adc_buffer[0];
                    
                    current_state = CALCULATE_CENTER;
                }
                break;

            case CALCULATE_CENTER:
                // 1. Evaluate Pan (Left/Right)
                if (readings.left > readings.right + 15) {
                    center.pan -= 10; // Move center left
                } else if (readings.right > readings.left + 15) {
                    center.pan += 10; // Move center right
                }

                // 2. Evaluate Tilt (Up/Down)
                if (readings.up > readings.down + 15) {
                    center.tilt += 10; // Move center up
                } else if (readings.down > readings.up + 15) {
                    center.tilt -= 10; // Move center down
                }
            
                TIM3 -> CCR3 = center.pan;
                TIM3 -> CCR2 = center.tilt;

                HAL_Delay(10);

                snprintf(msg, sizeof(msg), "X: %d | Y: %d | left: %ld | right: %ld | up: %ld | down: %ld\r\n", center.pan , center.tilt, readings.left, readings.right, readings.up, readings.down);

                HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), 100);

                current_state = SCAN_LEFT;
                break;
                
        }
        
    }

    return 0;
}

