################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PF/Buffer_Data.c \
../PF/PF_FileHandle.c \
../PF/PF_Manager.c \
../PF/PF_PageHandle.c \
../PF/hashmap.c \
../PF/list.c 

OBJS += \
./PF/Buffer_Data.o \
./PF/PF_FileHandle.o \
./PF/PF_Manager.o \
./PF/PF_PageHandle.o \
./PF/hashmap.o \
./PF/list.o 

C_DEPS += \
./PF/Buffer_Data.d \
./PF/PF_FileHandle.d \
./PF/PF_Manager.d \
./PF/PF_PageHandle.d \
./PF/hashmap.d \
./PF/list.d 


# Each subdirectory must supply rules for building sources it contributes
PF/%.o: ../PF/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/skaby/workspace/SBPDB/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


