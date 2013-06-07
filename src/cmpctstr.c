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

void cmpctstr(char *string,unsigned int size)
{
 register unsigned i, rsize = size;
 register char *rstring = string;
	for(i=0 ; i<rsize ; i++)
		rstring[i>>1] = (i&0x0001)?((rstring[i]<<8)|rstring[i>>1]):rstring[i];
}

void xpndstr(char *src,char*dest,unsigned int size)
{
 register unsigned i, rsize = size;
 register char *rsrc = src, *rdest = dest;
 	for(i=0 ; i<rsize ; i++)
 		rdest[i] = (i&0x0001)?rsrc[i>>1]>>8:(rsrc[i>>1]&0x00FF);
}
