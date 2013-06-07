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
#include <Taunix\x240\F240MMRS.H>
#include <Taunix\task.h>
#include <Taunix\ssecs.h>
#include <Taunix\sleep.h>
#include <Taunix\x240\sci.h>
*/

#include "f243mmrs.h"
#include "task.h"
#include "ssecs.h"
#include "sleep.h"
#include "sci.h"

#define TXPIPE_SIZE		80
#define RXPIPE_SIZE		16

struct _secs_settings secs_settings;
CDEVSW	secs;

int msmode;
int secs_state;

int tx_pipe,rx_pipe;
unsigned tx_pipe_buf[TXPIPE_SIZE],rx_pipe_buf[RXPIPE_SIZE];

unsigned *secs_rx_buf;
unsigned databuf_size,databuf_page;
/*
Every page in secs_rx_buf has such structure as follow:

        +-----------+--------     +--------------+
        |data length|Data.....    |              |
        +-----------+--------     +--------------+
 index :  0           1     ....   databuf_size-1
 
 First element is data length in this page.  
*/

int _secs_txrdy_flag=1,_secs_rxrdy_flag=1;

int secs_sci_flag,secs_sci_brr;
unsigned secsd_stack[1024];
void secsd();
void secs_init(int sci_flag,int sci_brr,int secs_mode,unsigned *data_buf,unsigned data_size,unsigned data_page)
{
 
 unsigned i;
	secs.d_open = 0;
	secs.d_close = 0;
	secs.d_read = secs_read;
	secs.d_write = secs_write;
	secs.d_ioctl = 0;
	                   
	msmode = secs_mode;
	secs_state = SECS_IDLE	;
	
	secs_settings.iSendTimeout = 4096;
	secs_settings.iReceiveTimeout = 4096;
	secs_settings.iNumberOfRetries = 3;	
	tx_pipe = pipe(tx_pipe_buf,TXPIPE_SIZE,PIPE_NONOVERWRITE);
	rx_pipe = pipe(rx_pipe_buf,RXPIPE_SIZE,PIPE_NONOVERWRITE);

	secs_rx_buf = data_buf;
	databuf_size = data_size-2;
	databuf_page = data_page;
	for(i=0 ; i<databuf_page ; i++)
		secs_rx_buf[i*databuf_size] = 0;
	secs_sci_flag = sci_flag;
	secs_sci_brr = sci_brr;
	task_create(secsd,secsd_stack,SECS_PRIORITY,"secsd");
}


unsigned long CountCHKSUM(void *buf,int size/* in bytes*/)
{
 unsigned long chksum = 0;
 unsigned 	   *uiBuf = (unsigned *)buf;
 int i;
 	for(i=0 ; i<size ; i++){
 		chksum += ( (i&0x0001)?uiBuf[i>>1]>>8:uiBuf[i>>1] ) & 0x00FF;
 	}
 	return chksum;
}

void _secs_rxrdy_callback(void *param)
{
	_secs_rxrdy_flag = 1;
	resume(SECS_PRIORITY);	/* secs daemond task */
}

int arbitrate(int iSECSCmd)
{unsigned buf[1];
 int retries = 0;
 int sndENQ = 1;

	switch(iSECSCmd){
	 case SECS_CMD_RECEIVE:
	 	buf[0] = SECS_CMD_EOT;
	 	sci.d_write(buf,1);	/* Send EOT back */
	 	return LINECONTROL_READYTORECEIVE;	/* Ready to receive */
	 		break;
	 case SECS_CMD_SEND:
	 	buf[0] = SECS_CMD_ENQ;
	 	_secs_rxrdy_flag = 0;
	 	
	 	for(retries=0 ; retries<secs_settings.iNumberOfRetries ; retries++){
	 		if(sndENQ)	sci.d_write(buf,1);	/* Send ENQ */
	 		sci.read_callback = _secs_rxrdy_callback;
	 		sci.d_read(buf,1);
	 		sleep(secs_settings.iReceiveTimeout);
	 		sci.read_callback = 0;
	 		
	 		if(_secs_rxrdy_flag){
	 			if( (buf[0] == SECS_CMD_ENQ) && /* Line contention */
	 				(msmode == SECS_SLAVEMODE) ){ /* and local is in slave mode */
	 				buf[0] = SECS_CMD_EOT;	
	 				sci.d_write(buf,1); 	/* Send EOT back */
	 				return	LINECONTROL_READYTORECEIVE; /* Ready to receive */
	 			}
	 			else if(buf[0] == SECS_CMD_EOT){
	 				return LINECONTROL_READYTOSEND;
	 			}
	 			else	sndENQ = 0;
	 		}
	 	}
	 	return LINECONTROL_IOERROR;
	}
}

