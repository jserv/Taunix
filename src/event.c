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

#include "task.h"
#include "event.h"

void event_init(EVENT *evt,int init_flag)
{
	evt->flag = init_flag;
    evt->wait_task = -1;
}

void event_set(EVENT *evt)
{
	disable_sched
		evt->flag = 1;	/* Set flag to 1 */
		if(evt->wait_task != -1){ /* There's task wait for event */
			resume(evt->wait_task);	/* resume it */
			evt->wait_task = -1;		/* Reset wait_task flag */
		}
	enable_sched				/* re-schedual */
}

void event_reset(EVENT *evt)
{
	evt->flag = 0;		/* Reset event flag */
}


int event_wait(EVENT *evt, int wait_mode)
{
	disable_sched
		if(!evt->flag)	/* flag is not set */
			if(wait_mode){ /* wait for event */
				evt->wait_task = current_task;
				suspend(current_task); /* self-suspend */
			}
	enable_sched				/* and re-schedual */
	/* If non_wait mode and flag not set,return 0 */
	if((!wait_mode) && !evt->flag)	return 0;    
	/* else return 1 */
	return 1;
}
