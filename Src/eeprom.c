/**
 * https://fscdn.rohm.com/en/products/databook/datasheet/ic/memory/eeprom/br24h64xxx-5ac-e.pdf
 */
#include "eeprom.h"
#define DEV_ADDR_GYRO (0x18 << 1)
#define EEPROM_DEV_ADDR (0x50 << 1)
#define EEPROM_START_REG_ADDR 0x0000
#define EEPROM_CHUNK_SIZE 4 // number of bytes that are written as a group

bool EEPROM_i2c_read_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size);
bool EEPROM_i2c_write_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size);
void print_hex(uint8_t *data_buff, uint8_t size);

uEEPROM new_EEPROM(void)
{
  uEEPROM self;
  return self;
}

bool EEPROM_load(uEEPROM *self)
{
  bool statusOk = true;
  printf("size: %d \r\n", sizeof(sEEPROM));
  statusOk &= EEPROM_i2c_read_wrapper(EEPROM_DEV_ADDR, EEPROM_START_REG_ADDR,
                                      self->raw, sizeof(self->raw));
  
  print_hex(&self->divided.server_url, sizeof(self->divided.server_url));

  return statusOk;
}

void EEPROM_print(uEEPROM *self)
{
  /*
  printf("LSB : %d \r\n", self->SHT40Settings.divided.tempEnabled);
  printf("3 : %d \r\n", self->SHT40Settings.divided.heaterMode);
  printf("size of : %d \r\n", sizeof(self->SHT40Settings));
  */
}

void print_hex(uint8_t *dataBuff, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++)
  {
    printf("reg %d : %#04x \t", i, dataBuff[i]);
    if (i % 4 == 0)
    {
      printf("\r\n");
    }
  }
  printf("\r\n");
  printf("string %s \r\n", dataBuff);
}

bool EEPROM_save(uEEPROM *self)
{
  bool statusOk = true;
  uEEPROM old = new_EEPROM();
  EEPROM_load(&old);
  uint16_t regSize = sizeof(self->raw);
  for (uint16_t addr = 0; addr < regSize; addr += EEPROM_CHUNK_SIZE)
  {
    uint8_t writeSize = EEPROM_CHUNK_SIZE;
    if (addr + EEPROM_CHUNK_SIZE > regSize)
    {
      writeSize = EEPROM_CHUNK_SIZE % addr;
    }

    for (uint8_t i = 0; i < writeSize; i++)
    {
      if (self->raw[addr + i] != old.raw[addr + i])
      {
        statusOk &= EEPROM_i2c_write_wrapper(EEPROM_DEV_ADDR, addr,
                                             self->raw[addr], writeSize);
                                             
        break;
      }
    }
  }

  return statusOk;
}

bool EEPROM_i2c_read_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size)
{
  bool statusOk = false;
  if (HAL_I2C_Mem_Read(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_16BIT, dataBuff, size, 100) == HAL_OK)
  {
    statusOk = true;
  }
  return statusOk;
}

bool EEPROM_i2c_write_wrapper(uint8_t devAddr, uint16_t regAddr, uint8_t *dataBuff, uint8_t size)
{
  printf("writing: %d\r\n",regAddr);
  bool statusOk = false;
  if (HAL_I2C_Mem_Write(&hi2c1, devAddr, regAddr, I2C_MEMADD_SIZE_16BIT, dataBuff, size, 100) == HAL_OK)
  {
    statusOk = true;
  }
  return statusOk;
}

