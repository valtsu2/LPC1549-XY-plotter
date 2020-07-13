/*
 * CommandHandler.cpp
 *
 *  Created on: 25.9.2019
 *      Author: Akseli
 */

#include "CommandHandler.h"
#include <cstring>

CommandHandler::CommandHandler(Debug *_debuger, MotorUser *_user) {
	cmdQue = xQueueCreate(30, sizeof(cmd));
	mUser = _user;
	debuger = _debuger;
	pwmCont = new PWMcontroller();
}

CommandHandler::CommandHandler() {

}
CommandHandler::~CommandHandler() {
	delete (pwmCont);
}

void CommandHandler::RunCommand() {
	cmd cmd;
	xQueueReceive(cmdQue, &cmd, portMAX_DELAY);

	switch (cmd.com) {
	case C_MOVE:
		mUser->SetSpeed(settings.plotSpeed);	//set speed
		mUser->GoSteps(cmd.motorcmd);			//move
		break;
	case C_LASER:
		pwmCont->setLazerPWM(cmd.lasercmd.power);	//set laser power
		vTaskDelay(100);
		break;
	case C_PEN:
		pwmCont->setPenPWM(cmd.pencmd.pos);	//set pen position
		vTaskDelay(100);
		break;
	case C_MESURE:
		mUser->MesureSteps(&xLast, &yLast);	//calibration
		break;
	default:
		break;
	}

}
void CommandHandler::AddMoveCommand(int x2, int y2, bool rescale) {
	int stepsX;
	int stepsY;

	mUser->GetSteps(&stepsX, &stepsY);

	//rescale is true when pen is down and moving
	if (rescale) {
		x2 = Map(x2, settings.widht * 100, stepsX);
		y2 = Map(y2, settings.heigth * 100, stepsY);
	}

	//x1/y1 is preiv location
	int x1 = xLast;
	int y1 = yLast;

	//debuger->Log("(%d,%d) to (%d,%d)", xLast, yLast, x2, y2);

	cmd cmd;
	cmd.com = C_MOVE;
	cmd.motorcmd.toX = x2;
	cmd.motorcmd.toY = y2;
	cmd.motorcmd.fromX = x1;
	cmd.motorcmd.fromY = y1;

	//calculates which way you are suposed to go
	if ((y2 - y1) < 0) {
		cmd.motorcmd.yMotor = D_RIGHT;
	} else {
		cmd.motorcmd.yMotor = D_LEFT;
	}

	if ((x2 - x1) < 0) {
		cmd.motorcmd.xMotor = D_RIGHT;
	} else {
		cmd.motorcmd.xMotor = D_LEFT;
	}

	cmd.motorcmd.dx = abs(x2 - x1);	//amount of steps
	cmd.motorcmd.sx = x1 < x2 ? 1 : -1;	//error correlation
	cmd.motorcmd.dy = -abs(y2 - y1);
	cmd.motorcmd.sy = y1 < y2 ? 1 : -1;
	cmd.motorcmd.err = cmd.motorcmd.dx + cmd.motorcmd.dy;
	cmd.motorcmd.acc = 0;

	//accerelation when rescale is off
	if (!rescale) {
		cmd.motorcmd.acc =
				abs(cmd.motorcmd.dy) > abs(cmd.motorcmd.dx) ?
						abs(cmd.motorcmd.dy) : abs(cmd.motorcmd.dx);
		debuger->Log("Accelerate: %d   DX: %d  DY: %d", cmd.motorcmd.acc,
				cmd.motorcmd.dx, cmd.motorcmd.dy);
	}
	if (penDown) {
		xQueueSendToBack(cmdQue, &cmd, portMAX_DELAY);
		xLast = x2;
		yLast = y2;
	} else {
		xNow = x2;
		yNow = y2;
		//debuger->Log("Last X: %d   Y: %d", xNow, yNow);
	}

//		debuger->Log("Running Command added");

}

//add pen command to que
void CommandHandler::AddPenCommand(int pos) {
	cmd cmd;
	cmd.com = C_PEN;
	cmd.pencmd.pos = pos;
	penDown = cmd.pencmd.pos <= settings.penDown;
	if (xNow != 0 || yNow != 0) {
		AddMoveCommand(xNow, yNow, false);
		yNow = 0;
		xNow = 0;
	}

	xQueueSendToBack(cmdQue, &cmd, portMAX_DELAY);
}

//adds laser command to que
void CommandHandler::AddLaserCommand(int power) {
	cmd cmd;
	cmd.com = C_LASER;
	cmd.lasercmd.power = power;
	penDown = cmd.lasercmd.power > 0;
	if (xNow > 0 || yNow > 0) {
		AddMoveCommand(xNow, yNow, false);
		yNow = 0;
		xNow = 0;
	}

	xQueueSendToBack(cmdQue, &cmd, portMAX_DELAY);
	//debuger->Log("Laser Command Added %d", power);
}
void CommandHandler::AddMesure() {
	cmd cmd;
	cmd.com = C_MESURE;
	xQueueSendToBack(cmdQue, &cmd, portMAX_DELAY);
}
int CommandHandler::Map(int value, int in_max, int out_max) {
	return (value * out_max / in_max);
}

// makes m10 command
void CommandHandler::GetSettings(char **c) {
	char com[80];
	snprintf(com, 80, "M10 XY %d %d 0.00 0.00 A%d B%d H0 S%d U%d D%d\r\n",
			settings.widht, settings.heigth, settings.xStepDir,
			settings.yStepDir, settings.plotSpeed, settings.penUp,
			settings.penDown);
	strcpy(*c, com);
	mUser->SetSpeed(settings.plotSpeed);
	pwmCont->setPenPWM(settings.penUp);
	penDown = false;
	AddMesure();
}

void CommandHandler::SetM5cmd(m5cmd com) {
	settings.plotSpeed = com.s;
	settings.heigth = com.h;
	settings.widht = com.w;
	settings.xStepDir = com.a;
	settings.yStepDir = com.b;
}

void CommandHandler::SetM2cmd(m2cmd com) {
	settings.penDown = com.down;
	settings.penUp = com.up;
}

