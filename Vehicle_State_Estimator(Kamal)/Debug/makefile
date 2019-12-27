################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/UART/subdir.mk
-include MCAL/SPI/subdir.mk
-include MCAL/DIO/subdir.mk
-include APP_MC1/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Vehicle_State_Estimator.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: Vehicle_State_Estimator.elf secondary-outputs

# Tool invocations
Vehicle_State_Estimator.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Vehicle_State_Estimator.map -mmcu=atmega32 -o "Vehicle_State_Estimator.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Vehicle_State_Estimator.lss: Vehicle_State_Estimator.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Vehicle_State_Estimator.elf  >"Vehicle_State_Estimator.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Vehicle_State_Estimator.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 Vehicle_State_Estimator.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Vehicle_State_Estimator.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
