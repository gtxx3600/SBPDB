################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../QL/gram.c \
../QL/ql.c \
../QL/qlprt.c \
../QL/query.c \
../QL/translate.c 

OBJS += \
./QL/gram.o \
./QL/ql.o \
./QL/qlprt.o \
./QL/query.o \
./QL/translate.o 

C_DEPS += \
./QL/gram.d \
./QL/ql.d \
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


