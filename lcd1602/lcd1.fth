2 output: rs
4 output: e
5 output: ld4
18 output: ld5
19 output: ld6
21 output: ld7
255 constant rw
rs off
e off

: write4bits
	dup 1 and      ld4 digw
	dup 2 and 2 /  ld5 digw
	dup 4 and 4 /  ld6 digw
	8 and 8 /      ld7 digw
;

: set4 0x3 write4bits 5 delay-ms ;
set4 set4 set4
0x02 write4bits \ set to 4-bit interface

: send \ value mode --
	rs swap digw
	dup 16 / write4bits
	write4bits
;

0x28 0 send \ set the # lines, font size, etc

