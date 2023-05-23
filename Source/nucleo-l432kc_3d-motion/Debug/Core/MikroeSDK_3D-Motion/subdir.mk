################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MikroeSDK_3D-Motion/flashupdate.c \
../Core/MikroeSDK_3D-Motion/i2cIO.c \
../Core/MikroeSDK_3D-Motion/interrupts.c \
../Core/MikroeSDK_3D-Motion/sensors.c \
../Core/MikroeSDK_3D-Motion/sf.c \
../Core/MikroeSDK_3D-Motion/system.c \
../Core/MikroeSDK_3D-Motion/vregs.c 

OBJS += \
./Core/MikroeSDK_3D-Motion/flashupdate.o \
./Core/MikroeSDK_3D-Motion/i2cIO.o \
./Core/MikroeSDK_3D-Motion/interrupts.o \
./Core/MikroeSDK_3D-Motion/sensors.o \
./Core/MikroeSDK_3D-Motion/sf.o \
./Core/MikroeSDK_3D-Motion/system.o \
./Core/MikroeSDK_3D-Motion/vregs.o 

C_DEPS += \
./Core/MikroeSDK_3D-Motion/flashupdate.d \
./Core/MikroeSDK_3D-Motion/i2cIO.d \
./Core/MikroeSDK_3D-Motion/interrupts.d \
./Core/MikroeSDK_3D-Motion/sensors.d \
./Core/MikroeSDK_3D-Motion/sf.d \
./Core/MikroeSDK_3D-Motion/system.d \
./Core/MikroeSDK_3D-Motion/vregs.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MikroeSDK_3D-Motion/%.o Core/MikroeSDK_3D-Motion/%.su Core/MikroeSDK_3D-Motion/%.cyclo: ../Core/MikroeSDK_3D-Motion/%.c Core/MikroeSDK_3D-Motion/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-MikroeSDK_3D-2d-Motion

clean-Core-2f-MikroeSDK_3D-2d-Motion:
	-$(RM) ./Core/MikroeSDK_3D-Motion/flashupdate.cyclo ./Core/MikroeSDK_3D-Motion/flashupdate.d ./Core/MikroeSDK_3D-Motion/flashupdate.o ./Core/MikroeSDK_3D-Motion/flashupdate.su ./Core/MikroeSDK_3D-Motion/i2cIO.cyclo ./Core/MikroeSDK_3D-Motion/i2cIO.d ./Core/MikroeSDK_3D-Motion/i2cIO.o ./Core/MikroeSDK_3D-Motion/i2cIO.su ./Core/MikroeSDK_3D-Motion/interrupts.cyclo ./Core/MikroeSDK_3D-Motion/interrupts.d ./Core/MikroeSDK_3D-Motion/interrupts.o ./Core/MikroeSDK_3D-Motion/interrupts.su ./Core/MikroeSDK_3D-Motion/sensors.cyclo ./Core/MikroeSDK_3D-Motion/sensors.d ./Core/MikroeSDK_3D-Motion/sensors.o ./Core/MikroeSDK_3D-Motion/sensors.su ./Core/MikroeSDK_3D-Motion/sf.cyclo ./Core/MikroeSDK_3D-Motion/sf.d ./Core/MikroeSDK_3D-Motion/sf.o ./Core/MikroeSDK_3D-Motion/sf.su ./Core/MikroeSDK_3D-Motion/system.cyclo ./Core/MikroeSDK_3D-Motion/system.d ./Core/MikroeSDK_3D-Motion/system.o ./Core/MikroeSDK_3D-Motion/system.su ./Core/MikroeSDK_3D-Motion/vregs.cyclo ./Core/MikroeSDK_3D-Motion/vregs.d ./Core/MikroeSDK_3D-Motion/vregs.o ./Core/MikroeSDK_3D-Motion/vregs.su

.PHONY: clean-Core-2f-MikroeSDK_3D-2d-Motion

