/*
 * Parser.cpp
 *
 *  Created on: 5.9.2019
 *      Author: Akseli
 */

#include "Parser.h"

Parser::Parser(Debug *_debuger, CommandHandler *_handler) {
	debuger = _debuger;
	handler = _handler;
	debuger->Log("Started");
}

Parser::~Parser() {

}

//deletes dot, because calculations are more precise with integer number
void Parser::CoordinateStrToInt(char *parsed, int *out) {
	char temp[10];

	int cur = 0;

	for (int i = 1; i < (int) strlen(parsed); ++i) {
		if (parsed[i] != '.') {
			temp[cur] = parsed[i];
			cur++;
		}
	}

	temp[cur] = '\0';

	*out = atoi(temp);

}

//checks the g-code
int Parser::InList(char *cmd) {
	int out = -1;

	for (int i = 0; i < 10; ++i) {
		if (strcmp(cmd, validCodes[i].gcode) == 0) {
			out = validCodes[i].code;
		}
	}
	return out;
}

//removes first char and makes int
int Parser::StrToInt(char *c) {
	c += 1;
	return atoi(c);
}

void Parser::ParseBuff(char *input) //G1, M10, M11, M2, M1, M5, M4, G28
		{
//	debuger->Log("Parse input");
	char *parsed = strtok(input, " ");
	m5cmd m5cmd;
	m2cmd m2cmd;
	coordinate cordi;
	int command = InList(parsed);

	while (parsed != NULL) {
		parsed = strtok(NULL, " ");

		switch (command) {
		case G1: // move command

			if (parsed[0] == 'X') {
				CoordinateStrToInt(parsed, &cordi.x);
//				debuger->Log("Int x: %d", cordi.x);
			} else if (parsed[0] == 'Y') {
				CoordinateStrToInt(parsed, &cordi.y);
//				debuger->Log("Int y: %d", cordi.y);
			} else if (parsed[0] == 'A') {
				cordi.a = (int) parsed[1];
			}
			break;
		case M10:	//connection command
		{
			char *m10Code = (char*) malloc(sizeof(char) * 80);
			handler->GetSettings(&m10Code);
			USB_send((uint8_t *) m10Code, strlen(m10Code));
			free(m10Code);
			break;
		}
		case M11://checks limit switched, but its not needed in the scope of our project
			debuger->Log("Switch querry");
			break;
		case M2:	//writes pen position
			if (parsed[0] == 'U') {
				m2cmd.up = StrToInt(parsed);
			} else if (parsed[0] == 'D') {
				m2cmd.down = StrToInt(parsed);
			}
			break;
		case M1:	//adds pen command
			if (parsed != NULL) {
				handler->AddPenCommand(atoi(parsed));
			}
			break;
		case M5://a and b motor direction, area height and width and movemement speed
			if (parsed[0] == 'A') {
				m5cmd.a = StrToInt(parsed);
			} else if (parsed[0] == 'B') {
				m5cmd.b = StrToInt(parsed);
			} else if (parsed[0] == 'H') {
				m5cmd.h = StrToInt(parsed);
			} else if (parsed[0] == 'W') {
				m5cmd.w = StrToInt(parsed);
			} else if (parsed[0] == 'S') {
				m5cmd.s = StrToInt(parsed);
			}
			break;
		case M4:	//laser
			if (parsed != NULL) {
				handler->AddLaserCommand(atoi(parsed));
			}
			break;
		case G28:	//go to 0 0
			handler->AddMoveCommand(0, 0, false);
			break;
		case SENTER:

			break;
		case MESURE_STEPS:		//measures steps
			handler->AddMesure();
			break;
		default:		//not real command
			debuger->Log("Not A Command: %s", (int) parsed, 0, 0);
			parsed = NULL;
			break;
		}
	}

	if (command == G1) {
		char buffer[90];
		snprintf(buffer, 90, "X: %d\r\nY: %d\r\n", cordi.x, cordi.y);
//		debuger->Log("Add to list");
		handler->AddMoveCommand(cordi.x, cordi.y, true);

		//debuger->Log(buffer);
	} else if (command == M5) {
		handler->SetM5cmd(m5cmd);
	} else if (command == M2) {
		handler->SetM2cmd(m2cmd);
	}
}

