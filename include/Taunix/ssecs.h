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
Establish date: 1999-7-26
Author : Max Yin
Orgnization : Tau Workshop

File : SSECS.h
Content:
	Serial Communication SECS Protocal
		Static version(without malloc/free)
	
-----------------------------------------------*/

#ifndef SSECS_H
#define SSECS_H
/*
#include <Taunix\pipe.h>
#include <Taunix\cdevsw.h>
*/
#include "pipe.h"
#include "cdevsw.h"
struct _secs_settings{
	int iSendTimeout,iReceiveTimeout,iNumberOfRetries;
};

extern struct _secs_settings secs_settings;

typedef struct _secsblock{
	unsigned int	uiDataLength;
	unsigned int *	uipData;
	unsigned long	ulDataCHKSUM;
	int				owner;
	/* owner is used to synchronize with task asking service */
}SECSBLOCK;

extern CDEVSW	secs;
void secs_init(int sci_flag,int sci_brr,int secs_mode,unsigned *data_buf,unsigned data_size,unsigned data_page);

int secs_read(void *buf,unsigned int size);
int secs_write(const void *buf,unsigned int size);
#define SECS_PRIORITY		5

#define SECS_MASTERMODE		0
#define SECS_SLAVEMODE      1

#define SECS_CMD_SEND		1
#define SECS_CMD_RECEIVE    2
#define SECS_CMD_ENQ		'Q' /* 5 */
#define SECS_CMD_EOT		'T' /* 4 */
#define SECS_CMD_ACK		'A' /* 6 */
#define SECS_CMD_NAK		'N' /* 21 */

#define LINECONTROL_READYTOSEND		1
#define LINECONTROL_READYTORECEIVE 	2
#define LINECONTROL_IDLE			-1
#define LINECONTROL_IOERROR			-2

#define SECS_IDLE					0
#define SECS_BUSY					1
#define SECS_TOOBUSY				2
#define SECS_SEND_COMPLETE			3
#define SECS_RECEIVE_COMPLETE		4
#define SECS_ERROR_SENDFAIL			0xFFFF
#define SECS_ERROR_RECEIVEFAIL		0xFFFE


/* Macro definition */
#define SECSBLOCK_ATTACHDATA(block,data)	(block)->uipData = (unsigned int*)data;
#define DISABLE_SECS	secs.d_read = sci_read;secs.d_write = sci_write;sci.owner_task=current_task;
#define ENABLE_SECS		secs.d_read = secs_read; secs.d_write=secs_write;sci.owner_task=5;

#endif
