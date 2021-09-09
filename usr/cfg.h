#pragma once
#include "stm32_conf.h"

#define PIN_I2C_SCL {GPIOB, {GPIO_PIN_6, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
#define PIN_I2C_SDA {GPIOB, {GPIO_PIN_7, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}}
const gpio_cfg_t scl = PIN_I2C_SCL;
const gpio_cfg_t sda = PIN_I2C_SDA;
const i2c_cfg_t i2c1_cfg = {I2C1, &scl, &sda, 100000, 100};
const ssd1306_cfg_t ssd1306_cfg = {SSD1306_Heigth, SSD1306_Width, SSD1306_Address, 
                        SSD1306_Heigth * SSD1306_Width / 8, &i2c1_cfg};
const bme280_cfg_t bme280_cfg = {BME280_Address, BME280_LengthBuf, &i2c1_cfg};