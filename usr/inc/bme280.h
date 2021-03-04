/* Parameters */
#define BME280_Address 0x76

/* Buffer parameters */
#define BME280_LengthBuf 1024

/* Type of data */
#define BME280_WriteBit 0x00
#define BME280_ReadBit	0x01

/* Filter parameters */
#define BME280_Oversampling_X1 	0x01
#define BME280_Oversampling_X2 	0x02
#define BME280_Oversampling_X4 	0x03
#define BME280_Oversampling_X8 	0x04
#define BME280_Oversampling_X16 0x05

/* Mode */
#define BME280_SleepMode	0x00
#define BME280_ForcedMode 	0x01
#define BME280_NormalMode 	0x03

/* Calibration type */
typedef long signed int BME280_S32_t;
typedef long unsigned int BME280_U32_t;
typedef long long signed int BME280_S64_t;

typedef struct {
    unsigned short dig_T1;
    signed short dig_T2;
    signed short dig_T3;
    unsigned short dig_P1;
    signed short dig_P2;
    signed short dig_P3;
    signed short dig_P4;
    signed short dig_P5;
    signed short dig_P6;
    signed short dig_P7;
    signed short dig_P8;
    signed short dig_P9;
    unsigned char dig_H1;
    signed short dig_H2;
    unsigned char dig_H3;
    signed short dig_H4;
    signed short dig_H5;
    signed char dig_H6;
} Calib_t;

typedef struct {
    uint32_t Pressure;
    uint32_t Temperature;
    uint32_t Humiduty;
} Measurement_t;

typedef struct {
    uint8_t ControlByte;
    uint8_t Settings;
    Calib_t Calib;
    Measurement_t Measurement;
} BME280_t;

void BME280_Init(void);
void BME280_Settings(uint8_t TempSampl, uint8_t PresSampl, uint8_t HumSampl);
void BME280_Measure(void);

extern BME280_t BME280;
