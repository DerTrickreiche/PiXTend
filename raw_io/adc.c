/*
 * adc.c
 *
 *  Created on: 01.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "SysTimer.h"

// #include "../pxdta.h"
#include "adc.h"

#define	NUMBER_OF_ADC		4

#define MAX_SAMPLES		64		// has to be a power of 2!


static volatile uint16_t adc_sample_buf[NUMBER_OF_ADC][MAX_SAMPLES];
static volatile uint8_t adc_lptr[NUMBER_OF_ADC];
static volatile uint8_t adc_hptr[NUMBER_OF_ADC];
static volatile uint16_t adc_sum[NUMBER_OF_ADC];
static volatile uint8_t adc_div[NUMBER_OF_ADC];
static uint8_t adc_freq[NUMBER_OF_ADC];

static uint16_t *ADCTick;
#define ADCTICK (*ADCTick)

void adc_init (uint8_t mask, uint8_t bufsize)
{
  uint8_t i;

  for (i = 0; i < NUMBER_OF_ADC; i++)
    {
      adc_set_samples (i, 4);		// 2^4 samples
      adc_freq[i] = 10;			// defaults to 5ms
    }

  ADCTick = request_SysTimer();
  //ADC: Use external reference
  //ADC: Start with ADC4 (ANALOG_IN 0)
  ADMUX = ADC_MUX;

  ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) // prescaler 128: ch channel is updated every 0.4ms (2.5kHz)
						      // f_adc = 125kHz --> convert every 13 cycles --> 9,615kHz --> ea
//      | (1 << ADEN)		// ADC: Enable
      | (1 << ADIE);		// Interrupt enable

  ADCTICK = 0;
}

void
adc_worker (void)
{
  register uint8_t Channel;
  static uint16_t lasttick;

  if (ADCTICK != lasttick)
    {
      if (ADCTICK == (100 + NUMBER_OF_ADC))
	ADCTICK = 0;
      lasttick = ADCTICK;

      Channel = ADCTICK % 4;
      if (((ADCTICK - Channel) % adc_freq[Channel]) == 0)
	{
	  ADCSRA &= ~(1 << ADEN);	// disable Conversion
	  ADMUX = ADC_MUX + Channel;	// advance to next channel

	  // Acknowledge the ADC Interrupt Flag and enabble conversion again
	  ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIF);
	}
    }
}

void adc_set_rate(uint8_t channel, uint8_t rate)
{
  adc_freq[channel] = rate;
}

void adc_set_samples (uint8_t channel, uint8_t samples)
{
  uint8_t sreg;
  uint8_t i;

  sreg = SREG;
  cli();
  adc_div[channel] = samples;
  for (i = 0; i < MAX_SAMPLES; i++)
    adc_sample_buf[channel][i] = 0;
  adc_lptr[channel] = 0;
  adc_hptr[channel] = ADC_SAMPLES(channel) & (MAX_SAMPLES - 1);
  adc_sum[channel] = 0;
  SREG = sreg;
}

uint16_t
get_adc_value (uint8_t channel)
{
  uint16_t retval;
  uint8_t sreg;

  sreg = SREG;
  cli();
  retval = adc_sum[channel] >> adc_div[channel];
  SREG = sreg;
  return (retval);
}

ISR(ADC_vect)
{
  uint8_t i;

  i = ADMUX & (NUMBER_OF_ADC - 1);

  adc_sum[i] -= adc_sample_buf[i][adc_lptr[i]];
  adc_sample_buf[i][adc_hptr[i]] = ADC;
  adc_sum[i] += adc_sample_buf[i][adc_hptr[i]]; // Read the ADC Result

  adc_lptr[i]++;
  adc_lptr[i] &= (MAX_SAMPLES - 1);

  adc_hptr[i]++;
  adc_hptr[i] &= (MAX_SAMPLES - 1);

}
