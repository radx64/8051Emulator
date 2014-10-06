/*
 * bytesArray.cpp
 *
 *  Created on: 29 lip 2013
 *      Author: rszewczy
 */

#include "BytesArray.h"
#include <cstdio>

BytesArray::BytesArray()
{
	// TODO Auto-generated constructor stub

}

BytesArray::~BytesArray()
{
	// TODO Auto-generated destructor stub
}

unsigned int BytesArray::size()
{
	return bytes.size();
}

void BytesArray::push(unsigned char byte)
{
	bytes.push_back(byte);
}

unsigned char BytesArray::getByteAtIndex(unsigned int index)
{
	if(index < this->size())
		return bytes[index];
	else
		throw new std::string("Out of bounds");
	return 0;
}

void BytesArray::clear()
{
	bytes.clear();
}

std::string BytesArray::toString()
{
	std::string result;
	char buff[256];
	unsigned int size = this->size();
	for(unsigned int i=0;i<size;++i)
	{
		sprintf(buff, "0x%02X", bytes[i]);
		result.append(buff);
		if(i<size-1) result.append(" ");
	}
	return result;
}
