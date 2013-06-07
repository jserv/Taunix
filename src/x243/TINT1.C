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

#include <Taunix\task.h>
#include <Taunix\x243\wdrti.h>
#include <Taunix\x243\f243mmrs.h>
#include <Taunix\sleep.h>
#include <Taunix\priList.h>
#include <Taunix\at.h>

void wakeup();
extern priNode 	taskSleepNodes[];
extern priList		sleepq;

inline void atd(void *param);
#include <ioports.h>
void c_int1()
{
	/* For 243 DSP */
	if(MMREGS[PIVR] != 0x0001)	return;
	/**/
	
	ISR_ENTRY
 	
 	wdrt_isr(); /* Deal with watch-dog timer */
	
	/* Decrease first task's diff in SLEEPQ */
	if(sleepq.contents && taskSleepNodes[sleepq.head].prio)
		taskSleepNodes[sleepq.head].prio--;
 	wakeup();
 	
 	/*atd(0);*/ /* If use AT module, remove comment to enable atd */
 	
 	/* For 243 DSP */
 	MMREGS[XINT1CR] = 0x8005;
 	/**/               
 	
 	ISR_EXIT
 	if(sched_flag == SCHED_DELAYED){
 		enable_sched;
 		disable_int;
 	}
}
 
/* For 243 DSP :
 Int_1 can handle PDPINT, ADCINT, XINT1, XINT2, SPIINT, RXINT and
 TXINT. But here, I only use int_1 to act as timer interrupt and 
 accept XINT1 only.
*/
