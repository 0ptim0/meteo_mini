#include "stm32_base.h"
#include "ssd1306.h"

void ssd1306_class::Write(void)
{
    cfg->i2c.Transmit(cfg->buf, cfg->ptr + 1, 
        (SSD1306_Address << 1) | SSD1306_WriteMode);
}

void ssd1306_class::Init(void)
{
    cfg->i2c.Init(cfg->i2c.cfg);
    Command(0xAE); // 0xAE - OFF display, OxAF - ON display
    Command(0xA8); // Set multiplex ratio:
    Command(0x1F); // 128x64
    Command(0xD3); // Vertical offset is
    Command(0x00); // ZERO
    Command(0x40); // Set startline (0x40 - 0x7F)
    Command(0xA0); // Set segment remap (not use)
    Command(0xC0); // Set output scan direction (not use)
    Command(0xDA); // Set COM pins hardware config (not use)
    Command(0x02); // 0xDA -> 0x02 standart (not use)
    Command(0x81); // Set contrast
    Command(0x88); // 0x00 - 0xFF
    Command(0xA4); // Entire display ON
    Command(0xA6); // A6 - normal, A7 - inverse
    Command(0xD5); // freq A7 only
    Command(0x80);
    Command(0x8D); // Charge pump enable
    Command(0x14);
    Command(0xAF); // Display ON
    Write();
}

void ssd1306_class::AddToBuf(uint8_t data)
{
    cfg->buf[cfg->ptr] = data;
    cfg->ptr++; 
    if(cfg->ptr > (cfg->size - 1)) {
        cfg->ptr = 0;
    }
}

void ssd1306_class::ControlByte(uint8_t type)
{
switch(type) {
    case SSD1306_Command:
        cfg->continuation = SSD1306_ResetContinuation;
        AddToBuf(SSD1306_Command);
        break;
    case SSD1306_Data:
        if (cfg->continuation == SSD1306_ResetContinuation) {
            AddToBuf(SSD1306_Data);
            cfg->continuation = SSD1306_SetContinuation;
        } else if(cfg->continuation == SSD1306_SetContinuation) {
            AddToBuf(SSD1306_NextOnlyData);
            cfg->continuation = SSD1306_DataContinue;
        }
        break;
    }
}

void ssd1306_class::Command(uint8_t cmd)
{
    ControlByte(SSD1306_Command);
    AddToBuf(cmd);
}

void ssd1306_class::Data(uint8_t data)
{
    ControlByte(SSD1306_Data);
    AddToBuf(data);
}
