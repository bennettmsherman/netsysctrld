################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/command/CommandParser.cpp 

OBJS += \
./src/command/CommandParser.o 

CPP_DEPS += \
./src/command/CommandParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/command/%.o: ../src/command/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I../src/system -I/usr/include/boost -I../src/runner -I../src/network -I../src/command -I../src/daemon -I../src/daemon/arg_parser -O2 -g -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


