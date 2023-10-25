/*
 * Motor.cpp
 *
 *  Created on: 11.9.2019
 *      Author: Valtsu
 */
#include "FreeRTOS.h"
#include "Motor.h"

Motor::Motor(PortPinPair _dire, PortPinPair _step, PortPinPair _lim1,
		PortPinPair _lim2, Debug *_debug) {
	dire = DigitalIoPin(_dire.port, _dire.pin, false, false, false);
	step = DigitalIoPin(_step.port, _step.pin, false, false, false);
	lim1 = new DigitalIoPin(_lim1.port, _lim1.pin, true, true, true);
	lim2 = new DigitalIoPin(_lim2.port, _lim2.pin, true, true, true);

	debuger = _debug;
	currentDirection = D_RIGHT;
}

Motor::~Motor() {
	delete (lim1);
	delete (lim2);

}
// Switches motor direction
void Motor::SwitchDirection() {
	currentDirection = currentDirection == D_LEFT ? D_RIGHT : D_LEFT;
	SetDirection(currentDirection);
}
// Set Motor direction
void Motor::SetDirection(Direction dir) {
	currentDirection = dir;
	if (currentDirection == D_RIGHT) {
		dire.Write(1);
	} else if (currentDirection == D_LEFT) {
		dire.Write(0);
	}
}

// Go step with vtaskdelay, used for calibration
void Motor::SlowGoSteps(int count, int speed) {
	for (int i = 0; i < count; ++i) {
		GoStep();
		vTaskDelay(speed);
	}
}

// Mesures step count between limit switches
void Motor::MesureSteps() {
	const int siz = 2;
	int stepCount = 0;

	const int safeZone = 10;

	int stepCountList[siz];
	int arrPlace = 0;

	SetDirection(D_RIGHT);

	DigitalIoPin *temp;

	debuger->Log("Start mesure");

	// If switches are not pressed go step
	while (!lim1->Read() && !lim2->Read()) {
		SlowGoSteps(1, 1);
	}

	// Swap switches place if lim1 is pressed
	if (lim1->Read()) {
		debuger->Log("Switch");
		temp = lim1;
		lim1 = lim2;
		lim2 = temp;
	}
//	SwitchDirection();
//	SlowGoSteps(250, 1);
//	SwitchDirection();

	while (arrPlace < siz) {

		SlowGoSteps(1, 1);
		stepCount++;

		if (lim2->Read()) {
			SetDirection(D_LEFT);
		} else if (lim1->Read()) {
			SetDirection(D_RIGHT);
		}

		if (lim1->Read() || lim2->Read()) {

			SlowGoSteps(300, 1);
			stepCount -= 300;
			vTaskDelay(100);

			SwitchDirection();

			while (!lim1->Read() && !lim2->Read()) {
				SlowGoSteps(1, 5);
				stepCount++;
			}
			SwitchDirection();

			stepCountList[arrPlace] = stepCount;
			arrPlace++;
			if (arrPlace < siz) {
				SlowGoSteps(250, 1);
				stepCount = 250;

			}
		}
	}

	SlowGoSteps(safeZone, 1);

	steps = stepCountList[1] - (2 * safeZone);
}

bool Motor::CheckSwitches() {
	return ((!lim2->Read() && currentDirection == D_RIGHT)
			|| (!lim1->Read() && currentDirection == D_LEFT));
}

void Motor::Center() {
	MesureSteps();
	SlowGoSteps(steps / 2, 1);
	curPlace = steps / 2;
	debuger->Log("Current place: %d", curPlace);
}

void Motor::GoStep() {
	step.Write(true);
	step.Write(true);
	step.Write(true);
	step.Write(true);
	step.Write(true);
	step.Write(true);
	step.Write(false);
	if (currentDirection == D_RIGHT) {
		curPlace++;
	} else {
		curPlace--;
	}
}
int Motor::GetPlace() {
	return curPlace;
}
int Motor::GetSteps() {
	return steps;
}

