################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MotionDriver_MikroSDK/Inc/_3D_Motion_click_main.c \
../Drivers/MotionDriver_MikroSDK/Inc/i2cIO.c \
../Drivers/MotionDriver_MikroSDK/Inc/interrupts.c \
../Drivers/MotionDriver_MikroSDK/Inc/sensors.c \
../Drivers/MotionDriver_MikroSDK/Inc/sf.c \
../Drivers/MotionDriver_MikroSDK/Inc/system.c \
../Drivers/MotionDriver_MikroSDK/Inc/vregs.c 

OBJS += \
./Drivers/MotionDriver_MikroSDK/Inc/_3D_Motion_click_main.o \
./Drivers/MotionDriver_MikroSDK/Inc/i2cIO.o \
./Drivers/MotionDriver_MikroSDK/Inc/interrupts.o \
./Drivers/MotionDriver_MikroSDK/Inc/sensors.o \
./Drivers/MotionDriver_MikroSDK/Inc/sf.o \
./Drivers/MotionDriver_MikroSDK/Inc/system.o \
./Drivers/MotionDriver_MikroSDK/Inc/vregs.o 

C_DEPS += \
./Drivers/MotionDriver_MikroSDK/Inc/_3D_Motion_click_main.d \
./Drivers/MotionDriver_MikroSDK/Inc/i2cIO.d \
./Drivers/MotionDriver_MikroSDK/Inc/interrupts.d \
./Drivers/MotionDriver_MikroSDK/Inc/sensors.d \
./Drivers/MotionDriver_MikroSDK/Inc/sf.d \
./Drivers/MotionDriver_MikroSDK/Inc/system.d \
./Drivers/MotionDriver_MikroSDK/Inc/vregs.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MotionDriver_MikroSDK/Inc/%.o Drivers/MotionDriver_MikroSDK/Inc/%.su Drivers/MotionDriver_MikroSDK/Inc/%.cyclo: ../Drivers/MotionDriver_MikroSDK/Inc/%.c Drivers/MotionDriver_MikroSDK/Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MotionDriver_MikroSDK-2f-Inc

clean-Drivers-2f-MotionDriver_MikroSDK-2f-Inc:
	-$(RM) ./Drivers/MotionDriver_MikroSDK/Inc/_3D_Motion_click_main.cyclo ./Drivers/MotionDriver_MikroSDK/Inc/_3D_Motion_click_main.d ./Drivers/MotionDriver_MikroSDK/Inc/_3D_Motion_click_main.o ./Drivers/MotionDriver_MikroSDK/Inc/_3D_Motion_click_main.su ./Drivers/MotionDriver_MikroSDK/Inc/i2cIO.cyclo ./Drivers/MotionDriver_MikroSDK/Inc/i2cIO.d ./Drivers/MotionDriver_MikroSDK/Inc/i2cIO.o ./Drivers/MotionDriver_MikroSDK/Inc/i2cIO.su ./Drivers/MotionDriver_MikroSDK/Inc/interrupts.cyclo ./Drivers/MotionDriver_MikroSDK/Inc/interrupts.d ./Drivers/MotionDriver_MikroSDK/Inc/interrupts.o ./Drivers/MotionDriver_MikroSDK/Inc/interrupts.su ./Drivers/MotionDriver_MikroSDK/Inc/sensors.cyclo ./Drivers/MotionDriver_MikroSDK/Inc/sensors.d ./Drivers/MotionDriver_MikroSDK/Inc/sensors.o ./Drivers/MotionDriver_MikroSDK/Inc/sensors.su ./Drivers/MotionDriver_MikroSDK/Inc/sf.cyclo ./Drivers/MotionDriver_MikroSDK/Inc/sf.d ./Drivers/MotionDriver_MikroSDK/Inc/sf.o ./Drivers/MotionDriver_MikroSDK/Inc/sf.su ./Drivers/MotionDriver_MikroSDK/Inc/system.cyclo ./Drivers/MotionDriver_MikroSDK/Inc/system.d ./Drivers/MotionDriver_MikroSDK/Inc/system.o ./Drivers/MotionDriver_MikroSDK/Inc/system.su ./Drivers/MotionDriver_MikroSDK/Inc/vregs.cyclo ./Drivers/MotionDriver_MikroSDK/Inc/vregs.d ./Drivers/MotionDriver_MikroSDK/Inc/vregs.o ./Drivers/MotionDriver_MikroSDK/Inc/vregs.su

.PHONY: clean-Drivers-2f-MotionDriver_MikroSDK-2f-Inc

