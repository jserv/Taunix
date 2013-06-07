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
		
/* Establish date : 1999-4-22
 * Author : Max Yin
 * Orgnization : Tau Workshop
 * File : F240mmrs.h
 * Content :
 *   TMS320C240, TMS320F240 memory-mapped registers and I/O ports on
 *   EVM board.
 */

#ifndef F240MMRS_H                                   
#define F240MMRS_H
                      
/* Global memory and CPU interrupt registers */                                   
#define IMR			0X0004
#define GREG		0X0005
#define IFR			0X0006
/* System configuration registers */
#define SYSCR		0X7018
#define SYSSR		0X701A
#define SYSIVR		0X701E
#define PIVR		0x701E
/* WD/RTI control registers */
#define RTICNTR		0X7021
#define WDCNTR		0X7023
#define WDKEY		0X7025
#define RTICR		0X7027
#define WDCR		0X7029
/* PLL clock control registers */
#define CKCR0		0X702B
#define CKCR1		0X702d
/* A/D modeule control registers */
#define ADCTRL1		0X7032
#define ADCTRL2		0X7034
#define ADCFIFO1	0X7036
#define ADCFIFO2	0X7038
/* Serial peripherial interface (SPI) configuration control registers */
#define SPICCR		0X7040
#define SPICTL		0X7041
#define SPISTS		0X7042
#define SPIBRR		0X7044
#define SPIEMU		0X7046
#define SPIBUF		0X7047
#define SPIDAT		0X7049
#define SPIPC1		0X704D
#define SPIPC2		0X704E
#define SPIPRI		0X704F
/* Serial communications interface (SCI) configuration control registers */
#define SCICCR		0X7050
#define SCICTL1		0X7051
#define SCIHBAUD	0X7052
#define SCILBAUD	0X7053
#define SCICTL2		0X7054
#define SCIRXST		0X7055
#define SCIRXEMU	0X7056
#define SCIRXBUF	0X7057
#define SCITXBUF	0X7059
#define SCIPC2		0X705E
#define SCIPRI		0X705F
/* External interrupt control registers */
#define XINT1CR		0X7070
#define NMICR		0X7072
#define XINT2CR		0X7078
#define XINT3CR		0X707A
/* Digital I/O control registers */
#define OCRA		0X7090
#define OCRB		0X7092
#define PADATDIR	0X7098
#define PBDATDIR	0X709A
#define PCDATDIR	0X709C
/* General purpose (GP) timer configuration control registers */
#define GPTCON		0X7400
#define T1CNT		0X7401
#define T1CMPR		0X7402
#define T1PR		0X7403
#define T1CON		0X7404
#define T2CNT		0X7405
#define T2CMPR		0X7406
#define T2PR		0X7407
#define T2CON		0X7408
#define T3CNT		0X7409
#define T3CMPR		0X740A
#define T3PR		0X740B
#define T3CON		0X740C
/* Full and simple compare unit registers */
#define COMCON		0X7411
#define ACTR		0X7413
#define SACTR		0X7414
#define DBTCON		0X7415
#define CMPR1		0X7417
#define CMPR2		0X7418
#define CMPR3		0X7419
#define SCMPR1		0X741A
#define SCMPR2		0X741B
#define SCMPR3		0X741C
/* Capture unit registers */
#define CAPCON		0X7420
#define CAPFIFO		0X7422
#define CAP1FIFO	0X7423
#define CAP2FIFO	0X7424
#define CAP3FIFO	0X7425
#define CAP4FIFO	0X7426
/* Event management (EV) interrupt control registers */
#define EVIMRA		0X742C
#define EVIMRB		0X742D
#define EVIMRC		0X742E
#define EVIFRA		0X742F
#define EVIFRB		0X7430
#define EVIFRC		0X7431
#define EVIVRA		0X7432
#define EVIVRB		0X7433
#define EVIVRC		0X7434
/* Wait-state generator control register */
#define WSGR		0XFFFF

/* F240 EVM board I/O addresses */
#ifdef	F240EVM
#define DAC1		0X0000
#define DAC2		0X0001
#define DAC3		0X0002
#define DAC4		0X0003
#define DACUPDATE	0X0004
#define DIPSW		0X0008
#define LEDS		0X000C
#define AVAILABLE	0X8000
#endif

extern volatile unsigned int *MMREGS;

#define MASK_INT1	0x00

#endif
