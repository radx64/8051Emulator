#include <gtest/gtest.h>
#include "../src/Assembler.h"


TEST(Assembler, DISABLED_isCreatingObject)
{
	Assembler a;
}

TEST(Assembler, DISABLED_canAssembleNOPInstruction)
{
	Assembler a;
	BytesArray code = a.getOpCodes("NOP");
	ASSERT_EQ(0x00,code.getByteAtIndex(0));
}
