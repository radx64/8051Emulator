################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Assembler.cpp \
../src/BytesArray.cpp \
../src/Processor.cpp 

OBJS += \
./src/Assembler.o \
./src/BytesArray.o \
./src/Processor.o 

CPP_DEPS += \
./src/Assembler.d \
./src/BytesArray.d \
./src/Processor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


