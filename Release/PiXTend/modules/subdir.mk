################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PiXTend/modules/pix_adc.c \
../PiXTend/modules/pix_giio.c \
../PiXTend/modules/pix_pwm.c 

OBJS += \
./PiXTend/modules/pix_adc.o \
./PiXTend/modules/pix_giio.o \
./PiXTend/modules/pix_pwm.o 

C_DEPS += \
./PiXTend/modules/pix_adc.d \
./PiXTend/modules/pix_giio.d \
./PiXTend/modules/pix_pwm.d 


# Each subdirectory must supply rules for building sources it contributes
PiXTend/modules/%.o: ../PiXTend/modules/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


