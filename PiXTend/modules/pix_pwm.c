/*
 * pix_pwm.c
 *
 *  Created on: 26.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "../PiXTend.h"
#include "../../RPiCom.h"
#include "../../raw_io/pwm.h"
#include "pix_pwm.h"

static void
pwm_register (void);
static void
pwm_init (void);
static void
pwm_output (void);
static void
pwm_ioctrl (void);

void *pwm_Mod_Info[] =
  { pwm_register, pwm_init, pwm_output,
  NULL, pwm_ioctrl,
  NULL };

static uint8_t pwmFlag;
static uint16_t pwmFreq;
static uint16_t pwmVal[2];
static uint16_t res;

void
static
pwm_register (void)
{
  REGISTER_SET_PWM;
  REGISTER_SET_SERVO;
  REGISTER_SET_PWM_CTRL;
}

static void
pwm_init (void)
{
  init_pwm ();
  setup_pwm (1023, 500);
  pwmVal[0] = 0xfff;
  pwmVal[1] = 0xfff;
  res = 1024;
}

static void
pwm_output (void)
{
  //------------------------------
  //-- SET values for PWM/Servo --
  //------------------------------

  if ((pwmVal[0] != RealOutput.pwm[0]) || (pwmVal[1] != RealOutput.pwm[1]))
    {
      if (pwmFlag & 0b00000001)
	{
	  //PWM0 & PWM1: Servo-Mode
	  //Set values from pwm-bytes
	  if (pwmFlag & 0b00000010)
	    {
	      //overdrive for servo0 enabled
	      //		OCR1A = 250+pwm0_value_low+pwm0_value_high-128;
	    }
	  else
	    {
	      //overdrive for servo0 disabled
	      set_servo_pwm (0, RealOutput.pwm[0]);
	    }

	  //Need for Overdrive?
	  if (pwmFlag & 0b00000100)
	    {
	      //overdrive for servo1 enabled
	    }
	  else
	    {
	      //overdrive for servo1 disabled
	      set_servo_pwm (1, RealOutput.pwm[1]);
	    }
	}
      else
	{
	  //PWM0 & PWM1: pwm-Mode
	  set_pwm (0, RealOutput.pwm[0]);
	  set_pwm (1, RealOutput.pwm[1]);
	}
    }
  pwmVal[0] = RealOutput.pwm[0];
  pwmVal[1] = RealOutput.pwm[1];
}

static void
pwm_ioctrl (void)
{
  uint8_t i;

  //------------------------------------
  //-- 	SET PWM control registers 	--
  //------------------------------------

  i = pwmFlag ^ RealOutput.stat.pwm.control; // all different bits are 1
  if (i)
  // some settings has changed
    {
      if (i & 0b00000001)
      // pwm-mode was changed
	{
	  if (RealOutput.stat.pwm.control & 0b00000001)
	  // new mode is Servo-Mode
	  // set everything
	    {
	      setup_servo_pwm ();
	      // Servo-Mode
	      if (i & 0b00000010)
	      // OD0 changed
		{
		}
	      if (i & 0b00000100)
	      // OD1 changed
		{
		}
	    }
	  else	// pwm-Mode
	    {
	      //Mode has changed to pwm-mode
	      setup_pwm (res, RealOutput.stat.pwm.timer);
	      pwmFreq = RealOutput.stat.pwm.timer;
	      if (i & 0b11100000)
	      //Prescaler has changed
		{
		}
	    }
	  pwmFlag = RealOutput.stat.pwm.control;
	}
    }

  if (!(RealOutput.stat.pwm.control & 0b00000001))
  // PWM-Mode
    {
      if (pwmFreq != RealOutput.stat.pwm.timer)
      // Timer changed
	{
	  // 	  ICR1 = RealOutput.stat.pwm.timer;
	  pwmFreq = RealOutput.stat.pwm.timer;
	  setup_pwm (res, pwmFreq);
	  pwmVal[0] = 0;
	  pwmVal[1] = 0;
	}
    }

}
