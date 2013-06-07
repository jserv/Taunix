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
 Project codename: fastTaunix
 Modified date : 1999-7-8
 Changes:
 	Remove all owner_task checking code.

*/

/*
	Design Change: date: 19990819.1150
	Changes:
	1. In sci_read/sci_write, synchronous mode is changed to use polling.
	   Polling technique will be faster than ininterrupt driven.
*/
/*
	Design Change: data: 19990914.1240
	Changes:
	1. Fixed some incompatibilities between x240 and x243 
*/

#include <Taunix\x240\sci.h>

CDEVSW	sci;

struct x_status	sci_rx_status,sci_tx_status;

void sci_init()
{
	sci.owner_task = -1;
	sci.caption = "sci";
	sci.d_open = sci_open;
	sci.d_close = sci_close;
	sci.d_read = sci_read;
	sci.d_write = sci_write;
	sci.d_ioctl = sci_ioctl;
	sci.read_callback = 0;
	sci.write_callback = 0;
	MMREGS[SCIPC2] = 0x22;	/* Set SCI TX/RX pins as SCI function */
	/* SCIPC2 in x243 has been disabled */
	MMREGS[SCICTL1] = 0x00;	/* software reset SCI */
	MMREGS[SCIPRI] = SCIPRI_LOW;
}

int sci_open(int flag,int brr)
{
	/*if(sci.owner_task != -1)*//* Someone owns sci */
		/*return -1;*/
	sci.owner_task = current_task;
	MMREGS[SCICTL1] = 0x00;	/* Set SW RESET to 0 (reset SCI flag)*/
	MMREGS[SCICCR] = flag;    	/* Set SCICCR */
	MMREGS[SCIHBAUD] = brr>>8;	/* Set high BRR */
	MMREGS[SCILBAUD] = brr;		/* Set low BRR */
	/* For 240 */
	MMREGS[SCICCR] |= 0x10;*/
	/* In SCICCR.4@240 is SCIENA, but in 243, it's LOOPBACK */
	MMREGS[SCICTL1] = 0x30; /* Set SW RESET to 1 (freeze SCI flag) */
	return 1;
}

int sci_close()
{
	/*if(sci.owner_task != current_task)	return -1;*/
	MMREGS[SCICTL1] = 0x00;
	MMREGS[SCICTL2] = 0x00;
	sci.owner_task = -1;
	sci.read_callback = sci.write_callback = 0;
	return 1;
}

int sci_setcallback(void (*read_callback)(void *param),
					void (*write_callback)(void *param))
{
	/*if(sci.owner_task != current_task)	return -1;*/
	sci.read_callback = read_callback;
	sci.write_callback = write_callback;
}

int sci_getstatus()
{
	return MMREGS[SCIRXST];
} 

int sci_read(void *buf,unsigned int size)
{
	sci_rx_status.x_buf = (unsigned*)buf;
	sci_rx_status.x_size = size;
	sci_rx_status.x_progress = 0;

	/*if(sci.owner_task != current_task)	return -1;*/
	MMREGS[SCICTL1] |= RX_ENABLE/*|RXERRINT_ENABLE*/; /* Enable Rx */
	
	if(sci.read_callback){	/* asynchronous mode */
		MMREGS[SCICTL2] |= RXBKINT_ENABLE; /* Enable SCI RX/BK Int. */
		return 0;
	}
	else{	/* synchronous mode */
		/*suspend(current_task);*/
		/* Now poll the transfer status */
		for(sci_rx_status.x_progress=0 ; sci_rx_status.x_progress<size ; sci_rx_status.x_progress++){
			while(!(MMREGS[SCIRXST] & SCI_RXRDY)); /* Wait for RX ready */
			/* Byte by byte assignd into buf */
			if(sci_rx_status.x_progress&0x0001)
				sci_rx_status.x_buf[sci_rx_status.x_progress>>1] |= MMREGS[SCIRXBUF] << 8;
			else
				sci_rx_status.x_buf[sci_rx_status.x_progress>>1] = MMREGS[SCIRXBUF];

		}
		return	sci_rx_status.x_progress;
	}
}

int sci_write(const void *buf,unsigned int size)
{
	/*if(sci.owner_task != current_task)	return -1;*/
 	sci_tx_status.x_buf = (unsigned*)buf;
	sci_tx_status.x_size = size;
	sci_tx_status.x_progress = 0;
	
	MMREGS[SCICTL1] |= TX_ENABLE;
	if(sci.write_callback){/* asynchronous mode */
		MMREGS[SCICTL2] |= TXINT_ENABLE;
		sci_tx_status.x_progress++;
		MMREGS[SCITXBUF] = sci_tx_status.x_buf[0];
		return 0;
	}
	else{	/* synchronous mode */
		/*if(size>1){*//* Since synchronous mode will send first byte at first,
						there is no need to check size>1 or not. Just begin at
						first byte */
			/*suspend(current_task);*/
			/* Try polling TXRDY @ SCICTL2 */
			for(sci_tx_status.x_progress=0 ; sci_tx_status.x_progress<size ;sci_tx_status.x_progress++){
				while(!(MMREGS[SCICTL2] & SCI_TXRDY));/* Wait for tx ready */
				MMREGS[SCITXBUF] = sci_tx_status.x_buf[sci_tx_status.x_progress>>1]>>
					((sci_tx_status.x_progress&0x0001)?8:0);
			}
		/*}*/
		/*else{*/
			/*MMREGS[SCICTL1] &= 0xFD;*/ /* Disable TXENA */
			/*MMREGS[SCICTL2] &= 0xFE;*/ /* Disable TX INT */
		/*}*/
		return	sci_tx_status.x_progress;
	}
}

