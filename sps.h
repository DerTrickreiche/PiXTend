/*
 * sps.h
 *
 *  Created on: 13.03.2016
 *      Author: harry
 */

#ifndef IO_SPS_H_
#define IO_SPS_H_

/************************************************************************
 *	µc-Status
 ************************************************************************/

#define UCC_WD		0	// wd status
#define UCC_RUN		4	// run-bit

#define WD_BIT		(1 << UCC_WD)
#define RUN_BIT		(1 << UCC_RUN)

#define UCS_RUN		0
#define UCS_DEbUG	1
#define UCS_JP8		2
#define UCS_JP9		3
#define UCS_SP		6	// slave present
#define UCS_MM		7	// monitor-mode

#define SP_BIT		(1 << UCS_SP)
#define MM_BIT		(1 << UCS_MM)

#define RUNNING		(Reg_uc  & (1 << UCC_RUN))
#define IDLE		(!RUNNING)



#define SLAVE_PRESENT	(Features & (1 << UCS_SP))


extern uint8_t Features;

extern IMap_t RealOutput;

void init_modules (void);
void register_module (uint16_t *mod_info[]);
void register_mod_spi_commands(void);
void sps_worker(void);

void sps_init();
void sps_reset(void);

uint8_t int_set_uc_ctrl(uint8_t count,  uint8_t cmd, uint8_t dta);
uint8_t int_set_RPi_stat(uint8_t count,  uint8_t cmd, uint8_t dta);
uint8_t int_get_uc_version(uint8_t count,  uint8_t cmd, uint8_t dta);
uint8_t int_get_uc_status (uint8_t count, uint8_t cmd, uint8_t dta);


#endif /* IO_SPS_H_ */
