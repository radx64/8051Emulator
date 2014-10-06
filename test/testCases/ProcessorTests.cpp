#include <gtest/gtest.h>
#include "../src/Processor.h"
#include <iostream>
TEST(Processor, canCreateObject)
{
	Processor p;
}

TEST(Processor, isInternalMemoryEmptyAtBoot)
{
	Processor p;
	for(int i=0;i<IRAM_SIZE;++i)
	{
		if(i==SP) continue; //skips Stack pointer value
		ASSERT_EQ(0x00,p.IRAM[i]);
	}
}

TEST(Processor, isInternalMemoryEmptyAfterReboot)
{
	Processor p;
	for(int i=0;i<IRAM_SIZE;++i)
	{
		p.IRAM[i] = i;
	}
	p.Reboot();
	for(int i=0;i<IRAM_SIZE;++i)
	{
		if(i==SP) continue; //skips Stack pointer value
		ASSERT_EQ(0x00,p.IRAM[i]);
	}
}

TEST(Processor, canGetMemoryDump)
{
	Processor p;
	ASSERT_LT(0,p.getMemoryDump().size());
}

TEST(Processor, canSetAndGetRegistersBankToZero)
{
	Processor p;
	p.setRegistersBank(0);
	ASSERT_EQ(0,p.getRegistersBank());
}

TEST(Processor, canSetAndGetRegistersBankToOtherThanZero)
{
	Processor p;
	p.setRegistersBank(2);
	ASSERT_EQ(2,p.getRegistersBank());
}

TEST(Processor, canSetRegisterAtBank0)
{
	Processor p;
	p.setRegister(R0,15);
	ASSERT_EQ(15,p.IRAM[R0]);
}


TEST(Processor, canGetRegisterAtBank0)
{
	Processor p;
	p.setRegister(R0,15);
	ASSERT_EQ(15,p.getRegister(R0));
}

TEST(Processor, canSetRegisterAtBankOtherThan0)
{
	Processor p;
	unsigned char bankSize = 8;
	p.setRegistersBank(1);
	p.setRegister(R0,15);
	ASSERT_EQ(15,p.IRAM[R0+bankSize]);
}

TEST(Processor, canGetRegisterAtBankOtherThan0)
{
	Processor p;
	p.setRegistersBank(1);
	p.setRegister(R0,15);
	ASSERT_EQ(15,p.getRegister(R0));
}

TEST(Processor, canLoadProgramToROM)
{
	Processor p;
	unsigned char simpleProgram[] = {0x05,0xE0,0x80,0xFC};	//Increment accumulator and jump to address -3
	p.loadHexProgramToROM(simpleProgram,4);
	for(int i=0; i<4;++i) ASSERT_EQ(simpleProgram[i],p.ROM[i]);
}

TEST(Processor, isProgramCounterValueZeroAtBoot)
{
	Processor p;
	ASSERT_EQ(0,p.getProgramCounter());
}

TEST(Processor, isProgramCounterValueZeroAtReboot)
{
	Processor p;
	p.PC = 0x05;
	p.Reboot();
	ASSERT_EQ(0,p.getProgramCounter());
}

TEST(Processor, isStepExecutingInstruction)
{
	Processor p;
	p.step();
	ASSERT_EQ((unsigned short int)1,p.getProgramCounter());
}

TEST(Processor, is5StepsExecutingInstructions)
{
	Processor p;
	for(int i=0; i<5;++i) p.step();
	ASSERT_EQ(5,p.getProgramCounter());
}

TEST(Processor_NOP, isNOP_0x00InstructionWorks)
{
	Processor p;
	unsigned char simpleNOPProgram[] = {0x00,0x00,0x00,0x00,0x00};
	p.loadHexProgramToROM(simpleNOPProgram,5);
	for(int i=0; i<5;++i) p.step();
	ASSERT_EQ(5,p.getProgramCounter());
}

TEST(Processor_INC, isINC_A_0x04InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x04,0x04};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(A));
}

