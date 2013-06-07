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

/* Establish date : 1999-8-21
 * Author : Max Yin
 * Organization : Tau Workshop
 * File : at.h
 * Content :
 *   At daemon declaration to perform periodic task
 */

#ifndef AT_H
#define AT_H

typedef struct _atentry{
	unsigned scheduled;
	unsigned period;
	unsigned period_count;
	unsigned (*job)();
}ATENTRY;
#define MAX_PERIODICJOB	10
extern ATENTRY	attab[MAX_PERIODICJOB];

void	atd_init();
int	at(unsigned (*job)(),unsigned peiord);

#define DEFINE_ATTAB_BEGIN	void attab_init(){
#define ATJOB(job_no,job_func,job_period)	attab[job_no].scheduled=1;\
									attab[job_no].period=attab[job_no].period_count=job_period;\
									attab[job_no].job=job_func;
#define DEFINE_ATTAB_END		}
#define atkill(job_no)		attab[jobno].sheduled=0;

#endif
