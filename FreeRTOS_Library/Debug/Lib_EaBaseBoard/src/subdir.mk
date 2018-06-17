################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib_EaBaseBoard/src/acc.c \
../Lib_EaBaseBoard/src/eeprom.c \
../Lib_EaBaseBoard/src/flash.c \
../Lib_EaBaseBoard/src/font5x7.c \
../Lib_EaBaseBoard/src/joystick.c \
../Lib_EaBaseBoard/src/led7seg.c \
../Lib_EaBaseBoard/src/light.c \
../Lib_EaBaseBoard/src/oled.c \
../Lib_EaBaseBoard/src/pca9532.c \
../Lib_EaBaseBoard/src/rgb.c \
../Lib_EaBaseBoard/src/rotary.c \
../Lib_EaBaseBoard/src/temp.c \
../Lib_EaBaseBoard/src/uart2.c 

OBJS += \
./Lib_EaBaseBoard/src/acc.o \
./Lib_EaBaseBoard/src/eeprom.o \
./Lib_EaBaseBoard/src/flash.o \
./Lib_EaBaseBoard/src/font5x7.o \
./Lib_EaBaseBoard/src/joystick.o \
./Lib_EaBaseBoard/src/led7seg.o \
./Lib_EaBaseBoard/src/light.o \
./Lib_EaBaseBoard/src/oled.o \
./Lib_EaBaseBoard/src/pca9532.o \
./Lib_EaBaseBoard/src/rgb.o \
./Lib_EaBaseBoard/src/rotary.o \
./Lib_EaBaseBoard/src/temp.o \
./Lib_EaBaseBoard/src/uart2.o 

C_DEPS += \
./Lib_EaBaseBoard/src/acc.d \
./Lib_EaBaseBoard/src/eeprom.d \
./Lib_EaBaseBoard/src/flash.d \
./Lib_EaBaseBoard/src/font5x7.d \
./Lib_EaBaseBoard/src/joystick.d \
./Lib_EaBaseBoard/src/led7seg.d \
./Lib_EaBaseBoard/src/light.d \
./Lib_EaBaseBoard/src/oled.d \
./Lib_EaBaseBoard/src/pca9532.d \
./Lib_EaBaseBoard/src/rgb.d \
./Lib_EaBaseBoard/src/rotary.d \
./Lib_EaBaseBoard/src/temp.d \
./Lib_EaBaseBoard/src/uart2.d 


# Each subdirectory must supply rules for building sources it contributes
Lib_EaBaseBoard/src/%.o: ../Lib_EaBaseBoard/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/CMSISv2p00_LPC17xx/inc" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/include" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/portable" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/demo_code" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_EaBaseBoard/inc" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_MCU/inc" -O1 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


