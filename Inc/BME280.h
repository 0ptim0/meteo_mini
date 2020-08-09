void BME_Begin(void);
void BME_Measure(int32_t *T, uint32_t *P, uint32_t *H);
/*void sbor(uint32_t *data, int reg);*/
#define add_BME280  0x76
#define W           0
#define R           1
#define ctrl_h      0xF2
#define ctrl_meas   0xF4
#define config      0xF5   
#define res_16      0x05
#define res_8       0x04
#define res_4       0x03
#define res_2       0x02
#define res_1       0x01
#define skip        0x00
#define forced      0x01
#define filt        0x02
#define reset       0xE0
#define com_reset   0xB6
#define start_reg   0xF7
#define reg_calib0  0x88
#define reg_calib26 0xE1

