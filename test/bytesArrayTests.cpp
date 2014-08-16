#include <gtest/gtest.h>
#include "../src/BytesArray.h"

TEST(bytesArray, isCreatingObject)
{
	BytesArray bytes;
}


TEST(bytesArray, canGetSizeOfEmptyArray)
{
	BytesArray bytes;
	ASSERT_EQ(0,bytes.size());
}

TEST(bytesArray, canPushByteToArray)
{
	BytesArray bytes;
	bytes.push(1);
	ASSERT_EQ(1,bytes.size());
}

TEST(bytesArray, canPushMultipleBytesToArray)
{
	BytesArray bytes;
	bytes.push(1);
	bytes.push(2);
	bytes.push(123);
	ASSERT_EQ(3,bytes.size());
}

TEST(bytesArray, canGetSpecificByteValue)
{
	BytesArray bytes;
	bytes.push(1);
	bytes.push(2);
	ASSERT_EQ(1,bytes.getByteAtIndex(0));
	ASSERT_EQ(2,bytes.getByteAtIndex(1));
}

TEST(bytesArray, throwsExceptionWhenCallingOutOfRange)
{
	BytesArray bytes;
	bytes.push(1);
	ASSERT_ANY_THROW(bytes.getByteAtIndex(10));
}

TEST(bytesArray, canCleanArray)
{
	BytesArray bytes;
	bytes.push(1);
	bytes.push(2);
	bytes.clear();
	ASSERT_EQ(0,bytes.size());
}

TEST(bytesArray, isToStringCorrectlyFormattedOnSingleByte)
{
	BytesArray bytes;
	bytes.push(1);
	ASSERT_STREQ("0x01", bytes.toString().c_str());
}

TEST(bytesArray, isToStringCorrectlyFormattedOnMultipleBytes)
{
	BytesArray bytes;
	bytes.push(1);
	bytes.push(2);
	bytes.push(255);
	ASSERT_STREQ("0x01 0x02 0xFF", bytes.toString().c_str());
}

