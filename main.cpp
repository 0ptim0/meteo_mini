#include "stm32_conf.h"

ssd1306_class ssd1306(&ssd1306_cfg);

void Test(void *pvParameters) 
{
    ssd1306.Init();
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

extern "C" {
    void I2C1_EV_IRQHandler()
    {
        ssd1306.i2c.EV_Handler();
    }

    void I2C1_ER_IRQHandler() 
    {
        ssd1306.i2c.ER_Handler();
    }
}

