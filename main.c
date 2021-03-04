#include "stm32f401_conf.h"

void vTask2(void *pvParameters) {
	for(;;) {
    vTaskDelay(1000);
    LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
	}
}

void vPrint(void) {
    SSD1306_SetCursorPage(0,0);
    SSD1306_PrintString("METEO");
    SSD1306_SetCursorPage(1,0);
    SSD1306_PrintString("Temp = ");
    SSD1306_PrintNum(BME280.Measurement.Temperature / 100.0);
    SSD1306_PrintString(" C ");
    SSD1306_SetCursorPage(2,0);
    SSD1306_PrintString("Humi = ");
    SSD1306_PrintNum(BME280.Measurement.Humiduty / 1024.0);
    SSD1306_PrintString(" %");
    SSD1306_SetCursorPage(3,0);
    SSD1306_PrintString("Pres = ");
    SSD1306_PrintNum(BME280.Measurement.Pressure * 0.00750062 / 256);
    SSD1306_PrintString(" mm/Hg ");
}

void vTest(void *pvParameters) {
    vTaskDelay(1000);
    I2C_Init_Once();
    SSD1306_Init();
    SSD1306_DisplayClear();
    BME280_Init();
    BME280_Settings(BME280_Oversampling_X1, BME280_Oversampling_X1, BME280_Oversampling_X1);
    BME280_Measure();
    for(;;) {
        SSD1306_DisplayClear();
        vTaskDelay(100);
        BME280_Measure();
        vPrint();
    }
}

int main(void) {
    gpio_config();
    xTaskCreate(vTest, "SSD1306 test", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask2, "LED toggle", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1) {
    }
}
