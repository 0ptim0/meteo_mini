#pragma once
#include "stm32_base.h"
#include "i2c.h"

/* Parameters for BME280 */

/* Address */
#define BME280_Address 0x76

/* Buffer parameters */
#define BME280_LengthBuf 128

/* Type of data */
#define BME280_WriteBit     0x00
#define BME280_ReadBit	    0x01

/* Filter parameters */
#define BME280_Oversampling_X1 	    0x01
#define BME280_Oversampling_X2 	    0x02
#define BME280_Oversampling_X4 	    0x03
#define BME280_Oversampling_X8 	    0x04
#define BME280_Oversampling_X16     0x05

/* Mode */
#define BME280_SleepMode	0x00
#define BME280_ForcedMode 	0x01
#define BME280_NormalMode 	0x03

/* Registers */
#define BME280_RESET_REG           0xE0
#define BME280_STATUS_REG          0xF3
#define BME280_CTRL_PT_REG         0xF4
#define BME280_CTRL_H_REG          0xF2
#define BME280_CONFIG_REG          0xF5
#define BME280_FIRST_PT_CALIB_REG  0x88
#define BME280_LAST_PT_CALIB_REG   0xA1
#define BME280_FIRST_H_CALIB_REG   0xE1
#define BME280_LAST_H_CALIB_REG    0xE6
#define BME280_FIRST_MEAS_REG      0xF7
#define BME280_LAST_MEAS_REG       0xFE
#define BME280_ResetByte           0xB6

/**
 * \file
 * \brief BME280 temperature, pressure, humidity sensor
 * \detailed Consists of defines and SSD1306 class
 */

/* Calibration type */
typedef long signed int bme280_s32_t;
typedef long unsigned int bme280_u32_t;
typedef long long signed int bme280_s64_t;

struct calib_t
{
    unsigned short dig_T1;
    signed short dig_T2;
    signed short dig_T3;
    unsigned short dig_P1;
    signed short dig_P2;
    signed short dig_P3;
    signed short dig_P4;
    signed short dig_P5;
    signed short dig_P6;
    signed short dig_P7;
    signed short dig_P8;
    signed short dig_P9;
    unsigned char dig_H1;
    signed short dig_H2;
    unsigned char dig_H3;
    signed short dig_H4;
    signed short dig_H5;
    signed char dig_H6;
};

struct meas_t
{
    double pres;
    double temp;
    double humi;
};

struct bme280_cfg_t
{
    const uint8_t address = BME280_Address;
    const uint16_t size = BME280_LengthBuf;
    const i2c_cfg_t *const i2c_cfg;
};

class bme280_class
{
private:
    const bme280_cfg_t *const cfg;
    uint8_t buf[BME280_LengthBuf];
    uint16_t ptr;
    uint8_t control;
    uint8_t settings;
    bme280_s32_t t_fine;
private:
    void AddToBuf(uint8_t data);
    void Command(uint8_t cmd);
    void Write(void);
    void Read(uint8_t *buf, uint8_t reg, uint8_t legnth);
    void Reset(void);
    void GetCalib(void);
    bme280_s32_t TempComp(bme280_s32_t adc_T);
    bme280_u32_t PresComp(bme280_s32_t adc_P);
    bme280_u32_t HumiComp(bme280_s32_t adc_H);
public:
    meas_t meas;
    i2c_class i2c;
    i2c_class *i2c_ptr;
    calib_t calib;
    bme280_class(const bme280_cfg_t *const cfg);
    bme280_class(const bme280_cfg_t *const cfg, i2c_class *i2c_ptr);
public:
    void Init(void);
    void Measure(void);
    void SetSamples(uint8_t temp, uint8_t pres, uint8_t humi);
};
