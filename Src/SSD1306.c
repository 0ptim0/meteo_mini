#include <stm32f4xx.h>
#include <SSD1306.h>
#include <clock.h>
#include <font.h>

_Bool font = font8;
_Bool first = 1;
void SSD1306_print(char *print);

void I2C2_Init() {
    /* GPIO Initialization 
    PB10 - SCL, PB3 - SDA -> AF OD High Speed, Pull Up */
    RCC->AHB1ENR    |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER    &= ~(GPIO_MODER_MODE3 | GPIO_MODER_MODE10);
    GPIOB->MODER    |= (GPIO_MODER_MODE3_1 | GPIO_MODER_MODE10_1);
    GPIOB->OTYPER   |= GPIO_OTYPER_OT3 | GPIO_OTYPER_OT10;
    GPIOB->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR10;
    GPIOB->PUPDR    |= GPIO_PUPDR_PUPD3_0 | GPIO_PUPDR_PUPD10_0;
    GPIOB->AFR[1]   |= GPIO_AFRH_AFSEL10_2;
    GPIOB->AFR[0]   |= GPIO_AFRL_AFSEL3_3 | GPIO_AFRL_AFSEL3_0;
    RCC->APB1ENR    |= RCC_APB1ENR_I2C2EN;
    /* I2C module Initialization
    APB1 - 42 MHz
    Standart mode
    CR2 - 42 MHz
    CCR*Tpclk = 5000ns */
    I2C2->CR1       |= I2C_CR1_SWRST;
    I2C2->CR1       &= ~I2C_CR1_SWRST;
    I2C2->CR2       |= 42;
    I2C2->CCR       |= 210;
    I2C2->TRISE     |= 43;
    I2C2->CR1       |= I2C_CR1_PE;
}

void I2C2_start(_Bool RW) {
    delay(100*us);
    I2C2->CR1   |= I2C_CR1_START;
    while(!(I2C2->SR1 & I2C_SR1_SB)){}
    (void)I2C2->SR1;
    I2C2->DR    = (add_SSD1306 | RW);
    while(!(I2C2->SR1 & I2C_SR1_ADDR)){}
    (void)I2C2->SR1;
    (void)I2C2->SR2;
    delay(1);
    first = 1;
}

void I2C2_stop() {
    delay(100*us);
    I2C2->CR1 |= I2C_CR1_STOP;
}

void I2C2_write(uint8_t data, _Bool DC) {
    while(!(I2C2->SR1 & I2C_SR1_TXE)){}
    if(DC == 1) {
        if(first == 1) {
            I2C2->DR    = 0x40;
            while(I2C2->SR1 & I2C_SR1_BTF){}
            first = 0;
            }
    }
    else {
        I2C2->DR    = 0x80;
        while(I2C2->SR1 & I2C_SR1_BTF){}         
    }
    while(!(I2C2->SR1 & I2C_SR1_TXE)){}
    I2C2->DR    = data;
    while(I2C2->SR1 & I2C_SR1_BTF){}
}


void SSD1306_Init() {
    I2C2_start(W);
    I2C2_write(0xAE,C);
    I2C2_write(0xA8, C);
        I2C2_write(0x1F, C);
    I2C2_write(0xD3, C);
        I2C2_write(0x00, C);
    I2C2_write(0x40, C);
    I2C2_write(0xA0, C);
    I2C2_write(0xC0, C);
    I2C2_write(0xDA, C);
        I2C2_write(0x02, C);
    I2C2_write(0x81, C);
        I2C2_write(0x80, C);
    I2C2_write(0xA4, C);
    I2C2_write(0xA6, C);
    I2C2_write(0xD5, C);
        I2C2_write(0x80, C);
    I2C2_write(0x8D, C);
        I2C2_write(0x14, C);
    I2C2_write(0xAF, C);
    I2C2_stop();
}

void SSD1306_clear() {
    I2C2_start(W);
    I2C2_write(0x20, C);
    I2C2_write(0x02, C);
    I2C2_write(0x00, C);
    I2C2_write(0x10, C);
    I2C2_stop();
    for(int j = 0; j < 4; j++){
        I2C2_start(W);
        I2C2_write(0xB0 + j, C);
        I2C2_stop();
        I2C2_start(W);
        for(int i = 0; i < 128; i++) {
            I2C2_write(0x00, D);
        }
        I2C2_stop();
    }
    I2C2_start(W);
    I2C2_write(0xB0, C);
       I2C2_write(0x00, C);
    I2C2_write(0x10, C);
    I2C2_stop();
}

void SSD1306_clearRow(uint8_t row){
    I2C2_start(W);
    I2C2_write(0xB0 + row, C);
    I2C2_write(0x00, C);
    I2C2_write(0x10, C);
    I2C2_stop();
    I2C2_start(W);
    for(int i = 0; i < 128; i++) {
        I2C2_write(0x00, D);
    }
    I2C2_stop();
    SSD1306_setCursor(row,0);
}


void SSD1306_setCursor(uint8_t row, uint8_t col) {
    I2C2_start(W);
    I2C2_write(0x00, C);
    I2C2_write(0x10, C);
    I2C2_write(0xB0 + row, C);
    I2C2_stop();
    I2C2_start(W);
        while(col > 0) {
            I2C2_write(0x00, D);
            col--;
        }
        I2C2_stop();   
}

void SSD1306_print(char *print) {
    I2C2_start(W);
    for(int i = 0; (*(print + i) != '\0' && i < 127); i++) {
        for(int j = 0; j < 6; j++) {
            I2C2_write(Font8[(*(print + i) - 32) * 6 + j], D);
        }
    }
    I2C2_stop();
}

void SSD1306_Begin() {
    I2C2_Init();
    SSD1306_Init();
    SSD1306_clear();
}
