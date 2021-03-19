#include "stm32f401_conf.h"

static _Bool _init = pdFALSE;

void vMeteoMeasure(void *pvParameters) {
    SSD1306_Init();
    SSD1306_DisplayClear();
    BME280_Init();
    BME280_Settings(BME280_Oversampling_X16, BME280_Oversampling_X16, BME280_Oversampling_X16);
    _init = pdTRUE;
    while(1) {
            BME280_Measure();
            vTaskDelay(100);
    }
}

void vMeteoPrint(void *pvParameters) {
    if(_init == pdFALSE) {
    	vTaskDelay(1000);
    }
    while(1) {
        SSD1306_SetCursorPage(0,0);
        SSD1306_PrintString("METEO");
        SSD1306_SetCursorPage(1,0);
        SSD1306_PrintString("Temp = ");
        SSD1306_PrintNum(BME280.Measurement.Temperature / 100.0);
        SSD1306_PrintString(" C");
        SSD1306_SetCursorPage(2,0);
        SSD1306_PrintString("Humi = ");
        SSD1306_PrintNum(BME280.Measurement.Humiduty / 1024.0);
        SSD1306_PrintString(" %");
        SSD1306_SetCursorPage(3,0);
        SSD1306_PrintString("Pres = ");
        SSD1306_PrintNum(BME280.Measurement.Pressure * 0.00750062 / 256);
        SSD1306_PrintString(" mm/Hg");
        vTaskDelay(1000);
    }
}

void vMeteoCLI(void *pvParameters) {
	while(1) {
        CLI_Transmit();
        vTaskDelay(100);
    }
}

void vMeteo(void) {
    USART_InitOnce();
    I2C_InitOnce();
    gpio_config();
    xTaskCreate(vMeteoMeasure, "MeteoMeasure", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vMeteoPrint, "MeteoPrint", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vMeteoCLI, "MeteoCLI", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}
