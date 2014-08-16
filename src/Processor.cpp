/*
 * Processor.cpp
 *
 *  Created on: 30 lip 2013
 *      Author: rszewczy
 */

#include "Processor.h"

Processor::Processor()
{
	Processor::Reboot();
	opCodesHandlers[0x00] = &Processor::NOP_0x00;
	opCodesHandlers[0x01] = &Processor::AJMP_page0_0x01;
	opCodesHandlers[0x02] = &Processor::LJMP_code_addr_0x02;
	opCodesHandlers[0x03] = &Processor::RR_A_0x03;
	opCodesHandlers[0x04] = &Processor::INC_A_0x04;
	opCodesHandlers[0x05] = &Processor::INC_direct_0x05;
	opCodesHandlers[0x06] = &Processor::INC_atR0_0x06;
	opCodesHandlers[0x07] = &Processor::INC_atR1_0x07;
	opCodesHandlers[0x08] = &Processor::INC_R0_0x08;
	opCodesHandlers[0x09] = &Processor::INC_R1_0x09;
	opCodesHandlers[0x0A] = &Processor::INC_R2_0x0A;
	opCodesHandlers[0x0B] = &Processor::INC_R3_0x0B;
	opCodesHandlers[0x0C] = &Processor::INC_R4_0x0C;
	opCodesHandlers[0x0D] = &Processor::INC_R5_0x0D;
	opCodesHandlers[0x0E] = &Processor::INC_R6_0x0E;
	opCodesHandlers[0x0F] = &Processor::INC_R7_0x0F;
	opCodesHandlers[0x10] = &Processor::JBC_bitaddr_reladr_0x10;

	opCodesHandlers[0x12] = &Processor::LCALL_code_addr_0x12;
	opCodesHandlers[0x13] = &Processor::RRC_A_0x13;
	opCodesHandlers[0x14] = &Processor::DEC_A_0x14;
	opCodesHandlers[0x15] = &Processor::DEC_direct_0x15;
	opCodesHandlers[0x16] = &Processor::DEC_atR0_0x16;
	opCodesHandlers[0x17] = &Processor::DEC_atR1_0x17;
	opCodesHandlers[0x18] = &Processor::DEC_R0_0x18;
	opCodesHandlers[0x19] = &Processor::DEC_R1_0x19;
	opCodesHandlers[0x1A] = &Processor::DEC_R2_0x1A;
	opCodesHandlers[0x1B] = &Processor::DEC_R3_0x1B;
	opCodesHandlers[0x1C] = &Processor::DEC_R4_0x1C;
	opCodesHandlers[0x1D] = &Processor::DEC_R5_0x1D;
	opCodesHandlers[0x1E] = &Processor::DEC_R6_0x1E;
	opCodesHandlers[0x1F] = &Processor::DEC_R7_0x1F;
	opCodesHandlers[0x20] = &Processor::JB_bitaddr_reladdr_0x20;
	opCodesHandlers[0x21] = &Processor::AJMP_page1_0x21;
	opCodesHandlers[0x22] = &Processor::RET_0x22;
	opCodesHandlers[0x23] = &Processor::Processor::RL_A_0x23;


	opCodesHandlers[0x41] = &Processor::AJMP_page2_0x41;

	opCodesHandlers[0x61] = &Processor::AJMP_page3_0x61;

	opCodesHandlers[0x81] = &Processor::AJMP_page4_0x81;

	opCodesHandlers[0xA1] = &Processor::AJMP_page5_0xA1;

	opCodesHandlers[0xA3] = &Processor::INC_DPTR_0xA3;

	opCodesHandlers[0xC1] = &Processor::AJMP_page6_0xC1;

	opCodesHandlers[0xE1] = &Processor::AJMP_page7_0xE1;



}
Processor::~Processor()
{

}

void Processor::setFlag(flagNames flag)
{
	unsigned char mask = 0b00000001;
	mask <<= flag;
	IRAM[PSW] |= mask;
}

bool Processor::getFlag(flagNames flag)
{
	unsigned char mask = 0b00000001;
	mask = mask << flag;
	IRAM[PSW] = IRAM[PSW] & mask;
	return IRAM[PSW];
}

void Processor::clearFlag(flagNames flag)
{
	unsigned char mask = 0b00000001;
	mask <<= flag;
	mask = ~mask;
	IRAM[PSW] &= mask;
}

