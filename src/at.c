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

/* Design note:
	Atd is designed that can be called from general purpose timer's call-back
	facility or any external timing trigger source. For example, on x240, one
	can assign atd(void *param) to any timer's call-back, or just call atd(0)
	in INT1 (real-time clock interrupt). It's up to your preference.

*/

#include <Taunix\at.h>

ATENTRY	attab[MAX_PERIODICJOB];

void atd_init()
{
 int i;
	/* Reset at table */
	for(i=0 ; i<MAX_PERIODICJOB ; i++)
		attab[i].scheduled = 0;
	/* Set the default at table */
	attab_init();
}

unsigned atd_flag=0;
inline void atd(void *param) 
/* Void *param argument is for compitibility with call-back facility */
{
 int i;
	if(atd_flag)	return;
	atd_flag = 1;
	
	for(i=0 ; i<MAX_PERIODICJOB ; i++){
		if(attab[i].scheduled){
			if((--attab[i].period_count)==0){
				attab[i].scheduled = attab[i].job();
				attab[i].period_count = attab[i].period;
				/*
					If job() return 0, then this job will be removed automatically.
				*/
			}
		}
	}
	atd_flag = 0;
}

int at(unsigned (*job)(),unsigned period)
{
 int i;
	/* Search for free entry to register job */
	for(i=0 ; i<MAX_PERIODICJOB ; i++){ /* Linear search */
		if(attab[i].scheduled==0){
			disble_sched
			attab[i].scheduled = 1;
			attab[i].period_count = attab[i].period = period;
			attab[i].job = job;
			enable_sched
			return i;
		}	
	}
	return -1;
}

/* Test Code begins:
#include <stdio.h>
unsigned print1();
unsigned print2();

DEFINE_ATTAB_BEGIN
	ATJOB(2,print1,10)
	ATJOB(1,print2,4)
DEFINE_ATTAB_END

unsigned print1()
{
	printf("	This is in print1 job.\n");
	return 1;
}

unsigned print2()
{
	printf("	This is in print2 job.\n");
	return 1;
}


int main(int argc, char **argv)
{
 int i;
	atd_init(100);
	for(i=0 ; i<100 ; i++){
		printf("In cycle %i:\n",i+1);
		atd((void*)0);
	}
}



 Test Code ends */
