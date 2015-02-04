################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/navX/IMU.cpp \
../src/navX/IMUAdvanced.cpp 

OBJS += \
./src/navX/IMU.o \
./src/navX/IMUAdvanced.o 

CPP_DEPS += \
./src/navX/IMU.d \
./src/navX/IMUAdvanced.d 


# Each subdirectory must supply rules for building sources it contributes
src/navX/%.o: ../src/navX/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