void Processor::clearIRAM()
{
	for (int i = 0; i < IRAM_SIZE; ++i)
	{
		IRAM[i] = 0x00;
	}
}

void Processor::Reboot()
{
	PC = 0;
	clearIRAM();
	resetStackPointer();
}

void Processor::setRegister(registerNames name, unsigned char value)
{
	unsigned char registerBankWidth = 8;
	if(name <= R7)
	{
		unsigned char address = name + (getRegistersBank()*registerBankWidth);
		IRAM[address] = value;
	}
	else
	{
		IRAM[name] = value;
	}
}

unsigned char Processor::getRegister(registerNames name)
{
	unsigned char registerBankWidth = 8;
	unsigned char address = name;
	if(name <= R7)
	{
		address = name + (getRegistersBank()*registerBankWidth);
	}
	return IRAM[address];
}


void Processor::setRegistersBank(unsigned char value)
{
	if(value < 4)
	{
		unsigned char mask = 0b11100111;
		IRAM[PSW] = IRAM[PSW] & mask;
		value = value << RS0;
		IRAM[PSW] = (IRAM[PSW] | value);
	}
	else throw new std::runtime_error("Register Bank out of range 0..3");
}

unsigned char Processor::getRegistersBank()
{
	unsigned char bankMask = 0b00011000;
	return (IRAM[PSW] & bankMask)>> RS0;
}

unsigned char Processor::getStackPointer()
{
	return IRAM[SP];
}

void Processor::resetStackPointer()
{
	IRAM[SP] = 0x07;
}

void Processor:: pushToStack(unsigned char value)
{
	IRAM[SP]++;
	unsigned char address = getStackPointer();
	IRAM[address] = value;
}

unsigned char Processor::popFromStack()
{
	unsigned char address = getStackPointer();
	unsigned char value = IRAM[address];
	IRAM[SP]--;
	return value;

}


std::string Processor::getMemoryDump()
{
	std::string result = "";
	char *buffer;
	buffer = new char[7];
	for(int i=0; i<32; ++i)
	{
		sprintf(buffer,"%02X:\t",i*8);
		result.append(buffer);
		for(int j=0; j<8; ++j)
		{
			sprintf(buffer,"0x%02X  ",IRAM[(i*8)+j]);
			result.append(buffer);
		}
		result.append("\n");
	}
	return result;
}

void Processor::loadHexProgramToROM(unsigned char* code, unsigned char size)
{
	for(unsigned int i=0; i< size; ++i)
	{
		ROM[i] = code[i];
	}
}

void Processor::step()
{
	processOpCode(ROM[PC]);
}

void Processor::processOpCode(unsigned char code)
{
	function f = opCodesHandlers[code];
	if(f != 0)	(this->*f)();
	else throw new std::runtime_error("Unknown OpCode found: " + code);
}

void Processor::incrementProgramCounter()
{
	PC++;
}

void Processor::incrementProgramCounter(unsigned char value)
{
	PC+= value;
}

unsigned short int Processor::getProgramCounter()
{
	return PC;
}


unsigned char Processor::getProgramCounterLowByte()
{
	unsigned char result = PC;
	return result;
}

unsigned char Processor::getProgramCounterHighByte()
{
	unsigned char result = (PC >> 8);
	return result;
}


void Processor::setBit(unsigned char bitAddress)
{
	unsigned char byteAddress = BITS + (bitAddress/8);
	unsigned char selectedBit = bitAddress % 8;
	unsigned char mask = 0b00000001;
	mask = mask << selectedBit;
	IRAM[byteAddress]|=mask;
}

bool Processor::getBit(unsigned char bitAddress)
{
	unsigned char byteAddress = BITS + (bitAddress/8);
	unsigned char selectedBit = bitAddress % 8;
	unsigned char mask = 0b00000001;
	mask = mask << selectedBit;
	unsigned char value = IRAM[byteAddress];
	value = value & mask;
	return value;
}

void Processor::clearBit(unsigned char bitAddress)
{
	unsigned char byteAddress = BITS + (bitAddress/8);
	unsigned char selectedBit = bitAddress % 8;
	unsigned char mask = 0b00000001;
	mask = mask << selectedBit;
	mask = ~mask;
	IRAM[byteAddress]&=mask;
}


/*
 * Block of Processor Specific Instructions
 */
void Processor::NOP_0x00()
{
	incrementProgramCounter();

}

