#include "stm32_base.h"
#include "rcc.h"

void rcc_class::InitClock() 
{
    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStructure.HSIState = RCC_HSI_ON;
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStructure.PLL.PLLMUL = RCC_PLL_MUL2;
    while(HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK);
    
    RCC_ClkInitStructure.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV1;
    while(HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_2) != HAL_OK);
}
