#include "stm32_base.h"
#include "ssd1306.h"

ssd1306_class::ssd1306_class(const ssd1306_cfg_t *const cfg) 
                            : cfg(cfg), i2c(cfg->i2c_cfg)
{
    i2c_ptr = &i2c;
}

ssd1306_class::ssd1306_class(const ssd1306_cfg_t *const cfg, i2c_class *i2c_ptr) 
                            : cfg(cfg), i2c({0}), i2c_ptr(i2c_ptr)
{
}


void ssd1306_class::Init(void)
{   
    i2c_ptr->Init();
    Command(0xAE); // 0xAE - OFF display, OxAF - ON display
    Command(0xA8); // Set multiplex ratio:
    Command(0x1F); // 128x32
    Command(0xD3); // Vertical offset is
    Command(0x00); // ZERO
    Command(0x40); // Set startline (0x40 - 0x7F)
    Command(0xA1); // Set segment remap (not use)
    Command(0xC8); // Set output scan direction (not use)
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

void ssd1306_class::Print(float number) 
{
    char data[10] = {0};
    float2char(number, data, 2);
    Print(data);
}

void ssd1306_class::Write(void)
{
    i2c_ptr->Transmit(buf, ptr, 
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
