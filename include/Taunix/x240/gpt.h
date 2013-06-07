/* ----------------------------------------------------------------------- 
*
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
		
/*-----------------------------------------------
Establish date: 1999-5-3	
Author : Max Yin
Orgnization : Tau Workshop

File : GPT.h
Content:
	General Purpose Timer
-----------------------------------------------*/
/*
 Modified date: 1999-9-28
 Changes:
 	1. 243 DSP's timer mode is changed to 4 instead of 6 modes in 240 DSP.
 	   Change mode setting to fit 243's need.
*/

#include <Taunix\x240\f240mmrs.h>
#include <Taunix\cdevsw.h>

#ifndef GPT_H
#define GPT_H

extern CDEVSW gpt;
typedef struct _gptresb{
	int owner_task;
	/*int mode;*/
	void (*callback)(void *param);
}GPTRESB;
/*extern GPTRESB	gptres[];*/

/* GPTimer flag used with gpt_open()*/
#define GPTIMER_1		0
#define GPTIMER_2		1
#define GPTIMER_3		2

/* Definition of GPTimer mode */ /* Don't need anymore */
#define GPTM_BLOCK		0 /* task use block mode to wait for timer expire */
#define GPTM_NONBLOCK	1 /* use non-block mode, timer will call back when expires */
/* Block or non-block mode is determined by the existance of callback function */
void gpt_init();
int gpt_open(int timer_flag,int mode);
int gpt_close();
int gpt_read(void *buf,unsigned int timer_flag);
int gpt_write(const void *buf,unsigned int timer_flag);
int gpt_ioctl(unsigned request, void *argp);
int gpt_setcallback(int timer_flag,void (*callback)(void *param));

/* Definitions of control register options */
/* GPTimer control register: TxCON */
/* Free, Soft Emulation control bits:TxCON.15-14 */
#define FS_STOP_IMMEDIATELY		0x0000
#define FS_STOP_AFTER_PERIOD    	0x4000
#define FS_NOT_AFFECT			0x8000
/* Count mode selection TMODE2-0 @ TxCON.13-11 */
/* This is for 240 DSP */
#define TMODE_HOLD				0x0000
#define TMODE_SINGLE_UP			0x0800
#define TMODE_COUNTI_UP			0x1000
#define TMODE_DIR_UP_DOWN		0x1800
#define TMODE_SINGLE_UP_DOWN		0x2000
#define TMODE_COUNTI_UP_DOWN	0x2800
/**/                  
/* This is for 243 DSP 
#define TMODE_HOLD				0x0000
#define TMODE_COUNTI_UP_DOWN		0x0800
#define TMODE_COUNTI_UP			0x1000
#define TMODE_DIR_UP_DOWN		0x1800
*/
/* Input clock prescaler TPS2-0 @ TxCON.10-8 */
#define TPS_DIV1				0x0000
#define TPS_DIV2				0x0100
#define TPS_DIV4				0x0200
#define TPS_DIV8				0x0300
#define TPS_DIV16				0x0400
#define TPS_DIV32				0x0500
#define TPS_DIV64				0x0600
#define TPS_DIV128				0x0700
/* GP Timer start with GPTimer 1 TSWT1 @ T2/3CON.7 */
#define TSWT1_SELFENABLE		0x0000
#define TSWT1_T1ENABLE			0x0080
/* Timer enable TENBLE1 @ TxCON.6 */
#define GPTIMER_ENABLE			0x0040
#define GPTIMER_DISABLE			0x0000
/* Clock source select TCLKS1-0 @ TxCON.5-4*/
#define TCLKS_INTERNAL			0x0000
#define TCLKS_EXTERNAL			0x0010
#define TCLKS_ROLLOVER_T2		0x0020
#define TCLKS_QEP				0x0030
/* Timer compare register reload condition TCLD1-0 @ TxCON.3-2 */
#define TCLD_ZERO				0x0000
#define TCLD_ZERO_PERIOD			0x0004
#define TCLD_IMMEDIATELY			0x0008
/* Timer compare enable TECMPR1 @ TxCON.1 */
#define TECMPR_ENABLE			0x0002
#define TECMPR_DISABLE			0x0000
/* Period register select SELT1PR1 @ TxCON.0 */
#define SELT1PR				0x0001

