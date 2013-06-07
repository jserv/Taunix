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

/*
#include <Taunix\task.h>
#include <Taunix\pipe.h>
*/
#include "task.h"
#include "pipe.h"
PIPE pipe_tab[MAX_TASK+MAX_PIPE];

void pipe_init()
{int i;
     for(i=0 ; i<MAX_TASK+MAX_PIPE ; i++)
          pipe_tab[i].buffer = (unsigned int*)0;
}

int pipe(unsigned int *buf, int size,int ow_mode)
{
 int i;
     for(i=MAX_TASK ; i<MAX_TASK+MAX_PIPE ; i++)
          if(pipe_tab[i].buffer == 0)  break;
     if( i == (MAX_TASK+MAX_PIPE))  return -1; /* no available pipe */
     pipe_tab[i].buffer = buf;
     pipe_tab[i].size = size;
     pipe_tab[i].head = pipe_tab[i].tail = 0;
     pipe_tab[i].count = 0;
     pipe_tab[i].ow_mode = ow_mode;
     return i;
}

int msgq(unsigned int *buf,int size)
{
	pipe_tab[current_task].buffer = buf;
    pipe_tab[current_task].size = size;
    pipe_tab[current_task].head = pipe_tab[current_task].tail = 0;
    pipe_tab[current_task].count = 0;
    pipe_tab[current_task].ow_mode = PIPE_OVERWRITE;
	return 1;	
}

int pipe_close(int phandle)
{
     if(phandle >= MAX_TASK+MAX_PIPE)  return -1;
     pipe_tab[phandle].buffer = (unsigned int*)0;
     return 1;
}

int msgq_close()
{
	return pipe_close(current_task);
}

int pipe_isEmpty(int phandle)
{
	return (pipe_tab[phandle].count)?0:1;
}

int msgq_isEmpty()
{
	return pipe_isEmpty(current_task);
}

int pipe_isFull(int phandle)
{
 	return (pipe_tab[phandle].count==pipe_tab[phandle].size)?1:0;
}

int msgq_isFull()
{
	return pipe_isFull(current_task);
}

int pipe_write(int phandle,unsigned int *buf, unsigned int size)
{
 int i;
     if(phandle >= MAX_TASK+MAX_PIPE) /* illegal pipe handle */
          return -1;
     if((pipe_tab[phandle].count == pipe_tab[phandle].size)
     		&& !pipe_tab[phandle].ow_mode) /* pipe full */
          return -1;
     if(pipe_tab[phandle].buffer == 0) return -1;

	 disable_sched
     for(i=0 ; i<size ; i++){
          pipe_tab[phandle].buffer[pipe_tab[phandle].tail++] = buf[i];
          if(pipe_tab[phandle].count<pipe_tab[phandle].size)
          	pipe_tab[phandle].count++;
          if(pipe_tab[phandle].tail == pipe_tab[phandle].size)
          	pipe_tab[phandle].tail=0;
          if((pipe_tab[phandle].count==pipe_tab[phandle].size)
          		&& !pipe_tab[phandle].ow_mode){ /* pipe full */
          			/* and pipe is not in over-write mode */
               i++;
               break;
          }
     }
     enable_sched
     return i;
}

int msgq_write(int task,unsigned int msg)
{
	return pipe_write(task,&msg,1);
}

int pipe_read(int phandle, unsigned int *buf, unsigned int size)
{
 int i;
     if(phandle >= MAX_TASK+MAX_PIPE) /* illegal pipe handle */
          return -1;
     if(pipe_tab[phandle].count == 0)  return 0;
     if(pipe_tab[phandle].buffer == 0) return -1;
                
     disable_sched
     for(i=0 ; i<size ; i++){
          buf[i] = pipe_tab[phandle].buffer[pipe_tab[phandle].head++];
          pipe_tab[phandle].count --;
          if(pipe_tab[phandle].head == pipe_tab[phandle].size)
          	pipe_tab[phandle].head = 0;
          if(pipe_tab[phandle].count == 0){
               i++;
               break;
          }
     }
     enable_sched
     return i;
}

int msgq_read(unsigned int *buf)
{
	if(pipe_tab[current_task].buffer==0)	return 0;
	return pipe_read(current_task,buf,1);
}
