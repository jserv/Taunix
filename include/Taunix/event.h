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
		
/* Establish date : 1999-4-28
 * Author : Max Yin
 * Orgnization : Tau Workshop
 * File : Event.h
 * Content :
 *   Event machemnism
 */

#ifndef EVENT_H
#define EVENT_H

typedef struct _event{
	int flag;
	int wait_task;
}EVENT;


void event_init(EVENT *evt,int init_flag);
void event_set(EVENT *evt);
void event_reset(EVENT *evt);
int event_wait(EVENT *evt,int wait_mode);


/* Definition of mode in event_wait(int wait_mode) */
#define EVT_NONWAIT		0
#define EVT_WAIT		1

#endif
