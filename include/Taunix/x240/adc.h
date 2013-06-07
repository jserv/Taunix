/* ----------------------------------------------------------------------- 
*
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
*   USA; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
* ----------------------------------------------------------------------- */

/*---------------------------------------------*\
|                                               |
|	TI DSP Realtime kernel project: Taunix  |
|		Taunix : TAUnix's UNIX-like     |
|                                               |
\*---------------------------------------------*/
		
/*-----------------------------------------------
Establish date: 1999-5-6
Author : Max Yin
Orgnization : Tau Workshop

File : ADC.h
Content:
	A/D Convertor Module for x240 and x243	
-----------------------------------------------*/

#include <Taunix\cdevsw.h>


#ifndef ADC_H
#define ADC_H
extern CDEVSW	adc;

void adc_init();
int adc_open(int,int);
int adc_close();
int adc_setcallback(void (*callback)());
int adc_read(void *buf,unsigned int fifo); /* fifo use ADCFIFO1/ADCFIFO2 */
int adc_ioctl(unsigned request, void *argp);


/* Request */
#define ADC_SET		0     	/* setup register */
#define ADC_READ	0x8000	/* read ADC status */
#define EOC		0x0080	/* End-of-conversion bit */
#define FIFO1STATUS	0x00C0	/* FIFO1 status */
#define FIFO2STATUS	0x0018	/* FIFP2 status */
#define INTFLAG	0x0100	/* ADC interrupt flag */

#define ADC_IMSTART	0x2000	/* ADC immediately start */
#define ADC_SOC	0x0001	/* ADC start */
#define ADC_SELCH	0x0002	/* ADC select channel */
/* ADC options */
/* ADC control register 1 */
#define ADC_CONRUN		0x0400 /* continue to run */
/* ADCINTEN:1@ADCTRL1.9 : enable ADC interrupt */
#define ADCINT_ENABLE	0x0200
#define ADCINT_DISABLE	0x0000
/* ADCxCHSEL:6-1@ADCTRL1.6-1 ,select ADC analog channel */
#define ADCH_1		0x0000
#define ADCH_2		0x0002
#define ADCH_3		0x0004
#define ADCH_4		0x0006
#define ADCH_5		0x0008
#define ADCH_6		0x000A
#define ADCH_7		0x000C
#define ADCH_8		0x000E
#define ADCH_9		0x0000
#define ADCH_10	0x0010
#define ADCH_11	0x0020
#define ADCH_12	0x0030
#define ADCH_13	0x0040
#define ADCH_14	0x0050
#define ADCH_15	0x0060
#define ADCH_16	0x0070
/* ADC control register 2 */  
/* ADCEVSOC:1@ADCTRL2.10 */
#define ADCEVSOC_DISABLE		0x0000
#define ADCEVSOC_ENABLE		0x0400
/* ADCEXTSOC:1@ADCTRL2.9 */
#define ADCEXTSOC_DISABLE	0x0000
#define ADCEXTSOC_ENABLE		0x0200
/* ADCPSCALE:3-1@ADCTRL2.2-0 */
#define ADCPS_4		0x0000
#define ADCPS_6		0x0001
#define ADCPS_8		0x0002
#define ADCPS_10		0x0003
#define ADCPS_12		0x0004
#define ADCPS_16		0x0005
#define ADCPS_20		0x0006
#define ADCPS_32		0x0007
/* For 243DSP, prescaler are a little different */

#ifdef TI_DSP32Cx243
#define ADCPS_1		0x0000
#define ADCPS_2		0x0001
/*#define ADCPS_4		0x0002*/
/*#define ADCPS_8		0x0003*/
/*#define ADCPS_12		0x0004*/
/*#define ADCPS_16		0x0005*/
#define ADCPS_24		0x0006
/*#define ADCPS_32		0x0007*/
#endif

/* FIFO status */
#define ADCFIFO_EMPTY		0
#define ADCFIFO_1ENTRY		1
#define ADCFIFO_2ENTRIES		2
#define ADCFIFO_OVERRUN		3

#endif
