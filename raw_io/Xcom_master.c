/*
 * Xcom.c
 *
 *	MASTER
 *	======
 *
 *  Created on: 10.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "Xcom_master.h"
#include "SysTimer.h"
#include "../PiX_ng.h"

//-----------------------------------------------------------------------------
//	Port-definitions for Xcom
//-----------------------------------------------------------------------------

#define XPORT		PORTA
#define XDDR		DDRA
#define XPIN		PINA

#define XPIN_CLK	PA0
#define XPIN_DTA	PA1
#define XPIN_REQ	PA2
#define XPIN_ACK	PA3

//-----------------------------------------------------------------------------
//	State-machiene
//-----------------------------------------------------------------------------

#define	STAT_WAIT	0
#define STAT_SEND_CMD	1
#define STAT_ACK_WAIT	2
#define STAT_SENDING	3
#define STAT_SEND_PLI	4
#define STAT_SEND_PL	5
#define STAT_RCV_PLI	6
#define STAT_RCV_DTA	7
#define STAT_REPLY	8

#define XIO_IDLE	0
#define XIO_REQ		1
#define XIO_RUNNING	2
#define XIO_DTA_RDY	3
#define XIO_HAS_DTA	4
#define XIO_CYCLE_RDY	5

typedef struct
{
  uint8_t cmd;
  uint8_t tpl;		// TX payload
  uint8_t rpl;		// RX payload
  uint8_t state;
  uint8_t ComStatus;
  uint8_t SlaveStatus;
  uint8_t *rbuf;
  uint8_t tbuf[XCOM_BUFSIZE];
  uint8_t tptr;
  uint8_t rptr;
} Xstat_t;

Xstat_t Xstat;

Xcom_buf_t XcomRxBuffer;

uint16_t *DelayTick;

#define DELAYTICK	(*DelayTick)

uint8_t
Xcom_init (void)
{
  uint8_t presense_flag;
  XDDR = XDDR & ~XPINS;			// GPIO-Pins 0-3 for XCOM
  XPORT = (XPORT & ~XPINS) | (1 << XPIN_ACK);

  Xstat.tpl = 0;
  Xstat.rpl = 0;
  Xstat.state = STAT_WAIT;
  Xstat.ComStatus = XIO_IDLE;
  Xstat.rbuf = (uint8_t*) &XcomRxBuffer.buf;

  /*
   *
   * check if slave is present...
   *
   */

  DelayTick = request_SysTimer();
  presense_flag = 0;
  Xstat.SlaveStatus = 0;
  DELAYTICK = 0;
  XREQUEST;
  while (DELAYTICK < 5)
    ;
  NO_XREQ;
  DELAYTICK = 0;
  while (DELAYTICK < 5)
    ;
  XREQUEST;
  DELAYTICK = 0;
  while ((DELAYTICK < 5) && !XACK)
    ;
  if (XACK)	// reaction on REQ
    {
      presense_flag++;
      NO_XREQ;
      _delay_us (500);			// wait 0,5ms for slave to become ready

      Xcom_cmd (XCMD_PING, 1, "X");
      _delay_us (500);

      DELAYTICK = 0;

      do
	Xcom_worker ();
      while ((DELAYTICK < 5) && (Xstat.ComStatus != XIO_CYCLE_RDY));

      if (DELAYTICK == 5)	// no slave
	{
#ifndef XCOM_DEBUG
	  XDDR &= ~XPINS;	// reset port
	  XPORT &= ~XPINS;
#endif
	}
      else
	Xstat.ComStatus = XIO_IDLE;
      if (Xstat.SlaveStatus == ~XCMD_PING)
	presense_flag++;
    }
#ifdef XCOM_DEBUG
  presense_flag = 2;
#endif

  return (presense_flag);
}

void
Xcom_ack (void)
{
  Xstat.ComStatus = XIO_IDLE;
}

static char
Xsend_byte (uint8_t b)
{
  register uint8_t bt, i;

  bt = b;
  for (i = 0; i < 8; i++)
    {
      if ((bt & 0b10000000) == 0b10000000)
	XDTAHI;
      else
	XDTALO;
      XCLKLO;
      _delay_us (TX_DELAY_HI);
      XCLKHI;
      _delay_us (TX_DELAY_LO);
      bt = bt << 1;
    }
  XDTAHI;
  return (b);
}

