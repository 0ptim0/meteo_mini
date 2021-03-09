#include "stm32f401_conf.h"

static void CLI_QueueSend(char *str) {
    uint8_t data;
    for (int i = 0; *(str + i) != '\0'; i++) {
        data = *(str + i);
        xQueueSend(USART_Queue, &data, 0);
    }
}

void CLI_Transmit(void) {
    char print_out[64] = {};
    CLI_QueueSend("\n-------------------------------\n");
    CLI_QueueSend("Temperature = ");
    CLI_QueueSend(ftc(BME280.Measurement.Temperature / 100.0, print_out, 63));
    CLI_QueueSend(" C\n");
    CLI_QueueSend("Humidity = ");
    CLI_QueueSend(ftc(BME280.Measurement.Humiduty / 1024.0, print_out, 63));
    CLI_QueueSend(" %\n");
    CLI_QueueSend("Pressure = ");
    CLI_QueueSend(ftc(BME280.Measurement.Pressure * 0.00750062 / 256, print_out, 63));
    CLI_QueueSend(" mm/Hg\n");
    USART_Transaction(USART2, USART_Queue);
}
