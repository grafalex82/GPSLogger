//Autogenerated file
//MCU name: STM32F373CB
//MCU  xml: STM32F373C(8-B-C)Tx.xml
//GPIO remap/alternate function xml: GPIO-STM32F373_gpio_v1_0_Modes.xml


#define VARIANT_PIN_LIST_DEFAULT \
   PIN(A,0), \
   PIN(A,1), \
   PIN(A,2), \
   PIN(A,3), \
   PIN(A,4), \
   PIN(A,5), \
   PIN(A,6), \
   PIN(A,8), \
   PIN(A,9), \
   PIN(A,10), \
   PIN(A,11), \
   PIN(A,12), \
   PIN(A,13), \
   PIN(A,14), \
   PIN(A,15), \
   PIN(B,0), \
   PIN(B,1), \
   PIN(B,2), \
   PIN(B,3), \
   PIN(B,4), \
   PIN(B,5), \
   PIN(B,6), \
   PIN(B,7), \
   PIN(B,8), \
   PIN(B,9), \
   PIN(B,14), \
   PIN(B,15), \
   PIN(C,13), \
   PIN(C,14), \
   PIN(C,15), \
   PIN(D,8), \
   PIN(E,8), \
   PIN(E,9), \
   PIN(F,0), \
   PIN(F,1), \
   PIN(F,6), \
   PIN(F,7), \



// --------------------ADC--------------------

// --------------------I2C--------------------

const stm32_af_pin_list_type chip_af_i2c_scl [] = {
//I2C1
    { I2C1  , GPIOA, GPIO_PIN_15 , GPIO_AF4_I2C1  }, 
    { I2C1  , GPIOB, GPIO_PIN_6  , GPIO_AF4_I2C1  }, 
    { I2C1  , GPIOB, GPIO_PIN_8  , GPIO_AF4_I2C1  }, 
//I2C2
    { I2C2  , GPIOA, GPIO_PIN_9  , GPIO_AF4_I2C2  }, 
    { I2C2  , GPIOF, GPIO_PIN_6  , GPIO_AF4_I2C2  }, 
}; 

const stm32_af_pin_list_type chip_af_i2c_sda [] = {
//I2C1
    { I2C1  , GPIOA, GPIO_PIN_14 , GPIO_AF4_I2C1  }, 
    { I2C1  , GPIOB, GPIO_PIN_7  , GPIO_AF4_I2C1  }, 
    { I2C1  , GPIOB, GPIO_PIN_9  , GPIO_AF4_I2C1  }, 
//I2C2
    { I2C2  , GPIOA, GPIO_PIN_10 , GPIO_AF4_I2C2  }, 
    { I2C2  , GPIOF, GPIO_PIN_7  , GPIO_AF4_I2C2  }, 
}; 

// --------------------I2S--------------------
#define STM32_CHIP_HAS_I2S

