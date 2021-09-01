#include "stm32_base.h"
#include "ssd1306.h"


const uint8_t Font8[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x2e, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x06, 0x00, 0x06, 0x00, 0x00,
    0x14, 0x3e, 0x14, 0x3e, 0x14, 0x00,
    0x00, 0x2e, 0x7f, 0x3a, 0x00, 0x00,
    0x26, 0x16, 0x08, 0x34, 0x32, 0x00,
    0x00, 0x0e, 0x0a, 0x0e, 0x00, 0x00,
    0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x1c, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x22, 0x1c, 0x00, 0x00, 0x00, 0x00,
    0x0a, 0x04, 0x0a, 0x00, 0x00, 0x00,
    0x08, 0x1c, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x60, 0x00, 0x00, 0x00,
    0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x00, 0x20, 0x1c, 0x02, 0x00, 0x00,
    0x00, 0x3e, 0x22, 0x3e, 0x00, 0x00,
    0x00, 0x24, 0x3e, 0x20, 0x00, 0x00,
    0x00, 0x3a, 0x2a, 0x2e, 0x00, 0x00,
    0x00, 0x2a, 0x2a, 0x3e, 0x00, 0x00,
    0x00, 0x0e, 0x08, 0x3e, 0x00, 0x00,
    0x00, 0x2e, 0x2a, 0x3a, 0x00, 0x00,
    0x00, 0x3e, 0x2a, 0x3a, 0x00, 0x00,
    0x00, 0x02, 0x02, 0x3e, 0x00, 0x00,
    0x00, 0x3e, 0x2a, 0x3e, 0x00, 0x00,
    0x00, 0x2e, 0x2a, 0x3e, 0x00, 0x00,
    0x00, 0x14, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x14, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x14, 0x22, 0x00, 0x00, 0x00,
    0x14, 0x14, 0x14, 0x00, 0x00, 0x00,
    0x22, 0x14, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x2a, 0x0e, 0x00, 0x00,
    0x7f, 0x41, 0x5d, 0x55, 0x5f, 0x00,
    0x00, 0x3e, 0x12, 0x3e, 0x00, 0x00,
    0x00, 0x3e, 0x2a, 0x3e, 0x00, 0x00,
    0x00, 0x3e, 0x22, 0x22, 0x00, 0x00,
    0x00, 0x3e, 0x22, 0x1c, 0x00, 0x00,
    0x00, 0x3e, 0x2a, 0x2a, 0x00, 0x00,
    0x00, 0x3e, 0x0a, 0x0a, 0x00, 0x00,
    0x00, 0x3e, 0x22, 0x3a, 0x00, 0x00,
    0x00, 0x3e, 0x08, 0x3e, 0x00, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x00, 0x00,
    0x00, 0x30, 0x20, 0x3e, 0x00, 0x00,
    0x00, 0x3e, 0x08, 0x36, 0x00, 0x00,
    0x00, 0x3e, 0x20, 0x20, 0x00, 0x00,
    0x3e, 0x04, 0x08, 0x04, 0x3e, 0x00,
    0x3e, 0x08, 0x10, 0x3e, 0x00, 0x00,
    0x00, 0x3e, 0x22, 0x3e, 0x00, 0x00,
    0x00, 0x3e, 0x0a, 0x0e, 0x00, 0x00,
    0x00, 0x3e, 0x22, 0x7e, 0x00, 0x00,
    0x00, 0x3e, 0x1a, 0x2e, 0x00, 0x00,
    0x00, 0x2e, 0x2a, 0x3a, 0x00, 0x00,
    0x00, 0x02, 0x3e, 0x02, 0x00, 0x00,
    0x00, 0x3e, 0x20, 0x3e, 0x00, 0x00,
    0x00, 0x1e, 0x20, 0x1e, 0x00, 0x00,
    0x1e, 0x20, 0x1e, 0x20, 0x1e, 0x00,
    0x00, 0x36, 0x08, 0x36, 0x00, 0x00,
    0x00, 0x06, 0x38, 0x06, 0x00, 0x00,
    0x00, 0x32, 0x2a, 0x26, 0x00, 0x00,
    0x00, 0x3e, 0x22, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x1c, 0x20, 0x00, 0x00,
    0x00, 0x22, 0x3e, 0x00, 0x00, 0x00,
    0x00, 0x08, 0x04, 0x08, 0x00, 0x00,
    0x20, 0x20, 0x20, 0x20, 0x00, 0x00,
    0x02, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3c, 0x14, 0x3c, 0x00, 0x00,
    0x00, 0x3e, 0x28, 0x38, 0x00, 0x00,
    0x00, 0x3c, 0x24, 0x24, 0x00, 0x00,
    0x00, 0x38, 0x28, 0x3e, 0x00, 0x00,
    0x00, 0x3c, 0x2c, 0x2c, 0x00, 0x00,
    0x00, 0x3c, 0x0a, 0x02, 0x00, 0x00,
    0x00, 0xb8, 0xa8, 0xf8, 0x00, 0x00,
    0x00, 0x3e, 0x08, 0x38, 0x00, 0x00,
    0x00, 0x00, 0x3a, 0x00, 0x00, 0x00,
    0x00, 0xc0, 0x80, 0xfa, 0x00, 0x00,
    0x00, 0x3e, 0x10, 0x2c, 0x00, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x00, 0x00,
    0x3c, 0x04, 0x3c, 0x04, 0x3c, 0x00,
    0x00, 0x3c, 0x04, 0x3c, 0x00, 0x00,
    0x00, 0x3c, 0x24, 0x3c, 0x00, 0x00,
    0x00, 0xfc, 0x24, 0x3c, 0x00, 0x00,
    0x00, 0x3c, 0x24, 0x7c, 0x00, 0x00,
    0x00, 0x3c, 0x04, 0x04, 0x00, 0x00,
    0x00, 0x2c, 0x2c, 0x3c, 0x00, 0x00,
    0x00, 0x04, 0x3e, 0x04, 0x00, 0x00,
    0x00, 0x3c, 0x20, 0x3c, 0x00, 0x00,
    0x00, 0x1c, 0x20, 0x1c, 0x00, 0x00,
    0x1c, 0x20, 0x1c, 0x20, 0x1c, 0x00,
    0x00, 0x2c, 0x10, 0x2c, 0x00, 0x00,
    0x00, 0xbc, 0xa0, 0xfc, 0x00, 0x00,
    0x00, 0x34, 0x2c, 0x24, 0x00, 0x00,
    0x08, 0x36, 0x22, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x00, 0x00,
    0x22, 0x36, 0x08, 0x00, 0x00, 0x00,
    0x08, 0x10, 0x08, 0x10, 0x00, 0x00
};

