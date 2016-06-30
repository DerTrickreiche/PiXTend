/*
 * timer.c
 *
 *  Created on: 21.03.2016
 *      Author: harry
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "SysTimer.h"


#define SECTICK		ttick[0]

uint32_t	SysTime;
static volatile uint16_t ttick[TIMER_MAX_REQ];
static volatile uint8_t first_free_timer;

/*****************************************************************************
 *
 * 1ms Timer-interrupt setup
 *
 *****************************************************************************/
void
init_SysTimer (void)
{
  uint8_t i;

  SysTime = 0;
  first_free_timer = 1;	// timer[0] is needed for systime
  TCCR2 = (1 << WGM21) | (1 << CS22);
  OCR2 = 249;	// 1ms
  TIMSK |= (1 << OCIE2);
  TIFR |= (1 << TOV2);
  for (i = 0; i < first_free_timer; i++)
    ttick[i] = 0;
  TCCR0 = (1 << CS01) | (1 << CS00);// Timer 0 freerunning for cycle-mesuaring
  sei();
}

/*****************************************************************************
 *
 * request next free systimer (1ms)
 *
 *****************************************************************************/
uint16_t *request_SysTimer(void)
{
  return(&ttick[first_free_timer++]);
}

/*****************************************************************************
 *
 * interrupt handler for timer 2
 *
 * called every millisecond
 *
 *****************************************************************************/

ISR( TIMER2_COMP_vect )                            // every 1ms
{
  register uint8_t i;

  for (i = 0; i < first_free_timer; i++)
    ttick[i]++;
  TIMSK |= (1 << OCIE2);
  if (SECTICK == 100)	// every 100ms
    {
      SECTICK = 0;
      SysTime++;
    }
}

