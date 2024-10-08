# TinyDMXSerial

This is a library for sending and receiving DMX packets using the Arduino platform, based on the backend parts of [DMXSerial](http://www.mathertel.de/Arduino/DMXSerial.aspx), but with a completely rewritten frontend.
You can implement DMX devices and DMX controllers with this library.

Because it uses the DMXSerial backend, the DMX communication implemented by the TinyDMXSerial library relies completely on the hardware support of a builtin USART / Serial port interface by using interrupts that handle all I/O transfer in the background.


## Supported Boards and processors

The supported processors and Arduino Boards are:
* Arduino UNO, Ethernet, Fio, Nano and Arduino Pro, Arduino Pro Mini (ATmega328 or ATmega168)
* Arduino Mega2560 (ATmega2560)
* Arduino Leonardo (Atmega32U4)
* Arduino nano Every (ATMEGA4809)
* ATmega8 boards - experimental
* ATtiny controllers - work in progress (focusing on attiny1634)

Other compatible boards may work as well.

You can find more detail on this library at https://github.com/thirstyice/TinyDMXSerial


## Compile for other Serial ports

The original library was written for Arduino 2009 and Arduino UNO boards that use the ATmega328 or ATmega168 chip.
These chips only have a single Universal Synchronous and Asynchronous serial Receiver and Transmitter (USART) a.k.a. the Serial port in the Arduino environment.
The DMXSerial library uses this port for sending or receiving DMX signals by default.

This conflicts a little bit with the usage of the programming mode the Arduino offers through then USB ports or serial interfaces but it is possible to build compatible DMX shields that don’t interfere with this usage if done right. The DMXShield is an example of this.

By using the Arduino Leonardo, MEGA or Every board you can write debugging information to the Serial port in your sketch for debugging or data exchange purpose as the processor supports multiple ports.
So maybe a DMX diagnostic sketch and debugging output in your code can profit from that.
The library also has support for logging errors to a `Print` compatible place of your choosing.

**Arduino Leonardo, Arduino Esplora**

When compiling for a Arduino Leonardo board the library will choose the **Serial1** port by default for DMX communication.

The Arduino Leonardo board uses a chip that has a USB 2.0 port available in addition to the USART. Therefore the “Serial” port is routed through the USB port that behaves like a serial port device and not the built-in USART. If you like to address the real Serial port you have to go with the Serial1.

When you look at the hardware and the registers definitions and manual for the processor the USART0 still exists but the definitions for addressing the registers have changed
(for example USART_RX_vect is now USART0_RX_vect). Therefore some adjustments had to be done.


**Arduino MEGA 2560**

When using the chip on the Arduino MEGA 2560 board you have more than one USART available and maybe you wish to use port 1 instead of port 0.
This can be done by enabling the definitions for port1 in the library in file `src\DMXSerial_avr.h` just uncomment the line

```CPP
#define DMX_USE_PORT1
```

**MEGAAVR processors like 4809 in Arduino nano Every**

For this processor the USART1 is used for DMX.


**ATmega8 boards**

I added the definitions of boards based on the ATmega8 too for experimental purpose. If you find problems with these boards, leave me a note please.



## Supported Shields

A suitable hardware is the Arduino platform plus a shield for the DMX physical protocol implementation.
You can find such a shield at: http://www.mathertel.de/Arduino/DMXShield.aspx.

Without or with some modification this library can also used with other DMX shields
that use the Serial port for connecting to the DMX bus.


## License

DMXSerial Copyright (c) 2005-2020 by Matthias Hertel,  http://www.mathertel.de/

TinyDMXSerial Copyright (c) 2024 Tauran - https://github.com/thirstyice

The detailed Software License Agreement can be found at: http://www.mathertel.de/License.aspx
