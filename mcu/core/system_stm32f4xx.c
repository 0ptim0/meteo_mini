#include "stm32f4xx.h"

/* Config SYSCLK = 84 MHz */
#define PLL_M 4   // 1 - 2 MHz optimal
#define PLL_N 168 // N mim 2
#define PLL_P 4   // P min 2
#define PLL_Q 7   // 48 MHz on USB
uint32_t SystemCoreClock = 84000000;

void SystemInit(void) {
    /* FPU settings ------------------------------------------------------------*/
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
    #endif
    /* Reset the RCC clock configuration to the default reset state ------------*/
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24003010;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;

    /* Enable HSE */
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));

    /* Configure AHB, APB  */
    RCC->CFGR &= ~RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* Configure PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (PLL_Q << 24) |
                    RCC_PLLCFGR_PLLSRC | (RCC_PLLCFGR_PLLP_0);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

    /* Configure FLASH */
    FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN |
                FLASH_ACR_LATENCY_2WS | FLASH_ACR_PRFTEN;

    /* Clock switch */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while(!(RCC->CFGR & RCC_CFGR_SWS));
}
