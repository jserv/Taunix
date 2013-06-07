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
Establish date: 1999-4-22	
Author : Max Yin
Orgnization : Tau Workshop

File : Task.h
Content:
	Taunix's task management
		o task_switch();
		o TCB structure
		o manipalation functions
-----------------------------------------------*/

#ifndef TASK_H
#define TASK_H


#define MAX_TASK_32
/*
#define MAX_TASK_16
*/
                            


#ifdef MAX_TASK_16
#define MAX_TASK	16
#endif

#ifdef MAX_TASK_32
#define MAX_TASK	32
#endif
     
/* Type definition of Task Control Block(TCB) */
typedef struct _tcb{
	void (*ret_addr)();		/* TOS */
	void *stack_pointer;	/* AR1 */
	void *stack_frame;		/* AR0 */
	int AR6;	/* reserved for compatibility with setjmp/longjmp */
	int AR7;    /* reserved for compatibility with setjmp/longjmp */
	char *caption;
	char state;
}TCB;

/* Definitions for task state */
#define TS_CURRENT	1
#define TS_READY	2
#define TS_SUSPEND	3
#define TS_PENDING	4
#define TS_INTERRUPT	5
#define TS_FREE		-1

/* 
	Taunix use an fixed-length array to maintain task's ready queue, 
   	and use index to access them. The index of the array is it's pri-
   	ority. Also is task ID.
*/

extern TCB	task_table[];
extern int	current_task;

extern unsigned int sched_flag;
extern unsigned int int_flag;
/* Operations on tasks */
void task_table_init();
int  task_create(void (*start_add)(),void *stack,int prio,char *caption);
int  task_kill(int prio /* task_id */);
void texit();
int  task_read(int task_id,TCB *tcb,int reserved);	/* for compatbility for file system */

int  suspend(int prio /* task_id*/);
int  resume(int prio /* task_id */);

/* Scheduler functions */
void resched();

/* Start task machenism */
void task_start();

/* Define some macros */
#define disable_int		asm("	SETC	INTM");/*{*/
#define enable_int		asm("	CLRC	INTM");/*}*/
#define disable_sched	sched_flag=0;
#define enable_sched	sched_flag=1;resched();
#define delay_sched		sched_flag=2;
#define SCHED_DISABLE	0
#define SCHED_ENABLE	1
#define SCHED_DELAYED	2
#define ISR_ENTRY		int_flag = 1;
#define ISR_EXIT		int_flag = 0;
#define TASK_RUNNING	/*asm("	clrc intm");*/while(1){
#define TASK_WAIT		/*asm("	clrc intm");*/while(1){suspend(current_task);
#define KSAT				}
#define DELAYED_SCHED		if(sched_flag == SCHED_DELAYED){\
								enable_sched;\
								disable_int;\
							}

/* Wait state setting */
void wsgen_24x(unsigned wait_state);
#endif
