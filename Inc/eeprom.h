#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "main.h"
#include "i2c.h"
#include <stdbool.h>

typedef struct sEEPROM
{
  
  int8_t utcH;
  uint8_t SHT40_tempEnabled : 1; //LSB
  uint8_t SHT40_humidityEnabled : 1;
  uint8_t SHT40_heaterMode : 2;
  uint8_t SHT40_doNotUse : 4;
  uint16_t interval_measurementS; //LSB
  uint16_t interval_measurementBatS;
  uint16_t interval_sendS;
  uint16_t interval_sendBatS;
  char wlan_SSID[32];
  char wlan_password[32];
  uint16_t server_port; //LSB
  char server_url[30];
  
  
} sEEPROM;

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