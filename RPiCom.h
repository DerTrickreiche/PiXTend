/*
 * RPiCom.h
 *
 *  Created on: 21.03.2016
 *      Author: harry
 */

#ifndef RPICOM_H_
#define RPICOM_H_

#define RPI_STAT_IDLE	0
#define RPI_STAT_RUN	1
#define RPI_RUN_RDY	2
#define RPI_STAT_AUTO	3
#define RPI_STAT_MAN	4
#define RPI_STAT_MAN_UNKNOWN	5
#define RPI_STAT_CRC16_ERROR	6

#define	ERR_CRC16		1
#define	ERR_CMD_UNKNOWN		2

/*****************************************************************************
 *
 * Interrupt-helper prototype
 *
 *****************************************************************************/

typedef uint8_t (*int_fn_t) (uint8_t count, uint8_t cmd, uint8_t dta);

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
 * 	uint8_t *Status;
 * }Client_Buffer_t;
 *
 *****************************************************************************/

void RPi_init(uint8_t AutoFrameSize, uint8_t max_num_of_cmds, void *client_info);

/*****************************************************************************
 *
 * Start communication
 *
 *****************************************************************************/
void RPi_start(void);

/*****************************************************************************
 *
 * Update TX-data
 *
 *****************************************************************************/
void RPi_update(void);

/*****************************************************************************
 *
 * Acknoledge
 *
 *****************************************************************************/
void RPi_ack(void);

/*****************************************************************************
 *
 * Register function for manual command
 *
 * fromc	lowest command
 * toc highest command
 * fn	pointer to helkper-function
 *
 *****************************************************************************/

void RPi_RegisterFunction(uint8_t fromc, uint8_t toc, int_fn_t fn);

/*****************************************************************************
 *
 * Worker-function
 * schould be called cyclic
 *
 *****************************************************************************/

void RPi_worker(void);

/*****************************************************************************
 *
 * Error-messages with error-number
 *
 *****************************************************************************/
void RPi_error(uint8_t error);

#endif /* RPICOM_H_ */
