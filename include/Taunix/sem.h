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
		
/* Establish date: 1999-4-23
 * Author : Max Yin
 * Orgnization : Tau Workshop
 * File : Sem.h
 * Content:
	Semaphore machanism
		o semget()
		o pend()
		o post()
		o sem_pendings()
		o sem_owner()
 */

/* Modified date : 19990824.1437
 * Changes :
 *   The original sem does not work very good, there I re-write the whole
 *   module. By using priList, the structure of sem should be more clear.
 */

#ifndef SEM_H
#define SEM_H

#define MAX_SEM		10 /* Define maximum of semaphores */
#define MAX_PENDING	5  /* Define maximum of pending tasks */

/* Declaration of PENDING structure */
#include "priList.h"

/* Define pending state */
#define PS_FREE	0
#define PS_PENDING	2
#define PS_GIVEUP	3

/* Declaration of SEM */
typedef struct _sem{
	int sem_state;
	int sem_value;
	int owner;
	priList	pending_list;
	priNode	pending_table[MAX_PENDING];
}SEM;

extern SEM	sem_table[];
/* Define semaphore state */
#define SS_FREE		0
#define SS_INUSE	1

/* Declare semaphore actions */
void sem_table_init();
int  semget();
int  sem_release(int sem_handle);
int  pend(int sem_handle,int sem_mode);
int post(int sem_handle);
int  sem_pendings(int sem_handle);
int  sem_owner(int sem_handle);

int read_sem(int sem_handle,SEM *sem,int reserved);

                 
/* Define semaphore mode */
#define SM_BLOCK	1
#define SM_NONBLOCK	0                 
                 
#endif
