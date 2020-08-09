#include <stm32f4xx.h>
#include <stdio.h>
char out[255];

void Clock_Init() {
    /* TIM2 from delay */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* TIM3 from ON/OFF interrupt */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 8399;
    TIM3->ARR = 10000;
    TIM3->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM3_IRQn);
}


void delay(int us) {
    TIM2->PSC = 83;
    TIM2->ARR = us;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->SR &= ~TIM_SR_UIF;
    while(!(TIM2->SR & TIM_SR_UIF));
    TIM2->CR1 &= ~(TIM_CR1_CEN);
}

char* converse(float d) {
    sprintf(out, "%.1f", d);
    return out;
}
