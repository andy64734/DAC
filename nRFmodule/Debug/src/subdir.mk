################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/tm_stm32f4_delay.c \
../src/tm_stm32f4_exti.c \
../src/tm_stm32f4_gpio.c \
../src/tm_stm32f4_nrf24l01.c \
../src/tm_stm32f4_spi.c \
../src/tm_stm32f4_usart.c 

OBJS += \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/tm_stm32f4_delay.o \
./src/tm_stm32f4_exti.o \
./src/tm_stm32f4_gpio.o \
./src/tm_stm32f4_nrf24l01.o \
./src/tm_stm32f4_spi.o \
./src/tm_stm32f4_usart.o 

C_DEPS += \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/tm_stm32f4_delay.d \
./src/tm_stm32f4_exti.d \
./src/tm_stm32f4_gpio.d \
./src/tm_stm32f4_nrf24l01.d \
./src/tm_stm32f4_spi.d \
./src/tm_stm32f4_usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -DKEIL_IDE -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib" -I"C:/Users/winstona/Documents/CE4020/nRFmodule/StdPeriph_Driver/inc" -I"C:/Users/winstona/Documents/CE4020/nRFmodule/inc" -I"C:/Users/winstona/Documents/CE4020/nRFmodule/CMSIS/device" -I"C:/Users/winstona/Documents/CE4020/nRFmodule/CMSIS/core" -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib/StdPeriph_Driver/inc" -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib/CMSIS/core" -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib/CMSIS/device" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


