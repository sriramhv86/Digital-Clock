
#include <Wire.h>
#include "RTClib.h"
#include "TimeLib.h"

RTC_DS1307 RTC;

int dataPin = 12;
int latchPin = 10;
int clockPin = 11;

//Buttons to alter time
const int buttonPin = 7;
const int buttonPin1 = 8;
int buttonState = HIGH;
int lastButtonState = HIGH;
int buttonState1 = HIGH;
int lastButtonState1 = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime1 = 0;
unsigned long debounceDelay1 = 50;
unsigned long savetimer;


byte dec_digits[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000010,0b11111000,0b10000000,0b10010000 };
byte decimal_point_mask = 0b01111111;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(buttonPin1, INPUT);
  digitalWrite(buttonPin1, HIGH);
 // Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
   
  }
  //RTC.adjust(DateTime(2014, 1, 21, 10, 40, 0));
}

void loop() {
  DateTime now = RTC.now();
  int thehour = now.hour();
  int theminute = now.minute();
  // switch between 24 hour clock and 12 hour clock
  if (thehour > 12 ) thehour -= 12;
  int hour_tens = thehour / 10;
  int hour_ones = thehour % 10;
  int minute_tens = now.minute() / 10;
  int minute_ones = now.minute() % 10;
  if (now.second()% 2 == 0) decimal_point_mask = 0b01111111;
  else decimal_point_mask = 0b11111111;
  //Button to increase minutes
  {
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonState) {
    lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
      buttonState = reading;
        if (buttonState == HIGH){
        theminute++; 
        if (theminute > 60) theminute -=60; 
        RTC.adjust(DateTime(2017, 2, 3, thehour, theminute, 0));
        }
      }    
    }
  lastButtonState = reading;
  }
  //Button to increase hours
{
    int reading1 = digitalRead(buttonPin1);
      if (reading1 != lastButtonState1) {
          lastDebounceTime1 = millis();
      }
      if ((millis() - lastDebounceTime1) > debounceDelay1) {
        if (reading1 != buttonState1) {
            buttonState1 = reading1;
              if (buttonState1 == HIGH) {
              thehour++;
              if (thehour > 12) thehour -=12;
              RTC.adjust(DateTime(2017, 2, 3, thehour, theminute, 0));
              }
        }     
       }
       lastButtonState1 = reading1;
}

  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[minute_ones]);
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[minute_tens]);
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[hour_ones] & decimal_point_mask); 
  shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[hour_tens]);
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
 // Serial.print(now.second());
//  Serial.print(",");
//  Serial.println(now.minute());
  // pause before next value:
  delay(100);
}


