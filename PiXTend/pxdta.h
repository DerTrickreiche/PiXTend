/*
 * pxdta.h
 *
 *  Created on: 10.03.2016
 *      Author: harry
 */

#ifndef PXDTA_H_
#define PXDTA_H_


/*-----------------------------------------------------------------------------
 *
 * ADC
 *
-----------------------------------------------------------------------------*/
#define	NUMBER_OF_ADC		4

typedef struct {
  uint8_t samples;
  uint8_t freq;
}adc_ctrl_t;

/*-----------------------------------------------------------------------------
 *
 * DHT11
 *
-----------------------------------------------------------------------------*/
#define NUMBER_OF_DHT11	4

typedef struct
{
  uint16_t Temperature[NUMBER_OF_DHT11];
  uint16_t Humidity[NUMBER_OF_DHT11];
} dht11_t;


/*-----------------------------------------------------------------------------
 *
 * PWM
 *
-----------------------------------------------------------------------------*/
#define	NUMBER_OF_PWM	2

typedef struct
  {
    uint8_t control;
    uint16_t timer;
  } pwm_ctrl_t;

typedef struct
{
  pwm_ctrl_t pwm;
  uint8_t gpio;		// 10
  uint8_t uc;			// 11
  adc_ctrl_t ai;
  uint8_t PiStatus;		// 14
} pxRegister_t;

typedef struct
{
  uint8_t dout;			// 0
  uint8_t relays;		// 1
  uint8_t gpio;			// 2
  uint16_t pwm[NUMBER_OF_PWM];	// 3
  pxRegister_t stat;		// 7
} IMap_t;

typedef struct
{
  uint8_t din;			// 0
  uint16_t adc[NUMBER_OF_ADC];	// 1
  uint8_t gpio;			// 9
  dht11_t dht11;		// 10
  uint8_t ucVersion[2];	 	// 26
  uint8_t ucStatus;		// 28
} OMap_t;

#endif /* PXDTA_H_ */
