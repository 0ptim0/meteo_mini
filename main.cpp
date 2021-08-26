#include "stm32_conf.h"

rcc oRCC;
gpio oGPIO;
i2c oI2C1(I2C1, 100);

uint8_t data[19] = {
    0xAE,
    0xA8,
    0x1F,
    0xD3,
    0x00,
    0x40,
    0xA0,
    0xC0,
    0xDA,
    0x02,
    0x81,
    0x88,
    0xA4,
    0xA6,
    0xD5,
    0x80,
    0x8D,
    0x14,
    0xAF
};

void Test(void *pvParameters) 
{       
    oI2C1.Init();
    oI2C1.Transmit(data, 19, 0x78);
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
