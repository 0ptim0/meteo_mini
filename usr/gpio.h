#include "stm32_base.h"

class gpio 
{
private:
    GPIO_InitTypeDef GPIO_InitStructure;
public:
    void Init();
};