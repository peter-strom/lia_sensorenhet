#include "sht40.h"

static void SHT40_read_from_sensor(SHT40 *self, uint8_t dev_addr, uint8_t command);

SHT40 new_SHT40()
{
    SHT40 self;
    self.humidityRH = 0;
    self.temperatureC = 0;
    return self;
}

void SHT40_read_high_precision(SHT40 *self)
{
    SHT40_read_from_sensor(self, SHT40_DEV_ADDR, SHT40_HIGH_PRECISION);
}

void SHT40_read_from_sensor(SHT40 *self, uint8_t dev_addr, uint8_t command)
{
    if (HAL_I2C_IsDeviceReady(&hi2c1, dev_addr, 1, 100) != HAL_OK)
    {
#ifdef DEBUG_MODE
        printf("error connecting to device address: %#04x\n\r", dev_addr >> 1);
#endif
    }
    else
    {
#ifdef DEBUG_MODE
        printf("connecting to device address: %#04x\n\r", dev_addr >> 1);
#endif
        uint8_t rx_buff[6];
        uint8_t tx_buff[1] = {command};
        HAL_I2C_Master_Transmit(&hi2c1, dev_addr, tx_buff, 1, 100);
        HAL_Delay(10);
        HAL_I2C_Master_Receive(&hi2c1, dev_addr, rx_buff, 6, 100);
#ifdef DEBUG_MODE
        printf("temp_data0: %#06x temp_data1: %#06x temp_checksum: %#06x rh_data0: %#06x rh_data1: %#06x rh_checksum: %#06x\n\r",
               rx_buff[0], rx_buff[1], rx_buff[2], rx_buff[3], rx_buff[4], rx_buff[5]);
#endif
        self->temperatureC = 175 * (rx_buff[0] << 8 | rx_buff[1]) / 65535 - 45;
        self->humidityRH = 125 * (rx_buff[3] << 8 | rx_buff[4]) / 65535 - 6;
    }
}

void SHT40_print(SHT40 *self)
{
    printf("temp: %d degrees C \n\r", self->temperatureC);
    printf("humidity: %d RH \n\r", self->humidityRH);
}
