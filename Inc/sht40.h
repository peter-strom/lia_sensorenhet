/**
 * @brief STH40 Humidity and Temperature Sensor
 * https://www.mouser.se/datasheet/2/682/Datasheet_SHT4x-3003109.pdf
 * @todo CRC check (not highest priority)
 *
 */
#ifndef __SHT40_H__
#define __SHT40_H__

#define SHT40_DEV_ADDR (0x44 << 1)
#define SHT40_HIGH_PRECISION 0xFD
#define SHT40_SERIAL_NUMBER 0x89
#define SHT40_HIGH_PRECISION_200MW_1S 0x39
#define SHT40_HIGH_PRECISION_200MW_100MS 0x32
#define SHT40_HIGH_PRECISION_110MW_1S 0x2F

#include "main.h"
#include "i2c.h"
#include <stdbool.h>
typedef struct SHT40
{
  uint8_t temperatureC;
  uint8_t humidityRH;
} SHT40;

SHT40 new_SHT40(void);
bool SHT40_read_high_precision(SHT40 *self, uint8_t heaterMode);
void SHT40_print(SHT40 *self);

#endif /*__SHT40_H__ */