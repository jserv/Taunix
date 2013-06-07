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

/* Modified date : 1999-9-13
 * Changes:
 *   1. In wdrt_init(), program cannot access RTICR originally in F240 chip.
 *      That will cause a reset on F243 chip.
 *   2. In wdrt_init(), use new sys_time to count ticks.
 */

/* Modified date : 1999-10-1
 * Changes:
 *   1. As 243 has no RTI, I use NMI to perform RTI, but need externl interrupt
 *      source. While Taunix initilizes, user has to config timer or something
 *      else (ex: extra external clock) to trig NMI.
 */

#include <Taunix\x240\F240mmrs.h>
#include <Taunix\x240\wdrti.h>

/*
CDEVSW		wdrt;
*/

SYS_TIME	sys_time;
void wdrt_init()
{       
 /* For some reason, the following code will cause a reset on F243 */
 /*MMREGS[RTICR] = RTI_DISABLE|RTIPS_DIV4;    */                     
 
 MMREGS[WDCR] = WD_DISABLE|WDCHK|WDPS_DIV64;
 MMREGS[WDKEY] = 0x55;
 MMREGS[WDKEY] = 0xAA;
/* 
 wdrt.d_open = 0x0000;
 wdrt.d_close = 0x0000;
 wdrt.d_read = wdrt_read;
 wdrt.d_write = wdrt_write;
 wdrt.d_ioctl = wdrt_ioctl;
 wdrt.read_callback = 0x0000;
 wdrt.write_callback = 0x0000;
*/

 /* Though, F243 has no realtime clock interrupt, there is no accurate
    real-time clock source. Taunix has to use another approach to achive 
    that. But before that, redefine the SYS_TIME's fields to count ticks.
 */

 sys_time.sec = 0;
 sys_time.ms = 0;
 sys_time.us = 0;

/*
 sys_time.tick=0;
 sys_time.Ktick = 0;
 sys_time.Mtick = 0; 
*/
 /* For 243DSP, I configure XINT1 as timer interrupt. 
 MMREGS[XINT1CR] = 0x0001;        
 */
 /* For 243DSP, XINT1 shares pin with IOPA.2 
 MMREGS[OCRA] |= 0x0040;
 */
 
 MMREGS[IMR] |= 0x0001; /* Enable INT1 */

}

void wdrt_isr()
{
 /*unsigned *reg_file = 0x0000;*/
 	/*wdrt_ioctl(RESET_WD,0);*/
 	WDRTI_RESETWD;
 /* For 240 */
	#ifdef WDCLK_15625
	sys_time.us += 256;
	#else
	sys_time.us += 244;
	#endif 
 */
	if(sys_time.us>1000){
		sys_time.ms ++;
		sys_time.us = 0;
		if(sys_time.ms==1000){
			sys_time.sec ++;
			sys_time.ms = 0;
		}
	}
/**/
}
/*
int wdrt_read(void *buf,unsigned int size)
{
 unsigned *temp_buf = (unsigned *)buf;
 unsigned *reg_file = 0x0000;
 	if(buf == (void*)0x0000)	return -1;
 	temp_buf[0] = reg_file[RTICR]<<8;
 	temp_buf[0] |= reg_file[WDCR];
 	return 1;
}

int wdrt_write(void *buf,unsigned int size)
{
 unsigned *temp_buf = (unsigned *)buf;
 unsigned *reg_file = 0x0000;
 	reg_file[WDCR] = temp_buf[0];
 	reg_file[RTICR] = temp_buf[0]>>8;
 	return 1;
}

int wdrt_ioctl(unsigned request,void *argp)
{
 unsigned *reg_file = 0x0000;
   	switch(request){
 	 case DISABLE_WD:
 	 	reg_file[WDCR] |= 0x40;
 	 		return 1;
 	 case ENABLE_WD:
 	 	reg_file[WDCR] = (reg_file[WDCR] & 0x2F)|WDCHK;     
 	 	reg_file[WDKEY] = 0x5555;
 	 	reg_file[WDKEY] = 0xAA;
 	 		return 1;
 	 case RESET_WD:
 	 	reg_file[WDKEY] = 0x5555;
  	 	reg_file[WDKEY] = 0xAAAA;
 	 		return 1;
 	 case DISABLE_RTI:
 	 	reg_file[RTICR] &= 0x0F;
 	 		return 1;
 	 case ENABLE_RTI:
 	 	reg_file[RTICR] |= 0x40;
 	 		return 1;
 	 default:
 	 	return -1;
 	}
}
*/
