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
Establish date: 1999-4-28
Author : Max Yin
Orgnization : Tau Workshop

File : Sleep.h
Content:
	Implementation of sleep() modual
-----------------------------------------------*/
/*
 Modified date: 19990820.1556
 Code modified:
	Rewrite the sleep queue mantainabce code. Use priList
	module to do the job.

*/
/*
#include <Taunix\task.h>
*/
#include "task.h"
#ifndef SLEEP_H
#define SLEEP_H
/*
typedef struct _sleepq{
	long diff;
	int prev;
    int next;    
}SLEEPQ;
*/
void sleepq_init();
int sleep(unsigned clk_tck);

#define SLEEP_EXPIRED		1
#define SLEEP_NOTEXPIRED	0

#endif
