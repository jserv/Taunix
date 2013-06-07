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
Establish date: 1999-4-26	
Author : Max Yin
Orgnization : Tau Workshop

File : PLLClock.h
Content:
	F240's PLL clock module device driver
	
-----------------------------------------------*/
/* 
 Project codename: fastTaunix
 Modified date : 1999-7-6
 Changes:
 1. Add a macro : PLLCLK_SETUP to replace all functions 
    in pllclock structure.
 2. Eliminate pllclock structure and its functions.
*/

#ifndef PLLCLOCK_H
#define PLLCLOCK_H
/*
#include <Taunix\cdevsw.h>

extern CDEVSW	pllclock;

void pllclock_init();

int pllclock_read(void *buf,unsigned int CKCRx);
int pllclock_write(void *buf,unsigned int CKCRx);
int pllclock_ioctl(unsigned request,void *argp);

#define PLLCLOCK_CKCR0	0
#define PLLCLOCK_CKCR1	1
*/
/* PLL clock control options */

/* Clock mode CLKMD(1:0)@CKCR0.7-6 */
#define CLKMD_CLKIN_2	0x00
#define CLKMD_CLKIN		0x40
#define CLKMD_PLL		0x80
/* PLL lock PLLOCK(1:0)@CKCR0.5-4, only PLLOCK(1) needed */
/* If PLLOCK==1,PLL is locked. If PLLOCK == 0, PLL is still locking */
#define PLLOCK	(MMREGS[CKCR0]&0x20)
/* PLL low power mode PLLPM(1:0)@CKCR0.3-2 */
#define PLLPM_DEFAULT	0x00
/* Analog clock enable ACLKENA(1)@CKCR0.1 */
#define ACLK_ENABLE		0x20
#define ACLK_DISABLE    0x00
/* PLL prescale PLLPS(1)@CKCR0.0 */
#define PLLPS_CPUCLK_4	0x00
#define PLLPS_CPUCLK_2	0x01
/* Clock input frequency CKINF(3:0)@CKCR1.7-4 */
#define CKINF_32		0x00
#define CKINF_30		0x10
#define CKINF_28		0x20
#define CKINF_26		0x30
#define CKINF_24		0x40
#define CKINF_22		0x50
#define CKINF_20		0x60
#define CKINF_18		0x70
#define CKINF_16		0x80
#define CKINF_14		0x90
#define CKINF_12		0xA0
#define CKINF_10		0xB0
#define CKINF_8			0xC0
#define CKINF_6			0xD0
#define CKINF_4			0xE0
#define CKINF_2			0xF0
/* PLL input devided by 2 PLLDIV(0)@CKCR1.3 */
#define PLLDIV_2		0x08
#define PLLDIV_1		0x00
/* PLL multiplication feedback ratios PLLFB(2:0)@CKCR1.1-0 */
#define PLLFB_1			0x00
#define PLLFB_2			0x01
#define PLLFB_3			0x02
#define PLLFB_4			0x03
#define PLLFB_5			0x04
#define PLLFB_9			0x05

#define PLLCLK_SETUP(REG,value)		MMREGS[REG]=value;

#endif
