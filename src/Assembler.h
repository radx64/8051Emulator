/*
 * Assembler.h
 *
 *  Created on: 29 lip 2013
 *      Author: rszewczy
 */

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <string>
#include <cstring>
#include <vector>
#include "BytesArray.h"


/*
 * Not finished!
 *
 */

class Assembler
{
public:
	Assembler();
	virtual ~Assembler();
	BytesArray getOpCodes(std::string code);

private:
};

#endif /* ASSEMBLER_H_ */
