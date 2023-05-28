#include "measure.h"
#include "sht40.h"
#include "../Drivers/protobuf/measure.pb-c.h"
#include "rtc.h"
#include "eeprom.h"

void measure(void)
{
  measureNow = 0;
  char timestamp[18];
  get_time(&hrtc, &timestamp);
  SHT40 tempHumiditySensor = new_SHT40();
  SHT40_read_high_precision(&tempHumiditySensor, eeprom.divided.SHT40_heaterMode);

  Measure__MeasureMsg measureMsg;
  measure__measure_msg__init(&measureMsg);
  measureMsg.humidity = tempHumiditySensor.humidityRH;
  measureMsg.temperature = tempHumiditySensor.temperatureC;
  measureMsg.timestamp = timestamp;
  measureMsg.size = 0x01;
  measureMsg.size = measure__measure_msg__get_packed_size(&measureMsg);
  uint8_t protoBuff[measureMsg.size];
  measure__measure_msg__pack(&measureMsg, &protoBuff);

#ifdef DEBUG_MODE
  printf("size: %d bytes \r\n", measureMsg.size);
  printf("temperature: %d \r\n",measureMsg.temperature);
  printf("humidity: %d \r\n",measureMsg.humidity);
  printf("hex: \r\n");
  for (uint8_t i = 0; i < measureMsg.size; i++)
  {
    printf("%2x ", protoBuff[i]);
  }
#endif
  if(measureMsg.temperature > 30)
  {
    HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FAN_CONTROL_GPIO_Port, FAN_CONTROL_Pin, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FAN_CONTROL_GPIO_Port, FAN_CONTROL_Pin, GPIO_PIN_RESET);
  }
  send_socket_message(&protoBuff, measureMsg.size);
}