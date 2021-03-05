#include "stm32f401_conf.h"

BME280_t BME280;
static uint8_t address;
static uint32_t val;
static short buf[512];

BME280_S32_t t_fine;

BME280_S32_t BME280_TempCompensate(BME280_S32_t adc_T) {
    BME280_S32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((BME280_S32_t)BME280.Calib.dig_T1 << 1)) )*
            ((BME280_S32_t)BME280.Calib.dig_T2))  >>  11;
    var2 = (((((adc_T >> 4) - ((BME280_S32_t)BME280.Calib.dig_T1)) *
            ((adc_T >> 4) - ((BME280_S32_t)BME280.Calib.dig_T1))) >> 12) *
            ((BME280_S32_t)BME280.Calib.dig_T3))  >>  14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128)  >>  8;
    return T;
}

BME280_U32_t BME280_PresCompensate(BME280_S32_t adc_P) {
    BME280_S64_t var1, var2, P; 
    var1 = ((BME280_S64_t)t_fine) - 128000;
    var2 = var1 * var1 * (BME280_S64_t)BME280.Calib.dig_P6;
    var2 = var2 + ((var1 * (BME280_S64_t)BME280.Calib.dig_P5) << 17);
    var2 = var2 + (((BME280_S64_t)BME280.Calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (BME280_S64_t)BME280.Calib.dig_P3) >> 8) + 
    ((var1 * (BME280_S64_t)BME280.Calib.dig_P2) << 12);
    var1 = (((((BME280_S64_t)1) << 47)+var1))*((BME280_S64_t)BME280.Calib.dig_P1) >> 33;
    if(var1 == 0) {
        return 0;
    }
    P = 1048576 - adc_P;
    P = (((P << 31) - var2) * 3125) / var1;
    var1 = (((BME280_S64_t)BME280.Calib.dig_P9) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((BME280_S64_t)BME280.Calib.dig_P8) * P) >> 19;
    P = ((P + var1 + var2)  >>  8) + (((BME280_S64_t)BME280.Calib.dig_P7) << 4);
    return (BME280_U32_t)P;
}

BME280_U32_t BME280_HumCompensate(BME280_S32_t adc_H) {
    BME280_S32_t H;
    H = (t_fine - ((BME280_S32_t)76800));
    H = (((((adc_H << 14) - (((BME280_S32_t)BME280.Calib.dig_H4) << 20) - (((BME280_S32_t)BME280.Calib.dig_H5) * 
    H)) + ((BME280_S32_t)16384)) >> 15) * (((((((H *
    ((BME280_S32_t)BME280.Calib.dig_H6)) >> 10) * (((H * ((BME280_S32_t)BME280.Calib.dig_H3))  >>  11) +
    ((BME280_S32_t)32768)))  >>  10) + ((BME280_S32_t)2097152)) * ((BME280_S32_t)BME280.Calib.dig_H2) +
    8192) >> 14));
    H = (H - (((((H >> 15) * (H >> 15)) >> 7) *
        ((BME280_S32_t)BME280.Calib.dig_H1)) >> 4));
    H = (H < 0 ? 0 : H);
    H = (H > 419430400 ? 419430400 : H);
    return (BME280_U32_t)(H >> 12);
}

static void BME280_Control_Byte(uint8_t type) {
    switch(type) {
    case BME280_WriteBit:
        BME280.ControlByte = BME280_WriteBit;
        break;
    case BME280_ReadBit:
        BME280.ControlByte = BME280_ReadBit;
        break;
    }
}

static void BME280_QueueSend(uint8_t data) {
    xQueueSend(I2C_Queue, &data, 0);
}

