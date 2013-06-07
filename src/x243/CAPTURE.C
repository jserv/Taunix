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
 Project codename: fastTaunix
 Modified date : 1999-7-8
 Changes:
 	Remove the owner_task checking.
 	

*/
/* Modified date: 1999-10-7
	Changes:
		1. Change ISR format to satisfy version 0.7.4.
		2. Since 243's event manager has been removed EVIVRA,B,C,
		   the peripheral interrupt vectors are reported by PIVR
		   and EVIFRA,B,C. Therefore the switch-case structure 
		   in ISR must change to fit 243's need.
*/

#include <Taunix\x243\capture.h>
#include <Taunix\task.h>

CDEVSW	capture;
struct cap_unit	cap_tab[4];

void cap_init()
{ 
 int i;
 	capture.caption = "capture";
 	capture.owner_task = -1;
 	capture.d_open = cap_open;
 	capture.d_close = cap_close;
 	capture.d_read = cap_read;
 	capture.d_write = 0;
 	capture.d_ioctl = cap_ioctl;
 	
 	MMREGS[CAPCON] = 0;
	for(i=0 ; i<4 ; i++){
		cap_tab[i].owner_task = -1;
		cap_tab[i].callback = 0;
	}
	MMREGS[CAPCON] = 0x8000;
}

int cap_open(int cap_flag,int mode)
{
/*	if(cap_tab[cap_flag].owner_task != -1)
		return -1;*/		/* unit has been owned */
	MMREGS[CAPCON] |= mode;
	if(cap_flag == CAPTURE1 || cap_flag == CAPTURE2)
		MMREGS[CAPCON] |= 0x2000; /* CAPQEPN disable, enable unit1&2 */
	else if(cap_flag == CAPTURE3)
		MMREGS[CAPCON] |= 0x1000; /* Enable unit 3 */
	/* For 240 
	else if(cap_flag == CAPTURE4)
		MMREGS[CAPCON] |= 0x0800;*/ /* Enable unit 4 */
	cap_tab[cap_flag].owner_task = current_task;
	/* For 240
	MMREGS[OCRB] |= (0x10 << cap_flag);
	*/
	/* For 243 */
	MMREGS[OCRA] |= (0x08 << cap_flag);
	/**/
	MMREGS[EVIMRC] |= (0x01<<cap_flag);
	return cap_flag;
}
int cap_close()
{
 int i;
 	for(i=CAPTURE1 ; i<=CAPTURE4 ; i++)
 		if(cap_tab[i].owner_task == current_task){
 			cap_tab[i].owner_task = -1;
 			/* For 240 
 			MMREGS[OCRB] &= (0xEF<<i);
 			*/
 			/* For 243 */
 			MMREGS[OCRA] &= (0xF7 << i);
 			/**/                        
 			MMREGS[EVIMRC] &= (0xFE << i);
 		}
}

int cap_read(void *buf,unsigned cap_unit)
{
 /*unsigned *cap_fifo = (unsigned*)buf;*/
 	/*if(cap_tab[cap_unit].owner_task != current_task)
 		return -1;*/	/* current task desn't own this unit */
 	if(MMREGS[CAPFIFO] & (0x0300 << (cap_unit<<1) )){ /* FIFO not empty */
 		/*cap_fifo[0]*/
 		*((unsigned*)buf) = MMREGS[CAP1FIFO+cap_unit];
 		return 1;
 	}
 	else	return 0;
 	/* If task ever set callback function, when capture unit interrupts 
 	   , ISR will call callback function, and will passes fifo status and 
 	   fifo content.
 	*/
}

int cap_ioctl(unsigned request, void *argp)
{
	switch(request&0xFF00){
	 case CAP_RESET:
	 	MMREGS[CAPCON] = 0;
	 	MMREGS[CAPCON] = 0x8000;
	 		break;
	 case CAP_QEPENABLE:
	 	/*if((cap_tab[CAPTURE1].owner_task != current_task) ||
	 		(cap_tab[CAPTURE2].owner_task != current_task))
	 		return -1;	*//* current task doesn't own CAP1 or CAP2 */
	 	MMREGS[CAPCON] |= 0x6000;
	 		break;
	 case CAP_QEPDISABLE:
	 	/*if((cap_tab[CAPTURE1].owner_task != current_task) ||
	 		(cap_tab[CAPTURE2].owner_task != current_task))
	 		return -1;*/	/* current task doesn't own CAP1 or CAP2 */
	 	MMREGS[CAPCON] &= 0x9FFF; /* Set CAPQEPN to 0 */
	 	MMREGS[CAPCON] |= 0x2000; /* Set CAPQEPN to 01b */
	 		break;
	 case CAP_CAP1EDGE:
	 	/*if(cap_tab[CAPTURE1].owner_task != current_task)	return -1;*/
	 	MMREGS[CAPCON] &= 0xFF3F;
	 	MMREGS[CAPCON] |= ((request&0x0003)<<6);
	 		break;
	 case CAP_CAP2EDGE:
	 	/*if(cap_tab[CAPTURE2].owner_task != current_task)	return -1;  */
	 	MMREGS[CAPCON] &= 0xFFCF;
	 	MMREGS[CAPCON] |= ((request&0x0003)<<4);
	 		break;
	 case CAP_CAP3EDGE:
	 	/*if(cap_tab[CAPTURE3].owner_task != current_task)	return -1;*/
	 	MMREGS[CAPCON] &= 0xFFF3;
	 	MMREGS[CAPCON] |= ((request&0x0003)<<2);
	 		break;
	/* For 240
	 case CAP_CAP4EDGE:*/
	 	/*if(cap_tab[CAPTURE4].owner_task != current_task)	return -1;  */
		/*
	 	MMREGS[CAPCON] &= 0xFFFC;
	 	MMREGS[CAPCON] |= (request&0x0003);
	 		break;
		*/
	}
	return 1;
}

int cap_setcallback(unsigned cap_unit,void (*callback)(unsigned *))
{
	/*if(cap_tab[cap_unit].owner_task != current_task)	return -1;*/
		cap_tab[cap_unit].callback = callback;
}

void c_int4()
{unsigned fifo[2];
 /* For 240
 unsigned which_cap = MMREGS[EVIVRC];
 */
 /* For 243 */
 unsigned which_cap = MMREGS[PIVR]-0x33;
 /**/
 	ISR_ENTRY
	if(cap_tab[which_cap].callback){ /* If callback has been set */
		fifo[0] = MMREGS[CAPFIFO]>>8; /* FIFO status */
		fifo[1] = MMREGS[which_cap+0x7423]; /* Top content of CAP?FIFO */
		cap_tab[which_cap].callback(fifo); /* Pass fifo to callback */
	}
	MMREGS[EVIFRC] |= (0x0001 << which_cap); /* Clear capture flag in EVM */
	ISR_EXIT
	if(sched_flag == SCHED_DELAYED){
		enable_sched
		disable_int
	}
	
}

