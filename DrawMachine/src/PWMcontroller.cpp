/*
 * PWMcontroller.cpp
 *
 *  Created on: 25 Sep 2019
 *      Author: Valtsu
 */

#include "PWMcontroller.h"
#include "FreeRTOS.h"
#include "task.h"
#include "chip.h"
#include "sct_15xx.h"

PWMcontroller::PWMcontroller() {
	laser = new DigitalIoPin(0, 12, false, false, false); //create laser object and turn it off at start
	laser->Write(false);
	initPWM();	//initialize PWM for laser and pen
}

PWMcontroller::~PWMcontroller() {
	delete (laser); //for every new there must be delete :)
}

//scales input value to [0-255] and sets pen
void PWMcontroller::setPenPWM(int PP) {
	const double maxPP = 255;
	const double minPP = 0;

	const double minPWM = 1000;
	const double maxPWM = 2000;

	if (PP >= minPP && PP <= maxPP) {
		int scaledValue = (maxPWM - minPWM) * ((PP - minPP) / (maxPP - minPP))
				+ minPWM;
		LPC_SCT0->MATCHREL[1].L = scaledValue;
	}
}

//scale input value to [0-255] and sets lazer
void PWMcontroller::setLazerPWM(int value) {
	const int maxValue = 255;
	const int maxPWM = 1000;

	if (value >= 0 && value <= maxValue) {
		int scaledValue = maxPWM * value / maxValue;
		LPC_SCT0->MATCHREL[1].H = scaledValue;

	}

	//laser must be turned totaly off when it should not write, because of small pulses emc
	if (value == 0) {
		LPC_SCT0->OUT[1].SET = (0 << 2);
		vTaskDelay(100);
	} else {
		LPC_SCT0->OUT[1].SET = (1 << 2);
		vTaskDelay(100);
	}

}

void PWMcontroller::initPWM() {
	Chip_SCT_Init(LPC_SCT0);

	LPC_SCT0->CONFIG |= (1 << 17); // two 16-bit timers
	LPC_SCT0->CONFIG |= (1 << 18);	//autolimit

	LPC_SCT0->CTRL_L |= (72 - 1) << 5;	//SCT clock
	LPC_SCT0->CTRL_H |= (72 - 1) << 5;	//SCT clock

	LPC_SCT0->MATCHREL[0].L = 20000 - 1; //50hz
	LPC_SCT0->MATCHREL[1].L = 1500; // 1000 min 2000 max

	LPC_SCT0->MATCHREL[0].H = 1000 - 1;	//1khz
	LPC_SCT0->MATCHREL[1].H = 500; // 0 min 1000 max

	//LOW PEN
	//event 0 is pulse up and event 1 is pulse down
	LPC_SCT0->EVENT[0].STATE = 0xFFFFFFFF;
	LPC_SCT0->EVENT[0].CTRL = (1 << 12);
	LPC_SCT0->EVENT[1].STATE = 0xFFFFFFFF;
	LPC_SCT0->EVENT[1].CTRL = (1 << 0) | (1 << 12);
	LPC_SCT0->OUT[0].SET = (1 << 0);
	LPC_SCT0->OUT[0].CLR = (1 << 1);

	//HIGH LASER
	//event 2 is pulse up  and event 3 is pulse down
	LPC_SCT0->EVENT[2].STATE = 0xFFFFFFFF;
	LPC_SCT0->EVENT[2].CTRL = (1 << 12) | (1 << 4);
	LPC_SCT0->EVENT[3].STATE = 0xFFFFFFFF;
	LPC_SCT0->EVENT[3].CTRL = (1 << 0) | (1 << 12) | (1 << 4);
	LPC_SCT0->OUT[1].SET = (0 << 2);
	LPC_SCT0->OUT[1].CLR = (1 << 3);

	LPC_SCT0->CTRL_L &= ~(1 << 2);	//enables pen
	LPC_SCT0->CTRL_H &= ~(1 << 2);	//enables laser
	Chip_SWM_Init();
	Chip_SWM_MovablePortPinAssign(SWM_SCT0_OUT0_O, 0, 10); //pen
	Chip_SWM_MovablePortPinAssign(SWM_SCT0_OUT1_O, 0, 12);	//laser
}

