#define add_SSD1306 0x78
#define R 1
#define W 0
#define C 0
#define D 1
#define font8 0
#define font16 1
void SSD1306_Begin(void);
void SSD1306_print(char *print);
void SSD1306_clear(void);
void SSD1306_clearRow(uint8_t row);
void SSD1306_setCursor(uint8_t row, uint8_t col);
