################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../QL/QL_Manager.c \
../QL/gram.c \
../QL/qlprt.c \
../QL/query.c \
../QL/translate.c 

OBJS += \
./QL/QL_Manager.o \
./QL/gram.o \
./QL/qlprt.o \
./QL/query.o \
./QL/translate.o 

C_DEPS += \
./QL/QL_Manager.d \
./QL/gram.d \
./QL/qlprt.d \
./QL/query.d \
./QL/translate.d 


# Each subdirectory must supply rules for building sources it contributes
QL/%.o: ../QL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/skaby/workspace/SBPDB/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


