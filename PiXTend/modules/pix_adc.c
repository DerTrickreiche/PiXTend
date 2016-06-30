/*
 * pix_adc.c
 *
 *  Created on: 25.03.2016
 *      Author: harry
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "../PiXTend.h"
#include "../../RPiCom.h"
#include "../../raw_io/adc.h"
#include "pix_adc.h"

#define	NUMBER_OF_ADC		4

#define MAX_SAMPLES		64		// has to be a power of 2!


/*-----------------------------------------------------------------------------
 *
 * ADC
 *
-----------------------------------------------------------------------------*/
typedef struct {
  uint8_t samples;
  uint8_t freq;
}adc_ioctrl_t;



void pix_adc_register(void);
void pix_adc_init (void);
void pix_adc_output (void);
void pix_adc_ioctrl (void);
void pix_adc_worker (void);



void *ADC_Mod_Info[] =
    {
	pix_adc_register,
	pix_adc_init,
	pix_adc_output,
	NULL,
	pix_adc_ioctrl,
	pix_adc_worker
    };

adc_ctrl_t	ai;

void pix_adc_register(void)
{
  REGISTER_GET_ADC;
  REGISTER_SET_AI_IOCTRL;
}


void pix_adc_init (void)
{
  adc_init(0xf0, MAX_SAMPLES);
}

void
pix_adc_ioctrl (void)
{
  //------------------------------------------
  //------- SET ADC control registers --------
  //------------------------------------------
  uint8_t c, i, j;
  if (RealOutput.stat.ai.samples != ai.samples)
    {
      for (c = 0; c < NUMBER_OF_ADC; c++)
	{
	  i = (RealOutput.stat.ai.samples >> (c * 2)) & 0b00000011;
	  j = (ai.samples >> (c * 2)) & 0b00000011;
	  if (i != j)
	    {
	      switch (i)
		{
		case 0:				// 16 Sample
		  adc_set_samples (c, 4);
		  break;
		case 1:				// 1 Samples
		  adc_set_samples (c, 0);
		  break;
		case 2:				// 8 Samples
		  adc_set_samples (c, 3);
		  break;
		case 3:				// 64 Samples
		  adc_set_samples (c, 6);
		  break;
		default:
		  break;
		}
	    }
	  ai.samples = RealOutput.stat.ai.samples;
	}
    }
  if (RealOutput.stat.ai.freq != ai.freq)
    {
      for (c = 0; c < NUMBER_OF_ADC; c++)
	{
	  i = (RealOutput.stat.ai.freq >> (c * 2)) & 0b00000011;
	  switch (i)
	    {
	    case 0:
	      adc_set_rate(i,5);	// 5 ms
	      break;
	    case 1:				// 10 ms
	      adc_set_rate(i,10);
	      break;
	    case 2:				// 20 ms
	      adc_set_rate(i,20);
	      break;
	    case 3:				// 50 ms
	      adc_set_rate(i,50);
	      break;
	    default:
	      break;
	    }
	}
      ai.freq = RealOutput.stat.ai.freq;
    };
}

void
pix_adc_output (void)
{
  register uint8_t c;
  //-----------------------------------
  //-- GET values from analog inputs --
  //-----------------------------------

  for (c = 0; c < NUMBER_OF_ADC; c++)
    {
      RPi.tx->adc[c] = get_adc_value (c);
    }

  ;
}

void
pix_adc_worker (void)
{
  adc_worker();
}





