#include "stm32_conf.h"

#define PIN_I2C_SCL {GPIOB, {GPIO_PIN_6, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_I2C_SDA {GPIOB, {GPIO_PIN_7, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
const gpio_cfg_t scl_cfg = PIN_I2C_SCL;
const gpio_cfg_t sda_cfg = PIN_I2C_SDA;
gpio_class scl(&scl_cfg);
gpio_class sda(&sda_cfg);

void Test(void *pvParameters) 
{
    scl.Init();
    sda.Init();
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

// extern "C" {
//     void I2C1_EV_IRQHandler()
//     {
//         ssd1306.cfg->i2c.EV_Handler();
//     }

//     void I2C1_ER_IRQHandler() 
//     {
//         ssd1306.cfg->i2c.ER_Handler();
//     }
// }

