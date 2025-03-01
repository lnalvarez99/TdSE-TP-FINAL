################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/app/src/app.c \
../Core/app/src/display.c \
../Core/app/src/logger.c \
../Core/app/src/task_actuator.c \
../Core/app/src/task_actuator_interface.c \
../Core/app/src/task_sensor.c \
../Core/app/src/task_setup.c \
../Core/app/src/task_setup_interface.c \
../Core/app/src/task_system.c \
../Core/app/src/task_system_interface.c 

OBJS += \
./Core/app/src/app.o \
./Core/app/src/display.o \
./Core/app/src/logger.o \
./Core/app/src/task_actuator.o \
./Core/app/src/task_actuator_interface.o \
./Core/app/src/task_sensor.o \
./Core/app/src/task_setup.o \
./Core/app/src/task_setup_interface.o \
./Core/app/src/task_system.o \
./Core/app/src/task_system_interface.o 

C_DEPS += \
./Core/app/src/app.d \
./Core/app/src/display.d \
./Core/app/src/logger.d \
./Core/app/src/task_actuator.d \
./Core/app/src/task_actuator_interface.d \
./Core/app/src/task_sensor.d \
./Core/app/src/task_setup.d \
./Core/app/src/task_setup_interface.d \
./Core/app/src/task_system.d \
./Core/app/src/task_system_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/app/src/%.o Core/app/src/%.su Core/app/src/%.cyclo: ../Core/app/src/%.c Core/app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../inc -I../Core/app/Inc -I"C:/Users/lauta/STM32CubeIDE/workspace_1.16.0/tdse-proyecto-final/Core/app/inc" -I"C:/Users/lauta/STM32CubeIDE/workspace_1.16.0/tdse-proyecto-final/Core/Inc" -I"C:/Users/lauta/STM32CubeIDE/workspace_1.16.0/tdse-proyecto-final/Core/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-app-2f-src

clean-Core-2f-app-2f-src:
	-$(RM) ./Core/app/src/app.cyclo ./Core/app/src/app.d ./Core/app/src/app.o ./Core/app/src/app.su ./Core/app/src/display.cyclo ./Core/app/src/display.d ./Core/app/src/display.o ./Core/app/src/display.su ./Core/app/src/logger.cyclo ./Core/app/src/logger.d ./Core/app/src/logger.o ./Core/app/src/logger.su ./Core/app/src/task_actuator.cyclo ./Core/app/src/task_actuator.d ./Core/app/src/task_actuator.o ./Core/app/src/task_actuator.su ./Core/app/src/task_actuator_interface.cyclo ./Core/app/src/task_actuator_interface.d ./Core/app/src/task_actuator_interface.o ./Core/app/src/task_actuator_interface.su ./Core/app/src/task_sensor.cyclo ./Core/app/src/task_sensor.d ./Core/app/src/task_sensor.o ./Core/app/src/task_sensor.su ./Core/app/src/task_setup.cyclo ./Core/app/src/task_setup.d ./Core/app/src/task_setup.o ./Core/app/src/task_setup.su ./Core/app/src/task_setup_interface.cyclo ./Core/app/src/task_setup_interface.d ./Core/app/src/task_setup_interface.o ./Core/app/src/task_setup_interface.su ./Core/app/src/task_system.cyclo ./Core/app/src/task_system.d ./Core/app/src/task_system.o ./Core/app/src/task_system.su ./Core/app/src/task_system_interface.cyclo ./Core/app/src/task_system_interface.d ./Core/app/src/task_system_interface.o ./Core/app/src/task_system_interface.su

.PHONY: clean-Core-2f-app-2f-src

