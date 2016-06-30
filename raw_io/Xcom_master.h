/*
 * Xcom.h
 *
 *	MASTER
 *	======
 *
 *  Created on: 10.03.2016
 *      Author: harry
 */

#ifndef XCOM_MASTER_H_
#define XCOM_MASTER_H_

#include "Xcom_common.h"

#define NO_XREQ		XPINHI(XPIN_REQ)
#define XREQUEST	XPINLO(XPIN_REQ)

#define XACK	(!(XPIN & (1 << XPIN_ACK)))

extern Xcom_buf_t XcomRxBuffer;

uint8_t Xcom_init(void);
void Xcom_worker(void);
uint8_t Xcom_avail(void);
uint8_t Xcom_cmd(uint8_t cmd, uint8_t size, void* pl);
void Xcom_status(uint8_t status);
void Xcom_ErrorMsg(uint8_t error);
void Xcom_ack(void);

#endif /* XCOM_MASTER_H_ */
