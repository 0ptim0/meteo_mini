#include "stm32f401_conf.h"

void vMeteoMeasure(void *pvParameters) {
    I2C_InitOnce();
    SSD1306_Init();
    SSD1306_DisplayClear();
    BME280_Init();
    BME280_Settings(BME280_Oversampling_X1, BME280_Oversampling_X1, BME280_Oversampling_X1);
    for(;;) {
            BME280_Measure();
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

void USART_QueueSend(char *str) {
    uint8_t data;
    for (int i = 0; *(str + i) != '\0'; i++) {
        data = *(str + i);
        xQueueSend(USART_Queue, &data, 0);
    }
}

void vMeteoTransmit(void *pvParameters) {
    USART_InitOnce();
    for(;;) {
        USART_QueueSend("PRIVET");
        vTaskDelay(1);
        USART_Transaction(USART2, USART_Queue);
        vTaskDelay(1000);
    }
}

void vMeteo(void) {
    gpio_config();
    //xTaskCreate(vMeteoMeasure, "MeteoMeasure", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vMeteoTransmit, "MeteoTransmit", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}
