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
    }
    void Init();
    int Transmit(uint8_t *pdata, uint16_t length);
    int Handle();
};