void Processor::INC_A_0x04()
{
	unsigned int value = getRegister(A);
	setRegister(A,value+1);
	incrementProgramCounter();
}

void Processor::INC_direct_0x05()
{
	incrementProgramCounter();
	unsigned int parameter = ROM[getProgramCounter()];
	IRAM[parameter]++;
	incrementProgramCounter();
}

void Processor::INC_atR0_0x06()
{
	unsigned int address = getRegister(R0);
	IRAM[address]++;
	incrementProgramCounter();
}

void Processor::INC_atR1_0x07()
{
	unsigned int address = getRegister(R1);
	IRAM[address]++;
	incrementProgramCounter();
}

void Processor::INC_R0_0x08()
{
	unsigned int value = getRegister(R0);
	setRegister(R0,value+1);
	incrementProgramCounter();
}

void Processor::INC_R1_0x09()
{
	unsigned int value = getRegister(R1);
	setRegister(R1,value+1);
	incrementProgramCounter();
}

void Processor::INC_R2_0x0A()
{
	unsigned int value = getRegister(R2);
	setRegister(R2,value+1);
	incrementProgramCounter();
}

void Processor::INC_R3_0x0B()
{
	unsigned int value = getRegister(R3);
	setRegister(R3,value+1);
	incrementProgramCounter();
}

void Processor::INC_R4_0x0C()
{
	unsigned int value = getRegister(R4);
	setRegister(R4,value+1);
	incrementProgramCounter();
}

void Processor::INC_R5_0x0D()
{
	unsigned int value = getRegister(R5);
	setRegister(R5,value+1);
	incrementProgramCounter();
}

void Processor::INC_R6_0x0E()
{
	unsigned int value = getRegister(R6);
	setRegister(R6,value+1);
	incrementProgramCounter();
}

void Processor::INC_R7_0x0F()
{
	unsigned int value = getRegister(R7);
	setRegister(R7,value+1);
	incrementProgramCounter();
}

void Processor::INC_DPTR_0xA3()
{
	unsigned char DPLvalue = getRegister(DPL);
	if(DPLvalue == 0xFF)
	{
		setRegister(DPL,0);
		unsigned char DPHvalue = getRegister(DPH);
		if(DPHvalue == 0xFF)
		{
			setRegister(DPH,0);
		}
		else
		{
			setRegister(DPH,DPHvalue+1);
		}
	}
	else
	{
		setRegister(DPL,DPLvalue+1);
	}
	incrementProgramCounter();
}

unsigned short int Processor::calculateNewProgramCounterUsingAJMP(
		unsigned char ajmpOpCode)
{
	unsigned char bitsShouldStay = 0b11111000;
	unsigned char parameter = ROM[getProgramCounter()];
	unsigned short int calculatedProgramCounter;
	calculatedProgramCounter = calculatedProgramCounter >> 8;
	calculatedProgramCounter &= bitsShouldStay;
	calculatedProgramCounter += (ajmpOpCode >> 5);
	calculatedProgramCounter = calculatedProgramCounter << 8;
	calculatedProgramCounter += parameter;
	return calculatedProgramCounter;
}

void Processor::AJMP_page0_0x01()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0x01;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}

void Processor::AJMP_page1_0x21()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0x21;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}

void Processor::AJMP_page2_0x41()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0x41;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}

void Processor::AJMP_page3_0x61()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0x61;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}

void Processor::AJMP_page4_0x81()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0x81;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}

void Processor::AJMP_page5_0xA1()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0xA1;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}

void Processor::AJMP_page6_0xC1()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0xC1;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}

void Processor::AJMP_page7_0xE1()
{
	incrementProgramCounter();
	unsigned char ajmpOpCode	= 0xE1;
	PC = calculateNewProgramCounterUsingAJMP(ajmpOpCode);
}


void Processor::LJMP_code_addr_0x02()
{
	incrementProgramCounter();
	unsigned char higherbyte = ROM[getProgramCounter()];
	incrementProgramCounter();
	unsigned char lowerbyte = ROM[getProgramCounter()];
	unsigned short int newProgramCounter;
	newProgramCounter = higherbyte << 8;
	newProgramCounter += lowerbyte;
	PC = newProgramCounter;
}

void Processor::RR_A_0x03()
{
	unsigned char value = getRegister(A);
	unsigned char carrybit = value & 0x01;
	value = (value >> 1);
	carrybit = (carrybit << 7);
	value |= carrybit;
	setRegister(A,value);
}

