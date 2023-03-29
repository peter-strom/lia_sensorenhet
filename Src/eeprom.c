#include "eeprom.h"
#define DEV_ADDR_GYRO (0x18 << 1)
#define EEPROM_DEV_ADDR (0x50 << 1)   // ok
#define EEPROM_UTC_ADDR_REG 0x0020 // ok
#define EEPROM_SHT40_SETTINGS_ADDR_REG 0x0040 // ok
#define EEPROM_INTERVAL_SETTINGS_ADDR_REG 0x0060 // ok
#define EEPROM_WLAN_SSID_ADDR_REG 0x0100 // ok
#define EEPROM_WLAN_PASS_ADDR_REG 0x0120 // ok
#define EEPROM_SERVER_SETTINGS_ADDR_REG 0x0140 // ok

void EEPROM_i2c_read(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff);

EEPROM new_EEPROM(void)
{
  EEPROM self;
  self.utcH = 0x00;
  self.SHT40Settings.raw[0] = 0x7F;
  self.intervalSettings;
  self.wlanSSID;
  self.wlanPassword;
  self.serverSettings;
  return self;
}

void EEPROM_load(EEPROM *self)
{
  EEPROM_i2c_read(EEPROM_DEV_ADDR, EEPROM_UTC_ADDR_REG, &self->utcH);
  EEPROM_i2c_read(EEPROM_DEV_ADDR, EEPROM_SHT40_SETTINGS_ADDR_REG, &self->SHT40Settings.raw);
  EEPROM_i2c_read(EEPROM_DEV_ADDR, EEPROM_INTERVAL_SETTINGS_ADDR_REG, &self->intervalSettings.raw);
  EEPROM_i2c_read(EEPROM_DEV_ADDR, EEPROM_WLAN_SSID_ADDR_REG, &self->wlanSSID);
  EEPROM_i2c_read(EEPROM_DEV_ADDR, EEPROM_WLAN_PASS_ADDR_REG, &self->wlanPassword);
  EEPROM_i2c_read(EEPROM_DEV_ADDR, EEPROM_SERVER_SETTINGS_ADDR_REG, &self->serverSettings.raw);
  printf("%s\r\n", (char)self->intervalSettings.divided.sendBatS);
}

void EEPROM_print(EEPROM *self)
{
  printf("LSB : %d \r\n", self->SHT40Settings.divided.tempEnabled);
  printf("3 : %d \r\n", self->SHT40Settings.divided.heaterMode);
  printf("size of : %d \r\n", sizeof(self->SHT40Settings));
}

void EEPROM_i2c_read(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff)
{
  if (HAL_I2C_IsDeviceReady(&hi2c1, devAddr, 1, 100) != HAL_OK)
  {
#ifdef DEBUG_MODE
    printf("error connecting to device address: %#04x\n\r", devAddr >> 1);
#endif
  }
  else
  {
#ifdef DEBUG_MODE
    printf("connecting to device address: %#04x\n\r", devAddr >> 1);
#endif
  HAL_I2C_Mem_Read(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_16BIT, dataBuff, sizeof(dataBuff), 100);


  }
}

  void i2c_test(void)
  {
    uint8_t buff[6];
    if (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_DEV_ADDR, 1, 100) != HAL_OK)
    {
      printf("error connecting to device address: %#04x\n\r", EEPROM_DEV_ADDR >> 1);
    }
    else
    {
      printf("connecting to device address: %#04x\n\r", EEPROM_DEV_ADDR >> 1);
      char server[] = "kungariket.se";
      // printf("size of kungariket.se : %d \n\r", sizeof(server));
      uint8_t tx_buff[8] = {0x00, 0x60, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F};

      // HAL_I2C_Mem_Write(&hi2c1, EEPROM_DEV_ADDR, EEPROM_SERVER_ADDR_REG, I2C_MEMADD_SIZE_16BIT, server, sizeof(server), 100);

      // HAL_I2C_Master_Transmit(&hi2c1,EEPROM_DEV_ADDR,tx_buff,sizeof(tx_buff),100);

      HAL_Delay(140);
      uint8_t rx_buff[40] = {0};
      // memset(&rx_buff,1,40);
      HAL_I2C_Mem_Read(&hi2c1, EEPROM_DEV_ADDR, EEPROM_SERVER_SETTINGS_ADDR_REG, I2C_MEMADD_SIZE_16BIT, rx_buff, 40, 100);

      for (uint8_t i = 0; i < sizeof(rx_buff); i++)
      {
        printf("reg %d : %#04x \t", i, rx_buff[i]);
        if (i % 4 == 0)
        {
          printf("\r\n");
        }
      }
      printf("\r\n");
      printf("string %s \r\n", rx_buff);
      // printf("register: %#04x temp_data0: %#06x temp_data1: %#06x temp_checksum: %#06x rh_data0: %#06x rh_data1: %#06x rh_checksum: %#06x\n\r", I2C_dev_reg, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]);
      // printf("temp: %d degrees C \n\r", t_degC);
      // printf("humidity: %d RH \n\r", rh_pRH);
    }
  }
