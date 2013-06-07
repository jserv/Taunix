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
	Modified date: 19990824.1437
	Code Modified:
		The original sem does not work very good, therefore I re-write
		the whole module. By using priList, the structure of sem
		should be more clear.
*/

/*#include <Taunix\task.h>
#include <Taunix\sem.h>
*/
/*#include <stdio.h>*/
#include "task.h"
#include "sem.h"


SEM	sem_table[MAX_SEM];

void sem_table_init()
{
 int i,j;
 	for(i=0 ; i<MAX_SEM ; i++){
 		sem_table[i].sem_state = SS_FREE;
        	for(j=0 ; j<MAX_PENDING ; j++)
			sem_table[i].pending_table[j].data = PS_FREE;
		List_init(&(sem_table[i].pending_list),sem_table[i].pending_table,MAX_PENDING);
	}
}                                

/* Get a free semaphore handle */
int semget()
{
 int free_sem;
 	disable_sched
 		for(free_sem=0 ; free_sem<MAX_SEM ; free_sem++)
 				if(sem_table[free_sem].sem_state == SS_FREE){
 				    sem_table[free_sem].sem_state = SS_INUSE;
 				    sem_table[free_sem].sem_value = 1;
					sem_table[free_sem].owner = -1;
 					break;
 				}
 	enable_sched
 	if(free_sem == MAX_SEM)	return	-1;
 	else					return	free_sem;
}

/* Release semaphore */
int sem_release(int sem_handle)
{
	if(sem_handle>= MAX_SEM /* Error handle */
			|| sem_table[sem_handle].sem_state == SS_FREE 
							/* semaphore is not allocated */
			|| sem_table[sem_handle].sem_value < 1 )
					/* Still has tasks in pending */
		return -1;                         
	disable_sched
		sem_table[sem_handle].sem_state = SS_FREE;
	enable_sched
}

/* Return how many pending tasks in semaphore */
int sem_pendings(int sem_handle)
{
	if(sem_handle>= MAX_SEM ||
			sem_table[sem_handle].sem_state == SS_FREE)
		return -1;
	return	(int)(sem_table[sem_handle].pending_list.contents +1);
}

/* Return owner */
int sem_owner(int sem_handle)
{
	if(sem_handle>= MAX_SEM ||
			sem_table[sem_handle].sem_state == SS_FREE)
		return -1;
	return sem_table[sem_handle].owner;
}

/* Pending semaphore */
int pend(int sem_handle,int sem_mode)
{
 int	free_node=0;
	if(sem_handle>= MAX_SEM /* Error handle */
			|| sem_table[sem_handle].sem_state == SS_FREE
					/* semaphore is not allocated */
			|| sem_table[sem_handle].pending_list.contents == MAX_PENDING)
					/* pending table is fullfilled */
		return -1;
	sem_table[sem_handle].sem_value -- ;
	disable_sched
		if(sem_table[sem_handle].sem_value < 0){	/* Semaphore has owner */
			/* find a free pending node */
			while(sem_table[sem_handle].pending_table[free_node].data != PS_FREE)
				free_node ++;
			sem_table[sem_handle].pending_table[free_node].prio = current_task;
			sem_table[sem_handle].pending_table[free_node].data = PS_PENDING;
			priList_insert(&(sem_table[sem_handle].pending_list),free_node);
		}
		else
			sem_table[sem_handle].owner = current_task;			
	enable_sched
	if(sem_table[sem_handle].owner == current_task){
/*		printf("Current task: %i owns semaphore: %i.\n",
			current_task,sem_handle);*/
		return 1;
	}
	switch(sem_mode){
	 case SM_NONBLOCK:
	 	return 0;
	 default:
	 case SM_BLOCK:
	 	suspend(current_task);
		/*printf("Current task %i has been suspended.\n",current_task);*/
	 	/* Return from suspend, but not by post() */
	 	if(sem_table[sem_handle].owner != current_task)	return 0;
	 	/* That means other task not holding semaphore resumes this task */
	 		break;
	}
	return 1;
}

/* Post semaphore */
int post(int sem_handle)
{int i;
 priList	*list = &(sem_table[sem_handle].pending_list);
	if(sem_table[sem_handle].sem_state == SS_FREE 
		|| sem_handle >= MAX_SEM )	return -1;
	if( current_task == sem_table[sem_handle].owner){ /* Owner releases semaphore */
/*		printf("Owner task: %i post semaphore: %i\n",
			current_task, sem_handle);*/
		if((++sem_table[sem_handle].sem_value)==1)	return 1;
		
		disable_sched
			/* resume next pending task */
			list->current = list->head;
			resume(list->nodePool[list->head].prio);
/*			printf("Resume task: %i, change owner to %i.\n",list->nodePool[list->head].prio,list->nodePool[list->head].prio);*/
			sem_table[sem_handle].owner = list->head;
			List_delete(list);
		enable_sched
		return 1;
	}
	else{	/* Not owner, so give up semaphore */
		disable_sched
			list->current = list->head;
			do{
				if(list->nodePool[list->current].prio == current_task){
					List_delete(list);
					break;
				}
				list->current = list->nodePool[list->current].next;
			}while(list->current != -1);
		enable_sched
		return 1;
	}
	return -1;
}


/* Test code begins :
int main(int argc, char **argv)
{
 int handle;
 priList *list;
	sem_table_init();
	current_task = 5;
	handle = semget();
	pend(handle,SM_BLOCK);
	current_task = 6;
	pend(handle,SM_BLOCK);
	current_task = 1;
	pend(handle,SM_BLOCK);
	current_task = 4;
	pend(handle,SM_BLOCK);
	current_task = 20;
	pend(handle,SM_BLOCK);
	current_task = 17;
	pend(handle,SM_BLOCK);
	list =  &(sem_table[handle].pending_list);
	list->current = list->head;
	do{
		printf("Pending task: %i\n",list->nodePool[list->current].prio);
		list->current = list->nodePool[list->current].next;
	}while(list->current != -1);
	printf("Press ENTER to continue...\n");
	getchar();
	current_task = 5;
	post(handle);
	current_task = 6;
	post(handle);
	current_task = 1;
	post(handle);
	list->current = list->head;
	do{
		printf("Pending task: %i\n",list->nodePool[list->current].prio);
		list->current = list->nodePool[list->current].next;
	}while(list->current != -1);


}
 Test code ends. */
