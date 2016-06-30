/*
 * adc_helper.c
 *
 *  Created on: 25.03.2016
 *      Author: harry
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "../PiXTend.h"
#include "adc.h"

#define GET_ADC_CMD 	0b00000011
#define	SET_AICTRL_CMD	0b10000111


/*
 * SPI-helper
 */

//-----------------------------------------------------------------------------
//	Function: get ADC
//	0b10000000	0x80
//-----------------------------------------------------------------------------
uint8_t
int_get_adc (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;
  val = dta;
  if (count == 0)
    val = 4;
  if (count == 1)
    val = RPi.tx->adc[(cmd - GET_ADC_CMD)] & 0xff;
  if (count == 2)
    val = RPi.tx->adc[(cmd - GET_ADC_CMD)] >> 8;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: set adc Ctrl
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_ai_ctrl (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;

  val = dta;
  if (count == 0)
    val = 4;
  if (count == 1)
    RealOutput.stat.ai.samples = dta;
  if (count == 2)
    RealOutput.stat.ai.freq = dta;
  return (val);
}



