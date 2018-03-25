################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/ringbuffer.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/tlv_dac.c 

OBJS += \
./src/main.o \
./src/ringbuffer.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/tlv_dac.o 

C_DEPS += \
./src/main.d \
./src/ringbuffer.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/tlv_dac.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F4 -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F446xx -I"C:/Users/sharana/Documents/Senior_Design/DAC/nucleo-f446re_stdperiph_lib" -I"C:/Users/sharana/Documents/Senior_Design/DAC/DAC/inc" -I"C:/Users/sharana/Documents/Senior_Design/DAC/nucleo-f446re_stdperiph_lib/CMSIS/core" -I"C:/Users/sharana/Documents/Senior_Design/DAC/nucleo-f446re_stdperiph_lib/CMSIS/device" -I"C:/Users/sharana/Documents/Senior_Design/DAC/nucleo-f446re_stdperiph_lib/StdPeriph_Driver/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


