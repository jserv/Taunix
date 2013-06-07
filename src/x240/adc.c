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
 	1. Remove all owner_task checking code.
 	2. Modify adc_read to let it be faster
 MOdified date : 1999-8-3
 Changes:
 	1. In adc_open(), driver will automatically set OCRA register
 	   to fit ADC's need.

*/

/*
	BUG FIX and change design : date 199908191110
	Changes:
	1. Synchronous mode now is using polling, not resume the owner task,
	   since the conversion will complete in 6us. Interrupt will cause more
	   instruction time.
	2. ADC driver has some design bugs(it seems that I forgot to enable ADC).
        I didn't fully test this code that time and still I need somebody to help
	   me to test Taunix kernel.
*/

#include <Taunix\x240\f240mmrs.h>
#include <Taunix\task.h>
#include <Taunix\x240\adc.h>

CDEVSW	adc;

void adc_init()
{
	adc.d_open = adc_open;
	adc.d_close = adc_close;
	adc.d_read = adc_read;
	adc.d_write = 0;
	adc.d_ioctl = adc_ioctl;
	adc.read_callback = 0;
	adc.owner_task = -1;
}

/*
  In adc_open, arg. channel_mode can assign conversion channel,
  and ADC running mode, one shot or continous mode.
*/
int adc_open(int ctrl1,int ctrl2)
{
	/*if(adc.owner_task != -1)	return -1;*/ /* ADC had been OWNED */
    adc.owner_task == current_task;
	/* For 240*/
    MMREGS[OCRA] |= 0x000F;
    /**//* For 243 DSP, ADCIN0~7 are not shared */
    
	MMREGS[ADCTRL1] = 0x9800|ctrl1; /* Suspend-soft enable */
	
	/* For 243 DSP 
	MMREGS[ADCTRL2] = 0x4800|ctrl2;
	*/
	/* 	In ADCTRL2, another IM and INTPRI are added.
		I set them to IM = 1(interrupt when FIFOn has at lease one word),
		INTPRI = 1(low priority, as compatible with Taunix240) 
	*/

	return 1;

}

int adc_close()
{
	/*if(adc.owner_task != current_task)	return -1;*/
	adc.owner_task = -1;
	MMREGS[ADCTRL1] = 0;
	return 1;
}

int adc_setcallback(void (*callback)())
{
	/*if(adc.owner_task != current_task)	return -1;*/
	adc.read_callback = callback;
	return 1;
}

int adc_read(void *buf,unsigned int fifo)
{
 unsigned *buffer = (unsigned*)buf;
 unsigned fifo_status;
 unsigned ioctl_request;
 	/*	if(adc.owner_task != current_task)	return -1; */
 		switch(fifo){
 		 case ADCFIFO1:
 		 	fifo_status = (MMREGS[ADCTRL2] & 0x0018)>>3;
 		 		break;
 		 case ADCFIFO2:
 		 	fifo_status = (MMREGS[ADCTRL2] & 0x00C0)>>6;
 		 		break;
 		 /*default:*/ /* fifo is not FIFO1 nor FIFO2 */
 		 		/*return -1;*/
 		}
 		
 		/* read FIFO status */
 		/*adc_ioctl(ioctl_request,&fifo_status);*/
 		switch(fifo_status){
 		 case ADCFIFO_EMPTY:/* FIFO is empty */
 		 	if(adc.read_callback){ /* use call-back */
				/* First, enable ADC interrupt, let ISR to callback */
				MMREGS[ADCTRL1] |= (ADCINT_ENABLE|ADC_IMSTART|ADC_SOC);
 		 		return 0;
			}
 		 	else{					/* else wait */
				/*suspend(current_task);*/
				/* Now, use polling */
				MMREGS[ADCTRL1] &= 0xFDFF; /*~ADCINT_ENABLE*//* Clear ADCINTEN */
				
				while(MMREGS[ADCTRL1] & 0x0080 /* EOC */ );
				
				/* Test ADCINTFLAG  & ADCEOC in ADCTRL1 */
				/* After ADCINTFLAG or ADCEOC has been set, the flow will go to default section
				   of switch-case block.
				*/
			}
 		 		
 		 default:
 		 /*case ADCFIFO_1ENTRY:*/	/* either resumed */
 		 /*case ADCFIFO_2ENTRIES:*/ /* or there is entry in FIFO */
 		 /*case ADCFIFO_OVERRUN:*/ 	/* return one entry in FIFO */
 		 
 		 		*buffer = MMREGS[fifo];
 		 		*buffer >>= 6;
 		 		return 1;
		}
		return 0; 		 
}	

int adc_ioctl(unsigned request,void *argp)
{
 unsigned *arg = (unsigned *)argp;
 /*if(adc.owner_task != current_task)	return -1;*/
    
    switch(request){
     case ADC_IMSTART:
     	MMREGS[ADCTRL1] |= ADC_IMSTART;
     		return 1;
     case ADC_SOC:
     	MMREGS[ADCTRL1] |= (ADC_SOC|ADC_IMSTART);
     		return 1;
     case ADC_SELCH:
     	MMREGS[ADCTRL1] &= 0xFF81; /* clear channel selection setting */
     	MMREGS[ADCTRL1] |= *arg; /* assign new channel */
     		return 1;
    }
    
 	switch(request&0x8000){
 	 case ADC_SET:
 		MMREGS[request&0x7FFF] = *arg;
 			return 1;	 	
 	 case ADC_READ:
 	 	switch(request&0x7FFF){
 	 	 case EOC:
 	 	 	*arg = MMREGS[ADCTRL1]&EOC;
 	 	 		return 1;
 	 	 case INTFLAG:
 	 	 	*arg = MMREGS[ADCTRL1]&INTFLAG;
 	 	 	 	return 1;
 	 	 case FIFO1STATUS:
 	 	 	*arg = (MMREGS[ADCTRL2]&FIFO1STATUS)>>6;
 	 	 		return 1;
 	 	 case FIFO2STATUS:
 	 	 	*arg = (MMREGS[ADCTRL2]&FIFO2STATUS)>>3;
 	 	 		return 1;
 	 	}
 	}
 	return -1;
}
