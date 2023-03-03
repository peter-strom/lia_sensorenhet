/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OPERATION1_Pin GPIO_PIN_13
#define OPERATION1_GPIO_Port GPIOC
#define WIFI_CHIP_EN_Pin GPIO_PIN_4
#define WIFI_CHIP_EN_GPIO_Port GPIOA
#define WIFI_WAKEUP_Pin GPIO_PIN_5
#define WIFI_WAKEUP_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_6
#define BUZZER_GPIO_Port GPIOA
#define LED_ERROR_Pin GPIO_PIN_7
#define LED_ERROR_GPIO_Port GPIOA
#define IRQ_POWER_SUPPLY_Pin GPIO_PIN_0
#define IRQ_POWER_SUPPLY_GPIO_Port GPIOB
#define IRQ_POWER_SUPPLY_EXTI_IRQn EXTI0_IRQn
#define IRQN_WIFI_Pin GPIO_PIN_1
#define IRQN_WIFI_GPIO_Port GPIOB
#define IRQ_OFF_Pin GPIO_PIN_2
#define IRQ_OFF_GPIO_Port GPIOB
#define WIFI_RESET_N_Pin GPIO_PIN_10
#define WIFI_RESET_N_GPIO_Port GPIOB
#define FAN_CONTROL_Pin GPIO_PIN_11
#define FAN_CONTROL_GPIO_Port GPIOB
#define OPERATION3_Pin GPIO_PIN_12
#define OPERATION3_GPIO_Port GPIOB
#define LED_ON_Pin GPIO_PIN_13
#define LED_ON_GPIO_Port GPIOB
#define IRQN_USBC_Pin GPIO_PIN_15
#define IRQN_USBC_GPIO_Port GPIOB
#define IRQN_USBC_EXTI_IRQn EXTI15_10_IRQn
#define SPI1_SSN_WIFI_Pin GPIO_PIN_8
#define SPI1_SSN_WIFI_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define IRQ_SENSOR_RESET_UPDATE_Pin GPIO_PIN_4
#define IRQ_SENSOR_RESET_UPDATE_GPIO_Port GPIOB
#define IRQ_SENSOR_RESET_UPDATE_EXTI_IRQn EXTI4_IRQn
#define IRQ_SENSOR_Pin GPIO_PIN_6
#define IRQ_SENSOR_GPIO_Port GPIOB
#define IRQ_SENSOR_EXTI_IRQn EXTI9_5_IRQn
#define OPERATION2_Pin GPIO_PIN_9
#define OPERATION2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
