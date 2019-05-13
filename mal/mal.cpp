#include "mal.h"

int life() { return 42; }

bool Every::rising() { 
    unsigned long now = millis();
    if(now<start) { start = now; } // timer overflow
    if(now-start < interval) return false;
    start = now;    
    return true; 
}