static void BME280_GetCalibValue(void) {
    static int i;
    while(xQueueReceive(I2C_Queue, &val, 0) != errQUEUE_EMPTY) {
        buf[i] = val;
        i++;
    }
    /* 0x88 - 0xA1 */
    BME280.Calib.dig_T1 = (buf[0] | (buf[1] << 8));
    BME280.Calib.dig_T2 = (buf[2] | (buf[3] << 8));
    BME280.Calib.dig_T3 = (buf[4] | (buf[5] << 8));
    BME280.Calib.dig_P1 = (buf[6] | (buf[7] << 8));
    BME280.Calib.dig_P2 = (buf[8] | (buf[9] << 8));
    BME280.Calib.dig_P3 = (buf[10] | (buf[11] << 8));
    BME280.Calib.dig_P4 = (buf[12] | (buf[13] << 8));
    BME280.Calib.dig_P5 = (buf[14] | (buf[15] << 8));
    BME280.Calib.dig_P6 = (buf[16] | (buf[17] << 8));
    BME280.Calib.dig_P7 = (buf[18] | (buf[19] << 8));
    BME280.Calib.dig_P8 = (buf[20] | (buf[21] << 8));
    BME280.Calib.dig_P9 = (buf[22] | (buf[23] << 8));
    BME280.Calib.dig_H1 = (buf[25]);
    /* 0xE1 - 0xE7 */
    BME280.Calib.dig_H2 = (buf[26] | (buf[27] << 8));
    BME280.Calib.dig_H3 = (buf[28]);
    BME280.Calib.dig_H4 = ((buf[29] << 4) | (buf[30] & 0x0F));
    BME280.Calib.dig_H5 = ((buf[30] & 0xF0) | (buf[31] << 4));
    BME280.Calib.dig_H6 = buf[32];
}

static void BME280_GetMeasureValue(void) {
    static int i;
    int T, P, H;
    while(xQueueReceive(I2C_Queue, &val, 0) != errQUEUE_EMPTY) {
        buf[i] = val;
        i++;
    }
    i = 0;

    P = (buf[0] << (20 - 8));
    P |= (buf[1] << (20 - 16));
    P |= (buf[2] >> 4);
    T = (buf[3] << (20 - 8));
    T |= (buf[4] << (20 - 16));
    T |= (buf[5] >> 4);
    H = (buf[6] << 8);
    H |= (buf[7]);

    BME280.Measurement.Pressure = BME280_PresCompensate(P);
    BME280.Measurement.Temperature = BME280_TempCompensate(T);
    BME280.Measurement.Humiduty = BME280_HumCompensate(H);

}

static void BME280_Write(void) {

	BME280_Control_Byte(BME280_WriteBit);
    address = (BME280_Address << 1) | (BME280.ControlByte);
    I2C_Transaction(I2C1, address, 0, I2C_Queue);
}

static void BME280_Read(uint8_t reg, uint8_t length) {
    BME280_Control_Byte(BME280_WriteBit);
    address = (BME280_Address << 1) | (BME280.ControlByte);
    BME280_QueueSend(reg);
    I2C_Transaction(I2C1, address, 0, I2C_Queue);
    BME280_Control_Byte(BME280_ReadBit);
    address = (BME280_Address << 1) | (BME280.ControlByte);
    I2C_Transaction(I2C1, address, length, I2C_Queue);
}

static void BME280_Reset(void) {
    BME280_QueueSend(0xE0);
    BME280_QueueSend(0xB6);
    BME280_Write();
    vTaskDelay(10);
}

void BME280_Settings(uint8_t TempSampl, uint8_t PresSampl, uint8_t HumSampl) {
    BME280.Settings = (PresSampl << 2) | (TempSampl << 5);
    BME280_QueueSend(0xF2);
    BME280_QueueSend(HumSampl);
    BME280_Write();
}

void BME280_Measure(void) {
    BME280_QueueSend(0xF4);
    BME280_QueueSend(BME280.Settings | BME280_ForcedMode);
    BME280_Write();
    vTaskDelay(10);
    BME280_Read(0xF7, 8);
    BME280_GetMeasureValue();
}

void BME280_Init(void) {
    BME280_Reset();
    BME280_Read(0x88, 26);
    BME280_GetCalibValue();
    BME280_Read(0xE1, 8);
    BME280_GetCalibValue();
}
