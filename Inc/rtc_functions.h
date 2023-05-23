#ifndef __RTC_FUNCTIONS_H__
#define __RTC_FUNCTIONS_H__

#include "main.h"
#include "rtc.h"
#include "driver/include/m2m_wifi.h"
#include "eeprom.h"

void set_rtc_time(RTC_HandleTypeDef *hrtc, tstrSystemTime *pstrSystemTime);
uint8_t calc_weekday(uint16_t year, uint8_t month, uint8_t day);
bool adjus_time(tstrSystemTime *sysTime, int adjHours);
void get_time(RTC_HandleTypeDef *hrtc, char *dataBuff);
void print_time(RTC_HandleTypeDef *hrtc);
void set_DTS(RTC_HandleTypeDef *hrtc);

#endif /*__RTC_FUNCTIONS_H__ */