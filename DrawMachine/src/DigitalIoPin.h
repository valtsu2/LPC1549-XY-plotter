/*
 * DigitalIoPin.h
 *
 *  Created on: 22.1.2019
 *      Author: Akseli
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

class DigitalIoPin
{
public:
	DigitalIoPin(int _port, int _pin, bool _input, bool _pullup, bool _invert);
	DigitalIoPin();
	virtual ~DigitalIoPin();
	bool Read();
	void Write(bool value);

private:
	int port;
	int pin;
	int input;
	int pullup;
	int invert;

	void InitPin();

};

#endif /* DIGITALIOPIN_H_ */
