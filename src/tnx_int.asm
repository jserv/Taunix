;/* ----------------------------------------------------------------------- 
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

	.global _c_int0
	.global _c_int1
	.global _c_int2
	.global _c_int3
	.global _c_int4
	.global _c_int5
	.global _c_int6
	.global _TNX_PHANTOMINT
	.global _c_int8 ;	TNX_TRAP
	.global _c_int9 ;	TNX_NMI
	.sect ".vectors"
INT_VETTAB:
	B	_c_int0
	B	_c_int1
;	B	_TNX_PHANTOMINT
	B	_c_int2
;	B	_TNX_PHANTOMINT
	B	_c_int3        
;	B	_TNX_PHANTOMINT
;	B	_c_int4        
	B	_TNX_PHANTOMINT
	B	_c_int5
;	B	_c_int6
	B	_TNX_PHANTOMINT
	B	_TNX_PHANTOMINT	;Replace int7
	B	_TNX_PHANTOMINT	;Replace int8
	B	_TNX_PHANTOMINT	;Replace int9
	B	_TNX_PHANTOMINT	;Replace intA
	B	_TNX_PHANTOMINT	;Replace intB
	B	_TNX_PHANTOMINT	;Replace intC
	B	_TNX_PHANTOMINT	;Replace intD
	B	_TNX_PHANTOMINT	;Replace intE
	B	_TNX_PHANTOMINT	;Replace intF
	B	_TNX_PHANTOMINT	;Replace int10
	B	_c_int8
	B	_TNX_PHANTOMINT ;_c_int9
	.end
