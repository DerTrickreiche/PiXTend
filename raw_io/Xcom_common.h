/*
 * Xcom_common.h
 *
 *  Created on: 11.03.2016
 *      Author: harry
 */

#ifndef XCOM_COMMON_H_
#define XCOM_COMMON_H_

#include "../PiXTend/pxdta.h"

#define XCOM_BUFSIZE	64

#define TX_DELAY_HI	15
#define TX_DELAY_LO	15

//-----------------------------------------------------------------------------
//	Commands
/*
 * Frame-structure:
 * 1. 			:	Command
 * 					bit 7: R/W
 * 						0 -> Write only
 * 						1 -> Read
 * 					bit [0..6]:	Command
 * 2. 		:		Sender Payloadsize
 * [3..n]: 			Payload
 * 				*** goes in Rx-Mode
 * n+1:				Received Payloadsize
 * [n+2..n+RPayloadSize]	Received data,
 */

//-----------------------------------------------------------------------------
#define	XCMD_RSTATUS		0b10000000
#define	XCMD_SERIAL_SEND	0b00000001
#define XCMD_GETRX_DTA		0b10000010
#define XCMD_ERRORMSG		0b00000011
#define XCMD_STATUS		0b00000100
#define XCMD_PING		0b00000101
#define XCMD_OUTPUT		0b00000110
#define XCMD_INPUT		0b10000111

#define STS_INIT_DIO		1
#define STS_INIT_ADC		2
#define STS_INIT_PWM		3
#define STS_INIT_DHT11		4
#define STS_INIT_SPS		5
#define STS_INIT_SPI		6

//-----------------------------------------------------------------------------
//	IO-helper
//-----------------------------------------------------------------------------

#define XPINS		((XPIN_CLK << 1) | (XPIN_REQ << 1) | (XPIN_ACK << 1) | (XPIN_DTA << 1))

#define XPINHI(p)	(XDDR &= ~(1 << p))
#define XPINLO(p)	(XDDR |= (1 << p))

#define XDTAHI		XPINHI(XPIN_DTA)
#define XDTALO		XPINLO(XPIN_DTA)

#define XCLKHI		XPINHI(XPIN_CLK)
#define XCLKLO		XPINLO(XPIN_CLK)

#define XRECEIVE	(XPORT |= (1 << XPIN_DTA)), (XDDR &= ~(1 << XPIN_DTA))
#define XSEND		(XPORT &= ~(1 << XPIN_DTA))
// #define XSEND		(XPORT = ~(1 < XPIN_DTA))), (XDDR |= (1 << XPIN_DTA))

#define XDTA	((XPIN & (1 << XPIN_DTA)) >> XPIN_DTA)

typedef struct
{
  dht11_t dht11;
  uint8_t gpio;
} SMap_t;		// 8 word dht11 + 1 byte gpio = 17 byte

typedef struct
{
  SMap_t io;
  uint16_t crc16;
} Sio_t;		// 17 byte io + 2 Byte crc16 = 19 byte

typedef union
{
  Sio_t rio;
  uint8_t buf[XCOM_BUFSIZE];

}Xcom_buf_t;

extern Xcom_buf_t Sbuf;


#endif /* XCOM_COMMON_H_ */
