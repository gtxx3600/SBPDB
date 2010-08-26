################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SM/SM_Manager.c \
../SM/dbcreate.c 

OBJS += \
./SM/SM_Manager.o \
./SM/dbcreate.o 

C_DEPS += \
./SM/SM_Manager.d \
./SM/dbcreate.d 


# Each subdirectory must supply rules for building sources it contributes
SM/%.o: ../SM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/skaby/workspace/SBPDB/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