/* Definitions of GPTimer control register GPTCON */
/* Bit15 - bit13: Timer status, read only */
/* Bit12-11 T3TOADC : ADC start by event of GP Timer 3 */
/* For 240 */
#define T3TOADC_NOEVENT			0x0000
#define T3TOADC_UFINT			0x0800/**/	/* underflow int. flag to start ADC *//**/
#define T3TOADC_PRINT			0x1000/**/	/* period int. flag to start ADC *//**/
#define T3TOADC_CPINT			0x1800/**/	/* compare int. flag to start ADC */
/* Bit10-9 T2TOADC : ADC start by event of GP Timer 2 */
#define T2TOADC_NOEVENT			0x0000
#define T2TOADC_UFINT			0x0200	/* underflow int. flag to start ADC */
#define T2TOADC_PRINT			0x0400	/* period int. flag to start ADC */
#define T2TOADC_CPINT			0x0600	/* compare int. flag to start ADC */
/* Bit7-8 T1TOADC : ADC start by event of GP Timer 1 */
#define T1TOADC_NOEVENT			0x0000
#define T1TOADC_UFINT			0x0080	/* underflow int. flag to start ADC */
#define T1TOADC_PRINT			0x0100	/* period int. flag to start ADC */
#define T1TOADC_CPINT			0x0180	/* compare int. flag to start ADC */
/* Bit 6 TCOMPOE compare output enable */
#define TCOMPOE_DISABLE			0x0000
#define TCOMPOE_ENABLE			0x0040
/* Bit 5-4 T3PIN polarity of GPTimer 3 */
/* For 240*/
#define T3PIN_FORCELOW			0x0000
#define T3PIN_ACTIVELOW			0x0010
#define T3PIN_ACTIVEHIGH			0x0020
#define T3PIN_FORCEHIGH			0x0030	
/**/
/* Bit 3-2 T2PIN polarity of GPTimer 2 */
#define T2PIN_FORCELOW			0x0000
#define T2PIN_ACTIVELOW			0x0004
#define T2PIN_ACTIVEHIGH		0x0008
#define T2PIN_FORCEHIGH			0x000C
/* Bit 1-0 T1PIN polarity of GPTimer 1 */
#define T1PIN_FORCELOW			0x0000
#define T1PIN_ACTIVELOW			0x0001
#define T1PIN_ACTIVEHIGH			0x0002
#define T1PIN_FORCEHIGH			0x0003		                                        
                                        
#define DEFINE_GTP_HANDLER(gpt_handler)	void gpt_handler(){
#define BEGIN_GPT_CASE(PARAM)		switch(PARAM){ /* switch Peripheral Vector Address Offset */
#define CASE_TPINT1					case 0x0027:
#define CASE_TCINT1					case 0x0028:
#define CASE_TUFINT1				case 0x0029:
#define CASE_TOFINT1				case 0x002A:
#define CASE_TPINT2					case 0x002B:
#define CASE_TCINT2					case 0x002C:
#define CASE_TUFINT2				case 0x002D:
#define CASE_TOFINT2				case 0x002E:
/* For 240 */
#define CASE_TPINT3					case 0x002F:
#define CASE_TCINT3					case 0x0030:
#define CASE_TUFINT3				case 0x0031:
#define CASE_TOFINT3				case 0x0032:
/**/
#define END_CASE					break;
#define END_GPT_CASE				}
#define END_GPT_HANDLER				}
#define TPINT1					 0x0027:
#define TCINT1					 0x0028:
#define TUFINT1				 0x0029:
#define TOFINT1				 0x002A:
#define TPINT2					 0x002B:
#define TCINT2					 0x002C:
#define TUFINT2				 0x002D:
#define TOFINT2				 0x002E:
/* For 240 */
#define TPINT3					 0x002F:
#define TCINT3					 0x0030:
#define TUFINT3				 0x0031:
#define TOFINT3				 0x0032:
/**/
/* GPT ioctl command */
#define GPT_SET				0x0000
#define GPT_READ				0x8000
#define GPT1_START				0x0001
#define GPT1_STOP				0x0002
#define GPT2_START				0x0003
#define GPT2_STOP				0x0004
/* For 240 */
#define GPT3_START				0x0005
#define GPT3_STOP				0x0006
/**/
#endif
