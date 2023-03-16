##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.19.2] date: [Wed Mar 15 14:45:29 CET 2023] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = afry_lia


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/stm32l4xx_it.c \
Src/stm32l4xx_hal_msp.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c \
Src/system_stm32l4xx.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c \
Src/main.c \
Src/usart.c \
Src/rtc.c \
Src/tim.c \
Src/gpio.c \
Src/i2c.c \
Src/spi.c \
Drivers/winc1500/host_drv/bsp/source/nm_bsp_stm32.c \
Drivers/winc1500/host_drv/bus_wrapper/source/nm_bus_wrapper_stm32.c \
Drivers/winc1500/host_drv/common/source/nm_common.c \
Drivers/winc1500/host_drv/driver/source/m2m_ate_mode.c \
Drivers/winc1500/host_drv/driver/source/m2m_crypto.c \
Drivers/winc1500/host_drv/driver/source/m2m_hif.c \
Drivers/winc1500/host_drv/driver/source/m2m_ota.c \
Drivers/winc1500/host_drv/driver/source/m2m_periph.c \
Drivers/winc1500/host_drv/driver/source/m2m_ssl.c \
Drivers/winc1500/host_drv/driver/source/m2m_wifi.c \
Drivers/winc1500/host_drv/driver/source/nmasic.c \
Drivers/winc1500/host_drv/driver/source/nmbus.c \
Drivers/winc1500/host_drv/driver/source/nmdrv.c \
Drivers/winc1500/host_drv/driver/source/nmi2c.c \
Drivers/winc1500/host_drv/driver/source/nmspi.c \
Drivers/winc1500/host_drv/driver/source/nmuart.c \
Drivers/winc1500/host_drv/socket/source/socket.c \
Drivers/winc1500/host_drv/spi_flash/source/flexible_flash.c \
Drivers/winc1500/host_drv/spi_flash/source/spi_flash.c \
Src/callbacks.c \
Src/winc_callbacks.c \
Src/winc.c \
Src/power.c \
Src/syscalls.c
# ASM sources
ASM_SOURCES =  \
startup_stm32l433xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32L433xx


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32L4xx_HAL_Driver/Inc \
-IDrivers/STM32L4xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32L4xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/winc1500/host_drv


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L433CCTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

########################
# openocd
########################
flash: all
	openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
