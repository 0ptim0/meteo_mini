#include "stm32_base.h"

class i2c 
{
private:
    I2C_HandleTypeDef I2C_InitStructure;
    int timeout;
public:
    i2c(I2C_TypeDef *I2C, int max_timeout_ms) 
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

        I2C_InitStructure.Instance = I2C;
        I2C_InitStructure.Init.ClockSpeed = 100000;
        I2C_InitStructure.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        I2C_InitStructure.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        I2C_InitStructure.Init.DutyCycle = I2C_DUTYCYCLE_2; // used only for FM
        I2C_InitStructure.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        I2C_InitStructure.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    }
    void Init(void);
    int Transmit(uint8_t *pdata, uint16_t length, uint16_t address);
    int EV_Handler(void);
    int ER_Handler(void);

};
