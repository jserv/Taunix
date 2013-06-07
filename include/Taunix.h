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
Establish date: 1999-8-10	
Author : Max Yin
Orgnization : Tau Workshop

File : Taunix.h
Content:
	This file contains all file you need to use Taunix/x240.

Modified date: 1999-10-31
	Add items to use Taunix/x243
	


------------------------------------------------*/
/* Taunix 0.7.4 for TI DSP 32Cx240 32Cx243 */

/* Task management */
#include <Taunix\task.h>

/* Sleep module */
#include <Taunix\sleep.h>

/* IPC */
#include <Taunix\pipe.h>
#include <Taunix\event.h>
#include <Taunix\sem.h>

/* MISC module */
#include <Taunix\ssecs.h>

/* Devices on 240 dsp chip */
#ifdef TI_DSP32Cx240
#include <Taunix\x240\F240mmrs.h>
#include <Taunix\x240\adc.h>
#include <Taunix\x240\capture.h>
#include <Taunix\x240\gpt.h>
#include <Taunix\x240\pllclock.h>
#include <Taunix\x240\pwm.h>
#include <Taunix\x240\sci.h>
#include <Taunix\x240\wdrti.h>
#endif

/* Devices on 243 dsp chip */
#ifdef TI_DSP32Cx243
#include <Taunix\x243\F243mmrs.h>
#include <Taunix\x243\adc.h>
#include <Taunix\x243\capture.h>
#include <Taunix\x243\gpt.h>
#include <Taunix\x243\pllclock.h>
#include <Taunix\x243\pwm.h>
#include <Taunix\x243\sci.h>
#include <Taunix\x243\wdrti.h>
#endif