TEST(Processor_INC, isINC_A_0x04atMaxValueMakes0)
{
	Processor p;
	unsigned char simpleProgram[] = {0x04};
	p.loadHexProgramToROM(simpleProgram,1);
	p.setRegister(A,0xFF);
	p.step();
	ASSERT_EQ(0,p.getRegister(A));
}

TEST(Processor_INC, isINC_direct_0x05InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x05,0xE0,0x05,0xE0};
	p.loadHexProgramToROM(simpleProgram,4);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(A));
}

TEST(Processor_INC, isINC_direct_0x05AtMaxValueMakes0)
{
	Processor p;
	unsigned char simpleProgram[] = {0x05,0xE0};
	p.loadHexProgramToROM(simpleProgram,2);
	p.setRegister(A,0xFF);
	p.step();
	ASSERT_EQ(0,p.getRegister(A));
}

TEST(Processor_INC, isINC_atR0_0x06InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x06,0x06,0x06,0x06};
	unsigned addresInR0 = 0x10;
	p.loadHexProgramToROM(simpleProgram,4);
	p.setRegister(R0,addresInR0);
	p.step();
	p.step();
	p.step();
	p.step();
	ASSERT_EQ(4,p.IRAM[addresInR0]);
}

TEST(Processor_INC, isINC_atR1_0x07InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x07,0x07,0x07,0x07};
	unsigned addresInR1 = 0x10;
	p.loadHexProgramToROM(simpleProgram,4);
	p.setRegister(R1,addresInR1);
	p.step();
	p.step();
	p.step();
	p.step();
	ASSERT_EQ(4,p.IRAM[addresInR1]);
}

TEST(Processor_INC, isINC_R0_0x08InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x08,0x08};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R0));
}

TEST(Processor_INC, isINC_R1_0x09InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x09,0x09};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R1));
}


TEST(Processor_INC, isINC_R2_0x0AInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x0A,0x0A};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R2));
}

TEST(Processor_INC, isINC_R3_0x0BInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x0B,0x0B};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R3));
}

TEST(Processor_INC, isINC_R4_0x0CInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x0C,0x0C};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R4));
}

TEST(Processor_INC, isINC_R5_0x0DInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x0D,0x0D};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R5));
}

TEST(Processor_INC, isINC_R6_0x0EInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x0E,0x0E};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R6));
}

TEST(Processor_INC, isINC_R7_0x0FInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x0F,0x0F};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(2,p.getRegister(R7));
}

TEST(Processor_INC, isINC_DPTR_0xA3InstructionCarriesOverflowFromDPL_to_DPH)
{
	Processor p;
	unsigned char simpleProgram[] = {0xA3,0xA3};
	p.loadHexProgramToROM(simpleProgram,2);
	p.setRegister(DPL,0xFF);
	p.step();
	p.step();
	ASSERT_EQ(1,p.getRegister(DPL));
	ASSERT_EQ(1,p.getRegister(DPH));
}

// PC
//	0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0
//
// AJMP 0x21 -- 0x00
//  0 1 0 0 0 0 0 1 | 0 0 0 0 0 0 0 0
//  ^ ^ ^
//  '-'-'-----v-v-v
// NEW PC
//  0 0 0 0 0 0 1 0 | 0 0 0 0 0 0 0 0



TEST(Processor_AJMP, isAJMP_page0_0x01CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0x01,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0000,p.getProgramCounter());
}

TEST(Processor_AJMP, isAJMP_page1_0x21CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0x21,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0100,p.getProgramCounter());
}

TEST(Processor_AJMP, isAJMP_page2_0x41CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0x41,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0200,p.getProgramCounter());
}

TEST(Processor_AJMP, isAJMP_page3_0x61CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0x61,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0300,p.getProgramCounter());
}

TEST(Processor_AJMP, isAJMP_page4_0x81CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0x81,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0400,p.getProgramCounter());
}

TEST(Processor_AJMP, isAJMP_page5_0xA1CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0xA1,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0500,p.getProgramCounter());
}

