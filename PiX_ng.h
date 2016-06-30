/*
 * PIXio.h.h
 *
 *  Created on: 01.03.2016
 *      Author: harry
 */

#ifndef PIX_NG_H_
#define PIX_NG_H_


#define	PIX_HW_VERSION	12
#define	PIX_SW_VERSION	5

/************************************************************************
 * Features
 * 	bit 0	Jumper 8
 * 	bit 1	Jumper 9
 * 	bit 6	Slave present
************************************************************************/

#define	JP8STAT		1
#define JP9STAT 	2
#define HASSLAVE	6
extern uint8_t Features;


//#define ENABLE_XCOM

#ifdef ENABLE_XCOM
#define XCOM_DEBUG
#endif

#define	ENABLE_DIO
#define	ENABLE_ADC
#define	ENABLE_PWM
//#define	ENABLE_DHT11

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))


typedef struct
{
  OMap_t *tx;
  IMap_t *rx;
  uint8_t Status;
} RPi_Buffer_t;

extern RPi_Buffer_t RPi;

extern uint8_t Features;

#endif /* PIX_NG_H_ */
