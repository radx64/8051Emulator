################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/AllTest.cpp \
../test/AssemblerTests.cpp \
../test/ProcessorTests.cpp \
../test/bytesArrayTests.cpp 

OBJS += \
./test/AllTest.o \
./test/AssemblerTests.o \
./test/ProcessorTests.o \
./test/bytesArrayTests.o 

CPP_DEPS += \
./test/AllTest.d \
./test/AssemblerTests.d \
./test/ProcessorTests.d \
./test/bytesArrayTests.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I/cygdrive/d/userdata/rszewczy/workspace/TDD/contrib -I/cygdrive/d/userdata/rszewczy/workspace/TDD/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


