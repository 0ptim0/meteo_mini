#include "stm32_conf.h"

ssd1306 Display;

void Test(void *pvParameters) 
{   
    Display.Init();
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

