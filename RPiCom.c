/*
 * RPiCom.c
 *
 *  Created on: 21.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>
#include <util/crc16.h>

#include "RPiCom.h"

/************************************************************************
 *
 ************************************************************************/

#define	STAT_WAIT		0
#define	STAT_AUTO_REPLY		1
#define	STAT_MAN_CMD		2
#define	STAT_MAN_DTA		3
#define STAT_MAN_ACK		4
#define	STAT_AUTO_DTA		5
#define STAT_CRC16L		6
#define STAT_CRC16H		7
#define STAT_CRC16C		8
#define STAT_WAIT_RELEASE	9
#define	STAT_CYCLE_RDY		10

/************************************************************************
 *
 ************************************************************************/

#define START_MANUAL		(0xaa)
#define START_AUTO		(0x80)

/************************************************************************
 *
 ************************************************************************/

typedef struct
{
  uint8_t from;
  uint8_t to;
  int_fn_t fn;
} RPi_mcmd_t;

typedef struct
{
  uint8_t *tx;
  uint8_t *rx;
  uint8_t Status;
} Client_Buffer_t;

static volatile uint8_t FrameSize;
static volatile uint8_t *spi_rxbuf;
static volatile uint8_t *spi_txbuf;
static volatile RPi_mcmd_t *CmdTable;

static volatile uint8_t tempStat;

static volatile uint8_t SpiState;
static volatile uint8_t *SpiReady;

static volatile Client_Buffer_t *Client;

/*****************************************************************************
 *
 * Init RPi-communication
 *
 * template for client_info:
 *
 * typedef struct
 * {
 * 	uint8_t *tx;
 * 	uint8_t *rx;
 * 	uint8_t Status;
 * }Client_Buffer_t;
 *
 *****************************************************************************/

void
RPi_init (uint8_t AutoFrameSize, uint8_t max_num_of_cmds, void *client_info)
{

  FrameSize = AutoFrameSize;
//  FrameSize = 0x1c;
  /*
   * populate client-data-areas
   */
  Client = client_info;
  SpiReady = &Client->Status;
  Client->rx = calloc (AutoFrameSize, 1);
  Client->tx = calloc (AutoFrameSize, 1);

  /*
   * populate mirror buffers
   */
  spi_rxbuf = calloc (AutoFrameSize, 1);
  spi_txbuf = calloc (AutoFrameSize, 1);

  /*
   * create command-table
   */
  CmdTable = calloc (max_num_of_cmds + 1, sizeof(RPi_mcmd_t));

  DDRB |= (1 << PB6);
//  DDR_SPI |= (1 << DD_MISO);

  *SpiReady = 0;
  SpiState = STAT_WAIT;
  tempStat = 0;
  *SpiReady = RPI_STAT_IDLE;

}

/*****************************************************************************
 *
 * Start communication
 *
 *****************************************************************************/
void
RPi_start (void)
{
  //Enable SPI interface and SPI interrupt source
  SPCR = (1 << SPE) | (1 << SPIE);
}

/*****************************************************************************
 *
 * Register function for manual command
 *
 * fromc	lowest command
 * toc highest command
 * fn	pointer to helkper-function
 *
 *****************************************************************************/

void
RPi_RegisterFunction (uint8_t fromc, uint8_t toc, int_fn_t fn)
{
  uint8_t i;
  for (i = 0; CmdTable[i].fn != NULL; i++)
    ;
  CmdTable[i].from = fromc;
  CmdTable[i].to = toc;
  CmdTable[i].fn = fn;
}

/*****************************************************************************
 *
 * Worker-function
 * schould be called cyclic
 *
 *****************************************************************************/

void
RPi_worker (void)
{
  if (PINB & (1 << PB4))	// SS deselect;
    {
      switch (SpiState)
	{
	case STAT_WAIT_RELEASE:
	  *SpiReady = tempStat;
	  break;
	default:
	  *SpiReady = RPI_STAT_IDLE;
	  break;
	}
      SpiState = STAT_WAIT;
    }
}

/*****************************************************************************
 *
 * Update TX-data
 *
 *****************************************************************************/
void
RPi_update (void)
{
  uint8_t sreg;

  sreg = SREG;
  cli();
  if (*SpiReady == RPI_STAT_IDLE)
    memcpy(spi_txbuf,Client->tx, FrameSize);
  SREG = sreg;
}

/*****************************************************************************
 *
 * Acknoledge
 *
 *****************************************************************************/
void
RPi_ack (void)
{
  *SpiReady = RPI_STAT_IDLE;
  tempStat = RPI_STAT_IDLE;
}

/*****************************************************************************
 *
 * Error-messages with error-number
 *
 *****************************************************************************/
void
RPi_error (uint8_t error)
{
  ;
}

