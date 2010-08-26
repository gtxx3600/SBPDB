################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../IX/BTNode.c \
../IX/IX_IndexHandle.c \
../IX/IX_IndexScan.c \
../IX/IX_Manager.c 

OBJS += \
./IX/BTNode.o \
./IX/IX_IndexHandle.o \
./IX/IX_IndexScan.o \
./IX/IX_Manager.o 

C_DEPS += \
./IX/BTNode.d \
./IX/IX_IndexHandle.d \
./IX/IX_IndexScan.d \
./IX/IX_Manager.d 


# Each subdirectory must supply rules for building sources it contributes
IX/%.o: ../IX/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/skaby/workspace/SBPDB/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


