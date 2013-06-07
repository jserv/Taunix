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

/* Project codename: fastTaunix
 * Modified date : 1999-7-7
 * Changes:
 *   1. Remove all owner_task checking
 *   2. Modify gpt_read()/gpt_write()
 */

/* Modified date : 1999-8-3
 * Changes:
 *   1. In gpt_open(), driver will automatically set OCRA register to fit
 *      timer's need
 */

/* Modified date: 1999-9-28
 * Changes:
 *   1. Change "gptimer" to "gpt".
 *   2. In gpt_init(), I forgot to set call-back function to NULL.
 *       Now this has been corrected.
 */

#include <Taunix\task.h>
#include <Taunix\x240\gpt.h>

CDEVSW	gpt;
GPTRESB	gptres[3];

void gpt_init()
{
	gpt.d_open = gpt_open;
	gpt.d_close = gpt_close;
	gpt.d_read = gpt_read;
	gpt.d_write = gpt_write;
	gpt.d_ioctl = gpt_ioctl;
	gptres[0].owner_task = -1;
	gptres[0].callback = 0;
	gptres[1].owner_task = -1;
	gptres[1].callback = 0;
	gptres[2].owner_task = -1;
	gptres[2].callback = 0;
}

int gpt_open(int timer_flag, int mode)
{
	/*if(timer_flag<GPTIMER_1 || timer_flag>GPTIMER_3)*/
		/*return -1;*/	/* Error GPTIMER flag */
	/*if(gptres[timer_flag].owner_task!=-1)*/
		/*return -1;*/ /* timer has been opened */
	/*disable_sched*/
	gptres[timer_flag].owner_task = current_task;
	/*enable_sched*/
	MMREGS[OCRA] |= 0x3800;
	return timer_flag;
}
	
int gpt_close()
{
 int i;          
 	for(i=GPTIMER_1 ; i<=GPTIMER_3 ; i++)
 		if(gptres[i].owner_task == current_task){
 			gptres[i].owner_task = -1;
 			break;
 		}
 	return 1;
}

int gpt_read(void *buf, unsigned reg_selector)
{
	*((unsigned*)buf) = MMREGS[reg_selector];
	return 1;
}

int gpt_write(const void *buf, unsigned reg_selector)
{
	MMREGS[reg_selector] = *((unsigned *)buf);
	return 1;
}
/*int gpt_read(void *buf , unsigned int timer_reg*/ /*timer_flag*//*)

{
 unsigned *gptreg = (unsigned *)buf;   */
 	/*switch(timer_flag){
 	 case GPTIMER_1:
 	 case T1CNT:
 		gptreg[0] = MMREGS[T1CNT];
 		break;
  	 case GPTIMER_2:
 	 case T2CNT:
 		gptreg[0] = MMREGS[T2CNT];
 		break;
 	 case GPTIMER_3:
 	 case T3CNT:
 		gptreg[0] = MMREGS[T3CNT];
 		break;
 	 case GPTCON:
 		gptreg[0] = MMREGS[GPTCON];
 		break;
    }*/
    /*
    *gptreg = MMREGS[timer_reg];
 	return 1;
}

int gpt_write(void *buf, unsigned int timer_reg*//*timer_flag*//*)

{
 unsigned *gptreg = (unsigned *)buf;*/
 /*if(timer_flag>GPTCON)
 	timer_flag = (timer_flag-T1CNT)>>2;
 if(timer_flag<GPTIMER_1 || timer_flag>GPTIMER_3)	return -1;
 if(gptres[timer_flag].owner_task!=current_task)
	return -1; */
	/* current task doesn't own this timer */
/*
 	switch(timer_flag){
 	 case GPTIMER_1:
  		MMREGS[T1CNT] = gptreg[0];
 		break;
  	 case GPTIMER_2:
  		MMREGS[T2CNT] = gptreg[0];
 		break;
 	 case GPTIMER_3:
  		MMREGS[T3CNT] = gptreg[0];
 		break;
 	}
 	MMREGS[timer_reg] = *gptreg;
 	return 1;
}*/

int gpt_setcallback(int timer_flag, void (*callback)(void *param))
{
	/*if(timer_flag<GPTIMER_1 || timer_flag>GPTIMER_3)	return -1;
	if(gptres[timer_flag].owner_task!=current_task)
		return -1;*/ /* current task doesn't own this timer */
	gptres[timer_flag].callback = callback;
	return 1;
}

int gpt_ioctl(unsigned  request,void *argp)
{unsigned int *arg = (unsigned int *)argp;
/*
 int timer_flag = ((request&0x7FFF)-T1CNT);
 if(timer_flag >= 0){*/ /* timer_flag = -1 means GPTCON */
/* 	timer_flag >>= 2;
 	if(timer_flag<GPTIMER_1 || timer_flag>GPTIMER_3)	return -1;
 	if(gptres[timer_flag].owner_task != current_task)	return -1;
 }*/
	 	switch(request){
	 	 case GPT1_START:
/*			if(gptres[0].owner_task != current_task)	return -1;*/
			MMREGS[EVIMRA] |= 0x0780;	/* set event manager */
			MMREGS[OCRA] |= 0x0800;		/* set I/O pin : T1PWM/T1CMP */
			MMREGS[T1CON] |= GPTIMER_ENABLE;/* 0x0040 */
			break;
	 	 case GPT1_STOP:
			/*if(gptres[0].owner_task != current_task)	return -1;*/
			MMREGS[T1CON] &= 0xFFBF; /* ~0x0040 = 0xFFBF */
			MMREGS[EVIMRA] &= 0xF87F;
			MMREGS[OCRA] &= 0xF7FF;
			break;
	 	 case GPT2_START:
			/*if(gptres[1].owner_task != current_task)	return -1;*/
			MMREGS[EVIMRB] |= 0x000F;
			MMREGS[OCRA] |= 0x1000;
			MMREGS[T2CON] |= GPTIMER_ENABLE;/* 0x0040 */
			break;
	 	 case GPT2_STOP:
			/*if(gptres[1].owner_task != current_task)	return -1;*/
			MMREGS[T2CON] &= 0xFFBF; /* ~0x0040 = 0xFFBF */
			MMREGS[EVIMRB] &= 0xFFF0;
			MMREGS[OCRA] &= 0xEFFF;
			break;
		/* For 240 */
	 	 case GPT3_START: /**/
			/*if(gptres[2].owner_task != current_task)	return -1;*//**/
			MMREGS[EVIMRB] |= 0x00F0;
			MMREGS[OCRA] |= 0x2000;
			MMREGS[T3CON] |= GPTIMER_ENABLE;*//* 0x0040 *//**/
			break;
	 	 case GPT3_STOP:/**/
			/*if(gptres[2].owner_task != current_task)	return -1;*//**/
			MMREGS[T3CON] &= 0xFFBF; /**//* ~0x0040 = 0xFFBF */ /* */
			MMREGS[EVIMRB] &= 0xFF0F;
			MMREGS[OCRA] |= 0xDFFF;
			break; /**/
	 	}
 		return 1;
}
