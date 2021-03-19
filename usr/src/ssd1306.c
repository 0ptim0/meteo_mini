#include "stm32f401_conf.h"
#include "font.h"
#include <stdio.h>

SSD1306_t SSD1306;
static QueueHandle_t SSD1306_Queue;

static void SSD1306_ControlByte(uint8_t type) {
    switch(type) {
    case SSD1306_Command:
        SSD1306.ControlByte = SSD1306_CommandBit;
        break;
    case SSD1306_Data:
        SSD1306.ControlByte = SSD1306_DataBit;
        break;
    case SSD1306_OnlyData:
        SSD1306.ControlByte = SSD1306_NextOnlyDataBit;
        SSD1306.ContinuationBit = 1;
        break;
    }
}

static void SSD1306_QueueSend(uint8_t data, uint8_t type) {
    if(SSD1306.ContinuationBit != 1) {
        SSD1306_ControlByte(type);
        xQueueSend(SSD1306_Queue, &(SSD1306.ControlByte), 100);
    }
    xQueueSend(SSD1306_Queue, &data, 100);
}

static void SSD1306_Write(void) {
    uint8_t address = (SSD1306_Address << 1) | SSD1306_WriteMode;
    I2C_Transaction(I2C1, address, 0, SSD1306_Queue);
    SSD1306.ContinuationBit = 0;
}

void SSD1306_Init(void) {
	SSD1306_Queue = xQueueCreate(SSD1306_LengthBuf, sizeof(uint8_t));
    SSD1306_QueueSend(0xAE, SSD1306_Command); // 0xAE - OFF display, OxAF - OFN display
    SSD1306_QueueSend(0xA8, SSD1306_Command); // Set multiplex ratio:
    SSD1306_QueueSend(0x1F, SSD1306_Command); // 128x64
    SSD1306_QueueSend(0xD3, SSD1306_Command); // Vertical offset is
    SSD1306_QueueSend(0x00, SSD1306_Command); // ZERO
    SSD1306_QueueSend(0x40, SSD1306_Command); // Set startline (0x40 - 0x7F)
    SSD1306_QueueSend(0xA0, SSD1306_Command); // Set segment remap (not use)
    SSD1306_QueueSend(0xC0, SSD1306_Command); // Set output scan direction (not use)
    SSD1306_QueueSend(0xDA, SSD1306_Command); // Set COM pins hardware config (not use)
    SSD1306_QueueSend(0x02, SSD1306_Command); // 0xDA -> 0x02 standart (not use)
    SSD1306_QueueSend(0x81, SSD1306_Command); // Set contrast
    SSD1306_QueueSend(0x88, SSD1306_Command); // 0x00 - 0xFF
    SSD1306_QueueSend(0xA4, SSD1306_Command); // Entire display ON
    SSD1306_QueueSend(0xA6, SSD1306_Command); // A6 - normal, A7 - inverse
    SSD1306_QueueSend(0xD5, SSD1306_Command); // freq A7 only
    SSD1306_QueueSend(0x80, SSD1306_Command); //
    SSD1306_QueueSend(0x8D, SSD1306_Command); // Charge pump enable
    SSD1306_QueueSend(0x14, SSD1306_Command); //
    SSD1306_QueueSend(0xAF, SSD1306_Command); // Display ON
    SSD1306_Write();
    SSD1306.X = 0;
    SSD1306.Y = 0;
    SSD1306.Inv = 0;
    SSD1306.ContinuationBit = 0;
}

void SSD1306_SetCursorPage(uint8_t page, uint8_t col) {
    if(col > SSD1306_Width) {
        return;
    }
    SSD1306_QueueSend(0x00, SSD1306_Command);         // Init null column
    SSD1306_QueueSend(0x10, SSD1306_Command);
    SSD1306_QueueSend(0xB0 + page, SSD1306_Command);  // Set page number
    for(int i = 0; i < col; i++) {                    // Shift pointer
        SSD1306_QueueSend(0x00, SSD1306_OnlyData);
    }
    SSD1306_Write();
}

void SSD1306_DisplayClear(void) {
    SSD1306_QueueSend(0x00, SSD1306_Command);
    SSD1306_QueueSend(0x10, SSD1306_Command);
    for(int i = 0; i < (SSD1306_Heigth / 8); i++) {
        SSD1306_QueueSend(0xB0 + i, SSD1306_Command);
        for(int j = 0; j < SSD1306_Width; j++) {
            SSD1306_QueueSend(0x00, SSD1306_Data);
        }
        SSD1306_Write();
    }
    SSD1306_SetCursorPage(0,0);
}

void SSD1306_PrintString(char *print) {
    for(int i = 0; (*(print + i) != '\0' && i < 127); i++) {
        for(int j = 0; j < 6; j++) {
            SSD1306_QueueSend(Font8[(*(print + i) - 32) * 6 + j], SSD1306_OnlyData);
        }
    }
    SSD1306_Write();
}

void SSD1306_PrintNum(float print) {
    char print_out[127] = {};
    SSD1306_PrintString(ftc(print, print_out, 126));
}

void SSD1306_DisplayInverse(void) {
    if(SSD1306.Inv) {
        SSD1306_QueueSend(0xA6, SSD1306_Command);
        SSD1306.Inv = 0;
    } else {
        SSD1306_QueueSend(0xA7, SSD1306_Command);
        SSD1306.Inv = 1;
    }
    SSD1306_Write();
}