TEST(Processor_AJMP, isAJMP_page6_0xC1CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0xC1,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0600,p.getProgramCounter());
}
TEST(Processor_AJMP, isAJMP_page7_0xE1CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0xE1,0x00};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	ASSERT_EQ(0x0700,p.getProgramCounter());
}

TEST(Processor_StackSupport, isStackPointerEquals07AfterReboot)
{
	Processor p;
	ASSERT_EQ(0x07,p.getStackPointer());
}

TEST(Processor_StackSupport, isPushingValueIncrementsStackPointer)
{
	Processor p;
	p.pushToStack(0x00);
	ASSERT_EQ(0x08,p.getStackPointer());
}

TEST(Processor_StackSupport, isPushingValueToStackCorrectly)
{
	Processor p;
	p.pushToStack(0x01);
	ASSERT_EQ(0x01,p.IRAM[p.getStackPointer()]);
}

TEST(Processor_StackSupport, isPopingValueDecrementsStackPointer)
{
	Processor p;
	p.pushToStack(0x10);
	ASSERT_EQ(0x10,p.popFromStack());
}

TEST(Processor_LJMP, isLJMP_code_addr_0x02_CalculatesCorrectProgramCounter)
{
	Processor p;
	unsigned char simpleProgram[] = {0x02,0x12,0x34};
	p.loadHexProgramToROM(simpleProgram,3);
	p.step();
	ASSERT_EQ(0x1234,p.getProgramCounter());
}

TEST(Processor_RR_A, isRR_A_0x03_isWorkingCorrectly)
{
	Processor p;
	unsigned char simpleProgram[] = {0x03};
	p.setRegister(A,0b00000001);
	p.loadHexProgramToROM(simpleProgram,1);
	p.step();
	ASSERT_EQ(0b10000000,p.getRegister(A));
}

TEST(Processor_setBit, isSetBitSetsCorrectBit)
{
	Processor p;
	p.setBit(0x00);
	p.setBit(0x01);
	ASSERT_EQ(0x03,p.IRAM[BITS]);
}

TEST(Processor_clearBit, isClearBitCleansCorrectBit)
{
	Processor p;
	p.setBit(0x00);
	p.setBit(0x01);
	p.clearBit(0x00);
	p.clearBit(0x01);
	ASSERT_EQ(0x00, p.IRAM[BITS]);
}

TEST(Processor_getBit, isGetBitGetsCorrectBit)
{
	Processor p;
	p.setBit(0x00);
	ASSERT_TRUE(p.getBit(0x00));
	ASSERT_FALSE(p.getBit(0x01));
}



TEST(Processor_JBC, isJBC_bitaddr_reladdr_0x10WorkingCorrectly_JumpsWhenBit0x00isSet)
{
	Processor p;
	unsigned char simpleProgram[] = {0x10,0x00,0x10};
	p.setBit(0x00);
	p.loadHexProgramToROM(simpleProgram,3);
	p.step();
	ASSERT_EQ(0x00,p.IRAM[BITS]);
	ASSERT_EQ(0x12,p.getProgramCounter());
}

TEST(Processor_JBC, isJBC_bitaddr_reladdr_0x10WorkingCorrectly_NotJumpsWhenBit0x00isNotSet)
{
	Processor p;
	unsigned char simpleProgram[] = {0x10,0x00,0x10};
	p.clearBit(0x00);
	p.loadHexProgramToROM(simpleProgram,3);
	p.step();
	ASSERT_EQ(0x00,p.IRAM[BITS]);
	ASSERT_EQ(0x03,p.getProgramCounter());
}

TEST(Processor_LCALL, isLCALL_code_addr_0x12WorkingCorectly)
{
	Processor p;
	unsigned char simpleProgram[] = {0x12,0x10,0x10};
	p.loadHexProgramToROM(simpleProgram,3);
	p.step();
	ASSERT_EQ(0x00,p.IRAM[p.getStackPointer()]);
	ASSERT_EQ(0x03,p.IRAM[p.getStackPointer()-1]);
	ASSERT_EQ(0x1010,p.getProgramCounter());
}

