################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Autonomous.cpp \
../src/CTankDrive.cpp \
../src/Robot.cpp 

OBJS += \
./src/Autonomous.o \
./src/CTankDrive.o \
./src/Robot.o 

CPP_DEPS += \
./src/Autonomous.d \
./src/CTankDrive.d \
./src/Robot.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


