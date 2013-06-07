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
		
/* Establish date : 1999-6-8
 * Author : Max Yin
 * Orgnization : Tau Workshop
 * File : PWM.h
 * Content :
 *   PWM Modual
 */

#include <Taunix\x243\f243mmrs.h>
#include <Taunix\cdevsw.h>

#ifndef PWM_H
#define PWM_H
extern CDEVSW	pwm;

typedef struct _pwmb{
 unsigned T1;
 unsigned T2;
 unsigned T3;
} PWMB;

void pwm_init();
int pwm_open(int flag,int mode);
int pwm_close();
int pwm_write(const void *pwmb,unsigned int pwm_selectot);
int pwm_ioctl(unsigned int request,void *argq);
                              
/* Definitions for pwm_open()'s flag */
#define PWM_SYMMETRIC	0x2800
#define PWM_ASYMMETRIC	0x1000
/* Definitions for pwm_open()'s mode */
#define FPWM			 0x8000   /* PWM selector */
/* For 240
#define SPWM			 0x4000   *//* PWM selector */

#define PWM1_LOW   		 0x0000
#define PWM1_HIGH	 	 0x0003
#define PWM1_ACTIVELOW   0x0001
#define PWM1_ACTIVEHIGH	 0x0002
#define PWM2_LOW   		 0x0000
#define PWM2_HIGH	 	 0x0009
#define PWM2_ACTIVELOW   0x0004
#define PWM2_ACTIVEHIGH	 0x0008
#define PWM3_LOW   		 0x0000
#define PWM3_HIGH	 	 0x0030
#define PWM3_ACTIVELOW   0x0010
#define PWM3_ACTIVEHIGH	 0x0020
#define PWM4_LOW   		 0x0000
#define PWM4_HIGH	 	 0x0090
#define PWM4_ACTIVELOW   0x0040
#define PWM4_ACTIVEHIGH	 0x0080
#define PWM5_LOW   		 0x0000
#define PWM5_HIGH	 	 0x0300
#define PWM5_ACTIVELOW   0x0100
#define PWM5_ACTIVEHIGH	 0x0200
#define PWM6_LOW   		 0x0000
#define PWM6_HIGH	 	 0x0900
#define PWM6_ACTIVELOW   0x0400
#define PWM6_ACTIVEHIGH	 0x0800
/* For 240. x243 doesn't have simple compare unit 
#define SPWM1_LOW  		 0x0000
#define SPWM1_HIGH	 	 0x0003
#define SPWM1_ACTIVELOW  0x0001
#define SPWM1_ACTIVEHIGH 0x0002
#define SPWM2_LOW  		 0x0000
#define SPWM2_HIGH	 	 0x0009
#define SPWM2_ACTIVELOW  0x0004
#define SPWM2_ACTIVEHIGH 0x0008
#define SPWM3_LOW  		 0x0000
#define SPWM3_HIGH	 	 0x0030
#define SPWM3_ACTIVELOW  0x0010
#define SPWM3_ACTIVEHIGH 0x0020
*/

/* Definitions for pwm_ioctl() */
#define PWM_RUN				1
#define PWM_HOLD			2
#define PWM_TIMEBASE		3
#define FPWM_ACTION			4
/* For 240
#define SPWM_ACTION			5
*/
#define PWM_DEADBAND		6
#define PWM_DBT1_ENABLE		7
#define PWM_DBT1_DISABLE	8
#define PWM_DBT2_ENABLE		9
#define PWM_DBT2_DISABLE	10
#define PWM_DBT3_ENABLE		11
#define PWM_DBT3_DISABLE	12
/* Macros */
#define DIRECTFPWMBLOCK(FPWMBLOCK)	FPWMBLOCK=(PWMB*)0x7417
/* For 240
#define DIRECTSPWMBLOCK(SPWMBLOCK)	SPWMBLOCK=(PWMB*)0x741A
*/
#endif
