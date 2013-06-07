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

/* Designe change:
 * Date: 19990927.1555
 * Code modified:
 *   In resume(), sometimes used in callback function, it will cause the
 *   program to reset. The reason should be the hardware stack overflow
 *   (maybe). Therefore, I reduce the calling depth in resume().
 *   Re-introduce int_flag and change the function of shced_flag.
 *   While in interrupt(int_flag == 1), resume() will not call resched().
 *   Instead, modify sched_flag to SCHED_DELAYED. After the program goes back
 *   to the ISR, the ISR has to check if sched_flag is delayed. If so call
 *   reshced() at this very moment.
*/

/* Designe change:
 * Date: 19991021.1520
 * Code modified:
 *   Originally, switch_task() is not protected by "disable_int".
 *   It's possible interrupted while kernel switches task. Therefore I
 *   modified resched() to let swtich_task be inside disable/enable_int
 *   section. But, if a task is new created, the way it get CPU is a little
 *   different from the tasks already running. While resched() calls
 *   switch_task(), the last step, longjmp(to,1), the CPU will go to the
 *   starting address of a task function directly and ignores the statement
 *   next to switch_task() in resched(). For a new-born task, this is a
 *   problem that interrupt will not be enabled, but for a already-running
 *   task, that's ok!
 *
 *   I can not find a good place to re-enable interrupt while starting running
 *   a new task, so I rewrite longjmp(), new routine is tlongjmp().
 *   These two routines are totally the same, except that I added an
 *   instruction, CLRC INTM, before RET in tlongjmp(). Now everytime
 *   tlongjmp() is called, interrupt will be enabled.
 */

/* Bug fix:1999-11-2
 * There may be ghost tasks in Taunix.
 * In suspend, it will not check if the task_id task is free, not a existing
 * task. And it will do the suspension and return success. If it happens,
 * suspends a non-existing task, and then resume that task, you will get a
 * ghost task. System may get crash.
 */

#include <setjmp.h>
/*#include <Taunix\task.h>*/
#include "task.h"

TCB	task_table[MAX_TASK];
volatile unsigned int *MMREGS = 0x0000;
int	current_task=-1;
unsigned sched_flag = 1;
unsigned int_flag = 0;
unsigned char prio_catalog=0;
unsigned char prio_seq[4]={0,0,0,0};
const unsigned char rdy_index[16]={0,3,2,2,1,1,1,1,0,0,0,0,0,0,0,0};

void tlongjmp(void *, int);
inline void switch_task(TCB *from, TCB *to)
{
	
	if(setjmp((void*)from) == 0){
		tlongjmp((void *)to,1); 
	}

	/*  In Taunix, use setjmp/longjmp to perform context switch
		for future compatibility and decrease hardware dependence.
		By means of setjmp, it saves TOS, stack pointer, stack
		frame, and AR6, AR7(thease two register is used by C compiler
		as register variables). It's enough for task to give up CPU
		by itself, but not true for interrupt. In interrupt service
		routine, ALL registers should be saved. 
	*/

}
/*
#define switch_task(from,to)	if(setjmp((void*)from)==0)	longjmp((void*)to,1);
*/
int task_create(void (*start_addr)(), void *stack, int prio,char *caption)
{
 unsigned char catalog,seq;
	if(prio >= MAX_TASK)				return -1;/* Invalid priority */
	if(task_table[prio].state != TS_FREE)	return -1;/* Task block is in use */
	/* fill out TCB */
	task_table[prio].ret_addr = start_addr;
	task_table[prio].stack_frame = stack;
	task_table[prio].stack_pointer = stack;
	task_table[prio].caption = caption;
	/* Calculate TCB mask */  
	catalog = prio/(MAX_TASK/4);
	seq = prio % (MAX_TASK/4);
	/* disable interrupt and mask prio_catalog/prio_seq */
	disable_int
		prio_catalog |= (0x08>>catalog);
		prio_seq[catalog] |= (0x80 >>seq);
		task_table[prio].state = TS_READY;	/* Set this task as ready */
	enable_int
	if(current_task != -1)
		resched();
	return 1;
}

