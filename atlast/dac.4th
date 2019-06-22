1
variable start usecs start !
variable now
\ variable q  : q! 1 q ! ;

\ : -usecs usecs 1000000 mod ;
\ : -usecs now @ start @  - 8000 mod ;
: secs usecs float 1000000.0 f/ ;

440.0 constant freq
: vol secs  freq f* 2.0 f/ 3.1412 f/  sin 127.0 f* fix 128 + ;

: looper 
\ 0 q !
begin 
	key? if exit then 
	usecs now !
	now @ start @ - 125  >=  if
		vol 25 dacw
		now @ start !
	then	
again  
;

