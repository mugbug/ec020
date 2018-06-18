################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/common.c \
../src/cr_startup_lpc17.c \
../src/main.c 

OBJS += \
./src/common.o \
./src/cr_startup_lpc17.o \
./src/main.o 

C_DEPS += \
./src/common.d \
./src/cr_startup_lpc17.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/FreeRTOS_Library" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/Lib_EaBaseBoard/inc" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/Lib_MCU/inc" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/FreeRTOS_Library/demo_code" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/CMSISv2p00_LPC17xx/inc" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/FreeRTOS_Library/include" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/FreeRTOS_Library/portable" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/Example01/src/Services" -I"/home/pedro/Documents/dev/LPCxpresso/EC020/firmware_src/Example01/src" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


