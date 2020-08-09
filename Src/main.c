#include <stm32f4xx.h>
#include <SSD1306.h>
#include <clock.h>
#include <BME280.h>

uint32_t P, H;
int32_t T;
void print_result(int32_t T, uint32_t P, uint32_t H);

int main() {
	Clock_Init();
	SSD1306_Begin();
	SSD1306_setCursor(0, 27);
	SSD1306_print("FW ECLIPSE");
	BME_Begin();
	BME_Measure(&T, &P, &H);
	delay(1 * ms);
	BME_Measure(&T, &P, &H);
	delay(1 * ms);
	while (1) {
		BME_Measure(&T, &P, &H);
		print_result(T, P, H);
		delay(1 * s);
	}
}

void print_result(int32_t T, uint32_t P, uint32_t H) {
	SSD1306_setCursor(1, 0);
	SSD1306_print("Temp    ");
	SSD1306_print(converse(T / 100.0));
	SSD1306_print(" &C ");
	SSD1306_setCursor(2, 0);
	SSD1306_print("Pres   ");
	SSD1306_print(converse(P * 0.00750062 / 256));
	SSD1306_print(" mm/Hg ");
	SSD1306_setCursor(3, 0);
	SSD1306_print("Humi    ");
	SSD1306_print(converse(H / 1024.0));
	SSD1306_print(" % ");
}
