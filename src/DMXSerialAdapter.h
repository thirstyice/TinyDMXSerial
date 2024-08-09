#pragma once


// register interrupt for receiving data and frameerrors that calls _DMXReceived()
void _DMXReceived(uint8_t data, uint8_t frameerror);
void _DMXTransmitted();


// These functions all exist in the processor specific implementations:
void _DMX_init();
void _DMX_setMode();
void _DMX_writeByte(uint8_t data);
void _DMX_flush();


// ----- Serial UART Modes -----

// There are 4 different modes required while receiving and sending DMX using the Serial

// State of receiving DMX Bytes
typedef enum {
  OFF = 0, // Turn off
  RONLY = 1, // Receive DMX data only
  RDATA = 2, // Receive DMX data + Interrupt
  TBREAK = 3, // Transmit DMX Break + Interrupt on Transmission completed
  TDATA = 4, // Transmit DMX Data + Interrupt on Register empty
  TDONE = 5 // Transmit DMX Data + Interrupt on Transmission completed
} __attribute__((packed)) DMXUARTMode;


// Baud rate for DMX protocol
#define DMXSPEED 250000L

// the break timing is 10 bits (start + 8 data + 1 (even) parity) of this speed
// the mark-after-break is 1 bit of this speed plus approx 6 usec
// 100000 bit/sec is good: gives 100 usec break and 16 usec MAB
// 1990 spec says transmitter must send >= 92 usec break and >= 12 usec MAB
// receiver must accept 88 us break and 8 us MAB
#define BREAKSPEED 100000L

#define BREAKFORMAT SERIAL_8E2
#define DMXFORMAT SERIAL_8N2
#define DMXREADFORMAT SERIAL_8N1


// ----- include processor specific definitions and functions.

#if defined(ARDUINO_ARCH_AVR)
#include "DMXSerial_avr.h"

#elif defined(ARDUINO_ARCH_MEGAAVR)
#include "DMXSerial_megaavr.h"

#endif
