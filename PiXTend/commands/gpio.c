/*
 * gpio.c
 *
 *  Created on: 26.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "../PiXTend.h"
#include "gpio.h"


/******************************************************************************
 * SPI interrupt helper
 *****************************************************************************/

//-----------------------------------------------------------------------------
//	Function: set digital out
//	0b10000000	0x80
//-----------------------------------------------------------------------------
uint8_t
int_set_dio (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;
  val = dta;
  if (count == 0)
    val = 3; // Payloadsize
  if (count == 1)
    RealOutput.dout = dta;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: get digital in
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_get_dio (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;

  val = dta;
  if (count == 0)
    val = 3; // Payloadsize
  if (count == 1)
    val = RPi.tx->din;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: set relays
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_relays (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;
  val = dta;
  if (count == 0)
    val = 3; // Payloadsize
  if (count == 1)
    RealOutput.relays = dta;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: get gpio
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_get_gpio (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;
  val = dta;
#ifndef XCOM
  if (count == 0)
  val = 3; // Payloadsize
  if (count == 1)
  val = RealOutput.gpio;
  //gpio;
#endif
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: set gpio
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_gpio (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;
  val = dta;

#ifndef XCOM
  if (count == 0)
  val = 3; // Payloadsize
  if (count == 1)
  RPi.tx->gpio = val;
#endif
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: gpio Ctrl
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_gpio_ctrl (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;

  val = dta;
#ifndef XCOM
  if (count == 0)
  val = 3;
  if (count == 1)
  RealOutput.stat.gpio = dta;
#endif
  return (val);
}

