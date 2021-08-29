#include "stm32_conf.h"


void Test(void *pvParameters) 
{   
    while(1) {
        vTaskDelay(1000);
    }
}

int main(void) 
{
    HAL_Init();
    xTaskCreate(Test, "Test", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1) {
    }
}