/************************************************************************
 *
 * SPI - Interrupt
 *
 ************************************************************************/

ISR(SPI_STC_vect)
{

  uint8_t RxTemp;
  uint8_t TxTemp;
  uint8_t i;
  uint8_t *TmpBuffer;

  static uint8_t cmd;
  static uint8_t pls;
  static uint8_t BytesSend;
  static uint8_t rx_ptr;
  static uint8_t tx_ptr;
  static int_fn_t cmdFn;
  static uint16_t rxcrc16, txcrc16, Newcrc16;

  uint8_t sreg;

  sreg = SREG;
  cli();

  RxTemp = SPDR;
  TxTemp = 0;

  //+++++++++++++++++++++++++++++++++++++++++
  //Process the reveived data from SPI (SPDR)
  //+++++++++++++++++++++++++++++++++++++++++

  switch (SpiState)
    {
    case STAT_WAIT:
      *SpiReady = RPI_STAT_RUN;
      if (RxTemp == START_MANUAL)
	{
	  TxTemp = (~START_MANUAL & 0xff);
	  SpiState = STAT_MAN_CMD;
	}
      else
	{
	  if (RxTemp == START_AUTO)
	    {
	      // initialize buffers
	      rxcrc16 = txcrc16 = 0xffff;
	      tx_ptr = rx_ptr = 0;
	      TxTemp = START_AUTO;
	      SpiState = STAT_AUTO_REPLY;
	    }
	}
      break;

//------------------------------------------------------------------------------
//		Automatic mode
//------------------------------------------------------------------------------
    case STAT_AUTO_REPLY:
      if (RxTemp == 0xff)
	{
	  TxTemp = spi_txbuf[tx_ptr++];
	  txcrc16 = _crc16_update (txcrc16, TxTemp);
	  SpiState = STAT_AUTO_DTA;
	}
      else
	SpiState = STAT_WAIT_RELEASE;
      break;

    case STAT_AUTO_DTA:
      spi_rxbuf[rx_ptr++] = RxTemp;
      rxcrc16 = _crc16_update (rxcrc16, RxTemp);
      TxTemp = spi_txbuf[tx_ptr++];
      txcrc16 = _crc16_update (txcrc16, TxTemp);
      if (tx_ptr == FrameSize)
	{
	  SpiState = STAT_CRC16L;
	}
      break;

    case STAT_CRC16L:
      spi_rxbuf[rx_ptr++] = RxTemp;	// Last rx byte
      rxcrc16 = _crc16_update (rxcrc16, RxTemp);
      TxTemp = txcrc16 & 0xff;		// first TX Low-Byte of crc16
      SpiState = STAT_CRC16H;
      break;

    case STAT_CRC16H:
      TxTemp = txcrc16 >> 8;		// second TX High Byte of crc16
      Newcrc16 = RxTemp;	// first RX Low Byte of crc16
      SpiState = STAT_CRC16C;
      break;

    case STAT_CRC16C:
      Newcrc16 += RxTemp << 8;
      if (Newcrc16 == rxcrc16)
	{				// valid crc16
	  TmpBuffer = spi_rxbuf;
	  spi_rxbuf = Client->rx;
	  Client->rx = TmpBuffer;
	  tempStat = RPI_STAT_AUTO;
	}
      else
	{
	  tempStat = RPI_STAT_CRC16_ERROR;
	}
      TxTemp = START_AUTO;
      SpiState = STAT_WAIT_RELEASE;
      break;

      /*------------------------------------------------------------------------------
       *
       * Manual mode
       *
       ------------------------------------------------------------------------------*/
    case STAT_MAN_CMD:
      cmd = RxTemp;
      BytesSend = 0;
      cmdFn = NULL;

      for (i = 0;
	  ((cmd < CmdTable[i].from) || (cmd > CmdTable[i].to))
	  	  && (CmdTable[i].fn != NULL); i++)
	;
      cmdFn = CmdTable[i].fn;
      if (cmdFn != NULL)
	{
	  pls = cmdFn (BytesSend++, cmd, RxTemp);
	  SpiState = STAT_MAN_DTA;
	}
      else
	{
	  tempStat = RPI_STAT_MAN_UNKNOWN;
	  SpiState = STAT_WAIT_RELEASE;

	}
      TxTemp = RxTemp;
      break;

    case STAT_MAN_DTA:
      TxTemp = cmdFn (BytesSend++, cmd, RxTemp);

      if (BytesSend == pls)
	{
	  TxTemp = 0;
	  SpiState = STAT_WAIT_RELEASE;
	  tempStat = RPI_STAT_MAN;
	}
      break;

    case STAT_WAIT_RELEASE:
      TxTemp = 0;
      break;
    }

  SPDR = TxTemp;
  SREG = sreg;
}

