/*
 * CommandHandler.h
 *
 *  Created on: 25.9.2019
 *      Author: Akseli
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "MotorUser.h"
#include "PWMcontroller.h"

enum Commands {
	C_LASER, C_MOVE, C_PEN, C_MESURE, C_CENTER
};

struct penCommand {
	int pos;
};

struct laserCommand {
	int power;
};

struct cmd {
	Commands com;
	motorCommand motorcmd;
	penCommand pencmd;
	laserCommand lasercmd;
};

struct m10Settings {
	int heigth = 350;
	int widht = 300;
	int xStepDir = 0;
	int yStepDir = 0;
	int plotSpeed = 80;
	int penUp = 100;
	int penDown = 10;
};

struct m5cmd {
	int a;
	int b;
	int h;
	int w;
	int s;
};
struct m2cmd {
	int up;
	int down;
};

class CommandHandler {
public:
	CommandHandler(Debug *_debuger, MotorUser *_user);
	CommandHandler();
	virtual ~CommandHandler();
	void RunCommand();
	void AddMoveCommand(int x2, int y2, bool acce);
	void AddPenCommand(int pos);
	void AddLaserCommand(int power);
	void AddMesure();
	void GetSettings(char **c);
	void SetM5cmd(m5cmd com);
	void SetM2cmd(m2cmd com);
private:
	int Map(int value, int in_max, int out_max);
	QueueHandle_t cmdQue;
	MotorUser *mUser;
	Debug *debuger;

	m10Settings settings;
	PWMcontroller *pwmCont;

	int yLast = -1;
	int xLast = -1;
	int yNow = 0;
	int xNow = 0;
	bool penDown = false;

};

#endif /* COMMANDHANDLER_H_ */
