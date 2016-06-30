################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../raw_io/SysTimer.c \
../raw_io/Xcom_master.c \
../raw_io/adc.c \
../raw_io/pwm.c \
../raw_io/uart.c 

OBJS += \
./raw_io/SysTimer.o \
./raw_io/Xcom_master.o \
./raw_io/adc.o \
./raw_io/pwm.o \
./raw_io/uart.o 

C_DEPS += \
./raw_io/SysTimer.d \
./raw_io/Xcom_master.d \
./raw_io/adc.d \
./raw_io/pwm.d \
./raw_io/uart.d 


# Each subdirectory must supply rules for building sources it contributes
raw_io/%.o: ../raw_io/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


