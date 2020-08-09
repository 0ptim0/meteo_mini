#include <stm32f4xx.h>
#include <BME280.h>
#include <clock.h>

/*Calibration value*/
uint16_t dig_T1 =   0x6EE8;
int16_t dig_T2 =    0x68EE;
int16_t dig_T3 =    0x0032;
uint16_t dig_P1 =   0x8B85;
int16_t dig_P2 =    0xD65B;
int16_t dig_P3 =    0x0BD0;
int16_t dig_P4 =    0x1F0D;
int16_t dig_P5 =    0xFFC7;
int16_t dig_P6 =    0xFFF9;
int16_t dig_P7 =    0x300C;
int16_t dig_P8 =    0xD120;
int16_t dig_P9 =    0x1388;
uint8_t dig_H1 =    0x4B;
int16_t dig_H2 =    0x0157;
uint8_t dig_H3 =    0x00;
int16_t dig_H4 =    (0x16 << 4) | (0x2E & 0b1111);
int16_t dig_H5 =    (0x03 << 4) | ((0x2E & 0b11110000) >> 4);
signed char dig_H6 = 0x1E;

typedef long long signed int int64;
int32_t temper_int;

/* BUG: after reset SDA line down
    This function is reset BME after line up
	PB15 - power for BME280 module */
void fix_bug_i2c(void) {
    GPIOB->MODER    &= ~(GPIO_MODER_MODE15);
    GPIOB->MODER    |= GPIO_MODER_MODE15_0;
    GPIOB->OTYPER   &= ~GPIO_OTYPER_OT15;
    GPIOB->OSPEEDR  &= ~GPIO_OSPEEDR_OSPEED15;
    GPIOB->PUPDR    &= ~GPIO_PUPDR_PUPDR15;
    GPIOB->BSRR     |= GPIO_BSRR_BR15;
}

void I2C_Init() {
    /* GPIO Initialization 
    PB8, PB9 - AF OD High Speed, Pull Up 
    PB8 - SCL, PB9 - SDA
    PB15 - power*/
    RCC->AHB1ENR    |= RCC_AHB1ENR_GPIOBEN;
    fix_bug_i2c();
    GPIOB->MODER    &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
    GPIOB->MODER    |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
    GPIOB->OTYPER   |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;
    GPIOB->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9;
    GPIOB->PUPDR    |= GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0;
    GPIOB->AFR[1]   |= GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_2;
    GPIOB->BSRR     |= GPIO_BSRR_BS_8 | GPIO_BSRR_BS_9;
    GPIOB->BSRR     |= GPIO_BSRR_BS15;
    delay(1000);
    RCC->APB1ENR    |= RCC_APB1ENR_I2C1EN;
    /* I2C module Initialization
    APB1 - 42 MHz
    Standart mode
    CR2 - 42 MHz
    CCR*Tpclk = 5000ns */
    I2C1->CR1       |= I2C_CR1_SWRST;
    I2C1->CR1       &= ~I2C_CR1_SWRST;
    I2C1->CR2       |= 42;
    I2C1->CCR       |= 210;
    I2C1->TRISE     |= 43;
    I2C1->CR1       |= I2C_CR1_PE;
}

void start(_Bool RW) {
    I2C1->CR1   |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB)){}
    (void)I2C1->SR1;
    I2C1->DR    = (0x76<<1) | RW;
    while(!(I2C1->SR1 & I2C_SR1_ADDR)){}
    (void)I2C1->SR1;
    (void)I2C1->SR2;
}

void stop() {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void write(uint8_t reg, uint8_t data) {
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}
    I2C1->DR    = reg;
    while(I2C1->SR1 & I2C_SR1_BTF){}
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}
    I2C1->DR    = data;
    while(I2C1->SR1 & I2C_SR1_BTF){}
}

void BME_Init() {
    /* Initialization sequence
    oversampling p,t,h = 1
    filter coef = 2 */
    start(W);
    write(ctrl_h, res_1);
    write(ctrl_meas, (res_1 << 5) | (res_1 << 2) | forced);
    write(config, filt << 0);
    stop();
}