int task_kill(int task_id)
{
 int catalog, catalog_mask, seq_mask;
	if(task_id >= MAX_TASK)		return	-1; /* Invalid priority */
	/* Calculate TCB mask */                        
	catalog = task_id/(MAX_TASK/4);
	catalog_mask = ~(0x08>>catalog);
	seq_mask = ~(0x80>>(task_id%(MAX_TASK/4)));
	
	disable_int
		prio_seq[catalog] &= seq_mask;
		if(prio_seq[catalog]==0)
			prio_catalog &= catalog_mask;
		task_table[task_id].state = TS_FREE;
	enable_int
	if(task_id == current_task)	resched();
	return 1;
}

void texit()
{
	task_kill(current_task);
}

int suspend(int task_id)
{
 int catalog, catalog_mask, seq_mask;
	if(task_id >= MAX_TASK)		return	-1; /* Invalid priority */
	if(task_table[task_id].state == TS_FREE)	return -1; /* no such task, it's a ghost task */
	/* Calculate TCB mask */                        
	catalog = task_id/(MAX_TASK/4);
	catalog_mask = ~(0x08>>catalog);
	seq_mask = ~(0x80>>(task_id%(MAX_TASK/4)));
	
	disable_int
		prio_seq[catalog] &= seq_mask;
		if(!prio_seq[catalog])
			prio_catalog &= catalog_mask;
		task_table[task_id].state = TS_SUSPEND;
	enable_int
	if(task_id == current_task)	resched();
	return 1;
}

int resume(int task_id)
{
 int catalog, catalog_mask, seq_mask;
	if(task_id >= MAX_TASK)		return	-1; /* Invalid priority */
	if(task_table[task_id].state != TS_SUSPEND)	return 1;
	/* Calculate TCB mask */                        
	catalog = task_id/(MAX_TASK/4);
	catalog_mask = 0x08>>catalog;
	seq_mask = 0x80>>(task_id%(MAX_TASK/4));
	
	disable_int
		prio_catalog |= catalog_mask;
		prio_seq[catalog] |= seq_mask;
		task_table[task_id].state = TS_READY;
	if(!int_flag) enable_int
	
	if(task_id<current_task){
		if(int_flag){
		/* Now is in interrupt. change sched_flag to tell
		   ISR to re-schedule. */
			delay_sched;
			return 0;
		}
		resched();
	}
	return 1;
}

void resched()
{
 int catalog_temp, seq_temp1, seq_temp2, highest_task;
 int prev_task;
 	if(!sched_flag)	return;
	disable_int
 #ifdef MAX_TASK_32
 	catalog_temp = rdy_index[prio_catalog];
 	seq_temp1 = prio_seq[catalog_temp];
 	seq_temp2 =	seq_temp1 >> 4;
 	highest_task = (seq_temp2?rdy_index[seq_temp2]:(rdy_index[seq_temp1]+4))
						+catalog_temp*8;
 #endif
 #ifdef MAX_TASK_16	
 	catalog_temp = rdy_index[prio_catalog];
 	seq_temp1 = (prio_seq[catalog_temp])>>4;
 	highest_task = rdy_index[seq_temp1]+catalog_temp*4;
 #endif
	prev_task = current_task;
	current_task = highest_task;
 	task_table[current_task].state = TS_CURRENT;	
	
 	if(highest_task != prev_task)
 		switch_task(&task_table[prev_task],&task_table[highest_task]); 
 	enable_int
}

void task_start()
{
 int catalog_temp, seq_temp1, seq_temp2;
 #ifdef MAX_TASK_32
 	catalog_temp = rdy_index[prio_catalog];
 	seq_temp1 = prio_seq[catalog_temp];
 	seq_temp2 =	seq_temp1 >> 4;
 	current_task = (seq_temp2?rdy_index[seq_temp2]:(rdy_index[seq_temp1]+4))
						+catalog_temp*8;
 #endif
 #ifdef MAX_TASK_16	
 	catalog_temp = rdy_index[prio_catalog];
 	seq_temp1 = (prio_seq[catalog_temp])>>4;
 	current_task = rdy_index[seq_temp1]+catalog_temp*4;
 #endif
/* 	int_flag = 0;*/
 	task_table[current_task].state = TS_CURRENT;
 	tlongjmp((void*)&task_table[current_task],1);

}                                                                 


void task_table_init()
{
 int i;
 	for(i=0 ; i<MAX_TASK ; i++)
 		task_table[i].state = TS_FREE;
 	MMREGS = (void*)0x0;
}
