# Better Deepsleep for the ESP8266
##### And with better I mean longer
This small library allows the ESP8266 to sleep longer than the normal maximal time of 3.5 hours.
It does this by breaking it up into parts of 1 hour and remembering how many times to sleep 1 hour in the RTC memory.
