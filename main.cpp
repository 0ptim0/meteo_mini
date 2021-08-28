#include "stm32_conf.h"

rcc oRCC;
gpio oGPIO;
i2c oI2C1(I2C1, 100);
ssd1306 Display;

void Test(void *pvParameters) 
{   
    oI2C1.Init();
    Display.Init();
    while(1) {
        vTaskDelay(1000);
    }
}

int main(void) 
{
    HAL_Init();
    oRCC.InitClock();
    oRCC.InitPeriph();
    oGPIO.Init();
    xTaskCreate(Test, "Test", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1) {
    }
}

extern "C" {
    void I2C1_EV_IRQHandler()
    {
        oI2C1.EV_Handler();
    }

    void I2C1_ER_IRQHandler() 
    {
        oI2C1.ER_Handler();
    }
}
