/*
 * Parser.h
 *
 *  Created on: 5.9.2019
 *      Author: Akseli
 */

#ifndef PARSER_H_
#define PARSER_H_
#include "FreeRTOS.h"
#include <queue.h>
#include <cstring>
#include <stdlib.h>
#include "Debug.h"
#include "CommandHandler.h"
#include "user_vcom.h"

struct coordinate {
	int x;
	int y;
	int a;
};

enum G_CODES {
	G1, M10, M11, M2, M1, M5, M4, G28, SENTER, MESURE_STEPS
};

struct GCode {
	char gcode[4];
	G_CODES code;
};

const struct GCode validCodes[10] = { { "G1", G1 }, { "M10", M10 },
		{ "M11", M11 }, { "M2", M2 }, { "M1", M1 }, { "M5", M5 }, { "M4", M4 },
		{ "G28", G28 }, { "f", SENTER }, { "me", MESURE_STEPS } };

class Parser {
public:
	Parser(Debug *_debuger, CommandHandler *_handler);
	virtual ~Parser();
	void ParseBuff(char *input);

private:
	void CoordinateStrToInt(char *parsed, int *out);
	int InList(char *cmd);
	int StrToInt(char *c);
	Debug *debuger;
	CommandHandler *handler;

};

#endif /* PARSER_H_ */
