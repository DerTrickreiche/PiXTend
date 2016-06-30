/*
 * pwm.c
 *
 *  Created on: 27.03.2016
 *      Author: Harry L.
 */

#include <avr/io.h>

static uint16_t res;
static uint16_t max_pwm;

void
init_pwm (void)
{
  DDRD |= (1 << PD4) | (1 << PD5);
  //Timer 1 (16bit) for Servo/PWM-Functions
  //Fast-PWM-Mode, TOP is ICR1-Register
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11) | (0 << WGM10);

  TCCR1A |= ((1 << COM1A1) | (1 << COM1B1));

}

void
setup_pwm (uint16_t range, uint16_t frequency)
{
  uint32_t t;

  res = range-1;
  t = F_CPU / frequency;			// Prescaler 1
  if (t < 0x10000)
    {
      TCCR1B = (TCCR1B & 0xf8) | (1 << CS10);
      max_pwm = t;
      ICR1 = t;
    }
  else
    {
      t /= 8;			// Prescaler 8
      if (t < 0x10000)
	{
	  TCCR1B = (TCCR1B & 0xf8) | (1 << CS11);
	  max_pwm = t;
	  ICR1 = t;
	}
      else
	{
	  t /= 8;		// Prescaler 64
	  if (t < 0x10000)
	    {
	      TCCR1B = (TCCR1B & 0xf8) | ((1 << CS10) | (1 << CS11));
	      max_pwm = t;
	      ICR1 = t;
	    }
	  else
	    {
	      t /= 4;		// Prescaler 256
	      if (t < 0x10000)
		{
		  TCCR1B = (TCCR1B & 0xf8) | 1 << CS12;
		  max_pwm = t;
		  ICR1 = t;

		}
	      else
		{
		  t /= 4;	// Prescaler 1024
		  if (t < 0x10000)
		    {
		      TCCR1B = (TCCR1B & 0xf8) | ((1 << CS12) | (1 << CS10));
		      max_pwm = t;
		      ICR1 = t;
		    }
		  else
		    {
		      // out of range - all off!
		      PORTD &= ~(1 << PD4);
		      TCCR1A &= ~(1 << COM1A1);
		      PORTD &= ~(1 << PD5);
		      TCCR1A &= ~(1 << COM1B1);
		    }
		}
	    }
	}
    }
}

void
set_pwm (uint8_t channel, uint16_t val)
{
  uint32_t p;

  if ((val == 0) || (val == res))
    {
      if (channel == 0)
	{
	  if (val == 0)
	    {
	      PORTD &= ~(1 << PD5);		// set permanent low
	      TCCR1A &= ~(1 << COM1A1);
	    }
	  else
	    {
	      PORTD |= (1 << PD5);		// set permanent high
	      TCCR1A &= ~(1 << COM1A1);
	    }
	}
      else
	{
	  if (val == 0)
	    {
	      PORTD &= ~(1 << PD4);		// set permanent low
	      TCCR1A &= ~(1 << COM1B1);
	    }
	  else
	    {
	      PORTD |= (1 << PD4);		// set permanent high
	      TCCR1A &= ~(1 << COM1B1);
	    }

	}

    }
  else
    {
      p = ((((uint32_t) max_pwm << 16) / res) * val) >> 16;
      if (channel == 0)
	{
	  OCR1A = (uint16_t) p;
	  TCCR1A |= (1 << COM1A1);
	}
      else
	{
	  OCR1B = (uint16_t) p;
	  TCCR1A |= (1 << COM1B1);
	}
    }
}


/*
 *  Servo stuff
 */
void
setup_servo_pwm (void)
{
  // --> set prescaler to 64, overflow after 20ms and 1ms "ON"-time
  TCCR1B = (TCCR1B & 0b11111000) | 0b00000011;
  ICR1 = 5000;
  OCR1A = 249;
  OCR1B = 249;
}

void
set_servo_pwm (uint8_t channel, uint8_t pos)
{
  if (channel)
    OCR1B = 249L + pos;
  else
    OCR1A = 249L + pos;
}

