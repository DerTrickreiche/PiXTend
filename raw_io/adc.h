/*
 * adc.h
 *
 *  Created on: 01.03.2016
 *      Author: harry
 */

#ifndef ADC_H_
#define ADC_H_


#define ADC_MUX			(NUMBER_OF_ADC + 0x40)

#define ADC_SAMPLES(a)	(1 << adc_div[a])

void adc_init (uint8_t mask, uint8_t bufsize);
void adc_worker(void);

/*
 * ioctrl-functions
 */
void adc_set_samples(uint8_t channel, uint8_t samples);
void adc_set_rate(uint8_t channel, uint8_t rate);

/*
 * get real data
 */
uint16_t get_adc_value(uint8_t channel);

#endif /* ADC_H_ */
