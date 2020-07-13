/*
 * Motor.h
 *
 *  Created on: 11.9.2019
 *      Author: Valtsu
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "FreeRTOS.h"
#include "DigitalIoPin.h"
#include "semphr.h"
#include "Debug.h"

enum Direction
{
	D_LEFT, D_RIGHT
};

struct PortPinPair
{
	int port;
	int pin;
};

class Motor
{
public:

	Motor(PortPinPair _dire, PortPinPair _step, PortPinPair _lim1, PortPinPair _lim2, Debug *_debug);
	virtual ~Motor();
	void SwitchDirection();
	void SetDirection(Direction dir);
	void MesureSteps();
	void SlowGoSteps(int count, int speed);
	bool CheckSwitches();
	void GoStep();
	void Center();
	int GetPlace();
	int GetSteps();

private:

	DigitalIoPin dire;
	DigitalIoPin step;
	DigitalIoPin *lim1;
	DigitalIoPin *lim2;

	Direction currentDirection;

	Debug *debuger;
	int steps = 0;
	volatile int curPlace;
};

#endif /* MOTOR_H_ */
