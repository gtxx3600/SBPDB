################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RM/AttrOpFunc.c \
../RM/RID.c \
../RM/RM_FileHandle.c \
../RM/RM_FileScan.c \
../RM/RM_Manager.c \
../RM/RM_Record.c 

OBJS += \
./RM/AttrOpFunc.o \
./RM/RID.o \
./RM/RM_FileHandle.o \
./RM/RM_FileScan.o \
./RM/RM_Manager.o \
./RM/RM_Record.o 

C_DEPS += \
./RM/AttrOpFunc.d \
./RM/RID.d \
./RM/RM_FileHandle.d \
./RM/RM_FileScan.d \
./RM/RM_Manager.d \
./RM/RM_Record.d 


# Each subdirectory must supply rules for building sources it contributes
RM/%.o: ../RM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/skaby/workspace/SBPDB/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


