################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Example01.axf

# Tool invocations
Example01.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/CMSISv2p00_LPC17xx/Debug" -L"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/FreeRTOS_Library/Debug" -L"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_EaBaseBoard/Debug" -L"/home/pedro/Documents/dev/LPCxpresso/lpcxpresso-board-project/Lib_MCU/Debug" -Xlinker --gc-sections -Xlinker -Map=Example01.map -mcpu=cortex-m3 -mthumb -T "example01_Debug.ld" -o "Example01.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Example01.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size Example01.axf; # arm-none-eabi-objcopy -O ihex Example01.axf Example01.hex ;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
