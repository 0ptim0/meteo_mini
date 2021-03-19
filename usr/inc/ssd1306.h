/* Size of buf */
#define SSD1306_LengthBuf 8192

/* Display parameters */
#define SSD1306_Heigth 32
#define SSD1306_Width 128
#define SSD1306_Address 0x3C

/* Type of data */
#define SSD1306_Command 0
#define SSD1306_Data 1
#define SSD1306_OnlyData 2

/* Bits in control byte */
#define SSD1306_WriteMode 0x00
#define SSD1306_ReadMode 0x01
#define SSD1306_CommandBit 0x80
#define SSD1306_DataBit 0xC0
#define SSD1306_NextOnlyDataBit 0x40

/* Public function */
void SSD1306_Init(void);
void SSD1306_DisplayClear(void);
void SSD1306_SetCursorPage(uint8_t page, uint8_t col);
void SSD1306_PrintString(char *print);
void SSD1306_PrintNum(float print);

typedef struct {
    uint16_t X;
    uint16_t Y;
    _Bool Inv;
    _Bool ContinuationBit;
    uint8_t ControlByte;
} SSD1306_t;
