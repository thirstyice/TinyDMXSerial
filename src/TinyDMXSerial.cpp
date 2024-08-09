#include "TinyDMXSerial.h"

#include "DMXSerialAdapter.h"

bool dmxRunning = false;

TinyDMXSerial* currentInstance = nullptr;

TinyDMXSerial* TinyDMXSerial::setLoggingOutput(Print* out) {
	logger = out;
	return this;
}

template <typename T>
void TinyDMXSerial::log(T item) {
	if (logger!=nullptr) {
		logger->print(item);
	}
}

uint8_t TinyDMXSerial::getPinValueForMode(DMXMode mode) {
	bool isHigh = false;
	if (mode == DMXMode::Sender) {
		isHigh = true;
	}
	isHigh += modePinInverted;
	return isHigh ? HIGH : LOW;
}

void TinyDMXSerial::begin(DMXMode mode, int16_t startAddress) {
	if (mode == DMXMode::Disabled) {
		end();
		return;
	}
	if (dmxRunning) {
		logln(F("Failed to begin DMX: Already Running"));
		return;
	}
	setStartAddress(startAddress);
	dmxMode = mode;
	dmxRunning = true;
	currentInstance = this;
	recvState.state = DMXRecvState::Startup;
	dataAddr = 0;

	_DMX_init();

	pinMode(pin, OUTPUT);
	digitalWrite(pin, getPinValueForMode(dmxMode));
	switch (dmxMode) {
		case DMXMode::Receiver:
			_DMX_setMode(DMXUARTMode::RDATA);
			_DMX_flush();
		break;
		case DMXMode::Sender:
			_DMX_setMode(DMXUARTMode::TBREAK);
			_DMX_writeByte((uint8_t)0);
		break;
		default:
		break;
	}
}

void TinyDMXSerial::end() {
	dmxMode = DMXMode::Disabled;
	if (dmxRunning == false) {
		return;
	}
	if (currentInstance != this) {
		logln(F("Failed to end DMX: Not the current instance"));
		return;
	}
	_DMX_setMode(DMXUARTMode::OFF);
	digitalWrite(pin, getPinValueForMode(DMXMode::Disabled));
	dmxRunning = false;
	currentInstance = nullptr;
	updated = false;
}

TinyDMXSerial* TinyDMXSerial::setStartAddress(uint16_t startAddress) {
	if (startAddress >= 512) {
		logln(F("Start Address too high, rounding down"));
		startAddr = 511;
	} else {
		startAddr = startAddress;
	}
	endAddr = startAddr + dataLength - 1;
	if (endAddr >= 512) {
		logln(F("Data length too long for start address, data will be truncated"));
		endAddr = 511;
	}
	return this;
}

void TinyDMXSerial::didReceive(uint8_t data, uint8_t frameError) {
	if (recvState.state == DMXRecvState::Startup) {
		// This is the first frame; ignore it
		recvState.state = DMXRecvState::Idle;
		return;
	}
	if (frameError) {
		// This is a break condition
		recvState.state = DMXRecvState::Break;
		dataAddr = 0;
	} else if (recvState.state == DMXRecvState::Break) {
		// This is the start code
		if (data == 0) {
			// Normal DMX data follows
			recvState.state = DMXRecvState::Data;
			lastRecv = millis();
			dataAddr = 0;
		} else {
			// Not normal DMX; ignore it
			recvState.state = DMXRecvState::Done;
		}
	} else if (recvState.state == DMXRecvState::Data) {
		if (dataAddr>=startAddr && dataAddr<=endAddr) {
			if (dmxData[dataAddr-startAddr] != data) {
				updated = true;
				dmxData[dataAddr-startAddr] = data;
			}
		}
		dataAddr++;
		if (dataAddr >= 512) {
			recvState.state = DMXRecvState::Done;
		}
	}

	if (recvState.state == DMXRecvState::Done) {
		recvState.state = DMXRecvState::Idle;
	}
}

void TinyDMXSerial::didTransmit() {
	if (dmxMode == DMXMode::Sender && dataAddr >= 512) {
		_DMX_setMode(DMXUARTMode::TBREAK);
		_DMX_writeByte((uint8_t)0);
		dataAddr = -1;
	} else if (dataAddr == -1) {
		_DMX_setMode(DMXUARTMode::TDATA);
		_DMX_writeByte((uint8_t)0);
		dataAddr = 0;
	} else {
		if (dataAddr < endAddr) {
			if (dataAddr < startAddr) {
				_DMX_writeByte((uint8_t)0);
			} else {
				_DMX_writeByte(dmxData[dataAddr - startAddr]);
			}
			dataAddr++;
		} else {
			_DMX_setMode(DMXUARTMode::TDONE);
			_DMX_writeByte(dmxData[dataAddr-startAddr]);
			dataAddr = 512;
		}
	}
}

unsigned long TinyDMXSerial::lastReceivedTime() {
	return lastRecv;
}

void _DMXReceived(uint8_t data, uint8_t frameError) {
	currentInstance->didReceive(data, frameError);
}

void _DMXTransmitted() {
	currentInstance->didTransmit();
}