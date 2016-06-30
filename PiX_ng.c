/*
 * main.c
 *
 *  Created on: 04.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include <stdint.h>
#include <string.h>

#include "raw_io/SysTimer.h"
#include "RPiCom.h"
#include "PiXTend/PiXTend.h"
#include "PiX_ng.h"

RPi_Buffer_t RPi;

static uint16_t *DebugTick, *ProcTick;

#define DEBUGTICK	(*DebugTick)

#ifdef ENABLE_XCOM
#include "raw_io/Xcom_master.h"

static uint16_t *SlaveTick;
#define SL_TICK (*SlaveTick)
#endif

/*****************************************************************************
 *
 * main loop
 *
 *****************************************************************************/

int
main (void)
{

  /*****************************************************************************
   *
   * initialize and register all modules
   *
   *****************************************************************************/

  init_modules();

#ifdef ENABLE_DIO
  register_module((uint16_t**)gpio_Mod_Info);
#endif

#ifdef ENABLE_ADC
  register_module((uint16_t**)ADC_Mod_Info);
#endif

#ifdef ENABLE_PWM
  register_module((uint16_t**)pwm_Mod_Info);
#endif


  /*****************************************************************************
   *
   * initilaize 1ms timer-interrupt and enable interrupts globaly
   *
   *****************************************************************************/

  init_SysTimer ();
  DebugTick = request_SysTimer ();
  ProcTick = request_SysTimer ();

  /*****************************************************************************
   *
   * initilaize spi-communication and data-structures
   *
   *****************************************************************************/

  RPi_init (MAX(sizeof(OMap_t), sizeof(IMap_t)), 20, &RPi);

  /*****************************************************************************
   *
   * register SPI-commands from all modules
   *
   *****************************************************************************/

  register_mod_spi_commands();

  sps_init ();

#ifdef ENABLE_XCOM
  /*****************************************************************************
   *
   * check if slave present and enable it
   *
   *****************************************************************************/

  if (Xcom_init ())
    {
      Features |= SP_BIT;
      SlaveTick = request_SysTimer();
    }
#endif

  RPi_start ();	// Finaly start SPI-communication

  DDRD |= ((1 << PD4) | (1 << PD5));

  DEBUGTICK = 0;

  /*****************************************************************************
   *
   * main loop starts her
   *
   *****************************************************************************/

  while (1)
    {

      /*****************************************************************************
       *
       * call the components workers
       *
       *****************************************************************************/

      RPi_worker ();
      sps_worker ();

/*	  switch (RPi.Status)
	    {
	    case RPI_STAT_IDLE:
	      PORTD |= ((1 << PD4) | (1 << PD5)); // 1 1;
	      break;
	    case RPI_STAT_RUN:
	      PORTD = PORTB & ~((1 << PD4) | (1 << PD5));	// 0 0
	      break;
	    case RPI_STAT_AUTO:
	      PORTD &= ~(1 << PD4);
	      PORTD |= (1 << PD5);	// 0 1
	      break;
	    case RPI_STAT_CRC16_ERROR:
	      PORTD &= ~(1 << PD5);
	      PORTD |= (1 << PD4);	// 1 0
	      break;
	    case RPI_STAT_MAN:
	    case RPI_STAT_MAN_UNKNOWN:
	      break;
	    }
	    */

      /*****************************************************************************
       *
       * some debugging helper
       *
       *****************************************************************************/

/*      if (DEBUGTICK == 2)
	{
	      DEBUGTICK = 0;
	}
*/

#ifdef ENABLE_XCOMM
      if (SLAVE_PRESENT)
	{
	  Xcom_worker ();
	  if (SL_TICK == 3)
	    {
/*	      if (make_crc16 (sizeof(SMap_t), (uint8_t *) &XcomRxBuffer.buf)
		  == XcomRxBuffer.rio.crc16)
		{
		  memcpy((void *)&SlaveMap,&XcomRxBuffer.rio.io,sizeof(SMap_t));
		}
*/
	      Xcom_ack ();
	      Xcom_cmd (XCMD_INPUT, 15, &RealOutput);
//	      Xcom_cmd (XCMD_INPUT, sizeof(IMap_t), &InputMap);
	      SL_TICK++;
	    }
	  if (SL_TICK == 16)
	    {
	      Xcom_ack ();
	      Xcom_cmd (XCMD_OUTPUT, sizeof(OMap_t), RPi.tx);
	      SL_TICK++;
	    }
	  if (SL_TICK >= 25)
	    {
	      SL_TICK = 0;
	    }

	}
#endif

      /*****************************************************************************
       *
       * main loop ends here
       *
       *****************************************************************************/

    }

}
