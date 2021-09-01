#pragma once
#include "stm32_base.h"
#include "i2c.h"
#include "font.h"

/* Default parameters for SSD1306 128x32 */

/* Display parameters */
#define SSD1306_Heigth 32
#define SSD1306_Width 128
#define SSD1306_Address 0x3C

/* Type of data */
#define SSD1306_Command 0x80
#define SSD1306_Data 0xC0
#define SSD1306_NextOnlyData 0x40
#define SSD1306_ResetContinuation 0x00
#define SSD1306_SetContinuation 0x01
#define SSD1306_DataContinue 0x02

/* Bits in control byte */
#define SSD1306_WriteMode 0x00
#define SSD1306_ReadMode 0x01

/**
 * \file 
 * \brief SSD1306 display
 * \detailed Consists of defines and SSD1306 class
 */

struct ssd1306_cfg_t
{
    uint32_t height; // TODO: optimizate
    uint32_t width;
    uint8_t address;
    uint32_t size;
    const i2c_cfg_t *const i2c_cfg;
};

class ssd1306_class
{
private:
    const ssd1306_cfg_t *const cfg;
    uint8_t buf[SSD1306_Heigth * SSD1306_Width / 8];
    uint16_t ptr;
    uint8_t continuation;
private:
    void AddToBuf(uint8_t data);
    void ControlByte(uint8_t type);
    void Command(uint8_t cmd);
    void Data(uint8_t data);
    void Write(void);
public:
    i2c_class i2c;
    ssd1306_class(const ssd1306_cfg_t *const cfg);
public:
    void Init(void);
    void SetCursor(uint8_t page, uint8_t col);
    void Clear(void);
    // void Print(char *string);

    template<typename num>
    void Print(num number);

};

template<>
void ssd1306_class::Print(const char *string);

template<>
void ssd1306_class::Print(char *string);

template<typename num>
void ssd1306_class::Print(num number)
{
    char data[10] = {0};
    float2char(number, data, 2);
    Print(data);
    ssd1306_class::Write();
}

template<>
void ssd1306_class::Print(const char *string)
{
    for(int i = 0; (*(string + i) != '\0' && i < ssd1306_class::cfg->width); i++) {
        for(int j = 0; j < 6; j++) {
            ssd1306_class::Data(Font8[(*(string + i) - ssd1306_class::cfg->height) * 6 + j]);
        }
    }
    ssd1306_class::Write();
}

template<>
void ssd1306_class::Print(char *string)
{
    for(int i = 0; (*(string + i) != '\0' && i < ssd1306_class::cfg->width); i++) {
        for(int j = 0; j < 6; j++) {
            ssd1306_class::Data(Font8[(*(string + i) - ssd1306_class::cfg->height) * 6 + j]);
        }
    }
    ssd1306_class::Write();
}