#include <iostream>
#include "Processor.h"

int main(int argc, char** argv)
{
	Processor p;
	p.Reboot();
	std::cout << "Hello world!" << std::endl;
	std::cout << "argc:\t"<< argc << std::endl;
	std::cout << "argv*:\t"<< argv << std::endl;		
	return 0;
}