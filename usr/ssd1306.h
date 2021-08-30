#include "stm32_base.h"
#include "i2c.h"

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
 * \brief SSD1306 display header
 * \detailed Consists of defines and SSD1306 class
 */

typedef struct ssd1306_cfg_t
{
    i2c_class i2c;
    static constexpr uint16_t size = SSD1306_Heigth * SSD1306_Width / 8;
    uint8_t buf[size];
    uint16_t ptr;
    uint8_t continuation;
};

class ssd1306_class
{
private:
    void AddToBuf(uint8_t data);
    void ControlByte(uint8_t type);
    void Command(uint8_t cmd);
    void Data(uint8_t data);
    void Write(void);
public:
    ssd1306_cfg_t *cfg;
    ssd1306_class() 
    {
        cfg->ptr = 0;
        cfg->continuation = 0;
    }
    void Init(void);

};
