#include <./timer.h>

void Timer_Init(void) {
    htim3.Instance = TIM2;
    htim3.Init.Prescaler = 15;
    htim3.Init.Period = 19999;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&htim3);

    TIM_OC_InitTypeDef sConfigOC = {0};

    sConfigOC.Pulse = 1500;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitTypeDef Pwm_channel = {0};
    Pwm_channel.Pin = GPIO_PIN_7;
    Pwm_channel.Pull = GPIO_NOPULL;
    Pwm_channel.Mode = GPIO_MODE_AF_PP;
    Pwm_channel.Speed = GPIO_SPEED_FREQ_LOW;
    Pwm_channel.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &Pwm_channel); 

}