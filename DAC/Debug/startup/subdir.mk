################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib" -I"C:/Users/winstona/Documents/CE4020/DAC/inc" -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib/CMSIS/core" -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib/CMSIS/device" -I"C:/Users/winstona/Documents/CE4020/nucleo-f446re_stdperiph_lib/StdPeriph_Driver/inc" -I"C:/Users/winstona/Documents/CE4020/TLV/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


