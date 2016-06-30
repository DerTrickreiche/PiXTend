/*
 * sps.c
 *
 *  Created on: 13.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "raw_io/SysTimer.h"
#include "PiXTend/PiXTend.h"
#include "PiX_ng.h"
#include "RPiCom.h"
#include "sps.h"

#include "PiXTend/PiXTend.h"

uint8_t Reg_uc;


uint16_t *AutoTick, *ProcTick;
#define AUTOTICK	(*AutoTick)
#define PROCTICK	(*ProcTick)

//static pxRegister_t Reg;

IMap_t RealOutput;

uint8_t Features;

IO_Mod_t **ModMap;	// array of pointer to IO_Mod_t - structs
uint8_t	Mods;

void sps_aquire_dta(void);
void sps_process_input(void);
void sps_worker(void);
void sps_control(void);

void register_mod_spi_commands(void)
{
  uint8_t i;
  /*
   *  register interrupt-helper
   *  have a look at PiXTend_cmds.h
   */
  REGISTER_SET_UC_CTRL;
  REGISTER_GET_UC_VERSION;
  REGISTER_GET_UC_STATUS;
  REGISTER_SET_RPI_STATUS;
  for(i=0; i < Mods; i++)
    if(ModMap[i]->Register != NULL)
	ModMap[i]->Register();
}

void
init_modules (void)
{
  ModMap = calloc (sizeof(uint16_t), 1);
  Mods = 0;
}

void
register_module (uint16_t *mod_info[])
{
  ModMap[Mods++] = (void*)mod_info;
  ModMap = realloc(ModMap, Mods * sizeof(uint16_t));
}


void
sps_init ()
{
uint8_t i;
  AutoTick = request_SysTimer ();
  ProcTick = request_SysTimer ();
  RPi.tx->ucStatus |= Features;
  sps_process_input ();	// call twice to initialy populate both buffer
  sps_process_input ();

  for(i=0; i < Mods; i++)
    if(ModMap[i]->Init != NULL)
	ModMap[i]->Init();
}

void
sps_control (void)
{
  //------------------------------------------
//-- Always SET microcontroller control register --
//------------------------------------------

//only change Watchdog state if the "reg" has changed
  if (RPi.rx->stat.uc != Reg_uc)
    {
      if (RPi.rx->stat.uc & WD_BIT)
	{
	  Reg_uc |= WD_BIT;
	  //activate Watchdog with two-seconds-timeout
	  wdt_enable(WDTO_2S);
	}
      else
	{
	  //disable watchdog timer
	  Reg_uc &= ~WD_BIT;
	  wdt_disable();
	}
      if (RPi.rx->stat.uc & RUN_BIT)
	{
	  Reg_uc |= RUN_BIT;
	  RealOutput.stat.uc |= (1 << UCS_RUN);
	}
      else
	{
	  Reg_uc &= ~RUN_BIT;
	  RPi.tx->ucStatus &= ~(1 << UCS_RUN);

	}

      Reg_uc = RPi.rx->stat.uc;
    }

}

/*
 * copy state of I/O to process-table
 */
void
sps_aquire_dta ()
{
  uint8_t i;
  sps_control ();
  RPi.tx->ucVersion[0] = PIX_SW_VERSION;
  RPi.tx->ucVersion[1] = PIX_HW_VERSION;
  RPi.tx->ucStatus = (Reg_uc & RUN_BIT) >> UCC_RUN;
  RPi.tx->ucStatus |= Features;
  for(i=0; i < Mods; i++)
    if (ModMap[i]->Output != NULL)
      ModMap[i]->Output();
  RPi_update();

}

void
sps_process_input (void)
{
  uint8_t i;

  /*
   * first set all controlls
   * next set all outputs
   */

  for(i=0; i < Mods; i++)
    {
      if (ModMap[i]->IOControl != NULL)
        ModMap[i]->IOControl();
      if (ModMap[i]->Input != NULL)
        ModMap[i]->Input();
    }
}

void
sps_worker (void)
{
uint8_t i;
  /*****************************************************************************
   *
   * update input every 3ms
   * read all physical inputs to OutputMap
   * calculate valid crc16
   *
   *****************************************************************************/
  //
  //
  if (PROCTICK > 3)
    {
      if (RPi.Status == RPI_STAT_IDLE)
	{
	  sps_aquire_dta ();
	  PROCTICK = 0;
	}
    }

  /*****************************************************************************
   *
   * if running in auto-mode check for timeout.
   *
   *****************************************************************************/
  if (AUTOTICK == 2000)	// Reset Auto-mode after 2s without auto-frame
    {
      Reg_uc &= ~RUN_BIT;
      RPi.tx->ucStatus &= ~(1 << UCS_RUN);
      AUTOTICK++;	// dont't call this again!
    }

  /*****************************************************************************
   *
   * if SPI-Data available, process them!
   *
   *****************************************************************************/
  switch (RPi.Status)
    {
    case RPI_STAT_IDLE:
      break;

    case RPI_STAT_RUN:
      break;

    case RPI_STAT_AUTO:
      if (RUNNING)
	memcpy ((void *) &RealOutput, (void *) RPi.rx, sizeof(IMap_t));
      else
	RealOutput.stat.uc = RPi.rx->stat.uc;	// Run-Mode changed ?

      RPi_ack ();
      sps_process_input ();
      AUTOTICK = 0;
      break;

    case RPI_STAT_CRC16_ERROR:

      RPi_ack ();	// get new data
      RPi_error (ERR_CRC16);
      // Reset run-state if WD is set
      if (Reg_uc & WD_BIT)	// Reset by WDT;
	{
	  sps_reset ();
	}
      break;

    case RPI_STAT_MAN:
      RPi_ack ();
      sps_process_input ();
      break;

    case RPI_STAT_MAN_UNKNOWN:
      RPi_error (ERR_CMD_UNKNOWN);
      RPi_ack ();
      break;

    default:
      break;
    }


  for(i=0; i < Mods; i++)
    if (ModMap[i]->Worker != NULL)
      ModMap[i]->Worker();

}

/*****************************************************************************
 *
 * using watchdog to simulate a hard reset
 *
 *****************************************************************************/

void
sps_reset (void)
{
  wdt_enable(WDTO_15MS);
  while (1)
    ;
}

//-----------------------------------------------------------------------------
//	Function: set uc Ctrl
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_uc_ctrl (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;

  val = dta;
  if (count == 0)
    val = 3;
  if (count == 1)
    RealOutput.stat.uc = dta;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: set RPi status
//	0b10000000	0x80
//-----------------------------------------------------------------------------

uint8_t
int_set_RPi_stat (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val;

  val = dta;
  if (count == 0)
    val = 3;
  if (count == 1)
    RealOutput.stat.PiStatus = dta;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: read microcontroller version
//	0b10001001	0x89
//-----------------------------------------------------------------------------
uint8_t
int_get_uc_version (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val = 0;

  if (count == 0)
    val = 4;
  if (count == 1)
    val = PIX_SW_VERSION;
  if (count == 2)
    val = PIX_HW_VERSION;
  return (val);
}

//-----------------------------------------------------------------------------
//	Function: read microcontroller status register
//	0b10001010	0x8a
//-----------------------------------------------------------------------------
uint8_t
int_get_uc_status (uint8_t count, uint8_t cmd, uint8_t dta)
{
  uint8_t val = 0;

  val = dta;
  if (count == 0)
    val = 4;
  if (count == 1)
    val = RPi.tx->ucStatus;
  return val;
}

