#pragma once
#include "stm32_base.h"
#include "rcc_periph.h"
#include "gpio.h"

enum class state_t {
    Done = 0,
    Busy,
    Timeout,
    Error,
};

struct i2c_cfg {
    I2C_HandleTypeDef I2C_InitStructure;
    gpio_class SCL;
    gpio_class SDA;
    state_t state;
    int timeout;
};

class i2c_class : public rcc_periph_class
{
public:
    i2c_cfg *cfg;
    i2c_class(i2c_cfg *cfg) : cfg(cfg) { };
    int Init(i2c_cfg *cfg);
    int Init(I2C_TypeDef *I2C);
    int ClockEnable(void);
    int Transmit(uint8_t *pdata, uint16_t length, uint16_t address);
    int EV_Handler(void);
    int ER_Handler(void);
};
