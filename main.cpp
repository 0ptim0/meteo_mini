#include "stm32_conf.h"

ssd1306_class ssd1306;
gpio_class gpio1;

void Test(void *pvParameters) 
{   
    gpio1.Init();
    ssd1306.cfg->i2c.cfg->I2C_InitStructure.Instance = I2C1;
    ssd1306.cfg->i2c.cfg->SCL.Init(GPIOB, GPIO_PIN_6, GPIO_MODE_AF_OD, GPIO_SPEED_FREQ_HIGH);
    ssd1306.cfg->i2c.cfg->SDA.Init(GPIOB, GPIO_PIN_7, GPIO_MODE_AF_OD, GPIO_SPEED_FREQ_HIGH);
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
        ssd1306.cfg->i2c.EV_Handler();
    }

    void I2C1_ER_IRQHandler() 
    {
        ssd1306.cfg->i2c.ER_Handler();
    }
}

