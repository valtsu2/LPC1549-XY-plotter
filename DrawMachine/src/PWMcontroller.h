/*
 * PWMcontroller.h
 *
 *  Created on: 25 Sep 2019
 *      Author: Valtsu
 */

#ifndef PWMCONTROLLER_H_
#define PWMCONTROLLER_H_
#include "DigitalIoPin.h"

class PWMcontroller {
public:
	PWMcontroller();
	virtual ~PWMcontroller();

	void setPenPWM(int pp);
	void setLazerPWM(int value);
private:
	void initPWM();
	DigitalIoPin *laser;
};

#endif /* PWMCONTROLLER_H_ */