const stm32_af_pin_list_type chip_af_i2s_ck [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_5  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_12 , GPIO_AF6_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_3  , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOA, GPIO_PIN_8  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_8  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOD, GPIO_PIN_8  , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOA, GPIO_PIN_1  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOB, GPIO_PIN_3  , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_mck [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_6  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_13 , GPIO_AF6_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_4  , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOA, GPIO_PIN_9  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_14 , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOA, GPIO_PIN_2  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOB, GPIO_PIN_4  , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_sd [] = {
//I2S1
    { SPI1  , GPIOB, GPIO_PIN_0  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_5  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOF, GPIO_PIN_6  , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOA, GPIO_PIN_10 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_15 , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOA, GPIO_PIN_3  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOB, GPIO_PIN_5  , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_ws [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_4  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_11 , GPIO_AF6_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_15 , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOA, GPIO_PIN_11 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_9  , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOA, GPIO_PIN_4  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOA, GPIO_PIN_15 , GPIO_AF6_SPI3  }, 
}; 

// --------------------SPI--------------------

const stm32_af_pin_list_type chip_af_spi_miso [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_6  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_13 , GPIO_AF6_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_4  , GPIO_AF5_SPI1  }, 
//SPI2
    { SPI2  , GPIOA, GPIO_PIN_9  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_14 , GPIO_AF5_SPI2  }, 
//SPI3
    { SPI3  , GPIOA, GPIO_PIN_2  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOB, GPIO_PIN_4  , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_spi_mosi [] = {
//SPI1
    { SPI1  , GPIOB, GPIO_PIN_0  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_5  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOF, GPIO_PIN_6  , GPIO_AF5_SPI1  }, 
//SPI2
    { SPI2  , GPIOA, GPIO_PIN_10 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_15 , GPIO_AF5_SPI2  }, 
//SPI3
    { SPI3  , GPIOA, GPIO_PIN_3  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOB, GPIO_PIN_5  , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_spi_nss [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_4  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_11 , GPIO_AF6_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_15 , GPIO_AF5_SPI1  }, 
//SPI2
    { SPI2  , GPIOA, GPIO_PIN_11 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_9  , GPIO_AF5_SPI2  }, 
//SPI3
    { SPI3  , GPIOA, GPIO_PIN_4  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOA, GPIO_PIN_15 , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_spi_sck [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_5  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_12 , GPIO_AF6_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_3  , GPIO_AF5_SPI1  }, 
//SPI2
    { SPI2  , GPIOA, GPIO_PIN_8  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_8  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOD, GPIO_PIN_8  , GPIO_AF5_SPI2  }, 
//SPI3
    { SPI3  , GPIOA, GPIO_PIN_1  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOB, GPIO_PIN_3  , GPIO_AF6_SPI3  }, 
}; 

// --------------------TIM--------------------

// --------------------TIM1--------------------

// --------------------USART--------------------

const stm32_af_pin_list_type chip_af_usart_rx [] = {
//USART1
    { USART1, GPIOA, GPIO_PIN_10 , GPIO_AF7_USART1}, 
    { USART1, GPIOB, GPIO_PIN_7  , GPIO_AF7_USART1}, 
//USART2
    { USART2, GPIOA, GPIO_PIN_3  , GPIO_AF7_USART2}, 
    { USART2, GPIOB, GPIO_PIN_4  , GPIO_AF7_USART2}, 
//USART3
    { USART3, GPIOB, GPIO_PIN_9  , GPIO_AF7_USART3}, 
}; 

const stm32_af_pin_list_type chip_af_usart_tx [] = {
//USART1
    { USART1, GPIOA, GPIO_PIN_9  , GPIO_AF7_USART1}, 
    { USART1, GPIOB, GPIO_PIN_6  , GPIO_AF7_USART1}, 
//USART2
    { USART2, GPIOA, GPIO_PIN_2  , GPIO_AF7_USART2}, 
    { USART2, GPIOB, GPIO_PIN_3  , GPIO_AF7_USART2}, 
//USART3
    { USART3, GPIOB, GPIO_PIN_8  , GPIO_AF7_USART3}, 
    { USART3, GPIOD, GPIO_PIN_8  , GPIO_AF7_USART3}, 
}; 

const stm32_chip_adc1_channel_type chip_adc1_channel[] = {
    { GPIOA, GPIO_PIN_0  , ADC_CHANNEL_0  }, 
    { GPIOA, GPIO_PIN_1  , ADC_CHANNEL_1  }, 
    { GPIOA, GPIO_PIN_2  , ADC_CHANNEL_2  }, 
    { GPIOA, GPIO_PIN_3  , ADC_CHANNEL_3  }, 
    { GPIOA, GPIO_PIN_4  , ADC_CHANNEL_4  }, 
    { GPIOA, GPIO_PIN_5  , ADC_CHANNEL_5  }, 
    { GPIOA, GPIO_PIN_6  , ADC_CHANNEL_6  }, 
    { GPIOB, GPIO_PIN_0  , ADC_CHANNEL_8  }, 
    { GPIOB, GPIO_PIN_1  , ADC_CHANNEL_9  }, 
};

const stm32_clock_freq_list_type chip_clock_freq_list[] = {
    {I2C1  , HAL_RCC_GetPCLK1Freq },  
    {I2C2  , HAL_RCC_GetPCLK1Freq },  
    {SPI2  , HAL_RCC_GetPCLK1Freq },  
    {SPI3  , HAL_RCC_GetPCLK1Freq },  
    {USART2, HAL_RCC_GetPCLK1Freq },  
    {USART3, HAL_RCC_GetPCLK1Freq },  

    {SPI1  , HAL_RCC_GetPCLK2Freq },  
    {USART1, HAL_RCC_GetPCLK2Freq },  
};
