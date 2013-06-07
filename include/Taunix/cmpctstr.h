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
 CMPCTSTR.H
 Declarations of string tools, just fitting to SCI module
 
*/
/* In TI DSP, its basic memory unit is WORD, 2 bytes.
   And it's the same with char type data. Therefore
   it will waste space if your data type is based on 
   1-byte basis. So one can use cmpctstr/xpndstr to 
   compact/expand strings.
   
   The following figures are what they do:
   
   cmpctstr:
   
                Brfore call      After call
char *string   +-----+-----+    +-----+-----+
         [0]   |     |  T  |    |  e  |  T  |
               +-----+-----+    +-----+-----+
         [1]   |     |  e  |    |  t  |  s  |
               +-----+-----+    +-----+-----+
         [2]   |     |  e  |    |     |     |
               +-----+-----+    +-----+-----+
         [3]   |     |  t  |    |     |     |
               +-----+-----+    +-----+-----+    
                Upper Lower      Upper Lower
               
    xpndstr:
                Brfore call      After call
char *src      +-----+-----+    +-----+-----+   char *dest
         [0]   |  e  |  T  |    |     |  T  |
               +-----+-----+    +-----+-----+
         [1]   |  t  |  s  |    |     |  e  |
               +-----+-----+    +-----+-----+
         [2]   |     |     |    |     |  s  |
               +-----+-----+    +-----+-----+
         [3]   |     |     |    |     |  t  |
               +-----+-----+    +-----+-----+    
                Upper Lower      Upper Lower
*/
    
void cmpctstr(char *string,unsigned int size);
void xpndstr(char *src,char *dest,unsigned int size);
