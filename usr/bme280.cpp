#include "stm32_base.h"
#include "bme280.h"


bme280_class::bme280_class(const bme280_cfg_t *const cfg) 
                            : cfg(cfg), i2c(cfg->i2c_cfg)
{
    i2c_ptr = &i2c;
}

bme280_class::bme280_class(const bme280_cfg_t *const cfg,
                            i2c_class *i2c_ptr) 
                : cfg(cfg), i2c({0}), i2c_ptr(i2c_ptr)
{
}

void bme280_class::Init(void)
{
    Reset();
    GetCalib();
    SetSamples(BME280_Oversampling_X16, BME280_Oversampling_X16, BME280_Oversampling_X16);
}

void bme280_class::Reset(void)
{   
    AddToBuf(BME280_RESET_REG);
    AddToBuf(BME280_ResetByte);
    Write();
    vTaskDelay(1);
}

void bme280_class::GetCalib(void)
{
    /* 0x88 - 0xA1 */
    Read(buf,BME280_FIRST_PT_CALIB_REG, 
            BME280_LAST_PT_CALIB_REG - BME280_FIRST_PT_CALIB_REG + 1);
    calib.dig_T1 = (buf[0] | (buf[1] << 8));
    calib.dig_T2 = (buf[2] | (buf[3] << 8));
    calib.dig_T3 = (buf[4] | (buf[5] << 8));
    calib.dig_P1 = (buf[6] | (buf[7] << 8));
    calib.dig_P2 = (buf[8] | (buf[9] << 8));
    calib.dig_P3 = (buf[10] | (buf[11] << 8));
    calib.dig_P4 = (buf[12] | (buf[13] << 8));
    calib.dig_P5 = (buf[14] | (buf[15] << 8));
    calib.dig_P6 = (buf[16] | (buf[17] << 8));
    calib.dig_P7 = (buf[18] | (buf[19] << 8));
    calib.dig_P8 = (buf[20] | (buf[21] << 8));
    calib.dig_P9 = (buf[22] | (buf[23] << 8));
    calib.dig_H1 = (buf[25]);
    /* 0xE1 - 0xE7 */
    Read(buf, BME280_FIRST_H_CALIB_REG, 
        BME280_LAST_H_CALIB_REG - BME280_FIRST_H_CALIB_REG + 2); // one register is divided
    calib.dig_H2 = (buf[0] | (buf[1] << 8));
    calib.dig_H3 = (buf[2]);
    calib.dig_H4 = ((buf[3] << 4) | (buf[4] & 0x0F));
    calib.dig_H5 = ((buf[4] & 0xF0) | (buf[5] << 4));
    calib.dig_H6 = buf[6];
}

void bme280_class::Measure(void)
{
    int T, P, H;
    uint8_t lbuf[BME280_LAST_MEAS_REG - BME280_FIRST_MEAS_REG + 1];
    AddToBuf(BME280_CTRL_PT_REG);
    AddToBuf(settings | BME280_ForcedMode);
    Write();
    Read(lbuf, BME280_FIRST_MEAS_REG, 
            BME280_LAST_MEAS_REG - BME280_FIRST_MEAS_REG + 1);
    
    P = (lbuf[0] << (20 - 8));
    P |= (lbuf[1] << (20 - 16));
    P |= (lbuf[2] >> 4);
    T = (lbuf[3] << (20 - 8));
    T |= (lbuf[4] << (20 - 16));
    T |= (lbuf[5] >> 4);
    H = (lbuf[6] << 8);
    H |= (lbuf[7]);

    meas.pres = PresComp(P) * 0.00750062 / 256;
    meas.temp = TempComp(T) / 100.0;
    meas.humi = HumiComp(H) / 1024.0;
}

void bme280_class::SetSamples(uint8_t temp, uint8_t pres, uint8_t humi)
{
    settings = (pres << 2) | (temp << 5);
    AddToBuf(BME280_CTRL_H_REG);
    AddToBuf(humi);
    Write();
}

void bme280_class::Write(void)
{
    i2c_ptr->Transmit(buf, ptr, (cfg->address << 1) | BME280_WriteBit);
    ptr = 0;
}

void bme280_class::Read(uint8_t *buf, uint8_t reg, uint8_t legnth)
{
    AddToBuf(reg);
    Write();
    i2c_ptr->Receive(buf, legnth, (cfg->address << 1) | BME280_ReadBit);
}

void bme280_class::AddToBuf(uint8_t data)
{
    buf[ptr] = data;
    ptr++; 
    if(ptr > (cfg->size - 1)) {
        ptr = 0;
    }
}

bme280_s32_t bme280_class::TempComp(bme280_s32_t adc_T) 
{
    bme280_s32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((bme280_s32_t)calib.dig_T1 << 1)) )*
            ((bme280_s32_t)calib.dig_T2))  >>  11;
    var2 = (((((adc_T >> 4) - ((bme280_s32_t)calib.dig_T1)) *
            ((adc_T >> 4) - ((bme280_s32_t)calib.dig_T1))) >> 12) *
            ((bme280_s32_t)calib.dig_T3))  >>  14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128)  >>  8;
    return T;
}

bme280_u32_t bme280_class::PresComp(bme280_s32_t adc_P) 
{
    bme280_s64_t var1, var2, P; 
    var1 = ((bme280_s64_t)t_fine) - 128000;
    var2 = var1 * var1 * (bme280_s64_t)calib.dig_P6;
    var2 = var2 + ((var1 * (bme280_s64_t)calib.dig_P5) << 17);
    var2 = var2 + (((bme280_s64_t)calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (bme280_s64_t)calib.dig_P3) >> 8) + 
    ((var1 * (bme280_s64_t)calib.dig_P2) << 12);
    var1 = (((((bme280_s64_t)1) << 47)+var1))*((bme280_s64_t)calib.dig_P1) >> 33;
    if(var1 == 0) {
        return 0;
    }
    P = 1048576 - adc_P;
    P = (((P << 31) - var2) * 3125) / var1;
    var1 = (((bme280_s64_t)calib.dig_P9) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((bme280_s64_t)calib.dig_P8) * P) >> 19;
    P = ((P + var1 + var2)  >>  8) + (((bme280_s64_t)calib.dig_P7) << 4);
    return (bme280_u32_t)P;
}

bme280_u32_t bme280_class::HumiComp(bme280_s32_t adc_H) 
{
    bme280_s32_t H;
    H = (t_fine - ((bme280_s32_t)76800));
    H = (((((adc_H << 14) - (((bme280_s32_t)calib.dig_H4) << 20) - (((bme280_s32_t)calib.dig_H5) * 
    H)) + ((bme280_s32_t)16384)) >> 15) * (((((((H *
    ((bme280_s32_t)calib.dig_H6)) >> 10) * (((H * ((bme280_s32_t)calib.dig_H3))  >>  11) +
    ((bme280_s32_t)32768)))  >>  10) + ((bme280_s32_t)2097152)) * ((bme280_s32_t)calib.dig_H2) +
    8192) >> 14));
    H = (H - (((((H >> 15) * (H >> 15)) >> 7) *
        ((bme280_s32_t)calib.dig_H1)) >> 4));
    H = (H < 0 ? 0 : H);
    H = (H > 419430400 ? 419430400 : H);
    return (bme280_u32_t)(H >> 12);
}
