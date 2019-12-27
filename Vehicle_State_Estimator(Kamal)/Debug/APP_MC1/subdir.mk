################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP_MC1/AVR_MC1.c \
../APP_MC1/PBConfg.c \
../APP_MC1/main.c 

OBJS += \
./APP_MC1/AVR_MC1.o \
./APP_MC1/PBConfg.o \
./APP_MC1/main.o 

C_DEPS += \
./APP_MC1/AVR_MC1.d \
./APP_MC1/PBConfg.d \
./APP_MC1/main.d 


# Each subdirectory must supply rules for building sources it contributes
APP_MC1/%.o: ../APP_MC1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\H\eclipse-workspace\Vehicle_State_Estimator" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


