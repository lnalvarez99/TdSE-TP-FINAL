################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/startup/startup_stm32f103rbtx.s 

OBJS += \
./Core/startup/startup_stm32f103rbtx.o 

S_DEPS += \
./Core/startup/startup_stm32f103rbtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/startup/%.o: ../Core/startup/%.s Core/startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-startup

clean-Core-2f-startup:
	-$(RM) ./Core/startup/startup_stm32f103rbtx.d ./Core/startup/startup_stm32f103rbtx.o

.PHONY: clean-Core-2f-startup

