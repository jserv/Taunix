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

/*
#include <Taunix\task.h>
#include <Taunix\sleep.h>
#include <Taunix\priList.h>
*/
#include "task.h"
#include "sleep.h"
#include "priList.h"

/*#define MAX_TASK	32*/
unsigned _sleepq_processing = 0;
priNode	taskSleepNodes[MAX_TASK];
priList	sleepq;

void sleep_init()
{
	List_init(&sleepq,taskSleepNodes,MAX_TASK);
}

int sleep(unsigned clk_tck)
{
	taskSleepNodes[current_task].prio = clk_tck;
	disable_sched
		_sleepq_processing = 1;
		diffList_insert(&sleepq,current_task);
		_sleepq_processing = 0;
		suspend(current_task);
	enable_sched
	
	if(taskSleepNodes[current_task].prio>0){
		/* Sleep time is not expired */
		disable_sched
			_sleepq_processing = 1;
			sleepq.current = current_task;
			List_delete(&sleepq);
			_sleepq_processing = 0;
		enable_sched
		return SLEEP_NOTEXPIRED;
	}
	return SLEEP_EXPIRED;
}

inline void wakeup()
{
	if(sleepq.contents == 0)	return;	/* no sleeping task */
	if(_sleepq_processing)	return;	/* some one lock the sleep queue */
	if(taskSleepNodes[sleepq.head].prio)	return; /* no task expired */

	/*disable_sched*/
		do{
			sleepq.current = sleepq.head;
			resume(sleepq.head);
			List_delete(&sleepq);
		}while(sleepq.contents && 
				taskSleepNodes[sleepq.head].prio==0);
	/*enable_sched*/
}
