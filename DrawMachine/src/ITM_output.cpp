/*
 * ITMoutput.cpp
 *
 *  Created on: 23.1.2019
 *      Author: Valtsu
 */

#include "ITM_output.h"
#include "ITM_write.h"

ITM_output::ITM_output()
{
	ITM_init();
}

ITM_output::~ITM_output(){}

void ITM_output::print(std::string input)
{
	input = input + '\n';
	ITM_write(input.c_str());
}
/*void ITM_output::print(int input)
{
	std::string str = std::to_string(input);
	print(str);
}*/

