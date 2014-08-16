/*
 * Assembler.cpp
 *
 *  Created on: 29 lip 2013
 *      Author: rszewczy
 */

#include "Assembler.h"

Assembler::Assembler()
{
	// TODO Auto-generated constructor stub

}

Assembler::~Assembler()
{
	// TODO Auto-generated destructor stub
}




BytesArray Assembler::getOpCodes(std::string code)
{
	BytesArray b;

	size_t instructionBegin = 0;
	size_t instructionLength = 0;

	std::string instruction;
	std::vector<std::string> parameters;


	instructionLength = code.find(' ');
	instruction = code.substr(instructionBegin, instructionLength);

	if(instruction == "ADD")
	{
		b.push(0x24);
		b.push(0x01);
	}
	else
	b.push(0);
	return b;
}
