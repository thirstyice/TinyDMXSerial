/*******************************************************************************
* Project: TinyDMXSerial                                                       *
* Filename: /src/TinyDMXSerial.h                                               *
*                                                                              *
* Created: 2024-08-09                                                          *
* Author: thirstyice                                                           *
*                                                                              *
* Copyright (c) 2024 Tauran - https://github.com/thirstyice                    *
* For details see TinyDMXSerial/LICENSE                                        *
*                                                                              *
*******************************************************************************/

#pragma once

#include <Arduino.h>

typedef enum {
	Disabled,
	Receiver,
	Sender,
} DMXMode;

struct DMXRecvState : Printable {
	enum {
		Startup,
		Idle,
		Break,
		Data,
		Done
	} state;
	size_t printTo(Print& p) const override {
		String output = "";
		switch (state) {
			case Startup:
				output = F("Startup");
			break;
			case Idle:
				output = F("Idle");
			break;
			case Break:
				output = F("Break");
			break;
			case Data:
				output = F("Data");
			break;
			case Done:
				output = F("Done");
			break;
			default:
			break;
		}
		p.print(output);
		return output.length();
	}
};

class TinyDMXSerial {
public:
	TinyDMXSerial(
		uint8_t* const buffer,
		uint16_t numChannels = 512,
		uint8_t modePin = 2,
		bool invertModePin = false
	) :
		dmxData(buffer),
		dataLength(numChannels),
		modePinInverted(invertModePin),
		pin(modePin)
	{};

	void begin(DMXMode mode, int16_t startAddress=0);
	void end();
	TinyDMXSerial* setLoggingOutput(Print*);
	TinyDMXSerial* setStartAddress(uint16_t startAddress);
	TinyDMXSerial* resetUpdated() {updated = false; return this;}
	unsigned long lastReceivedTime();
	bool dataUpdated() {return updated;}


	// Interrupt handlers
	void didReceive(uint8_t data, uint8_t frameError);
	void didTransmit();

private:
	template <typename T>
	void log(T);
	template <typename T>
	void logln(T item) {log(item); log('\n');}
	uint8_t getPinValueForMode(DMXMode);
	uint8_t* const dmxData;
	Print* logger = nullptr;
	DMXMode dmxMode;
	const uint16_t dataLength;
	const bool modePinInverted;
	const uint8_t pin;
	int16_t startAddr = 0;
	int16_t endAddr = 0;
	int16_t dataAddr = 0;
	volatile unsigned long lastRecv = 0;
	DMXRecvState recvState;
	bool updated = false;
};