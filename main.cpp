#include "stm32_conf.h"

rcc RCC1;
gpio GPIO1;
i2c I2C1_Obj(I2C1, 1024, 100);

void Test(void *pvParameters) 
{       
    I2C1_Obj.Init();
    uint8_t data = 0x12;
    while(1) {
        vTaskDelay(1000);
        I2C1_Obj.Transmit(&data, 1, 0x78);
    }
}


int main(void) 
{
    HAL_Init();
    RCC1.InitClock();
    RCC1.InitPeriph();
    GPIO1.Init();
    xTaskCreate(Test, "Test", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1) {
    }
}

extern "C" {
    void I2C1_EV_IRQHandler()
    {
        I2C1_Obj.EV_Handler();
    }

    void I2C1_ER_IRQHandler() 
    {
        I2C1_Obj.ER_Handler();
    }
}
