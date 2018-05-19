################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gpio/GpioController.cpp 

OBJS += \
./src/gpio/GpioController.o 

CPP_DEPS += \
./src/gpio/GpioController.d 


# Each subdirectory must supply rules for building sources it contributes
src/gpio/%.o: ../src/gpio/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I../src/system -I/usr/include/boost -I../src/network -I../src/gpio -I../src/command -I../src/daemon -I../src/daemon/arg_parser -O2 -g -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


