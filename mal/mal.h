#pragma once
#include <Arduino.h>

int life();


class Every {
  public:
  Every(unsigned long interval) : interval(interval) {}
  bool rising();
  private:
  unsigned long interval, start = millis();
  
};
