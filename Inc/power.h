#ifndef __POWER_H__
#define __POWER_H__

#include "main.h"
#include "tim.h"

typedef enum
{
  OUTLET,
  BATTERY
} PSUmode;

void psu_init(void);
void psu_isr(void);
void power_off_isr(uint16_t required_time_ms);

#endif /*__POWER_H__ */