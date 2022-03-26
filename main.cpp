#include "stm32_conf.h"

ssd1306_class ssd1306(&ssd1306_cfg);
bme280_class bme280(&bme280_cfg, &ssd1306.i2c);
rcc_class rcc;

void Test(void *pvParameters)
{
    rcc.InitClock();
    ssd1306.Init();
    bme280.Init();
    ssd1306.Clear();
    ssd1306.SetCursor(0,34);
    ssd1306.Print("METEO-mini");
    ssd1306.SetCursor(1,10);
    ssd1306.Print("Temperature");
    ssd1306.SetCursor(2,10);
    ssd1306.Print("Humidity");
    ssd1306.SetCursor(3,10);
    ssd1306.Print("Pressure");
    bme280.Measure();

    while(1) {
        bme280.Measure();
        vTaskDelay(1000);

        ssd1306.SetCursor(1,90);
        ssd1306.Print("     ");
        ssd1306.SetCursor(1,90);
        ssd1306.Print(bme280.meas.temp);

        ssd1306.SetCursor(2,90);
        ssd1306.Print("     ");
        ssd1306.SetCursor(2,90);
        ssd1306.Print(bme280.meas.humi);

        ssd1306.SetCursor(3,90);
        ssd1306.Print("     ");
        ssd1306.SetCursor(3,90);
        ssd1306.Print(bme280.meas.pres);
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
