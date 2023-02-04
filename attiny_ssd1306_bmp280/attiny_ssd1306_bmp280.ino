/*  BatteryTesterAttiny.ino  -
  2015 Copyright (c) Andreas Spiess  All right reserved.
  Author: Andreas Spiess
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

//#include <TinyOzOLEDlimpfish.h>
#include <TinyOzOLED.h>
#include <TinyWireM.h>

#include <forcedClimate.h>



// ATtiny Pin 5 = SDA                   ATtiny Pin 6 = (D1) to LED2
// ATtiny Pin 7 = SCK                    ATtiny Pin 8 = VCC (2.7-5.5V)

#define FET 1          // ATtiny Pin 6
#define ADCVOLT A2     // ATtiny Pin 3
#define ADCCURRENT A3  // ATtiny Pin 2

const float FACTOR = 2.2;    // Adjust for 2:1 Voltage divider and internal reference

// Create an instance of a climate sensor. The parameters I2C bus and I2C address are optional. For example:
// - ForcedClimate climateSensor = ForcedClimate(Wire, 0x76);
// - ForcedClimate climateSensor = ForcedClimate(TinyWireM, 0x77);
ForcedClimate climateSensor = ForcedClimate(TinyWireM, 0x76);



void setup() {
  analogReference(INTERNAL);      // use precise internal reference

  climateSensor.begin();

  OzOled.init();  //initialze OLED display
  
  OzOled.clearDisplay();           //clear the screen and set start position to top left corner
  OzOled.setNormalDisplay();       //Set display to Normal mode

  OzOled.sendCommand(0xA1);        // set Orientation
  OzOled.sendCommand(0xC8);

  pinMode(FET, OUTPUT);
}

void loop() {
  OzOled.clearDisplay();
  displayTemp();
  delay(2000);
  /*OzOled.clearDisplay();
  displayPres();
  delay(2000);*/
}

void displayHello() {
  OzOled.printString("Helloo", 0, 0, 6);
}

void displayTemp() {
  int32_t temp = climateSensor.getTemperatureCelcius(true);
  float tmp1 = temp/100;
  float tmp2 = temp%100;
  char tmp[8];
  dtostrf(tmp1, 2, 1, tmp);
  
  OzOled.printBigNumber(tmp, 0, 0, 5);

  dtostrf(tmp2, 2, 0, tmp);

  OzOled.printBigNumber(tmp, 9, 0, 5);
  //OzOled.printBiggerString(result, 0, 0, 5);
  /*OzOled.printString("Temp:", 0, 0, 6);
  OzOled.setCursorXYPixel(60,10);
  OzOled.printNumber(tmp1);
  OzOled.setCursorXYPixel(80,10);
  OzOled.printChar('.');
  OzOled.setCursorXYPixel(90,10);
  OzOled.printNumber(tmp2);*/
}

void displayPres() {
  int32_t pres = climateSensor.getPressure();
  float tmp1 = pres/100;
  char tmp[16];
  dtostrf(tmp1, 2, 2, tmp);
  OzOled.printBigNumber(tmp, 0, 0, 5);
  //OzOled.printBiggerString(tmp, 0, 0, 5);

  /*OzOled.printString("Pressure:");
  OzOled.setCursorXYPixel(80,10);
  OzOled.printNumber(tmp1);*/
}