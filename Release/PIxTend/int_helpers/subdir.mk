################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PIxTend/int_helpers/adc.c 

OBJS += \
./PIxTend/int_helpers/adc.o 

C_DEPS += \
./PIxTend/int_helpers/adc.d 


# Each subdirectory must supply rules for building sources it contributes
PIxTend/int_helpers/%.o: ../PIxTend/int_helpers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/harry/Labor/PiXTend/PiXtend/PiX_rpi_lib" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1600000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


