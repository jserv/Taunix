/* ----------------------------------------------------------------------- 
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
		
/* Establish date : 1999-4-27
 * Author : Max Yin
 * Orgnization : Tau Workshop
 * File : WDRTI.h
 * Content :
 *   Watch-Dog/Real-Time Interrupt (WDRTI)
 */

/* Project codename : fastTaunix
 * Modified date : 1999-7-6
 * Changes:
 *   1. Eliminate wdrti structure and its functions. Only wdrti_init() is
 *      preserved for initialize watch-dog and realtime interrupt.
 *   2. Add some macros to access watch-dog and realtime interrupt register.
 */

/* Modified date : 1999-9-13
 * Changes :
 *   Primariy changes for TMS320F243 DSP chip.
 *   1. Redefine sys_time strucure because of the lack of real-time clock
 *   on x243.
 */

#ifndef WDRTI_H
#define WDRTI_H

#define WDCLK_15625

#define WDCLK_16384


/*
extern CDEVSW	wdrt;
*/

void wdrt_init();
/*
int wdrt_read(void *buf,unsigned int size);
int wdrt_write(void *buf,unsigned int size);
int wdrt_ioctl(unsigned request,void *argp);
void wdrt_isr();
*/

/* Definitions for parameters */
/* RTI Control Register */
/* RTI ENA(0)@RTICR.6 */
#define RTI_ENABLE		0x40
#define RTI_DISABLE		0x00
/* RTIPS(2:0)@RTICR.2-0 */
               /* WDCLK in kHz :  16.384				15.625 */
                     /*         freq	Overflow	freq	Overflow */
                        /*    	(Hz)	  Time 		(Hz)	  Time */
#define RTIPS_DIV4		0x00 /*	4096    244.14us   3906.25      256us  */
#define RTIPS_DIV16		0x01 /*	1024    976.56us    976.56    1.024ms  */
#define RTIPS_DIV64		0x02 /*	 256      3.91ms    244.14    4.096ms  */
#define RTIPS_DIV128	0x03 /*	 128      7.81ms    122.07    8.192ms  */
#define RTIPS_DIV256	0x04 /*	  64     15.63ms     61.04   16.384ms  */
#define RTIPS_DIV512	0x05 /*	  32     31.25ms     30.52   32.768ms  */
#define RTIPS_DIV2048	0x06 /*	   8    125.00ms      7.63  131.072ms  */
#define RTIPS_DIV16384	0x07 /*	   1      1.00ms      0.95   1.049sec  */
                                                                         
/* WD Timer Control Register (WDCR) */
/* WDDIS(0)@WDCR.6 */
#define WD_ENABLE		0x00
#define WD_DISABLE		0x40
                        
/* WDCHK(2:0)@WDCR.5-3 */
#define WDCHK		0x28
/* WDPS(2:0)@WDCR.2-0 */
               /* WDCLK in kHz :  16.384				15.625 */
                     /*         freq	Overflow	freq	Overflow */
                        /*    	(Hz)	  Time 		(Hz)	  Time */
#define WDPS_DIV1		0x00  /*  64	15.63ms    61.04      16.38ms  */
#define WDPS_DIV2		0x02  /*  32    31.25ms    30.52      32.77ms  */
#define WDPS_DIV4		0x03  /*  16    62.50ms    15.26      65.54ms  */
#define WDPS_DIV8		0x04  /*   8   125.00ms     7.63     131.07ms  */
#define WDPS_DIV16		0x05  /*   4   250.00ms     3.81     262.14ms  */
#define WDPS_DIV32		0x06  /*   2   500.00ms     1.91     524.29ms  */
#define WDPS_DIV64		0x07  /*   1   1.0   sec    0.95     1.05 sec  */

/* Define WD reset reguest */
#define DISABLE_WD		1
#define ENABLE_WD		2
#define RESET_WD		3
#define DISABLE_RTI		4
#define ENABLE_RTI		5

typedef struct _sys_time{
/* For 243 
	unsigned tick;
	unsigned Ktick, Mtick;
*/
/* For 240 */
	unsigned sec, ms, us;
/**/
}SYS_TIME;

extern SYS_TIME	sys_time;  

/* Macros definitions */
#define WDRTI_RESETWD		MMREGS[WDKEY] = 0x5555;MMREGS[WDKEY] = 0xAAAA
#define WDRTI_ENABLEWD		MMREGS[WDCR] = (MMREGS[WDCR] & 0x2F)|WDCHK;\
 	 						MMREGS[WDKEY] = 0x5555;\
 	 						MMREGS[WDKEY] = 0xAA
#define WDRTI_DISABLEWD		MMREGS[WDCR] |= 0x40
#define WDRTI_ENABLERTI		MMREGS[RTICR] |= 0x40
#define WDRTI_DISABLERTI	MMREGS[RTICR] &= 0x0F

#endif
