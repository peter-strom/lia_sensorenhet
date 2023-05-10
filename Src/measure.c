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

  measureMsg.size = measure__measure_msg__get_packed_size(&measureMsg);
  uint8_t protoBuff[measureMsg.size];
  measure__measure_msg__pack(&measureMsg, &protoBuff);

#ifdef DEBUG_MODE
  printf("size: %d bytes \r\n", measureMsg.size);
  printf("hex: \r\n");
  for (uint8_t i = 0; i < measureMsg.size; i++)
  {
    printf("%2x ", protoBuff[i]);
  }
#endif
  send_socket_message(&protoBuff, measureMsg.size);
}