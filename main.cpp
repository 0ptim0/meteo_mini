#include "stm32_conf.h"

rcc RCC1;
gpio GPIO1;

int main(void) {
    HAL_Init();
    RCC1.InitClock();
    RCC1.InitPeriph();
    GPIO1.Init();
    while(1) {
    }
}
