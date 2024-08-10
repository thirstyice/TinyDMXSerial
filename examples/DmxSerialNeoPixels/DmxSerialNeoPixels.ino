// - - - - -
// DmxSerialNeoPixels.ino: Sample DMX application for retrieving 3 DMX values:
//
// Copyright (c) 2016 by Matthias Hertel, http://www.mathertel.de
//
// TinyDMXSerial - DMXSerial backend with a revised frontend
// Copyright (c) 2024 Tauran - https://github.com/thirstyice
//
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
//
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 06.09.2016 Creation of DmxSerialNeoPixels sample.
// 27.08.2017 working with new DMXSerial DMXProbe mode.
//            cleanup.
//
// 2024-08-09 Implemented TinyDMXSerial frontend
//
// - - - - -

#include <Arduino.h>
#include <TinyDMXSerial.h>

#include "ws2812.h"

// Constants for demo program

const int RedPin =    9;  // PWM output pin for Red Light.
const int GreenPin =  6;  // PWM output pin for Green Light.
const int BluePin =   5;  // PWM output pin for Blue Light.

// number of RGB neopixels, RGB channels are transfered
// warning: try with 12 first and scale up carefully.
#define PIXELS 60

// first DMX start address
#define DMXSTART 0

// number of DMX channels used
#define DMXLENGTH (PIXELS*3)

uint8_t data[DMXLENGTH];
TinyDMXSerial DMX(data, DMXLENGTH);

// Initialize DMXSerial and neo pixel output
void setup () {
  int n = 0;
  DMX.begin(DMXMode::Receiver, DMXSTART);

  // enable pwm outputs
  pinMode(RedPin,   OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin,  OUTPUT);

  // setup the neopixel output
  setupNeopixel();

  // give them a decent color...
  for (int p = 0; p < PIXELS; p++) {
    data[n++]=5;
    data[n++]=10;
    data[n++]=20;
  }
  updateNeopixel(data, PIXELS);

} // setup ()


// do constantly fetch DMX data and update the neopixels.
void loop() {
  // If there's data
  if (millis() - DMX.lastReceivedTime() < 1000) {
    analogWrite(RedPin,   data[0]);
    analogWrite(GreenPin, data[1]);
    analogWrite(BluePin,  data[2]);
    updateNeopixel(data, PIXELS);

  } else {
    // don't update the Neopixels but signal a red.
    analogWrite(RedPin,   100);
    analogWrite(GreenPin, 0);
    analogWrite(BluePin,  0);
  } // if

} // loop()


// The End.
