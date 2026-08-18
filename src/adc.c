#include <adc.h>

void ADC_Init(void){
    __HAL_RCC_ADC12_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef adc = {0};

    adc.Pin = GPIO_PIN_0;
    adc.Mode = GPIO_MODE_ANALOG;
    adc.Pull = GPIO_NOPULL;
    adc.Speed = GPIO_SPEED_FREQ_LOW;
    //adc.Alternate = ADC12_COMMON;
    //We don't need an alternate instant because 

    HAL_GPIO_Init(GPIOA, &adc);

    adc1.Instance = ADC1;
    adc1.Init.Resolution = ADC_RESOLUTION_12B; //because ADC register has 12 bits to store data.
    adc1.Init.ScanConvMode = ADC_SCAN_DISABLE; //Since we are workign with only one channel, we can disab;e scannign for multiple channels.
    adc1.Init.ContinuousConvMode = ENABLE; //Triggers multiple readings are once, if DISABLED, it only triggers one reading at a time.
    adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT; // ALIGNS our 12 bit to the right of the 16 bit register instead of the left so that our range is 2^12 and not 2^16
    adc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Allows the software to trigger the conversion on demand.

    // 1. Tell it we are only doing exactly ONE conversion
    adc1.Init.NbrOfConversion = 1;

    // 2. Tell it to wave the "Finished" flag (EOC) after a single channel is done
    adc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    // 3. Divide the system clock so the ADC doesn't run too fast and crash
    adc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;

    HAL_ADC_Init(&adc1);

    HAL_ADCEx_Calibration_Start(&adc1, ADC_SINGLE_ENDED); //Sicne ADC is very fast, this essentially states that ADC can calibrate itself.

    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;

    // HAL_ADC_ConfigChannel(&adc1, &sConfig);


}