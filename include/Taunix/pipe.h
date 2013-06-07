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
Establish date: 1999-5-4
Author : Max Yin
Orgnization : Tau Workshop

File : pipe.h
Content:
	Inter-Process Communication: Pipe
-----------------------------------------------*/

#ifndef PIPE_H
#define PIPE_H

#define MAX_PIPE    10

typedef struct _pipe{
     unsigned int *buffer;
     int  size;
     int  head;
     int  tail;
     int  count;
     int  ow_mode;
}PIPE;

extern PIPE    pipe_tab[];
void pipe_init();
int pipe(unsigned int *pbuf,int size,int mode);
int pipe_close(int phandle);
int pipe_read(int phandle,unsigned int *buf,unsigned int size);
int pipe_write(int phandle,unsigned int *buf,unsigned int size);
int pipe_isEmpty(int phandle);
int pipe_isFull(int phandle);
int msgq(unsigned int *pbuf,int size);
int msgq_close();
int msgq_read(unsigned int *buf);
int msgq_write(int task,unsigned int msg);
int msgq_isEmpty();
int msgq_isFull();

#define PIPE_OVERWRITE		1
#define PIPE_NONOVERWRITE   0

#endif
