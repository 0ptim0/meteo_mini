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

struct i2c_cfg_t {
    I2C_TypeDef *I2C;
    const gpio_cfg_t *scl_cfg;
    const gpio_cfg_t *sda_cfg;
    int speed;
    int timeout;
};

class i2c_class : public rcc_periph_class
{
private:
    const i2c_cfg_t *const cfg;
    I2C_HandleTypeDef I2C_InitStructure;
    gpio_class SCL;
    gpio_class SDA;
    state_t state;
public:
    i2c_class(const i2c_cfg_t *const cfg);
    int Init(void);
    int ClockEnable(void);
    int Transmit(uint8_t *pdata, uint16_t length, uint16_t address);
    int EV_Handler(void);
    int ER_Handler(void);
};
