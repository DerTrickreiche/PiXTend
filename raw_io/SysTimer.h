/*
 * timer.h
 *
 *  Created on: 21.03.2016
 *      Author: harry
 */

#ifndef SYSTIMER_H_
#define SYSTIMER_H_

#define TIMER_MAX_REQ	12

/*****************************************************************************
 *
 * 1ms Timer-interrupt setup
 *
 *****************************************************************************/
void init_SysTimer (void);

/*****************************************************************************
 *
 * request next free systimer (1ms)
 *
 *****************************************************************************/
uint16_t *request_SysTimer(void);



#endif /* SYSTIMER_H_ */
