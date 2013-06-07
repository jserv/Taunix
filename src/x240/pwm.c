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
 *   Remove all owner_task checking code.
 */

#include <Taunix\task.h>
#include <Taunix\x240\pwm.h>

CDEVSW	pwm;

void pwm_init()
{
	pwm.d_open = pwm_open;
	pwm.d_close = pwm_close;
	pwm.d_read = 0;
	pwm.d_write = pwm_write;
	pwm.d_ioctl = pwm_ioctl;
	pwm.caption = "pwm";
	pwm.owner_task = -1;
}

int pwm_open(int flag,int mode)
{
	/*if(pwm.owner_task != -1)*/ /* pwm has been owned */
	/*	return -1;*/
	pwm.owner_task = current_task;
	
	/* Disable INT2 */
	MMREGS[IMR]	&= 0xFFFD;
	/* Set GPTimer 1 */
	MMREGS[T1CON] = 0xC000|flag;
	
	/* Set compare unit's action control register */
	switch(mode&0xC000){
	 case FPWM:
	 	MMREGS[ACTR] = (mode & 0x3FFF);
	 		break;
	 case SPWM: /* For 240 */
	 	MMREGS[SACTR] = (mode & 0x3FFF);
	 	MMREGS[OCRA] |= 0x0700;
	 		break;
		/**/
	}
	/* Set COMCON */
	MMREGS[COMCON] = 0x0300;/* Use full and simple compare */
							/* but still not enable compare unit */
	return 1;
}

int pwm_close()
{
	/*if(current_task != pwm.owner_task)	return -1;*/
	/* Reset IMR */
	MMREGS[IMR] |= 0x0020;
	
	/* Reset COMCON */
	MMREGS[COMCON] = 0;
	
	pwm.owner_task = -1;
	return 1;
}

int pwm_write(const void *pwmb,unsigned int pwm_selector)
{   
 PWMB	*pwm_block,*userPWMB;
	/*if(current_task != pwm.owner_task)	return -1;*/
	/* Choose PWM */
	switch(pwm_selector){
	 case FPWM:
	 	 DIRECTFPWMBLOCK(pwm_block);
	 	 	break;
	 case SPWM: /* For 240 */
	 	DIRECTSPWMBLOCK(pwm_block);
	 		break;
		/**/
	}
	userPWMB = (PWMB*)pwmb;
	pwm_block->T1 = userPWMB->T1;
	pwm_block->T2 = userPWMB->T2;
	pwm_block->T3 = userPWMB->T3;
	
	return 1;
}

int pwm_ioctl(unsigned request,void *argp)
{
/* 
 unsigned *timebase;
 unsigned *action;
 unsigned *deadband;
*/
 unsigned *uArgp = (unsigned *)argp;
 
	/*if(current_task != pwm.owner_task)	return -1;*/
	switch(request){
	 case PWM_RUN:
	 	MMREGS[COMCON] |= 0x8007;
	 	MMREGS[T1CNT] = 0;
	 	MMREGS[T1CON] |= 0x0040;
	 		break;
	 case PWM_HOLD:
	 	MMREGS[COMCON] &= 0x7FF8;
	 	MMREGS[T1CON] &= 0x00B0;
	 		break;
	 case PWM_TIMEBASE:
	 	/*timebase = (unsigned *)argp;*/
	 	MMREGS[T1PR] = *uArgp;/**timebase;*/
	 		break;
	 case FPWM_ACTION:
	 	/*action = (unsigned *)argp;*/
	 	MMREGS[ACTR] = *uArgp;/**action;*/
	 		break;
	 /* For 240 */
	 case SPWM_ACTION:/* */
	 	/*action = (unsigned *)argp;*/ /**/
	 	MMREGS[SACTR] = *uArgp;/*action;*//**/
	 	MMREGS[OCRA] |= 0x0700;
	 		break;
		/* */
	 case PWM_DEADBAND:
	 	/*deadband = (unsigned *)argp;*/
	 	/**deadband <<= 8;*/
	 	*uArgp <<= 8;
	 	MMREGS[DBTCON] = 0;
	 	MMREGS[DBTCON] = *uArgp;/**deadband;*/
	 	MMREGS[DBTCON] |= 0x00E0;
	 		break;
	 case PWM_DBT1_ENABLE:
	 	MMREGS[DBTCON] |= 0x0020;
	 		break;
	 case PWM_DBT1_DISABLE:
	 	MMREGS[DBTCON] &= 0xFFC0;
	 		break;
	 case PWM_DBT2_ENABLE:
	 	MMREGS[DBTCON] |= 0x0040;
	 		break;
	 case PWM_DBT2_DISABLE:    
	 	MMREGS[DBTCON] &= 0xFFA0;
	 		break;
	 case PWM_DBT3_ENABLE:
	 	MMREGS[DBTCON] |= 0x0080;
	 		break;
	 case PWM_DBT3_DISABLE:
	 	MMREGS[DBTCON] &= 0x0060;
	 		break;
	}
	return 1;
}