inline void sci_tx_isr() 
{                     
	if(sci_tx_status.x_progress < sci_tx_status.x_size){ /* data need to be sent */
		MMREGS[SCITXBUF] = sci_tx_status.x_buf[sci_tx_status.x_progress>>1]>>
					((sci_tx_status.x_progress&0x0001)?8:0);
		/* If sci_tx_status.x_progress is odd, shift left 8 bits */
		sci_tx_status.x_progress++;
	}
	else{	/* Transfer complete */
		MMREGS[SCICTL1] &= 0xFD; /* Disable TXENA */
		MMREGS[SCICTL2] &= 0xFE; /* Disable TX INT */
		if(sci.write_callback)	/* if callback function had been set */
			sci.write_callback(&sci_tx_status.x_progress);
		/*else*/
			/*resume(sci.owner_task);*/ /* Otherwise, resume suspended task */
		/* Synchronous mode is using pollg to transfer data.
		   It's no need to resume task.
		 */
	}
}

inline void sci_rx_isr() 
{  					
 unsigned cb_param;
	if( MMREGS[SCIRXST] & SCI_RXRDY ){
		if(sci_rx_status.x_progress&0x0001)
			sci_rx_status.x_buf[sci_rx_status.x_progress>>1] |= MMREGS[SCIRXBUF] << 8;
		else
			sci_rx_status.x_buf[sci_rx_status.x_progress>>1] = MMREGS[SCIRXBUF];
		sci_rx_status.x_progress++;
	}
	if((sci_rx_status.x_progress == sci_rx_status.x_size) || 
						(MMREGS[SCIRXST]&SCI_BRKDT)){
	/* Receiving complete or rx break detected */
		/* stop receiving ... */
		MMREGS[SCICTL1] &= 0xBE; /* Disable RXERR INT and RX */
		MMREGS[SCICTL2] &= 0xFD; /* Disable RX/BK INT */
		if(sci.read_callback){	/* If callback function has be set */
			cb_param = sci_rx_status.x_progress;
			sci.read_callback(&cb_param);
		}
		/*else*/		/* Otherwise resume suspended task */
		/*	resume(sci.owner_task);*/
		/* Synchronous mode is using pollg to receive data.
		   It's no need to resume task.
		 */
	}
}

extern unsigned int int_flag;
void c_int5()
{
 unsigned cb_param;
 	ISR_ENTRY

	switch(MMREGS[/*PIVR*/SYSIVR]){
	 case 0x0005:	/* SPI INT */
	 		break;
	 case 0x0006:	/* RX INT */
	 	sci_rx_isr();
	 		break;
	 case 0x0007:	/* TX INT */
		sci_tx_isr();
			break;
	}
	ISR_EXIT
	DELAYED_SCHED
}

int sci_ioctl(unsigned request,void *argp)
{unsigned sci_ctl;
 char *toTx = (char *)argp;
 unsigned *rv;
	/*if(sci.owner_task != current_task)	return -1;*/
	switch(request){
	 case SCI_STOPTX:
		MMREGS[SCICTL1] &= 0xFD; /* Disable TXENA */
		MMREGS[SCICTL2] &= 0xFE; /* Disable TX INT */
		if(sci.write_callback)	/* if callback function had been set */
			sci.write_callback(&sci_tx_status.x_progress);
		break;	 	
	 case SCI_STOPRX:
		MMREGS[SCICTL1] &= 0xBE; /* Disable RXERR INT and RX */
		MMREGS[SCICTL2] &= 0xFD; /* Disable RX/BK INT */
		if(sci.read_callback)	/* If callback function has be set */
			sci.read_callback(&sci_rx_status.x_progress);
		break;
	 case SCI_SWRESET:
	 	MMREGS[SCICTL1] &= 0xDF;
	 	MMREGS[SCICTL1] |= 0x20;
	 		break;
	 case SCI_EMRGTX:
	 	sci_ctl = MMREGS[SCICTL1]<<8|MMREGS[SCICTL2];
	    MMREGS[SCICTL1] |= 0x02;
	    MMREGS[SCICTL2] &= 0xFE; /* Disable TX INT */
	    MMREGS[SCITXBUF] = toTx[0];
	    while(!(MMREGS[SCICTL2]&0xC0)); /* wait for TX complete */
	    MMREGS[SCICTL1] = sci_ctl>>8;
	    MMREGS[SCICTL2] = sci_ctl;
	 		break;
	 case SCI_TXPROGRESS:
		rv = (unsigned *)argp;
		rv[0] = sci_tx_status.x_progress;
			break;
	case SCI_RXPROGRESS:                                      
		rv = (unsigned *)argp;
		rv[0] = sci_rx_status.x_progress;
			break;
	}
	return 1;
}
