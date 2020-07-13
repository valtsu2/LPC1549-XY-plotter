/*
 * Debug.h
 *
 *  Created on: 5.9.2019
 *      Author: Akseli
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "FreeRTOS.h"
#include <queue.h>
#include "ITM_output.h"

//wrapper class for ITM_Write
class Debug
{
public:
	Debug();
	virtual ~Debug();
	void Log(const char *format, int d1 = 0, int d2 = 0, int d3 = 0, int d4 = 0);
	void Run();

private:
	const int DEBUG_JONO_SIZE = 20;
	QueueHandle_t debugJono;

	struct debugEvent
	{
		const char *format;
		int data[4];

	};

};

#endif /* DEBUG_H_ */
