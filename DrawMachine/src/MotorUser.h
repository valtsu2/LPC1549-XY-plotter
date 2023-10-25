/*
 * MotorUser.h
 *
 *  Created on: 18.9.2019
 *      Author: Valtsu
 */

#ifndef MOTORUSER_H_
#define MOTORUSER_H_
#include "Motor.h"
#include "Debug.h"
#include "queue.h"

struct motorCommand
{
	Direction yMotor;
	Direction xMotor;
	int toX;
	int toY;
	int fromX;
	int fromY;
	int dx;
	int sx;
	int dy;
	int sy;
	int err;
	int err2;
	int acc;
};

class MotorUser
{
public:
	MotorUser();
	MotorUser(Debug *_debuger);
	virtual ~MotorUser();

	void GoSteps(motorCommand com);
	void SetSpeed(int pros);
	void SetDirection(Direction xDir, Direction yDir);
	void GetSteps(int *x, int *y);

	void HandlerMotor();
	void MesureSteps(int *x, int *y);

private:
	Debug *debuger;

	Motor *motorX;
	Motor *motorY;

	PortPinPair stepY = { 0, 27 };
	PortPinPair direY = { 0, 28 };
	PortPinPair limit1X = { 0, 9 };
	PortPinPair limit2X = { 0, 29 };

	PortPinPair stepX = { 0, 24 };
	PortPinPair direX = { 1, 0 };
	PortPinPair limit1Y = { 1, 3 };
	PortPinPair limit2Y = { 0, 0 };

	QueueHandle_t readyToRun;
	motorCommand currentMotorCommad;


	volatile bool runRITNormal;
	volatile int ritRounds;

	SemaphoreHandle_t sema;

	int maxpps = 3000;
	int pps = 2000;
	int xLast;
	int yLast;

	void StartRit();
	bool UseAcceleration(int *counter, int accMaxSpeed);
};

#endif /* MOTORUSER_H_ */
