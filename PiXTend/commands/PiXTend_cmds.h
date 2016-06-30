/*
 * PiXTend_cmds.h
 *
 *  Created on: 25.03.2016
 *      Author: harry
 */


#ifndef PIXTEND_CMDS_H_
#define PIXTEND_CMDS_H_

/*
 * General IO
 */
// 0x01 CMD = 0b00000001 	set Digital Ouput
#define      REGISTER_SET_DIO		RPi_RegisterFunction(0x01, 0x01, int_set_dio)
// 0x02 CMD = 0b00000010	get Digital Input
#define      REGISTER_GET_DIO		RPi_RegisterFunction(0x02, 0x02, int_get_dio)
#define      REGISTER_SET_RELAYS	RPi_RegisterFunction(0x07, 0x07, int_set_relays) 	// 0x07 CMD = 0b00000111 set Relays
#define      REGISTER_SET_GPIO		RPi_RegisterFunction(0x08, 0x08, int_set_gpio)	// 0x08 CMD = 0b00001000	set GPIO
#define      REGISTER_GET_GPIO		RPi_RegisterFunction(0x09, 0x09, int_get_gpio)	// 0x09 CMD = 0b00001001	get GPIO
#define      REGISTER_SET_GPIO_CTRL	RPi_RegisterFunction(0x85, 0x85, int_set_gpio_ctrl)	// 0x85 CMD = 0b10000101 set GPIO-Control Register

/*
 * ADC
 *
 */
#define      REGISTER_GET_ADC		RPi_RegisterFunction(0x03, 0x06, int_get_adc)	// 0x03 CMD = 0b00000011	get ADC Channel [0..3]
#define      REGISTER_SET_AI_IOCTRL	RPi_RegisterFunction(0x87, 0x87, int_set_ai_ctrl)	// 0x87 CMD = 0b10000111 set ADC-Control Register

/*
 * DHT11
 */
#define      REGISTER_GET_DHT11_TEMP	RPi_RegisterFunction(0x0a, 0x0d, int_get_dht11_temp)	// 0x0a CMD = 0b00001010 get DHT112 [0..3] Temperature
#define      REGISTER_GET_DHT11_HUM	RPi_RegisterFunction(0x0e, 0x11, int_get_dht11_hum)	// 0x0d CMD = 0b00001101 get DHT112 [0..3] Humidity

/*
 * PWM
 */
#define      REGISTER_SET_SERVO		RPi_RegisterFunction(0x80, 0x81, int_set_servo)	// 0x80 CMD = 0b10000000 set Servo [0..1]
#define      REGISTER_SET_PWM		RPi_RegisterFunction(0x82, 0x83, int_set_pwm)	// 0x82 CMD = 0b10000010 set PWM [0..1]
#define      REGISTER_SET_PWM_CTRL	RPi_RegisterFunction(0x84, 0x84, int_set_pwm_ctrl)	// 0x84 CMD = 0b10000100 set PWM-Control Register

/*
 * SPS
 */
#define      REGISTER_SET_UC_CTRL	RPi_RegisterFunction(0x86, 0x86, int_set_uc_ctrl)	// 0x86 CMD = 0b10000110 set µC-Control Register
#define      REGISTER_SET_RPI_STATUS	RPi_RegisterFunction(0x88, 0x88, int_set_RPi_stat)	// 0x88 CMD = 0b10001000 set RPi-Status-Register
#define      REGISTER_GET_UC_VERSION	RPi_RegisterFunction(0x89, 0x89, int_get_uc_version)	// 0x89 CMD = 0b10001001 get µC & HW-Version
#define      REGISTER_GET_UC_STATUS	RPi_RegisterFunction(0x8a, 0x8a, int_get_uc_status)	// 0x8a CMD = 0b10001010 get uc Status

#endif /* PIXTEND_CMDS_H_ */
