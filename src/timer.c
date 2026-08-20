#include <./timer.h>

void Timer_Init(void) {
    __HAL_RCC_TIM3_CLK_ENABLE();

    htim3.Instance = TIM3;
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

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef Pwm_channel1 = {0};
    Pwm_channel1.Pin = GPIO_PIN_7; //PIN D11
    Pwm_channel1.Pull = GPIO_NOPULL;
    Pwm_channel1.Mode = GPIO_MODE_AF_PP;
    Pwm_channel1.Speed = GPIO_SPEED_FREQ_LOW;
    Pwm_channel1.Alternate = GPIO_AF2_TIM3;

    HAL_GPIO_Init(GPIOA, &Pwm_channel1); 

    GPIO_InitTypeDef Pwm_channel2 = {0};
    Pwm_channel2.Pin = GPIO_PIN_8; //PIN D1
    Pwm_channel2.Pull = GPIO_NOPULL;
    Pwm_channel2.Mode = GPIO_MODE_AF_PP;
    Pwm_channel2.Speed = GPIO_SPEED_FREQ_LOW;
    Pwm_channel2.Alternate = GPIO_AF2_TIM3;

    HAL_GPIO_Init(GPIOC, &Pwm_channel2);

}