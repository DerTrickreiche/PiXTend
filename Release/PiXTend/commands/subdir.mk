################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PiXTend/commands/adc.c \
../PiXTend/commands/gpio.c \
../PiXTend/commands/pwm.c 

OBJS += \
./PiXTend/commands/adc.o \
./PiXTend/commands/gpio.o \
./PiXTend/commands/pwm.o 

C_DEPS += \
./PiXTend/commands/adc.d \
./PiXTend/commands/gpio.d \
./PiXTend/commands/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
PiXTend/commands/%.o: ../PiXTend/commands/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


