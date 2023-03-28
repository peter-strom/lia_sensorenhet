#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "main.h"
#include "i2c.h"

typedef struct EEPROM
{
    int8_t utcH;
    uint8_t SHT40_settings;
    uint16_t m_intervalS;
    uint16_t m_interval_battS
} EEPROM;

void i2c_test(void);

#endif /*__EEPROM_H__ */