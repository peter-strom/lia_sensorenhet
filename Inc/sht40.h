/**
 * SHT40 temp and humidity sensor
 */
#ifndef __SHT40_H__
#define __SHT40_H__

#define SHT40_DEV_ADDR (0x44 << 1)
#define SHT40_HIGH_PRECISION 0xFD
#define SHT40_SERIAL_NUMBER 0x89
#define SHT40_HIGH_PRECISION_200MW_1S 0x39
#define SHT40_HIGH_PRECISION_200MW_100MS 0x32

#include "main.h"
#include "i2c.h"
typedef struct SHT40
{
    uint8_t temperatureC;
    uint8_t humidityRH;
} SHT40;

SHT40 new_SHT40(void);
void SHT40_read_high_precision(SHT40 *self);
void SHT40_print(SHT40 *self);

#endif /*__SHT40_H__ */