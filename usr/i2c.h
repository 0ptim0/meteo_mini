#include "stm32_base.h"

class i2c {
private:
    I2C_HandleTypeDef I2C_InitStructure;
    int timeout;
    int length;
public:
    i2c(I2C_TypeDef *I2C, int max_length, int max_timeout) {
        timeout = max_timeout;
        length = max_length;
        I2C_InitStructure.Instance = I2C;
        I2C_InitStructure.Init.ClockSpeed = I2C_SPEED_STANDARD(72000000, 100000);
    }
    void Init();
    int Transmit(uint8_t *pdata, uint16_t length);
    int Handle();
};
