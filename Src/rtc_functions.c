#include "rtc_functions.h"

static bool is_DST(uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour);
static bool is_leap_year(uint16_t year);

/**
 * @brief Set the rtc time from the wifi-cips sysTime and adjusts timezone and DTS
 *
 * @param hrtc RTC Handle
 * @param pstrSystemTime Winc1500 time struct
 */
void set_rtc_time(RTC_HandleTypeDef *hrtc, tstrSystemTime *pstrSystemTime)
{
  if (pstrSystemTime->u16Year > 0)
  {
    adjus_time(pstrSystemTime, eeprom.divided.utcH);
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    time.Hours = pstrSystemTime->u8Hour;
    time.Minutes = pstrSystemTime->u8Minute;
    time.Seconds = pstrSystemTime->u8Second;
    date.Year = (pstrSystemTime->u16Year - 2000);
    date.Month = pstrSystemTime->u8Month;
    date.Date = pstrSystemTime->u8Day;
    date.WeekDay = calc_weekday(pstrSystemTime->u16Year, pstrSystemTime->u8Month, pstrSystemTime->u8Day);

    HAL_RTC_SetDate(hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_SetTime(hrtc, &time, RTC_FORMAT_BIN);
    set_DTS(hrtc);
  }
}

 

/**
 * @brief calculates the weekday from a date
 * @details using Michael Keith and Tom Craver's expression
 *          https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Keith
 * @param[in] year
 * @param[in] month
 * @param[in] day
 * @return uint8_t (0 = sunday, 1 = monday, ..)
 */
uint8_t calc_weekday(uint16_t year, uint8_t month, uint8_t day)
{
  return (day += month < 3 ? year-- : year - 2, 23 * month / 9 + day + 4 + year / 4 - year / 100 + year / 400) % 7;
}

/**
 * @brief sets or unsets daylight saving time.
 * 
 * @param hrtc RTC handle 
 */
void set_DTS(RTC_HandleTypeDef *hrtc)
{
  RTC_TimeTypeDef time = {0};
  RTC_DateTypeDef date = {0};
  HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(hrtc, &date, RTC_FORMAT_BIN);

  if (is_DST(date.Month, date.Date, date.WeekDay, time.Hours))
  {
    HAL_RTC_DST_Add1Hour(hrtc);
  }
  else
  {
    HAL_RTC_DST_Sub1Hour(hrtc);
  }
}

/**
 * @brief function to check if current date is daylight saving time.
 *
 * @param month
 * @param day
 * @param weekDay 0=sunday, 1=monday, ..
 * @param hour
 * @return true if daylight saving
 */
static bool is_DST(uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour)
{
  if (month > 3 && month < 10)
  {
    return true;
  }

  // after last Sunday 2:00
  if (day - weekDay >= 25 && (weekDay > 0 || hour >= 2))
  {
    if (month == 3)
    {
      return true;
    }
  }
  else // before last Sunday 2:00
  {
    if (month == 10)
    {
      return true;
    }
  }

  return false;
}

/**
 * @brief calculates if the year is a leap year or not
 *
 * @param[in] year the year to check
 * @return true if leap year
 */
static bool is_leap_year(uint16_t year)
{
  if (year % 400 == 0)
  {
    return true;
  }
  else if (year % 100 == 0)
  {
    return false;
  }
  else if (year % 4 == 0)
  {
    return true;
  }
  return false;
}

/**
 * @brief takes the system time from the wifi-chip and adjust it by maximum +-24h
 *
 * @param[out] sysTime time struct from the wifichip
 * @param[in] adjHours amount of time in hours to adjust
 * @return true if time is adjusted
 */
bool adjus_time(tstrSystemTime *sysTime, int adjHours)
{
  uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (is_leap_year(sysTime->u16Year))
  {
    daysInMonth[1] = 29;
  }

  if (adjHours == 0 || adjHours > 24 || adjHours < -24)
  {
    return false;
  }

  if (adjHours > 0)
  {
    sysTime->u8Hour += adjHours;
    if (sysTime->u8Hour >= 24)
    {
      sysTime->u8Hour = sysTime->u8Hour - 24;

      if (sysTime->u8Day == daysInMonth[sysTime->u8Month - 1])
      {
        sysTime->u8Day = 0;
        if (sysTime->u8Month == 12)
        {
          sysTime->u8Month = 0;
          sysTime->u16Year++;
        }
        sysTime->u8Month++;
      }
      sysTime->u8Day++;
    }
  }
  else
  {
    sysTime->u8Hour += adjHours;
    if (sysTime->u8Hour > 0)
    {
      sysTime->u8Hour = sysTime->u8Hour + 24;
      if (sysTime->u8Day == 1)
      {
        if (sysTime->u8Month == 1)
        {
          sysTime->u8Month = 12;
          sysTime->u16Year--;
        }
        else
        {
          sysTime->u8Month--;
        }
        sysTime->u8Day = daysInMonth[sysTime->u8Month - 1] + 1;
      }
      sysTime->u8Day--;
    }
  }

  return true;
}

/**
 * @brief prints date and time
 * 
 * @param hrtc RTC handle
 */
void print_time(RTC_HandleTypeDef *hrtc)
{
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(hrtc, &date, RTC_FORMAT_BIN);
  printf("date: %02d-%02d-%02d time: %02d:%02d:%02d weekday: %d\r\n",
         date.Year, date.Month, date.Date, time.Hours, time.Minutes, time.Seconds, date.WeekDay);
}
