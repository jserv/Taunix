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

/* Modified date : 19990819.0155
 * Changes:
 *   Comment out the resume task statement in the timer interrupt.
 */

/* Modified date : 19990928.2013
 * Changes:
 *   1. Change ISR format to satisfy version 0.7.4.
 *   2. Since 243's event manager has been removed EVIVRA,B,C, the peripheral
 *      interrupt vectors are reported by PIVR and EVIFRA,B,C. Therefore the
 *      switch-case structure in ISR must change to fit 243's need.
 */

#include <Taunix\x240\f240mmrs.h>
#include <Taunix\task.h>
#include <Taunix\x240\gpt.h>

extern GPTRESB	gptres[];

void (*TNX_PDPINT)() = 0;
void (*TNX_CMP1INT)() = 0;
void (*TNX_CMP2INT)() = 0;
void (*TNX_CMP3INT)() = 0;
void (*TNX_SCMP1INT)() = 0;
void (*TNX_SCMP2INT)() = 0;
void (*TNX_SCMP3INT)() = 0;

void c_int2()
{unsigned param = MMREGS[EVIVRA]; /* 240 DSP */
 /*unsigned param = MMREGS[PIVR];*/ /* for 243 DSP */
	ISR_ENTRY
	BEGIN_EVM_CASE(param)
		CASE_TPINT1
		CASE_TCINT1
		CASE_TUFINT1
		CASE_TOFINT1
			if(gptres[GPTIMER_1].callback) /* NON-BLOCK mode */
	 			gptres[GPTIMER_1].callback(&param);
	 		/*else if(gptres[GPTIMER_1].owner_task != -1)*/ /* BLOCK mode */
	 			/*resume(gptres[GPTIMER_1].owner_task);*/
			/*
				It may not be proper to resume task here. Operation mode
				is always set as asynchronous mode. If callback is not been
				set, then timer will do nothing.
			*/
		END_CASE
	END_EVM_CASE
	MMREGS[EVIFRA] &= 0x000F;
	ISR_EXIT        
	
	if(sched_flag == SCHED_DELAYED){
		enable_sched;
		disable_int;
	}
}

void c_int3()
{unsigned param = MMREGS[EVIVRB]; /* for 240 DSP */
 /*unsigned param = MMREGS[PIVR];*/ /* for 243 DSP */
	ISR_ENTRY
	BEGIN_EVM_CASE(param)
		CASE_TPINT2
		CASE_TCINT2
		CASE_TUFINT2
		CASE_TOFINT2
			if(gptres[GPTIMER_2].callback) /* NON-BLOCK mode */
	 			gptres[GPTIMER_2].callback(&param);
	 		/*else if(gptres[GPTIMER_2].owner_task != -1)*/ /* BLOCK mode */
	 			/*resume(gptres[GPTIMER_2].owner_task);*/
			/*
				It may not be proper to resume task here. Operation mode
				is always set as asynchronous mode. If callback is not been
				set, then timer will do nothing.
			*/
		END_CASE
    /*	On 243, there is no GPTimer 3. */
		CASE_TPINT3
		CASE_TCINT3
		CASE_TUFINT3
		CASE_TOFINT3
			if(gptres[GPTIMER_3].callback)*/ /* NON-BLOCK mode */
	 			gptres[GPTIMER_3].callback(&param);
		 	/*else if(gptres[GPTIMER_3].owner_task != -1)*/ /* BLOCK mode */
		 		/*resume(gptres[GPTIMER_3].owner_task);*/
			/*
				It may not be proper to resume task here. Operation mode
				is always set as asynchronous mode. If callback is not been
				set, then timer will do nothing.
			*/
		END_CASE
	END_EVM_CASE
	MMREGS[EVIFRB] = 0x0000;
	ISR_EXIT
	
	if(sched_flag == SCHED_DELAYED){
		enable_sched;
		disable_int;
	}
}	