void Processor::JBC_bitaddr_reladr_0x10()
{
	incrementProgramCounter();
	unsigned char bitaddr = ROM[getProgramCounter()];
	incrementProgramCounter();
	signed char relativeJMPAddress = ROM[getProgramCounter()];
	if(getBit(bitaddr))
	{
		clearBit(bitaddr);
		PC += relativeJMPAddress;
	}
	else
	{
		incrementProgramCounter();
	}

}


void Processor::LCALL_code_addr_0x12()
{
	incrementProgramCounter();
	unsigned char addrHighByte = ROM[getProgramCounter()];
	incrementProgramCounter();
	unsigned char addrLowByte = ROM[getProgramCounter()];
	incrementProgramCounter();
	pushToStack(getProgramCounterLowByte());
	pushToStack(getProgramCounterHighByte());
	unsigned short newProgramCounter;
	newProgramCounter = (addrHighByte << 8) | addrLowByte;
	PC = newProgramCounter;
}

void Processor::RRC_A_0x13()
{
	unsigned char value = getRegister(A);
	unsigned char lastBit = value%2;
	unsigned char carryFlag = getFlag(CY);
	value >>=1;
	carryFlag <<=7;
	value |=carryFlag;
	if(lastBit!=0)
	{
		setFlag(CY);
	}
	else
	{
		clearFlag(CY);
	}
	setRegister(A,value);
	incrementProgramCounter();

}

void Processor::DEC_A_0x14()
{
	unsigned int value = getRegister(A);
	setRegister(A,value-1);
	incrementProgramCounter();
}

void Processor::DEC_direct_0x15()
{
	incrementProgramCounter();
	unsigned int parameter = ROM[getProgramCounter()];
	IRAM[parameter]--;
	incrementProgramCounter();
}

void Processor::DEC_atR0_0x16()
{
	unsigned int address = getRegister(R0);
	IRAM[address]--;
	incrementProgramCounter();
}

void Processor::DEC_atR1_0x17()
{
	unsigned int address = getRegister(R1);
	IRAM[address]--;
	incrementProgramCounter();
}

void Processor::DEC_R0_0x18()
{
	unsigned int value = getRegister(R0);
	setRegister(R0,value-1);
	incrementProgramCounter();
}

void Processor::DEC_R1_0x19()
{
	unsigned int value = getRegister(R1);
	setRegister(R1,value-1);
	incrementProgramCounter();
}

void Processor::DEC_R2_0x1A()
{
	unsigned int value = getRegister(R2);
	setRegister(R2,value-1);
	incrementProgramCounter();
}

void Processor::DEC_R3_0x1B()
{
	unsigned int value = getRegister(R3);
	setRegister(R3,value-1);
	incrementProgramCounter();
}

void Processor::DEC_R4_0x1C()
{
	unsigned int value = getRegister(R4);
	setRegister(R4,value-1);
	incrementProgramCounter();
}

void Processor::DEC_R5_0x1D()
{
	unsigned int value = getRegister(R5);
	setRegister(R5,value-1);
	incrementProgramCounter();
}

void Processor::DEC_R6_0x1E()
{
	unsigned int value = getRegister(R6);
	setRegister(R6,value-1);
	incrementProgramCounter();
}

void Processor::DEC_R7_0x1F()
{
	unsigned int value = getRegister(R7);
	setRegister(R7,value-1);
	incrementProgramCounter();
}

void Processor::JB_bitaddr_reladdr_0x20()
{
	incrementProgramCounter();
	unsigned char bitaddr = ROM[getProgramCounter()];
	incrementProgramCounter();
	signed char relativeJMPAddress = ROM[getProgramCounter()];
	if(getBit(bitaddr))
	{
		PC += relativeJMPAddress;
	}
	else
	{
		incrementProgramCounter();
	}

}

void Processor::RET_0x22()
{
	unsigned char higherPCByte = popFromStack();
	unsigned char lowerPCByte = popFromStack();
	unsigned short int newPC = higherPCByte;
	newPC <<= 8;
	newPC += lowerPCByte;
	PC = newPC;
}

void Processor::RL_A_0x23()
{
	unsigned char value = getRegister(A);
	unsigned char carrybit = value >> 7;
	value <<= 1;
	value |= carrybit;
	setRegister(A,value);
}
/**
 *
 *
 *
 */
