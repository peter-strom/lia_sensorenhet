#include "sht40.h"

static bool SHT40_read_from_sensor(SHT40 *self, uint8_t devAddr, uint8_t command, uint16_t delayMs);

SHT40 new_SHT40()
{
  SHT40 self;
  self.humidityRH = 0;
  self.temperatureC = 0;
  return self;
}

/**
 * @brief read sensor values and store tha data into a struct.
 * Note: the heater affect the temperature.
 * heater mode 0 = off
 * heater mode 1 = 200 mW for 1 s
 * heater mode 2 = 200 mW for 100 ms
 * heater mode 3 = 110 mW for 1 s
 * @param[out] self the struct to store data
 * @param[in] heaterMode operates the build in heater to remove condensed water 
 * @return true if successfull
 */
bool SHT40_read_high_precision(SHT40 *self, uint8_t heaterMode)
{
  bool statusOk = true;
  switch (heaterMode)
  {
  case 1:
    statusOk &= SHT40_read_from_sensor(self, SHT40_DEV_ADDR, SHT40_HIGH_PRECISION_200MW_1S, 1010);
    break;

  case 2:
    statusOk &= SHT40_read_from_sensor(self, SHT40_DEV_ADDR, SHT40_HIGH_PRECISION_200MW_100MS, 110);
    break;

  case 3:
    statusOk &= SHT40_read_from_sensor(self, SHT40_DEV_ADDR, SHT40_HIGH_PRECISION_110MW_1S, 1010);
    break;

  default:
    statusOk &= SHT40_read_from_sensor(self, SHT40_DEV_ADDR, SHT40_HIGH_PRECISION, 10);
  }
  return statusOk;
}

/**
 * @brief sends command and reads the result from the sensor
 * 
 * @param[out] self the struct to store data
 * @param[in] devAddr device 12c adress
 * @param[in] command sensor command to execute
 * @param[in] delayMs delay needed before reading the result
 * @return true if successfull
 */
static bool SHT40_read_from_sensor(SHT40 *self, uint8_t devAddr, uint8_t command, uint16_t delayMs)
{
  bool statusOk = true;
  uint8_t rxBuff[6];
  uint8_t txBuff[1] = {command};
  statusOk &= HAL_I2C_Master_Transmit(&hi2c1, devAddr, txBuff, 1, 100);
  HAL_Delay(delayMs);
  statusOk &= HAL_I2C_Master_Receive(&hi2c1, devAddr, rxBuff, 6, 100);
#ifdef DEBUG_MODE
  printf("temp_data0: %#06x temp_data1: %#06x temp_checksum: %#06x rh_data0: %#06x rh_data1: %#06x rh_checksum: %#06x\n\r",
         rxBuff[0], rxBuff[1], rxBuff[2], rxBuff[3], rxBuff[4], rxBuff[5]);
#endif
  self->temperatureC = 175 * (rxBuff[0] << 8 | rxBuff[1]) / 65535 - 45;
  self->humidityRH = 125 * (rxBuff[3] << 8 | rxBuff[4]) / 65535 - 6;
  return statusOk;
}

/**
 * @brief prints the data from the struct
 * 
 * @param[in] self the struct that stores the data
 */
void SHT40_print(SHT40 *self)
{
  printf("temp: %d degrees C \n\r", self->temperatureC);
  printf("humidity: %d RH \n\r", self->humidityRH);
}
