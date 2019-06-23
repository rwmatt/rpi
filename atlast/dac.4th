1
440 constant freq 	\ this is the frequncy we want to output
1000000 constant 1m 	\ work in microseconds
1m freq / constant T 	\ time period

2.0 3.14159 f* freq float f* 1m float f/ constant scale
\ you should use the mod operator to bring
\ usecs within a reasonable range for sine
: vol usecs T mod float scale f* sin 127.0 f* fix 128 + ;
\ the inner loop of tone takes about 28us
: tone begin vol 25 dacw key? until ;
tone \ press return key to exit
