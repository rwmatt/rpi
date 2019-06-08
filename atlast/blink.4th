2 output: led
: 500ms 500 delay-ms ;
: blink led on 500ms led off 500ms ;
: loop begin blink key? if "Finished" type exit then again ;
loop
