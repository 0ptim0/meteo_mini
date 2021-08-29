#pragma once
#include "stm32_base.h"
#include "rcc_periph.h"

typedef struct cfg {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_TypeDef *GPIO;
} gpio_cfg_t;

class gpio_base_class
{
protected:
    gpio_cfg_t *gpio_cfg;
public:
    virtual int Init(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN, uint16_t GPIO_MODE, uint16_t GPIO_SPEED) = 0;
};

class gpio_class : public gpio_base_class, public rcc_periph_class
{
public:
    gpio_class() { };
    int Init(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN, uint16_t GPIO_MODE, uint16_t GPIO_SPEED);
    int ClockEnable(void);
};
