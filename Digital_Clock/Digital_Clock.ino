/*
 
 Copyright 2014 Dean Reading
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 

 */
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
#include "SevSeg.h"

SevSeg sevseg; //Instantiate a seven segment controller object

void setup() {
  
  Wire.begin();
  RTC.begin();
 //RTC.adjust(DateTime(__DATE__, __TIME__));
// if you need set clock... just remove // from line above this

// part code for flashing LED
Wire.beginTransmission(0x68);
Wire.write(0x07); // move pointer to SQW address
// Wire.write(0x00); // turns the SQW pin off
 Wire.write(0x10); // sends 0x10 (hex) 00010000 (binary) to control register - turns on square wave at 1Hz
// Wire.write(0x13); // sends 0x13 (hex) 00010011 (binary) 32kHz

Wire.endTransmission();
  byte numDigits = 4;   
  byte digitPins[] = {6, 9, 10, 12};//change this according to design
  byte segmentPins[] = {7, 11, 14, 16, 17, 8, 13, 15};//this too
  

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(100);
}

void loop() {

 DateTime now = RTC.now();
 int hou;
 hou = now.hour();
 if(hou>12)
 {
  hou=hou-12;
 }
  int timp = hou*100+now.minute();
 // static int time1= 0;

  sevseg.setNumber(timp, 2);
  
  sevseg.refreshDisplay(); // Must run repeatedly
}

/// END ///
