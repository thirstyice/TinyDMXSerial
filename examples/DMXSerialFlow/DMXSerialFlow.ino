// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DMXSerialFlow.ino: Sample DMX application for sending 60 DMX values.
// Copyright (c) 2014-2015 by Matthias Hertel, http://www.mathertel.de
//
// TinyDMXSerial - DMXSerial backend with a revised frontend
// Copyright (c) 2024 Tauran - https://github.com/thirstyice
//
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
//
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 01.07.2013 published version of the example applikation
//
// 2024-08-09 Implemented TinyDMXSerial frontend
//
// This is an example of how to send a more complex RGB based pattern of colors
// over a DMX line.
// The values for one of the channels can be watched at the PWM outputs of the Arduino Board.
// - - - - -

#include <TinyDMXSerial.h>

// Constants for demo program

const int RedPin =    9;  // PWM output pin for Red Light.
const int GreenPin =  6;  // PWM output pin for Green Light.
const int BluePin =   5;  // PWM output pin for Blue Light.

uint8_t data[512];
TinyDMXSerial DMX(data);

void setup(void)
{
  // Serial.begin(57600); // only on Leonardo
  // Serial.println("DMXSerialFlow DMX Example");

  DMX.begin(DMXMode::Sender);

  pinMode(RedPin,   OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin,  OUTPUT);

  analogWrite(RedPin, 80);
  analogWrite(GreenPin, 80);
  analogWrite(BluePin, 80);
} // setup


// set 3 channels to a RGB value with the specified hue (0...764)
void setChannelRGB(int channel, int hue) {
  hue = hue % 765;
  if (hue < 256) {
    /// blue to red
    data[channel] = hue;
    data[channel+1] = 0;
    data[channel+2] = 255-hue;

  } else if (hue < 511) {
    /// red to green
    data[channel] = 255 - (hue-255);
    data[channel+1] = hue-255;
    data[channel+2] = 0;

  } else {
    /// green to blue
    data[channel] = 0;
    data[channel+1] = 255 - (hue-510);
    data[channel+2] = hue-510;
  } // if
} // setChannelRGB()


void loop(void)
{
  unsigned long now = millis();
  int testchannel = 0;

  // create some DMX test values: 5 RGB channels
  // adjust the "12" for changing the speed
  int alpha = (now / 12) % 765;

  // uncomment this line to have a scenario where DMX values are changed ~ 2 times the second
  // alpha = (alpha / 64) * 64;
  // uncomment this line to have a scenario where DMX values are changed every 5 seconds
  // alpha &= 0xFF00;

  // setup for 60 devices with RGB (3) channels; len = 180
  for (int n = 0; n < 60; n++) {
    setChannelRGB(n*3, alpha + n*64);
  } // for

  // send the DMX values for the testchannel to the PWM pins
  analogWrite(RedPin,   data[testchannel+0]);
  analogWrite(GreenPin, data[testchannel+1]);
  analogWrite(BluePin,  data[testchannel+2]);
}

// End