int secs_write(const void *buf,unsigned size)
{   
 SECSBLOCK	secs_block;
 	/* If tx pipe is full */
	if(pipe_tab[tx_pipe].count == TXPIPE_SIZE)	return SECS_TOOBUSY;
	/* Prepare the SECS block that will be send */
	secs_block.uipData = buf;
	secs_block.uiDataLength = size;
	secs_block.ulDataCHKSUM = CountCHKSUM(secs_block.uipData,size);
	secs_block.owner = current_task;
	
	pipe_write(tx_pipe,(unsigned *)&secs_block,5);/* Write block into pipe */
	if(secs_state == SECS_IDLE)	resume(SECS_PRIORITY);
	return 1;
}

int secs_read(void *buf,unsigned size)
{
 register unsigned int *uipblock, *rbuf = (unsigned int*)buf;
 unsigned int uiblock_ptr;
 unsigned int rx_DataLength;
 register unsigned int i,temp_Length;
 	if(pipe_tab[rx_pipe].count == 0)	return SECS_IDLE;
 	pipe_read(rx_pipe,&uiblock_ptr,1);
 	uipblock = (unsigned int *) uiblock_ptr;
 	rx_DataLength = uipblock[0];
 	uipblock[0] = 0;	/* free rx buf */
 	temp_Length = (rx_DataLength>size)?size:rx_DataLength;
 	temp_Length = (temp_Length>>1)+1;	
 	uipblock++;
 	for(i=0 ; i<temp_Length ; i++)
 		*rbuf++ = *uipblock++;
 	return rx_DataLength;
}

unsigned int *search_empty_rxbuf(){
 unsigned i;
 	for(i=0 ; i<databuf_page; i++)
 		if(secs_rx_buf[i*databuf_size] == 0)	break;
 		/* secs_rx_buf[i][0] == 0 means that page contains no data */
 	if(i==databuf_page)	return 0; /* No free page available */
 	return secs_rx_buf+i*databuf_size;
}                    

void secs_receive()
{
 SECSBLOCK	 secs_block;
 unsigned    block_ptr;
 unsigned	 ACK;
 unsigned 	 progress1=0,progress2=0;
 
	secs_block.uipData = search_empty_rxbuf();
	if(secs_block.uipData == 0) /* No free buffer */
		goto RETURN_NAK;
		
 	sci.read_callback = _secs_rxrdy_callback;
 	_secs_rxrdy_flag = 0;
 	sci.d_read(&secs_block.uiDataLength,2); /* read SECS block's data length */
 	sleep(secs_settings.iReceiveTimeout);
 	if((!_secs_rxrdy_flag) || (secs_block.uiDataLength>=databuf_size))
 		 /* cannot receive data length or data length is too large */
 		goto	RETURN_NAK;
 
 	secs_block.uipData[0] = secs_block.uiDataLength;

	/* prepare to receive data */
 	_secs_rxrdy_flag = 0;
 	sci.d_read(secs_block.uipData+1,secs_block.uiDataLength);
 	do{ 
 	    sleep(secs_settings.iReceiveTimeout);
 		progress1 = progress2;	/* save old progress */
 		sci.d_ioctl(SCI_RXPROGRESS,&progress2);		/* read new progress */
 	}while((progress1 != progress2) && !_secs_rxrdy_flag);/* if old progress == new progress, 
 														the tx is really time-out */
 	if(!_secs_rxrdy_flag){ /* cannot receive data */
 		((unsigned *)secs_block.uipData)[0] = 0; /* free secs_rx_buf */
 		goto RETURN_NAK;
	}
	
 	/* prepare to receive checksum */
 	_secs_rxrdy_flag = 0;
 	sci.d_read(&secs_block.ulDataCHKSUM,4);
 	sleep(secs_settings.iReceiveTimeout);
 	if(!_secs_rxrdy_flag){ /* cannot receive data chksum */
		((unsigned *)secs_block.uipData)[0] = 0; /* free secs_rx_buf */
		goto RETURN_NAK;
 	}
 	
 	if(secs_block.ulDataCHKSUM == 
 		CountCHKSUM(secs_block.uipData+1,secs_block.uiDataLength)){
 		/* Checksum OK */
 		ACK = SECS_CMD_ACK;
 		sci.d_write(&ACK,1);
 		if(pipe_isFull(rx_pipe)){
 		 unsigned *overrun_data;
 			pipe_read(rx_pipe,&block_ptr,1);
 			overrun_data = (unsigned*)block_ptr;
			overrun_data[0] = 0;
 		}
 		block_ptr = (unsigned)secs_block.uipData;
 		pipe_write(rx_pipe,&block_ptr,1);
 	}
 	else{ /* checksum not match */
		((unsigned *)secs_block.uipData)[0] = 0; /* free secs_rx_buf */
		goto RETURN_NAK;
 	}
 	sci.read_callback = 0;
 	return;


RETURN_NAK:
 		ACK = SECS_CMD_NAK;
 		sci.d_write(&ACK,1);	/* send NAK */
 		sci.read_callback = 0;
 		sci.d_ioctl(SCI_STOPRX,0);
 		return;		/* receive fails */
}

