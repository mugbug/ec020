################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib_MCU/src/lpc17xx_adc.c \
../Lib_MCU/src/lpc17xx_can.c \
../Lib_MCU/src/lpc17xx_clkpwr.c \
../Lib_MCU/src/lpc17xx_dac.c \
../Lib_MCU/src/lpc17xx_emac.c \
../Lib_MCU/src/lpc17xx_gpio.c \
../Lib_MCU/src/lpc17xx_i2c.c \
../Lib_MCU/src/lpc17xx_i2s.c \
../Lib_MCU/src/lpc17xx_libcfg_default.c \
../Lib_MCU/src/lpc17xx_mcpwm.c \
../Lib_MCU/src/lpc17xx_nvic.c \
../Lib_MCU/src/lpc17xx_pinsel.c \
../Lib_MCU/src/lpc17xx_pwm.c \
../Lib_MCU/src/lpc17xx_qei.c \
../Lib_MCU/src/lpc17xx_rit.c \
../Lib_MCU/src/lpc17xx_rtc.c \
../Lib_MCU/src/lpc17xx_spi.c \
../Lib_MCU/src/lpc17xx_ssp.c \
../Lib_MCU/src/lpc17xx_timer.c \
../Lib_MCU/src/lpc17xx_uart.c \
../Lib_MCU/src/lpc17xx_wdt.c 

OBJS += \
./Lib_MCU/src/lpc17xx_adc.o \
./Lib_MCU/src/lpc17xx_can.o \
./Lib_MCU/src/lpc17xx_clkpwr.o \
./Lib_MCU/src/lpc17xx_dac.o \
./Lib_MCU/src/lpc17xx_emac.o \
./Lib_MCU/src/lpc17xx_gpio.o \
./Lib_MCU/src/lpc17xx_i2c.o \
./Lib_MCU/src/lpc17xx_i2s.o \
./Lib_MCU/src/lpc17xx_libcfg_default.o \
./Lib_MCU/src/lpc17xx_mcpwm.o \
./Lib_MCU/src/lpc17xx_nvic.o \
./Lib_MCU/src/lpc17xx_pinsel.o \
./Lib_MCU/src/lpc17xx_pwm.o \
./Lib_MCU/src/lpc17xx_qei.o \
./Lib_MCU/src/lpc17xx_rit.o \
./Lib_MCU/src/lpc17xx_rtc.o \
./Lib_MCU/src/lpc17xx_spi.o \
./Lib_MCU/src/lpc17xx_ssp.o \
./Lib_MCU/src/lpc17xx_timer.o \
./Lib_MCU/src/lpc17xx_uart.o \
./Lib_MCU/src/lpc17xx_wdt.o 

C_DEPS += \
./Lib_MCU/src/lpc17xx_adc.d \
./Lib_MCU/src/lpc17xx_can.d \
./Lib_MCU/src/lpc17xx_clkpwr.d \
./Lib_MCU/src/lpc17xx_dac.d \
./Lib_MCU/src/lpc17xx_emac.d \
./Lib_MCU/src/lpc17xx_gpio.d \
./Lib_MCU/src/lpc17xx_i2c.d \
./Lib_MCU/src/lpc17xx_i2s.d \
./Lib_MCU/src/lpc17xx_libcfg_default.d \
./Lib_MCU/src/lpc17xx_mcpwm.d \
./Lib_MCU/src/lpc17xx_nvic.d \
./Lib_MCU/src/lpc17xx_pinsel.d \
./Lib_MCU/src/lpc17xx_pwm.d \
./Lib_MCU/src/lpc17xx_qei.d \
./Lib_MCU/src/lpc17xx_rit.d \
./Lib_MCU/src/lpc17xx_rtc.d \
./Lib_MCU/src/lpc17xx_spi.d \
./Lib_MCU/src/lpc17xx_ssp.d \
./Lib_MCU/src/lpc17xx_timer.d \
./Lib_MCU/src/lpc17xx_uart.d \
./Lib_MCU/src/lpc17xx_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
Lib_MCU/src/%.o: ../Lib_MCU/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/CMSISv2p00_LPC17xx/inc" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/include" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/portable" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/demo_code" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_EaBaseBoard/inc" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_MCU/inc" -O1 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


