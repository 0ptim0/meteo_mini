#include "stm32_conf.h"

ssd1306_class ssd1306(&ssd1306_cfg);

void Test(void *pvParameters) 
{
    ssd1306.Init();
    ssd1306.Clear();
    ssd1306.SetCursor(0,34);
    ssd1306.Print("METEO-mini");
    ssd1306.SetCursor(1,10);
    ssd1306.Print("Temperature");
    ssd1306.SetCursor(2,10);
    ssd1306.Print("Humidity");
    ssd1306.SetCursor(3,10);
    ssd1306.Print("Pressure");
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

extern "C" { // TODO: create handler
    void I2C1_EV_IRQHandler()
    {
        ssd1306.i2c.EV_Handler();
    }

    void I2C1_ER_IRQHandler() 
    {
        ssd1306.i2c.ER_Handler();
    }
}

