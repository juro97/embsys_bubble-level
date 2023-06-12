################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MikroSDK-ExampleCode/flashupdate.c \
../Core/MikroSDK-ExampleCode/i2cIO.c \
../Core/MikroSDK-ExampleCode/interrupts.c \
../Core/MikroSDK-ExampleCode/sensors.c \
../Core/MikroSDK-ExampleCode/sf.c \
../Core/MikroSDK-ExampleCode/system.c \
../Core/MikroSDK-ExampleCode/vregs.c 

OBJS += \
./Core/MikroSDK-ExampleCode/flashupdate.o \
./Core/MikroSDK-ExampleCode/i2cIO.o \
./Core/MikroSDK-ExampleCode/interrupts.o \
./Core/MikroSDK-ExampleCode/sensors.o \
./Core/MikroSDK-ExampleCode/sf.o \
./Core/MikroSDK-ExampleCode/system.o \
./Core/MikroSDK-ExampleCode/vregs.o 

C_DEPS += \
./Core/MikroSDK-ExampleCode/flashupdate.d \
./Core/MikroSDK-ExampleCode/i2cIO.d \
./Core/MikroSDK-ExampleCode/interrupts.d \
./Core/MikroSDK-ExampleCode/sensors.d \
./Core/MikroSDK-ExampleCode/sf.d \
./Core/MikroSDK-ExampleCode/system.d \
./Core/MikroSDK-ExampleCode/vregs.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MikroSDK-ExampleCode/%.o Core/MikroSDK-ExampleCode/%.su Core/MikroSDK-ExampleCode/%.cyclo: ../Core/MikroSDK-ExampleCode/%.c Core/MikroSDK-ExampleCode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-MikroSDK-2d-ExampleCode

clean-Core-2f-MikroSDK-2d-ExampleCode:
	-$(RM) ./Core/MikroSDK-ExampleCode/flashupdate.cyclo ./Core/MikroSDK-ExampleCode/flashupdate.d ./Core/MikroSDK-ExampleCode/flashupdate.o ./Core/MikroSDK-ExampleCode/flashupdate.su ./Core/MikroSDK-ExampleCode/i2cIO.cyclo ./Core/MikroSDK-ExampleCode/i2cIO.d ./Core/MikroSDK-ExampleCode/i2cIO.o ./Core/MikroSDK-ExampleCode/i2cIO.su ./Core/MikroSDK-ExampleCode/interrupts.cyclo ./Core/MikroSDK-ExampleCode/interrupts.d ./Core/MikroSDK-ExampleCode/interrupts.o ./Core/MikroSDK-ExampleCode/interrupts.su ./Core/MikroSDK-ExampleCode/sensors.cyclo ./Core/MikroSDK-ExampleCode/sensors.d ./Core/MikroSDK-ExampleCode/sensors.o ./Core/MikroSDK-ExampleCode/sensors.su ./Core/MikroSDK-ExampleCode/sf.cyclo ./Core/MikroSDK-ExampleCode/sf.d ./Core/MikroSDK-ExampleCode/sf.o ./Core/MikroSDK-ExampleCode/sf.su ./Core/MikroSDK-ExampleCode/system.cyclo ./Core/MikroSDK-ExampleCode/system.d ./Core/MikroSDK-ExampleCode/system.o ./Core/MikroSDK-ExampleCode/system.su ./Core/MikroSDK-ExampleCode/vregs.cyclo ./Core/MikroSDK-ExampleCode/vregs.d ./Core/MikroSDK-ExampleCode/vregs.o ./Core/MikroSDK-ExampleCode/vregs.su

.PHONY: clean-Core-2f-MikroSDK-2d-ExampleCode

