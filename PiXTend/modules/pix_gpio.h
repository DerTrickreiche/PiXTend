/*
 * dio.h
 *
 *  Created on: 13.03.2016
 *      Author: harry
 */

#ifndef IO_DIO_H_
#define IO_DIO_H_

#include "../commands/gpio.h"

extern void *gpio_Mod_Info[];
/*
 * SPI - helper
 */
uint8_t int_set_dio(uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_get_dio(uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_relays(uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_gpio(uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_get_gpio(uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_gpio_ctrl(uint8_t count,  uint8_t cmd, uint8_t dta);

#endif /* IO_DIO_H_ */
