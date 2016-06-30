/*
 * pwm.h
 *
 *  Created on: 26.03.2016
 *      Author: harry
 */

#ifndef PIXTEND_COMMANDS_PWM_H_
#define PIXTEND_COMMANDS_PWM_H_

/*
 * SPI-helper for manual mode
 */
uint8_t int_set_servo (uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_pwm (uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_pwm_ctrl (uint8_t count, uint8_t cmd, uint8_t dta);

#endif /* PIXTEND_COMMANDS_PWM_H_ */
