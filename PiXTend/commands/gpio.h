/*
 * gpio.h
 *
 *  Created on: 26.03.2016
 *      Author: harry
 */

#ifndef PIXTEND_COMMANDS_GPIO_H_
#define PIXTEND_COMMANDS_GPIO_H_

/******************************************************************************
 * SPI interrupt helper
 *****************************************************************************/

uint8_t int_set_dio (uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_get_dio (uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_relays (uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_get_gpio (uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_gpio (uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_gpio_ctrl (uint8_t count, uint8_t cmd, uint8_t dta);


#endif /* PIXTEND_COMMANDS_GPIO_H_ */
