#include "Arduino.h"
#include "BetterSleep.h"
#define USINHOUR 3600000000 // 3600000000

/*
  Initialize with a 'magic number', use any number you like but I recommend something that is obviously not random.
 */
BetterSleep betterSleep(123456); 

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("BetterSleep example sketch");
  Serial.println("Normal max time asleep (us): " + uint64ToString(ESP.deepSleepMax()));

  bool isGoingToSleep = betterSleep.checkSleep(); // Checks if it should go to sleep, always put this at the start of setup()
  Serial.print("isGoingToSleep: "); // almost always will be false, otherwise it is asleep already
  Serial.println(isGoingToSleep);
  uint64_t hour4us = 4 * USINHOUR; // This can't be done normally with the ESP deepsleep
  Serial.println("Going to sleep for 4 hours: " + uint64ToString(hour4us));
  betterSleep.sleep(hour4us);
}

void loop() {
  // Never reached
} 

String uint64ToString(uint64_t input) { // Helper function to print 64 bit integers, no need to include!
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c +='0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}
