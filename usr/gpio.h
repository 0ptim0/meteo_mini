#pragma once
#include "stm32_base.h"
#include "rcc_periph.h"

typedef struct cfg {
    GPIO_TypeDef *GPIO;
    GPIO_InitTypeDef GPIO_InitStructure;
} gpio_cfg_t;

class gpio_base_class
{
public:
    virtual int Init(void) = 0;
};

class gpio_class : public gpio_base_class, public rcc_periph_class
{
protected:
    const gpio_cfg_t *const cfg;
public:
    gpio_class(const gpio_cfg_t *const cfg);
    int Init(void);
    int SetConf(const gpio_cfg_t *const cfg);
    int ClockEnable(void);
};
