
#ifndef __CONF_WINC_H__
#define __CONF_WINC_H__

#include "main.h"
#include "spi.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define CONF_WINC_DEBUG 1
#define CONF_WINC_PRINTF printf

#define CONF_WINC_USE_SPI (1)
#define SPI_WIFI_HANDLE_POINTER (&hspi1)
//#define SPI_WIFI_INIT() MX_SPI1_Init()

#define CONF_WINC_PIN_CHIP_ENABLE WIFI_CHIP_EN_Pin
#define CONF_WINC_PORT_CHIP_ENABLE WIFI_CHIP_EN_GPIO_Port

#define CONF_WINC_PIN_WAKE WIFI_WAKEUP_Pin
#define CONF_WINC_PORT_WAKE WIFI_WAKEUP_GPIO_Port

#define CONF_WINC_SPI_INT_PIN IRQN_WIFI_Pin // falling edge
#define CONF_WINC_SPI_INT_PORT IRQN_WIFI_GPIO_Port
#define CONF_WINC_EXTI_IRQN IRQN_WIFI_EXTI_IRQn

#define CONF_WINC_PIN_RESET WIFI_RESET_N_Pin
#define CONF_WINC_PORT_RESET WIFI_RESET_N_GPIO_Port

#define CONF_WINC_SSN_PIN SPI1_SSN_WIFI_Pin
#define CONF_WINC_SSN_GPIO_PORT SPI1_SSN_WIFI_GPIO_Port

#define NM_EDGE_INTERRUPT (1)

/**SPI1 GPIO Configuration
   PA1     ------> SPI1_SCK
   PA11     ------> SPI1_MISO
   PA12     ------> SPI1_MOSI
   */
#define SPI_WIFI_SCK_PIN GPIO_PIN_1
#define SPI_WIFI_SCK_GPIO_PORT GPIOA

#define SPI_WIFI_MISO_PIN GPIO_PIN_11
#define SPI_WIFI_MISO_GPIO_PORT GPIOA

#define SPI_WIFI_MOSI_PIN GPIO_PIN_12
#define SPI_WIFI_MOSI_GPIO_PORT GPIOA

#define SPI_WIFI SPI1
#define SPI3_WIFI_AF GPIO_AF5_SPI1




#endif /*__CONF_WINC_H__ */