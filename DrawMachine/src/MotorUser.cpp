/*
 * MotorUser.cpp
 *
 *  Created on: 18.9.2019
 *      Author: Valtsu
 */

#include "MotorUser.h"
MotorUser::MotorUser() {

}
MotorUser::MotorUser(Debug *_debuger) {
	debuger = _debuger;
	motorX = new Motor(direX, stepX, limit1X, limit2X, debuger);
	motorY = new Motor(direY, stepY, limit1Y, limit2Y, debuger);
	readyToRun = xQueueCreate(10, sizeof(motorCommand));
	sema = xSemaphoreCreateBinary();
}

MotorUser::~MotorUser() {
	delete (motorX);
	delete (motorY);
}

void MotorUser::HandlerMotor() {
	// This used to check if a context switch is required
	portBASE_TYPE xHigherPriorityWoken = pdFALSE;
	// Tell timer that we have processed the interrupt.
	// Timer then removes the IRQ until next match occurs
	Chip_RIT_ClearIntStatus(LPC_RITIMER);
	// clear IRQ flag
	if (((runRITNormal && ritRounds == -1) || (!runRITNormal && ritRounds > 0))
			&& motorX->CheckSwitches() && motorY->CheckSwitches()) {
		if (!runRITNormal) {
			ritRounds--;
		}

		if (currentMotorCommad.fromX == currentMotorCommad.toX
				&& currentMotorCommad.fromY == currentMotorCommad.toY) {
			runRITNormal = false;
		} else {
			//Bresenham Algorithm
			currentMotorCommad.err2 = 2 * currentMotorCommad.err;
			if (currentMotorCommad.err2 >= currentMotorCommad.dy) {
				currentMotorCommad.err += currentMotorCommad.dy;
				currentMotorCommad.fromX += currentMotorCommad.sx;
				motorX->GoStep();
			}
			if (currentMotorCommad.err2 <= currentMotorCommad.dx) {
				currentMotorCommad.err += currentMotorCommad.dx;
				currentMotorCommad.fromY += currentMotorCommad.sy;
				motorY->GoStep();
			}
		}
	} else {
		Chip_RIT_Disable(LPC_RITIMER);
		// disable timer
		// Give semaphore and set context switch flag if a higher priority task was woken up
		xSemaphoreGiveFromISR(sema, &xHigherPriorityWoken);
	}
	// End the ISR and (possibly) do a context switch
	portEND_SWITCHING_ISR(xHigherPriorityWoken);
}

//goes steps
void MotorUser::GoSteps(motorCommand com) {
	currentMotorCommad = com;
	SetDirection(currentMotorCommad.xMotor, currentMotorCommad.yMotor);

//	debuger->Log("Stepit: %d %d %d", currentMotorCommad.xSteps, currentMotorCommad.ySteps, currentMotorCommad.multi);
//	debuger->Log("Rest: %d %d", currentMotorCommad.restX, currentMotorCommad.restY);

	runRITNormal = true;
	bool runLoop = false;
	int counter = 1;
	ritRounds = -1;

	if (currentMotorCommad.acc >= 250) {
		runRITNormal = false;
		runLoop = true;
	}

	do {
		if (!runRITNormal) {
			runLoop = UseAcceleration(&counter, 7000);
			debuger->Log("PPS: %d  Counter: %d", pps, counter);
		}

		StartRit();

	} while (runLoop);
	//debuger->Log("MOVE Current: (%d,%d) (%d,%d)", motorX->GetPlace(), motorY->GetPlace(), currentMotorCommad.toX, currentMotorCommad.toY);
}

//accelerates in 10 steps step by step
bool MotorUser::UseAcceleration(int *counter, int accMaxSpeed) {
	bool out = true;
	if (*counter <= 10) {
		pps = accMaxSpeed * *counter / 10;

		ritRounds = 10;

	} else if (*counter == 11) {
		ritRounds = currentMotorCommad.acc - 200;
		pps = accMaxSpeed;
	} else if (*counter > 11) {
		pps = accMaxSpeed * (22 - *counter) / 10;
		ritRounds = 10;
		if (*counter >= 21) {
			out = false;
		}
	}
	//debuger->Log("Rit Rounds %d ACC: %d   PPS: %d", ritRounds, currentMotorCommad.acc, pps);
	*counter += 1;
	return out;
}

void MotorUser::StartRit() {
	int us = 1000000 / pps;
	uint64_t cmp_value;

	// Determine approximate compare value based on clock rate and passed interval
	cmp_value = (uint64_t) Chip_Clock_GetSystemClockRate() * (uint64_t) us
			/ 1000000;

	// disable timer during configuration
	Chip_RIT_Disable(LPC_RITIMER);

	// enable automatic clear on when compare value==timer value
	// this makes interrupts trigger periodically
	Chip_RIT_EnableCompClear(LPC_RITIMER);
	// reset the counter
	Chip_RIT_SetCounter(LPC_RITIMER, 0);
	Chip_RIT_SetCompareValue(LPC_RITIMER, cmp_value);
	// start counting
	Chip_RIT_Enable(LPC_RITIMER);
	// Enable the interrupt signal in NVIC (the interrupt controller)
	NVIC_EnableIRQ(RITIMER_IRQn);

	// wait for ISR to tell that we're done
	if (xSemaphoreTake(sema, portMAX_DELAY) == pdTRUE) {
		// Disable the interrupt signal in NVIC (the interrupt controller)
		NVIC_DisableIRQ(RITIMER_IRQn);
	} else {
		// unexpected error
	}
}

//sets speed
void MotorUser::SetSpeed(int pros) {
	pps = maxpps * ((double) pros / 100);
}

//sets direction
void MotorUser::SetDirection(Direction xDir, Direction yDir) {
	motorX->SetDirection(xDir);
	motorY->SetDirection(yDir);
}

void MotorUser::GetSteps(int *x, int *y) {
	*x = motorX->GetSteps();
	*y = motorY->GetSteps();
}
//goes center and measures
void MotorUser::MesureSteps(int *x, int *y) {
	motorX->Center();
	motorY->Center();
	*x = motorX->GetPlace();
	*y = motorY->GetPlace();
}

