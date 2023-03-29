#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "main.h"
#include "i2c.h"
typedef struct sServerSettings
{
  uint16_t serverPort; //LSB
  char serverUrl[30];
} sServerSettings;

typedef union uServerSettings
{
  uint8_t raw[sizeof(sServerSettings)];
  sServerSettings divided;
} uServerSettings;

typedef struct sSTH40Settings
{
  uint8_t tempEnabled : 1; //LSB
  uint8_t humidityEnabled : 1;
  uint8_t heaterMode : 2;
  uint8_t doNotUse : 4;
} sSTH40Settings;

typedef union uSTH40Settings
{
  uint8_t raw[sizeof(sSTH40Settings)];
  sSTH40Settings divided;
} uSTH40Settings;

typedef struct sIntervalSettings
{
  uint16_t measurementS; //LSB
  uint16_t measurementBatS;
  uint16_t sendS;
  uint16_t sendBatS;
} sIntervalSettings;

typedef union uIntervalSettings
{
  uint8_t raw[sizeof(sIntervalSettings)];
  sIntervalSettings divided;
} uIntervalSettings;

typedef struct EEPROM
{
  int8_t utcH;
  uSTH40Settings SHT40Settings;
  uIntervalSettings intervalSettings;
  char wlanSSID[32];
  char wlanPassword[32];
  uServerSettings serverSettings;  

} EEPROM;

EEPROM new_EEPROM(void);
void EEPROM_load(EEPROM *self);
void i2c_test(void);

#endif /*__EEPROM_H__ */