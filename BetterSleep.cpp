/*
* BetterSleep.h - Library for having being able to sleep longer, using RTCmemory
* on the ESP8266, by Bart Grosman.
*/
#include "BetterSleep.h"
#include "Arduino.h"

#define INTSIZE 4
#define SLEEPTIMEHOURS 1 // Hours to sleep per check
#define USINHOUR 3600000000 // 3600000000
extern "C" {
  #include "user_interface.h" // this is for the RTC memory read/write functions
}

BetterSleep::BetterSleep(uint32_t magicNumber, uint32_t memOffset)
{
  _magicNumber = magicNumber;
  _magicOff = memOffset;
  _toSleepCountOff = memOffset + INTSIZE;
  this->setupRTCmemory();
}

void BetterSleep::setupRTCmemory()
{
  uint32_t magicInt;
  system_rtc_mem_read(_magicOff, &magicInt, INTSIZE);

  if (magicInt != _magicNumber){ // First boot
    // Set the reset counters to 0
    uint32_t toSleepCount = 0;
    system_rtc_mem_write(_toSleepCountOff, &toSleepCount, INTSIZE);

    // Set the magicInt
    magicInt = _magicNumber;
    system_rtc_mem_write(_magicOff, &magicInt, INTSIZE);
  }
}

bool BetterSleep::checkSleep() {
  uint32_t toSleepCount;
  system_rtc_mem_read(_toSleepCountOff, &toSleepCount, INTSIZE);

  if (toSleepCount > 0) {
    toSleepCount--;
    system_rtc_mem_write(_toSleepCountOff, &toSleepCount, INTSIZE); // update in rtcmem

    // sleep the ESP
    uint64_t deepSleepTime = SLEEPTIMEHOURS * USINHOUR;
    ESP.deepSleep(deepSleepTime);
    return true;
  } else {
    return false;
  }
}

void BetterSleep::sleep(uint64_t sleepTime) {
  uint64_t deepSleepTime = SLEEPTIMEHOURS * USINHOUR;

  if (sleepTime > deepSleepTime) { // deepsleeping for too long, needs to be chained
    uint32_t toSleepCount = sleepTime / deepSleepTime;
    uint64_t remainder = sleepTime % deepSleepTime;
    if (remainder > 0) {
      system_rtc_mem_write(_toSleepCountOff, &toSleepCount, INTSIZE); // update in rtcmem
      ESP.deepSleep(remainder);
    } else { // It's a perfect factor of deepSleepTime so already do one
      toSleepCount--;
      system_rtc_mem_write(_toSleepCountOff, &toSleepCount, INTSIZE); // update in rtcmem
      ESP.deepSleep(deepSleepTime);
    }
  }
  else { // Sleeping for less than SLEEPTIMEHOURS, go to sleep
    ESP.deepSleep(sleepTime);
  }
}