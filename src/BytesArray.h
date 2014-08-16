/*
 * bytesArray.h
 *
 *  Created on: 29 lip 2013
 *      Author: rszewczy
 */

#ifndef BYTESARRAY_H_
#define BYTESARRAY_H_

#include <vector>
#include <string>
class BytesArray
{
private:
	std::vector<unsigned char> bytes;


public:
	BytesArray();
	virtual ~BytesArray();
	unsigned int size();
	void push(unsigned char byte);
	void clear();
	unsigned char getByteAtIndex(unsigned int index);
	std::string toString();
};

#endif /* BYTESARRAY_H_ */
