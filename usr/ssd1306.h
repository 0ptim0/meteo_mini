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

class ssd1306 
{
private:
    static constexpr uint16_t size = SSD1306_Heigth * SSD1306_Width / 8;
    uint8_t buf[size];
    uint16_t ptr;
    uint8_t continuation;
    void AddToBuf(uint8_t data);
    void ControlByte(uint8_t type);
    void Command(uint8_t cmd);
    void Data(uint8_t data);
public:
    // typedef int (ssd1306::*transmit_disp)(uint8_t *, uint16_t, uint16_t);
    // typedef int (i2c::*transmit_i2c)(uint8_t *, uint16_t, uint16_t);
    // transmit_disp Transmit;
    ssd1306() {
        ptr = 0;
        continuation = 0;
        // Transmit = (oi2c.Transmit);
    }
    void Init(void);

};