TEST(Processor_flagSetSupport, canSetAndGet_P_Flag)
{
	Processor p;
	p.setFlag(P);
	ASSERT_TRUE(p.getFlag(P));
}

TEST(Processor_flagSetSupport, canSetAndGet_UD_Flag)
{
	Processor p;
	p.setFlag(UD);
	ASSERT_TRUE(p.getFlag(UD));
}
TEST(Processor_flagSetSupport, canSetAndGet_OV_Flag)
{
	Processor p;
	p.setFlag(OV);
	ASSERT_TRUE(p.getFlag(OV));
}

TEST(Processor_flagSetSupport, canSetAndGet_RS0_Flag)
{
	Processor p;
	p.setFlag(RS0);
	ASSERT_TRUE(p.getFlag(RS0));
}

TEST(Processor_flagSetSupport, canSetAndGet_RS1_Flag)
{
	Processor p;
	p.setFlag(RS1);
	ASSERT_TRUE(p.getFlag(RS1));
}
TEST(Processor_flagSetSupport, canSetAndGet_F0_Flag)
{
	Processor p;
	p.setFlag(F0);
	ASSERT_TRUE(p.getFlag(F0));
}
TEST(Processor_flagSetSupport, canSetAndGet_AC_Flag)
{
	Processor p;
	p.setFlag(AC);
	ASSERT_TRUE(p.getFlag(AC));
}

TEST(Processor_flagSetSupport, canSetAndGet_CY_Flag)
{
	Processor p;
	p.setFlag(CY);
	ASSERT_TRUE(p.getFlag(CY));
}

TEST(Processor_flagClearSupport, canClear_P_Flag)
{
	Processor p;
	p.setFlag(P);
	p.clearFlag(P);
	ASSERT_FALSE(p.getFlag(P));
}

TEST(Processor_flagClearSupport, canClear_UD_Flag)
{
	Processor p;
	p.setFlag(UD);
	p.clearFlag(UD);
	ASSERT_FALSE(p.getFlag(UD));
}

TEST(Processor_flagClearSupport, canClear_OV_Flag)
{
	Processor p;
	p.setFlag(OV);
	p.clearFlag(OV);
	ASSERT_FALSE(p.getFlag(OV));
}

TEST(Processor_flagClearSupport, canClear_RS0_Flag)
{
	Processor p;
	p.setFlag(RS0);
	p.clearFlag(RS0);
	ASSERT_FALSE(p.getFlag(RS0));
}

TEST(Processor_flagClearSupport, canClear_RS1_Flag)
{
	Processor p;
	p.setFlag(RS1);
	p.clearFlag(RS1);
	ASSERT_FALSE(p.getFlag(RS1));
}

TEST(Processor_flagClearSupport, canClear_F0_Flag)
{
	Processor p;
	p.setFlag(F0);
	p.clearFlag(F0);
	ASSERT_FALSE(p.getFlag(F0));
}

TEST(Processor_flagClearSupport, canClear_AC_Flag)
{
	Processor p;
	p.setFlag(AC);
	p.clearFlag(AC);
	ASSERT_FALSE(p.getFlag(AC));
}

TEST(Processor_flagClearSupport, canClear_CY_Flag)
{
	Processor p;
	p.setFlag(CY);
	p.clearFlag(CY);
	ASSERT_FALSE(p.getFlag(CY));
}

TEST(Processor_RRC, isRRC_A_0x13_isWorkingCorrectly)
{
	Processor p;
	unsigned char simpleProgram[] = {0x13};
	p.loadHexProgramToROM(simpleProgram,1);
	p.setRegister(A,0xFF);
	p.step();
	ASSERT_TRUE(p.getFlag(CY));
	ASSERT_EQ(0x7F,p.getRegister(A));
}

TEST(Processor_DEC, isDEC_A_0x14InstructionWorking)
{
	Processor p;
	p.setRegister(A,2);
	unsigned char simpleProgram[] = {0x14,0x14};
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(A));
}

