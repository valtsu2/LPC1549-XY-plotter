/*
 * ITMoutput.h
 *
 *  Created on: 23.1.2019
 *      Author: Valtsu
 */

#ifndef ITM_OUTPUT_H_
#define ITM_OUTPUT_H_
#include <string>
class ITM_output
{
public:
	ITM_output();
	virtual ~ITM_output();
	void print(std::string input);
	void print(int input);
};

#endif /* ITM_OUTPUT_H_ */
