/*
 * adc.h
 *
 *  Created on: 25.03.2016
 *      Author: harry
 */

#ifndef INT_HELPERS_ADC_H_
#define INT_HELPERS_ADC_H_

/*
 * SPI-helper for manual mode
 */
uint8_t int_get_adc(uint8_t count, uint8_t cmd, uint8_t dta);
uint8_t int_set_ai_ctrl(uint8_t count,  uint8_t cmd, uint8_t dta);



#endif /* INT_HELPERS_ADC_H_ */
