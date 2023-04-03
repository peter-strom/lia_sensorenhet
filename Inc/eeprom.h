/**
 * @brief BR24H64 eeprom
 * https://fscdn.rohm.com/en/products/databook/datasheet/ic/memory/eeprom/br24h64xxx-5ac-e.pdf
 *
 */
#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "main.h"
#include "i2c.h"
#include <stdbool.h>

#define EEPROM_DEV_ADDR (0x50 << 1)
#define EEPROM_START_REG_ADDR 0x0000
// Number of bytes that are written as
// a group and affect wrtie cycles
#define EEPROM_CHUNK_SIZE 4

/**
 * @brief eeprom settings struct
 *
 */
typedef struct sEEPROM
{
  int8_t utcH;
  uint8_t SHT40_tempEnabled : 1; // LSB
  uint8_t SHT40_humidityEnabled : 1;
  uint8_t SHT40_heaterMode : 2;
  uint8_t SHT40_doNotUse : 4;
  uint16_t interval_measurementS; // LSB
  uint16_t interval_measurementBatS;
  uint16_t interval_sendS;
  uint16_t interval_sendBatS;
  char wlan_SSID[32];
  char wlan_password[32];
  uint16_t server_port; // LSB
  char server_url[30];
} sEEPROM;

/**
 * @brief Union used to easy write and read data from eeprom
 *
 */
typedef union uEEPROM
{
  uint8_t raw[sizeof(sEEPROM)];
  sEEPROM divided;
} uEEPROM;

extern uEEPROM eeprom;

bool EEPROM_load(uEEPROM *self);
bool EEPROM_save(uEEPROM *self);
void EEPROM_print(uEEPROM *self);

#endif /*__EEPROM_H__ */