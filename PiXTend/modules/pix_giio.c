/*
 * dio.c
 *
 *  Created on: 13.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "../PiXTend.h"
#include "../../RPiCom.h"

static uint8_t ioctrl_gpio;

static void gpio_register (void);
static void gpio_init (void);
static void gpio_output (void);
static void gpio_input (void);
static void gpio_ioctrl (void);

void *gpio_Mod_Info[] =
  { gpio_register,
      gpio_init,
      gpio_output,
      gpio_input,
      gpio_ioctrl,
      NULL };

void
static gpio_register (void)
{
  REGISTER_GET_DIO;
  REGISTER_GET_GPIO;
  REGISTER_SET_DIO;
  REGISTER_SET_GPIO;
  REGISTER_SET_RELAYS;
  REGISTER_SET_GPIO_CTRL;
}

static void
gpio_init (void)
{
  //-----------------------------------
  //	Check "DO-PWM" jumper settings --
  //-----------------------------------
//  DDRD &= ~((1 << PD4) | (1 << PD5));
  DDRD |= (1 << PD6) | (1 << PD7);
  //PWM0/PWM1-Pins were set as inputs before
  PORTD = (1 << PD4) | (1 << PD5); //activate internal pull-ups for PWM0/PWM1 Pins

  //wait 10us before inputs are checked
  _delay_us (10);

  //PWM0
  if (PIND & (1 << PD5))
    {
      //jumper isn't set --> PWM0- (PD5) and (PD7) can be used as outputs
      PORTD &= ~(1 << PD5);
      DDRD |= (1 << PD5);
      //PD7 is already an output
      Features &= ~(1 << JP8STAT);
    }
  else
    {
      //jumper is set --> DO4-Pin can't be used as output and has to be configured as input. PWM0-Pin is now set as output
      DDRD &= ~(1 << PD7);	// PD7 as input
      DDRD |= (1 << PD5);
      PORTD &= ~(1 << PD7);
      Features |= (1 << JP8STAT);
    }

  //check PWM1 jumper
  if (PIND & (1 << PD4))
    {
      //jumper isn't set --> PWM1- (PD4) and DO5-Pin (PD6) can be used as outputs
      PORTD &= ~(1 << PD4);	// PullUp off
      DDRD |= (1 << PD4);	// PWM out
      //PD6 is already an output
      Features &= ~(1 << JP9STAT);
    }
  else
    {
      //jumper is set --> DO5-Pin can't be used as output and has to be configured as input. PWM1-Pin is now set as output
      DDRD &= ~(1 << PD6);
      DDRD |= (1 << PD4);
      PORTD &= ~(1 << PD6);
      Features |= (1 << JP9STAT);
    }

  //Define Input and Output Pins

  if (SLAVE_PRESENT)
    {
      DDRA &= 0b11110000;	// GPIOs Pin 4-7: Input
    }
  else
    DDRA = 0b00000000;	//GPIOs Pin 0-3: Inputs, AIs Pin 4-7: Input

  DDRB = 0b01000000;//DIs Pin 0-3: Input, SPI Pin 4-7: Inputs/Configurated by SPI
  DDRC = 0b11111111;		//DOs Pin 0-3: Output, Relais Pin 4-7: Output
//  DDRD = (1 << PD6) | (1 << PD7);	//DIs Pin 0-3: Input, PWM Pins 4-5: Inputs, DOs Pin 6-7: Output

  //Set all Outputs Low, deactivate all input pull-ups

  PORTA = PORTA & 0xF0;
  PORTC = 0x00;
  PORTD = 0x00;

}

static void
gpio_output (void)
{
  //------------------------------------
  //-- GET values from digital inputs --
  //------------------------------------

  RPi.tx->din = (PIND & 0b00001111) | (PINB << 4);

  //---------------------------------
  //-- GET values from gpio inputs --
  //---------------------------------

  if (SLAVE_PRESENT)
    RPi.tx->gpio = (PINA & 0b00001111);

}

static void
gpio_input (void)
{
  uint8_t i;
  //------------------------------------
  //-- 	SET GPIO control register 	--
  //------------------------------------

  if (!SLAVE_PRESENT)
    {
      if (RealOutput.stat.gpio != ioctrl_gpio)
	{
	  i = (RealOutput.stat.gpio & 0xf0) >> 4;
	  DDRA |= i & RealOutput.stat.gpio & 0x0f;
	  ioctrl_gpio = RealOutput.stat.gpio;
	}
    }
  PORTC = (RealOutput.dout & 0xf) | (RealOutput.relays << 4);
  if (!(Features & 0x02))	// seperate outputs on D4
      PORTD = (PORTD & 0x7f) | ((RealOutput.dout & 0x10) << 3);
  if (!(Features & 0x04))	// seperate outputs on D5
      PORTD = (PORTD & 0xbf) | ((RealOutput.dout & 0x10) << 1);

}

static void
gpio_ioctrl (void)
{
  uint8_t i;
  //------------------------------------
  //-- 	SET GPIO control register 	--
  //------------------------------------

  if (!SLAVE_PRESENT)
    {
      if (RealOutput.stat.gpio != ioctrl_gpio)
	{
	  i = (RealOutput.stat.gpio & 0xf0) >> 4;
	  DDRA |= i & RealOutput.stat.gpio & 0x0f;
	  ioctrl_gpio = RealOutput.stat.gpio;
	}
    }

}
