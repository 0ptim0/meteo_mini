#include "stm32_base.h"
#include "rcc_periph.h"
#include "gpio.h"

typedef struct i2c_cfg {
    I2C_HandleTypeDef I2C_InitStructure;
    gpio_class gpio;
    int timeout;
} i2c_cfg_t;

class i2c_class : public rcc_periph_class, public gpio_class
{
private:
    i2c_cfg_t *i2c_cfg;
public:
    i2c_class(I2C_TypeDef *I2C, uint16_t SCL_PIN, uint16_t SCL_PORT, 
                uint16_t SDA_PIN, uint16_t SDA_PORT, int max_timeout_ms) 
    {
        
        NVIC_SetPriorityGrouping(0);
        if(I2C == I2C1) {
            NVIC_EnableIRQ(I2C1_EV_IRQn);
            NVIC_EnableIRQ(I2C1_ER_IRQn);
            NVIC_SetPriority(I2C1_EV_IRQn, 11);
            NVIC_SetPriority(I2C1_ER_IRQn, 11);
        } else {
            NVIC_EnableIRQ(I2C2_EV_IRQn);
            NVIC_EnableIRQ(I2C2_ER_IRQn);
            NVIC_SetPriority(I2C2_EV_IRQn, 11);
            NVIC_SetPriority(I2C2_ER_IRQn, 11);
        }

        i2c_cfg->I2C_InitStructure.Instance = I2C;
        i2c_cfg->I2C_InitStructure.Init.ClockSpeed = 100000;
        i2c_cfg->I2C_InitStructure.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        i2c_cfg->I2C_InitStructure.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        i2c_cfg->I2C_InitStructure.Init.DutyCycle = I2C_DUTYCYCLE_2; // used only for FM
        i2c_cfg->I2C_InitStructure.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        i2c_cfg->I2C_InitStructure.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    }
    int Init(void);
    int Transmit(uint8_t *pdata, uint16_t length, uint16_t address);
    int ClockEnable(void);
    int EV_Handler(void);
    int ER_Handler(void);

};