TEST(Processor_DEC, isDEC_A_0x14at0ValueMakes0xFF)
{
	Processor p;
	p.setRegister(A,0);
	unsigned char simpleProgram[] = {0x14};
	p.loadHexProgramToROM(simpleProgram,1);
	p.step();
	ASSERT_EQ(0xFF,p.getRegister(A));
}

TEST(Processor_DEC, isDEC_direct_0x15InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x15,0xE0,0x15,0xE0};
	p.loadHexProgramToROM(simpleProgram,4);
	p.step();
	p.step();
	ASSERT_EQ(0xFE,p.getRegister(A));
}

TEST(Processor_DEC, isDEC_atR0_0x16InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x16,0x16,0x16,0x16};
	unsigned addresInR0 = 0x10;
	p.loadHexProgramToROM(simpleProgram,4);
	p.setRegister(R0,addresInR0);
	p.IRAM[addresInR0] = 4;
	p.step();
	p.step();
	p.step();
	p.step();
	ASSERT_EQ(0,p.IRAM[addresInR0]);
}

TEST(Processor_DEC, isDEC_atR1_0x17InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x17,0x17,0x17,0x17};
	unsigned addresInR1 = 0x10;
	p.loadHexProgramToROM(simpleProgram,4);
	p.setRegister(R1,addresInR1);
	p.IRAM[addresInR1] = 4;
	p.step();
	p.step();
	p.step();
	p.step();
	ASSERT_EQ(0,p.IRAM[addresInR1]);
}

TEST(Processor_DEC, isDEC_R0_0x18InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x18,0x18};
	p.setRegister(R0,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R0));
}

TEST(Processor_DEC, isDEC_R1_0x19InstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x19,0x19};
	p.setRegister(R1,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R1));
}

TEST(Processor_DEC, isDEC_R2_0x1AInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x1A,0x1A};
	p.setRegister(R2,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R2));
}

TEST(Processor_DEC, isDEC_R3_0x1BInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x1B,0x1B};
	p.setRegister(R3,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R3));
}

TEST(Processor_DEC, isDEC_R4_0x1CInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x1C,0x1C};
	p.setRegister(R4,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R4));
}

TEST(Processor_DEC, isDEC_R5_0x1DInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x1D,0x1D};
	p.setRegister(R5,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R5));
}

TEST(Processor_DEC, isDEC_R6_0x1EInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x1E,0x1E};
	p.setRegister(R6,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R6));
}

TEST(Processor_DEC, isDEC_R7_0x1FInstructionWorking)
{
	Processor p;
	unsigned char simpleProgram[] = {0x1F,0x1F};
	p.setRegister(R7,2);
	p.loadHexProgramToROM(simpleProgram,2);
	p.step();
	p.step();
	ASSERT_EQ(0,p.getRegister(R7));
}

TEST(Processor_JB, isJB_bitaddr_reladdr_0x20WorkingCorrectly_JumpsWhenBit0x00isSet)
{
	Processor p;
	unsigned char simpleProgram[] = {0x20,0x00,0x10};
	p.setBit(0x00);
	p.loadHexProgramToROM(simpleProgram,3);
	p.step();
	ASSERT_EQ(0x01,p.getBit(0x00));
	ASSERT_EQ(0x12,p.getProgramCounter());
}

TEST(Processor_RET, isRET_0x22WorkingCorrectly)
{
	Processor p;
	unsigned char simpleProgram[] = {0x12,0x00,0x05,0x00,0x00,0x022};
	p.loadHexProgramToROM(simpleProgram,6);
	p.step();
	p.step();
	ASSERT_EQ(0x03,p.getProgramCounter());
}

TEST(Processor_RL_A, isRR_A_0x23_isWorkingCorrectly)
{
	Processor p;
	unsigned char simpleProgram[] = {0x23};
	p.setRegister(A,0b00000001);
	p.loadHexProgramToROM(simpleProgram,1);
	p.step();
	ASSERT_EQ(0b00000010,p.getRegister(A));
}


