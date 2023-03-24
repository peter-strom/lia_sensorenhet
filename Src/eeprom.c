#include "eeprom.h"
#define DEV_ADDR_GYRO (0x18 << 1) 
#define DEV_ADDR_USBC (0x25 << 1) //ok

#define DEV_ADDR_EEPROM (0x50 << 1) //ok
#define DEV_ADDR_PRESSURE (0x5C << 1) //ok

/**
 * SHT40 temp and humidity sensor
*/
#define SHT40_DEV_ADDR (0x44 << 1) //ok
#define SHT40_HIGH_PRECISION 0xFD //6 bytes 
#define SHT40_SERIAL_NUMBER 0xFD //6 bytes 
/**
 * 2 * 8 bit registers example VL53L0X
 * tried one 16bit register. Did not work!
 *
 * 
 */
void i2c_test(void)
{
  uint8_t I2C_dev_address = SHT40_DEV_ADDR;
  uint16_t I2C_dev_reg = SHT40_SERIAL_NUMBER; // 0x108c  0x1070=0x51??
  uint8_t buff[6];
  if (HAL_I2C_IsDeviceReady(&hi2c1, I2C_dev_address, 1, 100) != HAL_OK)
  {
    printf("error connecting to device address: %#04x\n\r", I2C_dev_address >> 1);
  }
  else
  {
    printf("connecting to device address: %#04x\n\r", I2C_dev_address >> 1);
    uint8_t testbuff[1] = {SHT40_HIGH_PRECISION};
    HAL_I2C_Master_Transmit(&hi2c1,SHT40_DEV_ADDR,testbuff,1,100);
    HAL_Delay(10);
    HAL_I2C_Master_Receive(&hi2c1, SHT40_DEV_ADDR,buff,6,100);
    
    uint16_t temp = (buff[0] << 8 | buff[1]);
    uint8_t t_degC = 175 * temp / 65535 -45; 
    temp = (buff[3] << 8 | buff[4]);
    uint8_t rh_pRH = 125 * temp / 65535 -6; 
    //HAL_I2C_Mem_Read(&hi2c1, I2C_dev_address, I2C_dev_reg, I2C_MEMADD_SIZE_8BIT, buff, 6, 100);
    printf("register: %#04x temp_data0: %#06x temp_data1: %#06x temp_checksum: %#06x rh_data0: %#06x rh_data1: %#06x rh_checksum: %#06x\n\r", I2C_dev_reg, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]);
    printf("temp: %d degrees C \n\r", t_degC);
    printf("humidity: %d RH \n\r", rh_pRH);
 
    
  }
}
