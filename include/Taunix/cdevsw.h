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
		
/* Establish date : 1999-4-26
 * Author : Max Yin
 * Orgnization : Tau Workshop
 * File : CDEVSW.h
 * Content :
 *   Character DEVice SWitch(CDEVSW)
 */

#ifndef CDEVSW_H
#define CDEVSW_H

typedef struct _cdevsw{
	char *caption;
	int   owner_task;
	int	  (*d_open)(int flags,int mode);
	int   (*d_close)();
	int   (*d_read)(void *buf,unsigned int size);
	int   (*d_write)(const void *buf,unsigned int size);
	int   (*d_ioctl)(unsigned request,void *argp);
	int   (*d_xpoll)();
	void  (*read_callback)(void *param);
	void  (*write_callback)(void *param);
} CDEVSW;

#define BEGIN_EVM_CASE(EVIVR)	switch(EVIVR){
#define END_EVM_CASE			}

#endif
