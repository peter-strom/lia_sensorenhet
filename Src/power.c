#include "power.h"
static void psu_load_settings(PSUmode psuMode);
static void psu_update_mode(void);
static uint16_t measure_active_time(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
static void power_off(void);

/**
 * @brief init routine to update power supply mode
 *
 */
void psu_init(void)
{
  psu_update_mode();
}

/**
 * @brief interrupt service routine for power supply mode update
 *
 */
void psu_isr(void)
{
  psu_update_mode();
}

/**
 * @brief checks the value of IRQ_POWER_SUPPLY_Pin
 * OUTLET mode if high
 * BATTERY mode if low
 */
void psu_update_mode(void)
{
  if (GPIO_PIN_SET == HAL_GPIO_ReadPin(IRQ_POWER_SUPPLY_GPIO_Port, IRQ_POWER_SUPPLY_Pin))
  {
    psu_load_settings(OUTLET);
  }
  else
  {
    psu_load_settings(BATTERY);
  }
}

/**
 * @todo: read settings from eeprom, do not init timer if not enabled in settings.
 * @brief load power supply mode settings
 *
 * @param psu_mode OUTLET/BATTERY
 */
static void psu_load_settings(PSUmode psuMode)
{

  HAL_TIM_Base_DeInit(&htim2);
  if (psuMode == OUTLET)
  {
    htim2.Init.Period = 6000;
#ifdef DEBUG_MODE
    printf("PSU: outlet mode\r\n");
#endif
  }
  else
  {
    htim2.Init.Period = 12000;
#ifdef DEBUG_MODE
    printf("PSU: battery mode\r\n");
#endif
  }
  // HAL_TIM_Base_Init(&htim2);
  // HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief interrupt service routine for power off button
 * @todo add upload function
 * @param requiredTime_ms[in] the active time in ms required to start the routine
 */
void power_off_isr(uint16_t requiredTime_ms)
{
  uint16_t activeTime_ms = measure_active_time(IRQ_OFF_GPIO_Port, IRQ_OFF_Pin);

  if (activeTime_ms >= requiredTime_ms)
  {
#ifdef DEBUG_MODE
    printf("power_off!\r\n");
#endif
    power_off();
  }
}

/**
 * @brief measures the time the signal is active.
 * @details the measurement starts and stops on the flanks
 * @param GPIOx[IN] the GPIO port
 * @param GPIO_Pin[IN] the GPIO pin
 * @return uint16_t the measured time in ms.
 */
static uint16_t measure_active_time(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  static uint32_t tickStamp = -1;
  static uint16_t deltaTime_ms;
  deltaTime_ms = 0;

  if ((GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) && (tickStamp == -1))
  {
    tickStamp = HAL_GetTick();
  }
  else if ((GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) && (tickStamp > 0))
  {
    deltaTime_ms = HAL_GetTick() - tickStamp;
    tickStamp = -1;
  }
  return deltaTime_ms;
}

/**
 * @brief proper power off function from Piranha (ST forum)
 * @details https://community.st.com/s/question/0D53W00001bnh8dSAA/how-to-enter-standby-or-shutdown-mode-on-stm32
 * 
 */
static void power_off(void)
{
  // Configure wake-up features
  // WKUP1(PA0) - active high, WKUP4(PA2) - active low, pull-up
  PWR->PUCRA = PWR_PUCRA_PA2;                             // Set pull-ups for standby modes
  PWR->CR4 = PWR_CR4_WP4;                                 // Set wakeup pins' polarity to low level
  PWR->CR3 = PWR_CR3_APC | PWR_CR3_EWUP4 | PWR_CR3_EWUP1; // Enable pin pull configurations and wakeup pins
  PWR->SCR = PWR_SCR_CWUF;                                // Clear wakeup flags

  // Configure MCU low-power mode for CPU deep sleep mode
  PWR->CR1 |= PWR_CR1_LPMS_STANDBY; // PWR_CR1_LPMS_SHUTDOWN
  (void)PWR->CR1;                   // Ensure that the previous PWR register operations have been completed

  // Configure CPU core
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // Enable CPU deep sleep mode
#ifdef NDEBUG
  DBGMCU->CR = 0; // Disable debug, trace and IWDG in low-power modes
#endif

  // Enter low-power mode
  for (;;)
  {
    __DSB();
    __WFI();
  }
}