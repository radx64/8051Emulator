/*
 * Processor.h
 *
 *  Created on: 30 lip 2013
 *      Author: rszewczy
 */


#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <stdexcept>

#define IRAM_SIZE 256
#define ROM_SIZE  65536
#define ERAM_SIZE 65536

enum registerNames
{
	R0 	 = 0x00,
	R1 	 = 0x01,
	R2 	 = 0x02,
	R3 	 = 0x03,
	R4 	 = 0x04,
	R5 	 = 0x05,
	R6 	 = 0x06,
	R7 	 = 0x07,
	BITS = 0x20,
	A  	 = 0xE0,
	B  	 = 0xF0,
	PSW  = 0xD0,
	DPL  = 0x82,
	DPH  = 0x83,
	SP 	 = 0x81,
	P0 	 = 0x80,
	P1 	 = 0x90,
	P2 	 = 0xA0,
	P3 	 = 0xB0,
	TCON = 0x88,
	TMOD = 0x89,
	TL0  = 0x8A,
	TL1  = 0x8B,
	TH0  = 0x8C,
	TH1  = 0x8D,
	SCON = 0x98,
	SBUF = 0x99,
	IE 	 = 0xA8,
	IP 	 = 0xB8
};

enum flagNames
{
	P   = 0,
	UD  = 1,
	OV  = 2,
	RS0 = 3,
	RS1 = 4,
	F0  = 5,
	AC  = 6,
	CY  = 7
};

class Processor
{
public:
	unsigned char IRAM[IRAM_SIZE];
	unsigned char ROM[ROM_SIZE];
	unsigned char ERAM[ERAM_SIZE];
	unsigned short int PC;
	typedef void (Processor::*function)(void);

	std::map<unsigned char, function> opCodesHandlers;

	Processor();
	virtual ~Processor();
	void Reboot();
	void setRegister(registerNames name, unsigned char value);
	unsigned char getRegister(registerNames name);
	void setRegistersBank(unsigned char value);
	unsigned char getRegistersBank();
	std::string getMemoryDump();
	void loadHexProgramToROM(unsigned char* code, unsigned char size);
	void step();
	void processOpCode(unsigned char code);
	void incrementProgramCounter();
	void incrementProgramCounter(unsigned char value);

	unsigned short int getProgramCounter();
	unsigned char getProgramCounterLowByte();
	unsigned char getProgramCounterHighByte();
	unsigned char getStackPointer();
	void resetStackPointer();
	void pushToStack(unsigned char value);
	unsigned char popFromStack();
	void setBit(unsigned char bitAddress);
	bool getBit(unsigned char bitAddress);
	void clearBit(unsigned char bitAddress);

	void setFlag(flagNames flag);
	bool getFlag(flagNames flag);
	void clearFlag(flagNames flag);


	void NOP_0x00();
	void AJMP_page0_0x01();
	void LJMP_code_addr_0x02();
	void RR_A_0x03();
	void INC_A_0x04();
	void INC_direct_0x05();
	void INC_atR0_0x06();
	void INC_atR1_0x07();
	void INC_R0_0x08();
	void INC_R1_0x09();
	void INC_R2_0x0A();
	void INC_R3_0x0B();
	void INC_R4_0x0C();
	void INC_R5_0x0D();
	void INC_R6_0x0E();
	void INC_R7_0x0F();
	void JBC_bitaddr_reladr_0x10();

	void LCALL_code_addr_0x12();
	void RRC_A_0x13();
	void DEC_A_0x14();
	void DEC_direct_0x15();
	void DEC_atR0_0x16();
	void DEC_atR1_0x17();
	void DEC_R0_0x18();
	void DEC_R1_0x19();
	void DEC_R2_0x1A();
	void DEC_R3_0x1B();
	void DEC_R4_0x1C();
	void DEC_R5_0x1D();
	void DEC_R6_0x1E();
	void DEC_R7_0x1F();
	void JB_bitaddr_reladdr_0x20();
	void AJMP_page1_0x21();
	void RET_0x22();
	void RL_A_0x23();
	void AJMP_page2_0x41();
	void AJMP_page3_0x61();
	void AJMP_page4_0x81();
	void AJMP_page5_0xA1();

	void INC_DPTR_0xA3();

	void AJMP_page6_0xC1();
	void AJMP_page7_0xE1();
	unsigned short int calculateNewProgramCounterUsingAJMP(
			unsigned char ajmpLast3bitsOfOpCode);
	void clearIRAM();
};

#endif /* PROCESSOR_H_ */
