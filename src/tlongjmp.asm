;/*----------------------------------------------------------------------- 
;* 
;*   This program is free software; you can redistribute it and/or modify
;*   it under the terms of the GNU General Public License as published by
;*   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
;*   USA; either version 2 of the License, or (at your option) any later
;*   version.
;*   
;*   This program is distributed in the hope that it will be useful,
;*   but WITHOUT ANY WARRANTY; without even the implied warranty of
;*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;*   GNU General Public License for more details.
;*
;* ----------------------------------------------------------------------- */

	.global	_tlongjmp
_tlongjmp:
	MAR		*-
	LAR		AR2,*-
	LACC	*,0,AR2
	BCND	C_1,NEQ
	LACL	#1h
C_1:
	PSHD	*+
	LAR		AR1,*+
	LAR		AR0,*+
	LAR		AR6,*+
	LAR		AR7,*,AR1
	CLRC	INTM
	RET
	.end
