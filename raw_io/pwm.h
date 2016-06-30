/*
 * pwm.h
 *
 *  Created on: 27.03.2016
 *      Author: harry
 */

#ifndef RAW_IO_PWM_H_
#define RAW_IO_PWM_H_

void init_pwm(void);
void setup_servo_pwm(void);
void setup_pwm (uint16_t range, uint16_t frequency);
void set_servo_pwm(uint8_t channel, uint8_t pos);
void set_pwm (uint8_t channel, uint16_t val);

#endif /* RAW_IO_PWM_H_ */
