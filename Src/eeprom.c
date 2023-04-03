#include "eeprom.h"

bool EEPROM_i2c_read_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size);
bool EEPROM_i2c_write_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size);
void print_hex(uint8_t *data_buff, uint8_t size);

/**
 * @brief Loads data from eeprom to a struct.
 *
 * @param[out] self destination struct
 * @return true if successfull
 */
bool EEPROM_load(uEEPROM *self)
{
  bool statusOk = true;
  printf("size: %d \r\n", sizeof(sEEPROM));
  statusOk &= EEPROM_i2c_read_wrapper(EEPROM_DEV_ADDR, EEPROM_START_REG_ADDR,
                                      self->raw, sizeof(self->raw));
  return statusOk;
}

/**
 * @brief Saves data from struct to eeprom.
 * @details First checks for changes to save write cycles
 *          and writes the whole chunk (hopefully as a group)
 * 
 * @param[in] self source struct
 * @return true if successfull
 */
bool EEPROM_save(uEEPROM *self)
{
  bool statusOk = true;
  uEEPROM old;
  EEPROM_load(&old);
  uint16_t regSize = sizeof(self->raw);
  for (uint16_t addr = 0; addr < regSize; addr += EEPROM_CHUNK_SIZE)
  {
    uint8_t writeSize = EEPROM_CHUNK_SIZE;
    if (addr + EEPROM_CHUNK_SIZE > regSize)
    {
      writeSize = regSize % addr;
    }
#ifdef DEBUG_MODE
    printf("addr: %d writesize: %d \r\n", addr, writeSize);
#endif
    for (uint8_t i = 0; i < writeSize; i++)
    {
      if (self->raw[addr + i] != old.raw[addr + i])
      {
#ifdef DEBUG_MODE
        printf("old: %d - new: %d\r\n", old.raw[addr + i], self->raw[addr + i]);
        if (statusOk)
        {
          printf("writing OK\r\n");
        }
        else
        {
          printf("writing not OK\r\n");
        }
#endif
        statusOk &= EEPROM_i2c_write_wrapper(EEPROM_DEV_ADDR, addr,
                                             &self->raw[addr], writeSize);
        break;
      }
    }
  }

  return statusOk;
}

/**
 * @brief Prints all the settings from a struct.
 *
 * @param[in] self source struct to print
 */
void EEPROM_print(uEEPROM *self)
{
  printf("------------------eeprom-----------------------\r\n");
  printf("Time zone: %d \r\n", self->divided.utcH);
  printf("STH40 Heater mode: %d \r\n", self->divided.SHT40_heaterMode);
  printf("STH40 Humidity enabled: %d \r\n", self->divided.SHT40_humidityEnabled);
  printf("STH40 Temp enabled: %d \r\n", self->divided.SHT40_tempEnabled);
  printf("Interval measurements in seconds: %d \r\n", self->divided.interval_measurementS);
  printf("Interval measurements (batery mode) in seconds: %d \r\n", self->divided.interval_measurementBatS);
  printf("Interval send in seconds: %d \r\n", self->divided.interval_sendS);
  printf("Interval send (batery mode) in seconds: %d \r\n", self->divided.interval_sendBatS);
  printf("WLAN SSID: %s \r\n", self->divided.wlan_SSID);
  printf("WLAN password: %s \r\n", self->divided.wlan_password);
  printf("server url: %s \r\n", self->divided.server_url);
  printf("server port: %d \r\n", self->divided.server_port);
  printf("-----------------------------------------------\r\n");
}

/**
 * @brief i2c wrapper function to read data from eeprom.
 *
 * @param[in] devAddr device i2c address
 * @param[in] regAddr registeradress to start read from
 * @param[out] dataBuff databuffert to store the data in
 * @param[in] size how many bytes of data to read.
 * @return true if successfull
 */
bool EEPROM_i2c_read_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size)
{
  bool statusOk = false;
  if (HAL_I2C_Mem_Read(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_16BIT, dataBuff, size, 100) == HAL_OK)
  {
    statusOk = true;
  }
  return statusOk;
}

/**
 * @brief i2c wrapper function to write data to eeprom.
 *
 * @param[in] devAddr device i2c address
 * @param[in] regAddr registeradress to start read from
 * @param[out] dataBuff databuffert to store the data in
 * @param[in] size how many bytes of data to read.
 * @return true if successfull
 */
bool EEPROM_i2c_write_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size)
{
  bool statusOk = false;
  if (HAL_I2C_Mem_Write(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_16BIT, dataBuff, size, 100) == HAL_OK)
  {
    statusOk = true;
  }
  return statusOk;
}
