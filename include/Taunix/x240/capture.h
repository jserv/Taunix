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
|    TI DSP Realtime kernel project: Taunix     |
|         Taunix : TAUnix's UNIX-like           |
|                                               |
\*---------------------------------------------*/
		
/*-----------------------------------------------
Establish date: 1999-5-24
Author : Max Yin
Orgnization : Tau Workshop

File : CAPTURE.h
Content:
	Capture Unit Device
	
-----------------------------------------------*/

#include <Taunix\x240\F240mmrs.h>
#include <Taunix\cdevsw.h>


#ifndef CAPTURE_H
#define CAPTURE_H

extern CDEVSW	capture;
struct cap_unit{
	int owner_task;
	void (*callback)(unsigned *cap_fifo);
};

void cap_init();
int cap_open(int cap_flag,int cap_mode);  
int cap_close();
int cap_read(void *buf,unsigned cap_unit);
int cap_ioctl(unsigned request,void *args);
int cap_setcallback(unsigned cap_unit,void(*callback)(unsigned*));

/* Definitions for CAPCON used by cap_open */
#define CAPTURE1				0
#define CAPTURE2				1
#define CAPTURE3				2
#define CAPTURE4				4 /* Useless on x243 */
/* CAP34TSEL @ CAPCON.10 */
#define CAP34TSEL_GPTIMER2		0x0000
#define CAP34TSEL_GPTIMER3		0x0400
/* CAP12TSEL @ CAPCON.9 */
#define CAP12TSEL_GPTIMER2		0x0000
#define CAP12TSEL_GPTIMER3		0x0200

/* The following timer select options are for 243 DSP */
/* CAP34TSEL @ CAPCON.10 */
#define CAP3TSEL_GPTIMER2		0x0000
#define CAP3TSEL_GPTIMER1		0x0400
/* CAP12TSEL @ CAPCON.9 */
#define CAP12TSEL_GPTIMER2		0x0000
#define CAP12TSEL_GPTIMER1		0x0200

/* CAP4TOADC @ CAPCON.8 */
#define CAP4TOADC				0x0100
/* CAP1EDGE @ CAPCON.7-6 */
#define CAP1EDGE_RISING			0x0040
#define CAP1EDGE_FALLING			0x0080
#define CAP1EDGE_BOTH			0x00C0
/* CAP2EDGE @ CAPCON.5-4 */
#define CAP2EDGE_RISING			0x0010
#define CAP2EDGE_FALLING			0x0020
#define CAP2EDGE_BOTH			0x0030
/* CAP3EDGE @ CAPCON.3-2 */
#define CAP3EDGE_RISING			0x0004
#define CAP3EDGE_FALLING			0x0008
#define CAP3EDGE_BOTH			0x000C
/* CAP4EDGE @ CAPCON.1-0 */
#define CAP4EDGE_RISING			0x0001
#define CAP4EDGE_FALLING			0x0002
#define CAP4EDGE_BOTH			0x0003

/* Difinitions for CAPCON used by cap_ioctl */
#define CAP_RESET			0x1000
#define CAP_QEPENABLE		0x2000
#define CAP_QEPDISABLE		0x3000
#define CAP_CAP1EDGE		0x0100
#define CAP_CAP2EDGE		0x0200
#define CAP_CAP3EDGE		0x0400
#define CAP_CAP4EDGE		0x0800
#define CAP_RISINGEDGE		0x0001
#define CAP_FALLINGEDGE		0x0002
#define CAP_BOTHEDGE		0x0003

#endif