void secs_send()
{
 SECSBLOCK	secs_block;
 unsigned    block_ptr;
 int 		 retries;
 unsigned    ACK;
 	pipe_read(tx_pipe,(unsigned *)&secs_block,5);
 		
 	for(retries=0; retries<secs_settings.iNumberOfRetries ; retries++){
START_SEND:
 		sci.write_callback = 0;
 		sci.d_write(&secs_block.uiDataLength,2); /* send data length */
 		sci.d_write(secs_block.uipData,secs_block.uiDataLength);/* send data */
 		sci.d_write(&secs_block.ulDataCHKSUM,4);	/* send checksum */
 		sci.read_callback = _secs_rxrdy_callback;
 		_secs_rxrdy_flag = 0;
 		ACK = SECS_IDLE;
 		sci.d_read(&ACK,1);
 		sleep(secs_settings.iReceiveTimeout);
 		sci.read_callback = 0;
 		sci.d_ioctl(SCI_STOPRX,0);
		if(_secs_rxrdy_flag && ACK == SECS_CMD_ACK){ /* send successfully */
 			msgq_write(secs_block.owner,SECS_SEND_COMPLETE);
 			/*msgq_write(secs_block->owner,(unsigned)secs_block->uipData);*/
 			break;
 		} 		
 		else{ /* ACK not received */
 			for( ; retries<secs_settings.iNumberOfRetries ; retries++){
ARBITRATE_IN_SEND:
 		 		switch(arbitrate(SECS_CMD_SEND)){ /* prepare to re-transmit */
 			 	 case LINECONTROL_READYTOSEND:
 			 		goto START_SEND;
 			 	 case LINECONTROL_READYTORECEIVE:
 			 		secs_receive();	/* Data received , prepare to re-transmit */
 			 		goto ARBITRATE_IN_SEND;
 				}	
 			}
 		}
 	}
 	if(retries == secs_settings.iNumberOfRetries){ /* retries expires */
		msgq_write(secs_block.owner,SECS_ERROR_SENDFAIL);
		/*msgq_write(secs_block->owner,(unsigned)secs_block->uipData);*/
	}
}

void secsd()
{
 int arb_result;
 unsigned buf;
    sci.d_open(secs_sci_flag,secs_sci_brr);
    TASK_RUNNING
    
    	while(1){
	 		if(pipe_tab[tx_pipe].count){ /* something in TX pipe */
 				arb_result = arbitrate(SECS_CMD_SEND);
 				break;
 			}
    		else{
    			sci.read_callback = _secs_rxrdy_callback;
    			_secs_rxrdy_flag = 0;
    			sci.d_read(&buf,1);
    			if(!_secs_rxrdy_flag){
    				secs_state = SECS_IDLE;
    				suspend(SECS_PRIORITY);
    			}
    			sci.read_callback = 0;
    			sci.d_ioctl(SCI_STOPRX,0);
    			
    			if(_secs_rxrdy_flag && (buf==SECS_CMD_ENQ)){
    				secs_state = SECS_BUSY;
    				arb_result = arbitrate(SECS_CMD_RECEIVE);
    				break;
    			}
			}
    	}                      
    	
    	secs_state = SECS_BUSY;
 		switch(arb_result){
 		 case LINECONTROL_READYTOSEND:
 		  	secs_send();
  		 		break;
  		 case LINECONTROL_READYTORECEIVE:
 		 	secs_receive();
  		 		break;
  		 default:
  		 		break;
 		}
 		
 		arb_result = SECS_IDLE;
 	KSAT
}
