#include "stm32_base.h"

class i2c 
{
private:
    I2C_HandleTypeDef I2C_InitStructure;
    int timeout;
    int length;
public:
    i2c(I2C_TypeDef *I2C, int max_length, int max_timeout) 
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

        timeout = max_timeout;
        length = max_length;

        I2C_InitStructure.Instance = I2C;
        I2C_InitStructure.Init.ClockSpeed = 100000;
        I2C_InitStructure.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        I2C_InitStructure.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        I2C_InitStructure.Init.DutyCycle = I2C_DUTYCYCLE_2; // used only for FM
        I2C_InitStructure.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        I2C_InitStructure.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        HAL_I2C_Init(&I2C_InitStructure);
        LL_I2C_SetPeriphClock(I2C1, 42000000);
        LL_I2C_SetClockSpeedMode(I2C1, LL_I2C_CLOCK_SPEED_STANDARD_MODE);
        LL_I2C_SetClockPeriod(I2C1, 420);
        LL_I2C_SetRiseTime(I2C1, 43);
    }
    void Init(void);
    int Transmit(uint8_t *pdata, uint16_t length, uint16_t address);
    int EV_Handler(void);
    int ER_Handler(void);

};