void BME_reset() {
    start(W);
    write(reset, com_reset);
    stop();
    delay(1000);
}

/*Function from find calibration value*/
/*void sbor(uint32_t *data, int reg) {
    start(W);
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}
    I2C1->DR    = reg;
    while(I2C1->SR1 & I2C_SR1_BTF){}
    start(R);
    I2C1->CR1 |= I2C_CR1_ACK;
    for(int i = 0; i < 25; i++) {
        while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
        data[i] = (I2C1->DR);
    }
    while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_STOP;
    data[25] = (I2C1->DR);
    while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
    data[26] = (I2C1->DR);
}*/


int32_t T_compensation(int adc_T) {
    int32_t T;
    int32_t val1, val2;
    val1 = ((((adc_T>>3)-((int32_t)dig_T1<<1)))*
            ((int32_t)dig_T2)) >> 11;
    val2 = (((((adc_T>>4)-((int32_t)dig_T1))*
            ((adc_T>>4)-((int32_t)dig_T1)))>>12)*
            ((int32_t)dig_T3)) >> 14;
    temper_int = val1 + val2;
    T = (temper_int * 5 + 128) >> 8;
    return T;
}

uint32_t P_compensation(int adc_P) {
    int64 P, var1, var2; 
    var1 = ((int64)temper_int) - 128000;
    var2 = var1 * var1 * (int64)dig_P6;
    var2 = var2 + ((var1*(int64)dig_P5)<<17);
    var2 = var2 + (((int64)dig_P4)<<35);
    var1 = ((var1 * var1 * (int64)dig_P3)>>8) + 
    ((var1 * (int64)dig_P2)<<12);
    var1 = (((((int64)1)<<47)+var1))*((int64)dig_P1)>>33;
    if(var1 == 0) {
        return 0;
    }
    P = 1048576 - adc_P;
    P = (((P<<31) - var2) * 3125) / var1;
    var1 = (((int64)dig_P9) * (P>>13) * (P>>13))>>25;
    var2 = (((int64)dig_P8) * P) >> 19;
    P = ((P + var1 + var2) >> 8) + (((int64)dig_P7)<<4);
    return (uint32_t)P;
}

uint32_t H_compensation(int adc_H) {
    int32_t H;
    H = (temper_int - ((int32_t)76800));
    H = (((((adc_H<<14)-(((int32_t)dig_H4)<<20)- (((int32_t)dig_H5) * 
    H)) + ((int32_t)16384))>>15) * (((((((H *
    ((int32_t)dig_H6))>>10) * (((H * ((int32_t)dig_H3)) >> 11) +
    ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) +
    8192)>>14));
    H = (H - (((((H>>15) * (H>>15))>>7)*
        ((int32_t)dig_H1))>>4));
    H = (H < 0 ? 0 : H);
    H = (H > 419430400 ? 419430400 : H);
    return (uint32_t)(H>>12);
}

void BME_Measure(int32_t *T, uint32_t *P, uint32_t *H) {
    uint32_t data[3];
    start(W);
    write(ctrl_h, res_1);
    write(ctrl_meas, (res_1 << 5) | (res_1 << 2) | forced);
    write(config, filt << 0);
    stop();
    start(W);
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}
    I2C1->DR    = start_reg;
    while(I2C1->SR1 & I2C_SR1_BTF){}
    start(R);
    I2C1->CR1 |= I2C_CR1_ACK;
    for(int i = 0; i < 2; i++) {
        data[i] = 0;
        data[2] = 0;
        while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
        data[i] |= (I2C1->DR) << (20 - 8);
        while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
        data[i] |= (I2C1->DR) << (20 - 16);
        while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
        data[i] |= (I2C1->DR) << (20 - 20);
    }
    while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_STOP;
    data[2] |= (I2C1->DR) << (16 - 8);
    while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
    data[2] |= (I2C1->DR) << (16 - 16);
    *T = T_compensation(data[1]);
    *P = P_compensation(data[0]);
    *H = H_compensation(data[2]);      
}

void BME_Begin() {
    I2C_Init();
    BME_reset();
    BME_Init();
    delay(1*ms);
}