static char
Xread_byte (void)
{
  register uint8_t bt, i;

  bt = 0;
  for (i = 0; i < 8; i++)
    {
      XCLKLO;
      _delay_us (TX_DELAY_LO);
      if (XDTA)
	bt = (bt << 1) | 1;
      else
	bt = (bt << 1) & 0xfe;
      XCLKHI;
      _delay_us (TX_DELAY_HI);
    }
  return (bt);
}

/*
 * Main worker-trhead called cyclic from, main()
 */
void
Xcom_worker (void)
{
  switch (Xstat.state)
    {
    case STAT_WAIT:
      if (!XACK)
	if (Xstat.ComStatus == XIO_REQ)
	  {
	    XSEND;
	    XREQUEST;	// request bus
	    Xstat.ComStatus = XIO_RUNNING;
	    Xstat.state = STAT_ACK_WAIT;
	  }
      break;

    case STAT_ACK_WAIT:
      if (XACK)
	{
	  Xstat.state = STAT_SEND_CMD;
	  Xstat.tptr = Xstat.rptr = 0;
	}
      break;

    case STAT_SEND_CMD:
      Xsend_byte (Xstat.cmd);
      Xstat.state = STAT_SEND_PLI;

      break;

    case STAT_SEND_PLI:
      Xsend_byte (Xstat.tpl);
      Xstat.state = STAT_SEND_PL;

      break;

    case STAT_SEND_PL:
      Xsend_byte (Xstat.tbuf[Xstat.tptr++]);
      if (Xstat.tptr == Xstat.tpl)
	{
	  XRECEIVE;
	  if (Xstat.cmd & 0x80)	// expect received data
	    {
	      Xstat.state = STAT_RCV_PLI;
	    }
	  else
	    {
	      Xstat.state = STAT_REPLY;
	    }
	}
      break;

    case STAT_RCV_PLI:
      Xstat.rpl = Xread_byte ();
      if (Xstat.rpl > XCOM_BUFSIZE)
	Xstat.rpl = 1;
      Xstat.state = STAT_RCV_DTA;
      break;

    case STAT_RCV_DTA:
      Xstat.rbuf[Xstat.rptr++] = Xread_byte ();
      if (Xstat.rptr == Xstat.rpl)
	{
	  Xstat.ComStatus = XIO_HAS_DTA;
	  Xstat.state = STAT_REPLY;
	}
      break;

    case STAT_REPLY:
      Xstat.SlaveStatus = Xread_byte ();	// last byte
      NO_XREQ;		// Transfer completed
      if (Xstat.ComStatus == XIO_HAS_DTA)
	Xstat.ComStatus = XIO_DTA_RDY;
      else
	Xstat.ComStatus = XIO_CYCLE_RDY;
      Xstat.state = STAT_WAIT;
      break;

    default:
      Xstat.ComStatus = XIO_IDLE;
      Xstat.state = STAT_WAIT;
      break;
    }
}

/*
 * Public Funktions
 */
uint8_t
Xcom_avail (void)
{
  if (Xstat.ComStatus == XIO_HAS_DTA)
    return (Xstat.rpl);
  else
    return (0);
}

uint8_t
Xcom_cmd (uint8_t cmd, uint8_t size, void* pl)
{
  if ((size != 0) && (size <= XCOM_BUFSIZE))
    {
      while (Xstat.ComStatus >= XIO_REQ)
	Xcom_worker ();
      Xstat.cmd = cmd;
      Xstat.tpl = size;
      Xstat.tptr = 0;
      memcpy (&Xstat.tbuf, pl, size);
      Xstat.ComStatus = XIO_REQ;
      return (0);
    }
  else
    return (-1);	//
}

void
Xcom_status (uint8_t status)
{
  Xcom_cmd (XCMD_STATUS, 1, &status);

}
void
Xcom_ErrorMsg (uint8_t error)
{
  Xcom_cmd (XCMD_ERRORMSG, 1, &error);
}
