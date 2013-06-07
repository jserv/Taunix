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

asm("	.global	I$$SAVE");
asm("	.global I$$REST");
void TNX_PHANTOMINT()	{	asm("	CLRC	INTM");	}
void (*TNX_USER_TRAP)() = 0;
void (*TNX_USER_NMI)() = 0;

void c_int8()	/* For trap */
{
	if(TNX_USER_TRAP)
		TNX_USER_TRAP();
}

void c_int9()	/* For NMI */
{
	if(TNX_USER_NMI)
		TNX_USER_NMI();
}
