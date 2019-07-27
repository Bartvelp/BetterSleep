/*
* BetterSleep.h - Library for having being able to sleep longer, using RTCmemory
* on the ESP8266, by Bart Grosman.
*/
#ifndef BetterSleep_h
#define BetterSleep_h
#include "Arduino.h"

class BetterSleep
{
public:
  BetterSleep(uint32_t magicNumber, uint32_t memOffset = 90); // Default memoryOffset is 90 because that fits best in my usecase
  bool checkSleep();
  void sleep(unsigned long long int sleepTime);

private:
  uint32_t _magicOff;
  uint32_t _toSleepCountOff;
  uint32_t _magicNumber;
  void setupRTCmemory();
};

#endif