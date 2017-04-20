
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

int dataPin = 9;
int latchPin = 10;
int clockPin = 11;

byte dec_digits[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000010,0b11111000,0b10000000,0b10010000 };
byte decimal_point_mask = 0b01111111;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  DateTime now = RTC.now();
  int thehour = now.hour();
  // switch between 24 hour clock and 12 hour clock
  if (thehour > 12 ) thehour -= 12;
  int hour_tens = thehour / 10;
  int hour_ones = thehour % 10;
  int minute_tens = now.minute() / 10;
  int minute_ones = now.minute() % 10;
  if (now.second()% 2 == 0) decimal_point_mask = 0b01111111;
  else decimal_point_mask = 0b11111111;
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[minute_ones]);
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[minute_tens]);
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[hour_ones] & decimal_point_mask); 
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[hour_tens]);
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
  // pause before next value:
  delay(100);
}

