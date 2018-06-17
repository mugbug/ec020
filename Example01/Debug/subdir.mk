################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cr_startup_lpc17.c \
../custom_lib.c \
../main.c 

OBJS += \
./cr_startup_lpc17.o \
./custom_lib.o \
./main.o 

C_DEPS += \
./cr_startup_lpc17.d \
./custom_lib.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/demo_code" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/CMSISv2p00_LPC17xx/inc" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/include" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/portable" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_EaBaseBoard/inc" -I"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_MCU/inc" -O1 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