ssd1306_class::ssd1306_class(const ssd1306_cfg_t *const cfg) 
                : cfg(cfg), i2c(cfg->i2c_cfg)
{
}

void ssd1306_class::Init(void)
{   
    i2c.Init();
    Command(0xAE); // 0xAE - OFF display, OxAF - ON display
    Command(0xA8); // Set multiplex ratio:
    Command(0x1F); // 128x32
    Command(0xD3); // Vertical offset is
    Command(0x00); // ZERO
    Command(0x40); // Set startline (0x40 - 0x7F)
    Command(0xA0); // Set segment remap (not use)
    Command(0xC0); // Set output scan direction (not use)
    Command(0xDA); // Set COM pins hardware config (not use)
    Command(0x02); // 0xDA -> 0x02 standart (not use)
    Command(0x81); // Set contrast
    Command(0xFF); // 0x00 - 0xFF
    Command(0xA4); // Entire display ON
    Command(0xA6); // A6 - normal, A7 - inverse
    Command(0xD5); // freq A7 only
    Command(0x80);
    Command(0x8D); // Charge pump enable
    Command(0x14);
    Command(0xAF); // Display ON
    Write();
}

void ssd1306_class::SetCursor(uint8_t page, uint8_t col) 
{
    if(col > cfg->width) {
        return;
    }
    Command(0xB0 + page);
    Command(0x00 | (col & 0x0F));
    Command(0x10 | (col >> 4));
    Write();
}

void ssd1306_class::Clear(void) 
{
    for(int i = 0; i < (cfg->height / 8); i++) {
        Command(0x00);
        Command(0x10);
        Command(0xB0 + i);
        //Command(0x1F);
        for(int j = 0; j < (cfg->width); j++) {
            Data(0x00);
        }
        Write();
    }
    SetCursor(0,0);
}

void ssd1306_class::Print(char *string) 
{
    for(int i = 0; (*(string + i) != '\0' && i < cfg->width); i++) {
        for(int j = 0; j < 6; j++) {
            Data(Font8[(*(string + i) - cfg->height) * 6 + j]);
        }
    }
    Write();
}

void ssd1306_class::Write(void)
{
    i2c.Transmit(buf, ptr, 
        (cfg->address << 1) | SSD1306_WriteMode);
    ptr = 0;
}

void ssd1306_class::AddToBuf(uint8_t data)
{
    buf[ptr] = data;
    ptr++; 
    if(ptr > (cfg->size - 1)) {
        ptr = 0;
    }
}

void ssd1306_class::ControlByte(uint8_t type)
{
switch(type) {
    case SSD1306_Command:
        continuation = SSD1306_ResetContinuation;
        AddToBuf(SSD1306_Command);
        break;
    case SSD1306_Data:
        if (continuation == SSD1306_ResetContinuation) {
            AddToBuf(SSD1306_Data);
            continuation = SSD1306_SetContinuation;
        } else if(continuation == SSD1306_SetContinuation) {
            AddToBuf(SSD1306_NextOnlyData);
            continuation = SSD1306_DataContinue;
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
