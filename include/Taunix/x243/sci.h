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
		
/* Establish date : 1999-5-11
 * Author : Max Yin
 * Orgnization : Tau Workshop
 * File : sci.h
 * Content :
 *   Serial Communication Interface
 */

#include <Taunix\task.h>
#include <Taunix\x243\f243mmrs.h>
#include <Taunix\cdevsw.h>

#ifndef SCI_H
#define SCI_H

extern CDEVSW	sci;
struct x_status{
	unsigned *x_buf;
	unsigned x_size;
	unsigned x_progress;
};

extern struct x_status	sci_rx_status,sci_tx_status;


void sci_init();
int sci_open(int flag, int brr);
int sci_close();
int sci_read(void *buf,unsigned int size);	/* size in bytes */
int sci_write(const void *buf,unsigned int size);  /* size in bytes */
int sci_ioctl(unsigned request,void *argp);
int sci_setcallback(void (*readcallback)(void *param),void (*writecallback)(void *param));
int sci_getstatus();

/* Definitions of values to Baud Rate Register, used in sci_open */
/* 
              SYSCLK
	BRR = ---------------  -  1
	       SCI Baud * 8      
	                              SYSCLK
	if BRR = 0	then SCI Baud = ----------
	                                16

*/
#define SYSCLK5M_300  2082
#define SYSCLK5M_600  1040
#define SYSCLK5M_1200  519
#define SYSCLK5M_2400  259
#define SYSCLK5M_4800  129
#define SYSCLK5M_7200  85
#define SYSCLK5M_9600  64
#define SYSCLK5M_19200  31
#define SYSCLK5M_38400  15
#define SYSCLK5M_57600  9
#define SYSCLK5M_115200  4
#define SYSCLK10M_300  4165
#define SYSCLK10M_600  2082
#define SYSCLK10M_1200  1040
#define SYSCLK10M_2400  519
#define SYSCLK10M_4800  259
#define SYSCLK10M_7200  172
#define SYSCLK10M_9600  129
#define SYSCLK10M_19200  64
#define SYSCLK10M_38400  31
#define SYSCLK10M_57600  20
#define SYSCLK10M_115200  9
#define SYSCLK15M_300  6249
#define SYSCLK15M_600  3124
#define SYSCLK15M_1200  1561
#define SYSCLK15M_2400  780
#define SYSCLK15M_4800  389
#define SYSCLK15M_7200  259
#define SYSCLK15M_9600  194
#define SYSCLK15M_19200  96
#define SYSCLK15M_38400  47
#define SYSCLK15M_57600  31
#define SYSCLK15M_115200  15
#define SYSCLK20M_300  8332
#define SYSCLK20M_600  4165
#define SYSCLK20M_1200  2082
#define SYSCLK20M_2400  1040
#define SYSCLK20M_4800  519
#define SYSCLK20M_7200  346
#define SYSCLK20M_9600  259
#define SYSCLK20M_19200  129
#define SYSCLK20M_38400  64
#define SYSCLK20M_57600  42
#define SYSCLK20M_115200  20
#define SYSCLK25M_300  10415
#define SYSCLK25M_600  5207
#define SYSCLK25M_1200  2603
#define SYSCLK25M_2400  1301
#define SYSCLK25M_4800  650
#define SYSCLK25M_7200  433
#define SYSCLK25M_9600  324
#define SYSCLK25M_19200  161
#define SYSCLK25M_38400  80
#define SYSCLK25M_57600  53
#define SYSCLK25M_115200  26

/* Definitions for SCICR */
/* Stop bit @ SCICR.7  */
#define SCI_1STOPBIT		0x00
#define SCI_2STOPBITS		0x80
/* Parity @ SCICR.6 */
#define SCI_ODDPARITY		0x00
#define SCI_EVENPARITY		0x40
/* Parity enable @ SCICR.5*/
#define SCI_DISABLEPARITY	0x00
#define SCI_ENABLEPARITY	0x20
/* SCI ENA @ SCICR.4 */
#define SCI_ENABLE			0x10	/* Must be set for 240, not true for 243*/
/* ADDR/IDLE mode @ SCICR.3 */
#define SCI_IDLEMODE		0x00
#define SCI_ADDRMODE		0x08
/* SCI CHAR @ SCICR.2-0 */
#define SCI_1BITDATA		0x00
#define SCI_2BITDATA		0x01
#define SCI_3BITDATA		0x02
#define SCI_4BITDATA		0x03
#define SCI_5BITDATA		0x04
#define SCI_6BITDATA		0x05
#define SCI_7BITDATA		0x06
#define SCI_8BITDATA		0x07

/* Definitions for SCICTL1 */
/* RX Err Int ENA	@ SCICTL1.6 */
#define RXERRINT_DISABLE	0x00 
#define RXERRINT_ENABLE		0x40
/* CLOCK ENA @ SCICTL1.4 */
#define INCLOCK_DISABLE		0x00
#define INCLOCK_ENABLE		0x10
/* TXWAKE @ SCICTL1.3 */
#define TXWAKE_DISABLE		0x00
#define TXWAKE_ENABLE		0x08
/* SLEEP @ SCICTL1.2	*/
#define SLEEP_DISABLE		0x00
#define SLEEP_ENABLE		0x04
/* TXENA @ SCICTL1.1	*/
#define TX_DISABLE			0x00
#define TX_ENABLE			0x02
/* RXENA @ SCICTL1.0	*/
#define RX_DISABLE			0x00
#define RX_ENABLE			0x01
/* Definitions for SCICTL2 */
/* Test pattern for Tx ready */
#define SCI_TXRDY			0x80
/* Test pattern for Tx Empty */
#define SCI_TXEMPTY			0x40
/* RX/BK INT ENA @ SCICTL2.1 */
#define RXBKINT_DISABLE		0x00
#define RXBKINT_ENABLE		0x02
/* TX INT ENA @ SCICTL2.0 */
#define TXINT_DISABLE		0x00
#define TXINT_ENABLE		0x01
/* Definitions of test patterns for SCIRXST */
#define SCI_RXERR			0x80
#define SCI_RXRDY			0x40
#define SCI_BRKDT			0x20
#define SCI_FRAMEERR		0x10
#define SCI_OVERRUN			0x08
#define SCI_PARITYERR		0x04
#define SCI_RXWAKE			0x02
/* Definitions for SCIPRI */
#define SCITXPRI_HIGH	0x00
#define SCITXPRI_LOW	0x40
#define SCIRXPRI_HIGH	0x00
#define SCIRXPRI_LOW	0x20
#define SCIPRI_HIGH		0x00
#define SCIPRI_LOW		0x60
      
/* Definitions of requests of sci_ioctl */
#define SCI_STOPTX			1
#define SCI_STOPRX			2
#define SCI_SWRESET			3
#define SCI_EMRGTX			4
#define SCI_TXPROGRESS		5
#define SCI_RXPROGRESS		6

#endif
