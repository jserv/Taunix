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

/* Modified date: 19990819.0155
 * Changes:
 *   Comment out the resume task statement in the ADC interrupt.
 */

#include <Taunix\x240\f240mmrs.h>
#include <Taunix\task.h>
#include <Taunix\x240\adc.h>

void (*TNX_XINT1)() = 0;
void (*TNX_XINT2)() = 0;
void (*TNX_XINT3)() = 0;

void c_int6()
{unsigned param[2];

	ISR_ENTRY
/* For 240 */
	switch(MMREGS[SYSIVR]){

/* For 243 
	switch(MMREGS[PIVR]){
*/
	 case 0x0004:
		if(adc.read_callback){ /* call-back function is set */
			param[0] = MMREGS[ADCFIFO1]>>6;
			param[1] = MMREGS[ADCFIFO2]>>6;
			adc.read_callback(param);
		}
/*		else*/
			/*resume(adc.owner_task);*/ /* else task should be block */
									/* resume it */
	/*
		The operation speed of ADC, well maybe will be slower than CPU core.
	     But just a few instruction cycle. Therefore, blocked-I/O will be time-
	     comsuming
	*/
		break;
	 case 0x0001: /* XINT1 */
	 	if(TNX_XINT1)	TNX_XINT1();
	 	break;
	 case 0x0011: /* XINT2 */
	 	if(TNX_XINT2)	TNX_XINT2();
	 	break;
	 case 0x001F: /* XINT3*/        
	 	if(TNX_XINT3)	TNX_XINT3();
	 	break;
	}
	ISR_EXIT

	if(sched_flag == SCHED_DELAYED){
		enable_sched;
		disable_int;
	}
}
