#include <./uart_buffer.h>
#include <./timer.h>

void UART_init(void){
    __HAL_RCC_LPUART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef Uart = {0};

    Uart.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    Uart.Mode = GPIO_MODE_AF_PP;
    Uart.Pull = GPIO_NOPULL;
    Uart.Speed = GPIO_SPEED_FREQ_LOW;
    Uart.Alternate = GPIO_AF12_LPUART1;

    HAL_GPIO_Init(GPIOA, &Uart);

    hlpuart1.Instance = LPUART1;
    hlpuart1.Init.BaudRate = 115200;
    hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1.Init.StopBits = UART_STOPBITS_1;
    hlpuart1.Init.Parity = UART_PARITY_NONE;
    hlpuart1.Init.Mode = UART_MODE_TX_RX;

    HAL_UART_Init(&hlpuart1);

    HAL_NVIC_SetPriority(LPUART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LPUART1_IRQn);

    //Basically does the same as this LPUART1->CR1 |= USART_CR1_RXNEIE; It basically turns on the Not empty bit, indicating that someting has been recieved.
    __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_RXNE);
}

void LPUART1_IRQHandler(void) {
    if(LPUART1->ISR & USART_ISR_RXNE) {
        rx_data = LPUART1 -> RDR;

        if (rx_data == '1') {
            //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            TIM3->CCR2 = 2500;
        } else if (rx_data == '0') {
            //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            TIM3->CCR2 = 500;
        }

        LPUART1->TDR = rx_data;
    }
    
}
