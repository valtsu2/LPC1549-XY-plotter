/*
 * Debug.cpp
 *
 *  Created on: 5.9.2019
 *      Author: Akseli
 */

#include "Debug.h"

Debug::Debug()
{
	debugJono = xQueueCreate(DEBUG_JONO_SIZE, sizeof(debugEvent));
}

Debug::~Debug()
{
	// TODO Auto-generated destructor stub
}

void Debug::Run()
{
	ITM_output outputter;
	char buffer[100];
	debugEvent e;

	while (1)
	{
		xQueueReceive(debugJono, &e, portMAX_DELAY);
		snprintf(buffer, 100, e.format, e.data[0], e.data[1], e.data[2], e.data[3]);
		outputter.print(buffer);
	}
}

void Debug::Log(const char *format, int d1, int d2, int d3, int d4)
{
	debugEvent eve;
	eve.format = format;
	eve.data[0] = d1;
	eve.data[1] = d2;
	eve.data[2] = d3;
	eve.data[3] = d4;

	xQueueSendToBack(debugJono, &eve, portMAX_DELAY);
}

