#include "callbacks.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "driver/include/m2m_wifi.h"
#include "power.h"
#include "rtc_functions.h"
#include "winc.h"
#include "sht40.h"

#include "../Drivers/protobuf/testmsg.pb-c.h"

extern tpfNmBspIsr gpfIsr; // pointer to WINC HIF isr function
static ButtonMode resetUpdateButton = UPDATE_SETTINGS;
uint8_t rxBuff[12];

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
    power_off_isr(REQUIRED_BUTTON_PRESS_TIME_MS);
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
      char msg[] = "settings_update_request";
      send_socket_message(&msg, sizeof(msg));
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
    
    if (gconnected == NOT_CONNECTED)
    {
      return;
    }
    
    //SHT40 temp_humidity_ = new_SHT40();
    //SHT40_read_high_precision(&temp_humidity_, 0);
    
    Testmsg__Testmsg testmsg;
    testmsg__testmsg__init(&testmsg);
    char timestamp[18];
    get_time(&hrtc, &timestamp);
    //testmsg.humidity = temp_humidity_.humidityRH;
    //testmsg.temperature = temp_humidity_.temperatureC;
    testmsg.timestamp = timestamp;
    size_t protoBuffSize = testmsg__testmsg__get_packed_size(&testmsg);
    printf("size: %d bytes \r\n", protoBuffSize);
    uint8_t protoBuff[protoBuffSize];
    testmsg__testmsg__pack(&testmsg, &protoBuff);
    for (uint8_t i = 0; i < protoBuffSize; i++)
    {
      printf("%2x ", protoBuff[i]);
    }
    printf("\r\nhumidity: %d \r\n", testmsg.humidity);

    send_socket_message(&protoBuff, protoBuffSize);

#ifdef DEBUG_MODE
    printf("htim2:(timer interval) --- ");
    print_time(&hrtc);
#endif
  }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
// EXTI_IMR1_IM18_Msk;
// if(RTC_EXTI_LINE_ALARM_EVENT==1)
#ifdef DEBUG_MODE
  printf("rtc:(alarm) --- ");
  print_time(&hrtc);
#endif
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  HAL_UART_Receive_IT(&huart2, rxBuff, 1);
#ifdef DEBUG_MODE
  // printf("recieved by uart: %d \r\n", rxBuff[0]);
#endif
  if (rxBuff[0] == 49) // test on
  {
    HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(OPERATION1_GPIO_Port, OPERATION1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(OPERATION2_GPIO_Port, OPERATION2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(OPERATION3_GPIO_Port, OPERATION3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FAN_CONTROL_GPIO_Port, FAN_CONTROL_Pin, GPIO_PIN_SET);
  }
  else if (rxBuff[0] == 48) // test off
  {
    HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OPERATION1_GPIO_Port, OPERATION1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OPERATION2_GPIO_Port, OPERATION2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OPERATION3_GPIO_Port, OPERATION3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FAN_CONTROL_GPIO_Port, FAN_CONTROL_Pin, GPIO_PIN_RESET);
  }
  else if (rxBuff[0] == 50) // test reset
  {
    HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_SET);
  }
  else if (rxBuff[0] == 51) // irq-sensor set, shuld be trigged by gpio-interrupt EXTI9_5
  {
#ifdef DEBUG_MODE
    printf("sensor_alarm!\r\n");
#endif
    if (resetUpdateButton == UPDATE_SETTINGS)
    {
      resetUpdateButton = SENSOR_RESET;
    }
  }
}
