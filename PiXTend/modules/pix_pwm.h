/*
 * pix_pwm.h
 *
 *  Created on: 26.03.2016
 *      Author: harry
 */

#ifndef PIXTEND_MODULES_PIX_PWM_H_
#define PIXTEND_MODULES_PIX_PWM_H_

#include "../commands/pwm.h"

extern void *pwm_Mod_Info[];

/*
 * SPI - helper
 */
uint8_t int_set_pwm(uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_pwm_ctrl(uint8_t count, uint8_t cmd, uint8_t dta);



#endif /* PIXTEND_MODULES_PIX_PWM_H_ */
