#include "stm32_conf.h"

#define PIN_I2C_SCL {GPIOB, {GPIO_PIN_6, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_I2C_SDA {GPIOB, {GPIO_PIN_7, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
const gpio_cfg_t scl = PIN_I2C_SCL;
const gpio_cfg_t sda = PIN_I2C_SDA;
const i2c_cfg_t i2c1_cfg = {I2C1, &scl, &sda, 100000, 100};
i2c_class i2c1(&i2c1_cfg);


void Test(void *pvParameters) 
{
    i2c1.Init();
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

