// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialRecv.ino: Sample DMX application for retrieving 3 DMX values:
// address 1 (red) -> PWM Port 9
// address 2 (green) -> PWM Port 6
// address 3 (blue) -> PWM Port 5
//
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
//
// TinyDMXSerial - DMXSerial backend with a revised frontend
// Copyright (c) 2024 Tauran - https://github.com/thirstyice
//
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
//
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// 28.12.2011 changed to channels 1..3 (RGB) for compatibility with the DmxSerialSend sample.
// 10.05.2012 added some lines to loop to show how to fall back to a default color when no data was received since some time.
//
// 2024-08-09 Implemented TinyDMXSerial frontend
//
// - - - - -

#include <Arduino.h>

#include <TinyDMXSerial.h>

// Constants for demo program

const int RedPin = 9; // PWM output pin for Red Light.
const int GreenPin = 6; // PWM output pin for Green Light.
const int BluePin = 5; // PWM output pin for Blue Light.


const uint8_t numChannels = 3;

uint8_t data[numChannels];
TinyDMXSerial DMX(data, numChannels);

void setup() {
  DMX.begin(DMXMode::Receiver);

  // set some startup values
  data[0] = 80;
  data[1] =  0;
  data[2] = 0;

  // enable pwm outputs
  pinMode(RedPin, OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
}


void loop() {
  // Calculate how long since last data packet was received
  unsigned long lastPacket = millis() - DMX.lastReceivedTime();

  if (lastPacket < 5000) {
    // read recent DMX values and set pwm levels
    analogWrite(RedPin, data[0]);
    analogWrite(GreenPin, data[1]);
    analogWrite(BluePin, data[2]);

  } else {
    // Show pure red color, when no data was received since 5 seconds or more.
    analogWrite(RedPin, 255);
    analogWrite(GreenPin, 0);
    analogWrite(BluePin, 0);
  } // if
}

// End.
