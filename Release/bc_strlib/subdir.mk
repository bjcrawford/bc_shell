################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bc_strlib/append.c \
../bc_strlib/chop.c \
../bc_strlib/count_occurences.c \
../bc_strlib/is_blank.c \
../bc_strlib/lstrip.c \
../bc_strlib/prefixcmp_igncase.c \
../bc_strlib/replace.c \
../bc_strlib/rstrip.c \
../bc_strlib/str_copy.c \
../bc_strlib/strcmp_igncase.c \
../bc_strlib/strip.c \
../bc_strlib/suffixcmp_igncase.c 

OBJS += \
./bc_strlib/append.o \
./bc_strlib/chop.o \
./bc_strlib/count_occurences.o \
./bc_strlib/is_blank.o \
./bc_strlib/lstrip.o \
./bc_strlib/prefixcmp_igncase.o \
./bc_strlib/replace.o \
./bc_strlib/rstrip.o \
./bc_strlib/str_copy.o \
./bc_strlib/strcmp_igncase.o \
./bc_strlib/strip.o \
./bc_strlib/suffixcmp_igncase.o 

C_DEPS += \
./bc_strlib/append.d \
./bc_strlib/chop.d \
./bc_strlib/count_occurences.d \
./bc_strlib/is_blank.d \
./bc_strlib/lstrip.d \
./bc_strlib/prefixcmp_igncase.d \
./bc_strlib/replace.d \
./bc_strlib/rstrip.d \
./bc_strlib/str_copy.d \
./bc_strlib/strcmp_igncase.d \
./bc_strlib/strip.d \
./bc_strlib/suffixcmp_igncase.d 


# Each subdirectory must supply rules for building sources it contributes
bc_strlib/%.o: ../bc_strlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


