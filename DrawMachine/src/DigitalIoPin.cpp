/*
 * DigitalIoPin.cpp
 *
 *  Created on: 22.1.2019
 *      Author: Akseli
 */

#include "DigitalIoPin.h"
#include "chip.h"
#include "board.h"

DigitalIoPin::DigitalIoPin(int _port, int _pin, bool _input = true, bool _pullup = true, bool _invert = false)
{
	port = _port;
	pin = _pin;
	input = _input;
	pullup = _pullup;
	invert = _invert;

	InitPin();
}

DigitalIoPin::DigitalIoPin()
{

}

DigitalIoPin::~DigitalIoPin()
{

}

bool DigitalIoPin::Read()
{
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

void DigitalIoPin::Write(bool value)
{
	if (!input)
	{
		if (invert)
		{
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, !value);
		}
		else
		{
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, value);
		}

	}
}

void DigitalIoPin::InitPin()
{
	if (input)
	{
		if (pullup && !invert)
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
		}
		else if (invert && !pullup)
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_DIGMODE_EN | IOCON_INV_EN));
		}
		else if (!pullup && !invert)
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_DIGMODE_EN));
		}
		else if (pullup && invert)
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
		}

		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);

	}
	else
	{
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_INACT | IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}
}

