/*
 * pwm.c
 *
 *  Created on: 26.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "../PiXTend.h"
#include "pwm.h"


#define SET_SERVO_CMD	0b10000000
#define SET_PWM_CMD	0b10000010
#define SET_PWMCTRL_CMD	0b10000100


/*
 * SPI-helper for manual mode
 */
//-----------------------------------------------------------------------------
//	Function: set servo
//	0b10000000	0x80
//-----------------------------------------------------------------------------
uint8_t
int_set_servo (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;

  val = dta;
  if (count == 0)
    val = 3;
  if (count == 1)
    {
      RealOutput.pwm[cmd - SET_SERVO_CMD] = dta;
    }
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: set pwm
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_pwm (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;
  static uint16_t l;

  val = dta;
  val = dta;
  if (count == 0)
    val = 4;
  if (count == 1)
    l = (uint16_t)dta;
  if (count == 2)
    RealOutput.pwm[cmd - SET_PWM_CMD] = ((uint16_t)dta << 8) + l;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: set pwm ctrl
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_pwm_ctrl (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;
  static uint16_t l;

  val = dta;
  if (count == 0)
    val = 5;
  if (count == 1)
    RealOutput.stat.pwm.control = dta;
  if (count == 2)
    l = (uint16_t) dta;
  if (count == 3)
    RealOutput.stat.pwm.timer = ((uint16_t)dta <<8) + l;
  return (val);
}

