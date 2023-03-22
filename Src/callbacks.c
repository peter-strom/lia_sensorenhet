#include "callbacks.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "driver/include/m2m_wifi.h"
#include "power.h"

extern tpfNmBspIsr gpfIsr; // pointer to WINC HIF isr function
static ButtonMode resetUpdateButton = UPDATE_SETTINGS;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  // HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

  if (GPIO_Pin == IRQ_POWER_SUPPLY_Pin) // EXTI0
  {
    psu_isr();
  }

  if (GPIO_Pin == IRQN_WIFI_Pin) // EXTI1
  {
#ifdef DEBUG_MODE
    printf("EXTI_CALLBACK: WIFI\r\n");
#endif
    gpfIsr();
  }

  if (GPIO_Pin == IRQ_OFF_Pin) // EXTI2
  {
    printf("poweroff callbacl");
    //power_off_isr(REQUIRED_BUTTON_PRESS_TIME_MS);
  }

  if (GPIO_Pin == IRQ_SENSOR_RESET_UPDATE_Pin) // EXTI4
  {
    if (resetUpdateButton == SENSOR_RESET)
    {
      resetUpdateButton = UPDATE_SETTINGS;
#ifdef DEBUG_MODE
      printf("sensor_reset!\r\n");
#endif
    }
    else
    {
#ifdef DEBUG_MODE
      printf("update_settings!\r\n");
#endif
    }
  }

  if (GPIO_Pin == IRQ_SENSOR_Pin) // EXTI9_5
  {
    if (resetUpdateButton == UPDATE_SETTINGS)
    {
      resetUpdateButton = SENSOR_RESET;
#ifdef DEBUG_MODE
      printf("sensor_alarm!\r\n");
#endif
    }
  }

  if (GPIO_Pin == IRQN_USBC_Pin) // EXTI15_10
  {
#ifdef DEBUG_MODE
    printf("EXTI_CALLBACK: USBC \r\n");
#endif
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim2.Instance)
  {
#ifdef DEBUG_MODE
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    printf("htim2: time: %02d:%02d:%02d ---timer\r\n", time.Hours, time.Minutes, time.Seconds);
#endif
  }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
// EXTI_IMR1_IM18_Msk;
// if(RTC_EXTI_LINE_ALARM_EVENT==1)
#ifdef DEBUG_MODE
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(hrtc, &date, RTC_FORMAT_BIN);
  printf("hrtc: date: %02d-%02d-%02d time: %02d:%02d:%02d -alarm\r\n", date.Year, date.Month, date.Date, time.Hours, time.Minutes, time.Seconds);
#endif
}
