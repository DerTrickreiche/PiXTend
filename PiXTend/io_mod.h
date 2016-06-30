/*
 * io_mod.h
 *
 *  Created on: 25.03.2016
 *      Author: harry
 */

#ifndef IO_MOD_H_
#define IO_MOD_H_

typedef struct
{
  void (*Register)(void);
  void (*Init)(void);
  void (*Output)(void);
  void (*Input)(void);
  void (*IOControl)(void);
  void (*Worker)(void);
}IO_Mod_t;

#endif /* IO_MOD_H_ */
