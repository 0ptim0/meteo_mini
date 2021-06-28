#include "stm32_conf.h"

rcc Rcc1;

void Test(void *pvParameters) {
    while(1) {
        
    }
}

int main(void) {
    HAL_Init();
    Rcc1.Init();
    xTaskCreate(Test, "test", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1) {
    }
}
