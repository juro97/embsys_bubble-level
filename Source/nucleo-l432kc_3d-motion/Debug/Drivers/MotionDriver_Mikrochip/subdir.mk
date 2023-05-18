################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MotionDriver_Mikrochip/err.c \
../Drivers/MotionDriver_Mikrochip/flashupdate.c \
../Drivers/MotionDriver_Mikrochip/i2cIO.c \
../Drivers/MotionDriver_Mikrochip/lcd.c \
../Drivers/MotionDriver_Mikrochip/main.c \
../Drivers/MotionDriver_Mikrochip/mmain.c \
../Drivers/MotionDriver_Mikrochip/sf.c \
../Drivers/MotionDriver_Mikrochip/system.c \
../Drivers/MotionDriver_Mikrochip/utils.c \
../Drivers/MotionDriver_Mikrochip/vregs.c 

OBJS += \
./Drivers/MotionDriver_Mikrochip/err.o \
./Drivers/MotionDriver_Mikrochip/flashupdate.o \
./Drivers/MotionDriver_Mikrochip/i2cIO.o \
./Drivers/MotionDriver_Mikrochip/lcd.o \
./Drivers/MotionDriver_Mikrochip/main.o \
./Drivers/MotionDriver_Mikrochip/mmain.o \
./Drivers/MotionDriver_Mikrochip/sf.o \
./Drivers/MotionDriver_Mikrochip/system.o \
./Drivers/MotionDriver_Mikrochip/utils.o \
./Drivers/MotionDriver_Mikrochip/vregs.o 

C_DEPS += \
./Drivers/MotionDriver_Mikrochip/err.d \
./Drivers/MotionDriver_Mikrochip/flashupdate.d \
./Drivers/MotionDriver_Mikrochip/i2cIO.d \
./Drivers/MotionDriver_Mikrochip/lcd.d \
./Drivers/MotionDriver_Mikrochip/main.d \
./Drivers/MotionDriver_Mikrochip/mmain.d \
./Drivers/MotionDriver_Mikrochip/sf.d \
./Drivers/MotionDriver_Mikrochip/system.d \
./Drivers/MotionDriver_Mikrochip/utils.d \
./Drivers/MotionDriver_Mikrochip/vregs.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MotionDriver_Mikrochip/%.o Drivers/MotionDriver_Mikrochip/%.su Drivers/MotionDriver_Mikrochip/%.cyclo: ../Drivers/MotionDriver_Mikrochip/%.c Drivers/MotionDriver_Mikrochip/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MotionDriver_Mikrochip

clean-Drivers-2f-MotionDriver_Mikrochip:
	-$(RM) ./Drivers/MotionDriver_Mikrochip/err.cyclo ./Drivers/MotionDriver_Mikrochip/err.d ./Drivers/MotionDriver_Mikrochip/err.o ./Drivers/MotionDriver_Mikrochip/err.su ./Drivers/MotionDriver_Mikrochip/flashupdate.cyclo ./Drivers/MotionDriver_Mikrochip/flashupdate.d ./Drivers/MotionDriver_Mikrochip/flashupdate.o ./Drivers/MotionDriver_Mikrochip/flashupdate.su ./Drivers/MotionDriver_Mikrochip/i2cIO.cyclo ./Drivers/MotionDriver_Mikrochip/i2cIO.d ./Drivers/MotionDriver_Mikrochip/i2cIO.o ./Drivers/MotionDriver_Mikrochip/i2cIO.su ./Drivers/MotionDriver_Mikrochip/lcd.cyclo ./Drivers/MotionDriver_Mikrochip/lcd.d ./Drivers/MotionDriver_Mikrochip/lcd.o ./Drivers/MotionDriver_Mikrochip/lcd.su ./Drivers/MotionDriver_Mikrochip/main.cyclo ./Drivers/MotionDriver_Mikrochip/main.d ./Drivers/MotionDriver_Mikrochip/main.o ./Drivers/MotionDriver_Mikrochip/main.su ./Drivers/MotionDriver_Mikrochip/mmain.cyclo ./Drivers/MotionDriver_Mikrochip/mmain.d ./Drivers/MotionDriver_Mikrochip/mmain.o ./Drivers/MotionDriver_Mikrochip/mmain.su ./Drivers/MotionDriver_Mikrochip/sf.cyclo ./Drivers/MotionDriver_Mikrochip/sf.d ./Drivers/MotionDriver_Mikrochip/sf.o ./Drivers/MotionDriver_Mikrochip/sf.su ./Drivers/MotionDriver_Mikrochip/system.cyclo ./Drivers/MotionDriver_Mikrochip/system.d ./Drivers/MotionDriver_Mikrochip/system.o ./Drivers/MotionDriver_Mikrochip/system.su ./Drivers/MotionDriver_Mikrochip/utils.cyclo ./Drivers/MotionDriver_Mikrochip/utils.d ./Drivers/MotionDriver_Mikrochip/utils.o ./Drivers/MotionDriver_Mikrochip/utils.su ./Drivers/MotionDriver_Mikrochip/vregs.cyclo ./Drivers/MotionDriver_Mikrochip/vregs.d ./Drivers/MotionDriver_Mikrochip/vregs.o ./Drivers/MotionDriver_Mikrochip/vregs.su

.PHONY: clean-Drivers-2f-MotionDriver_Mikrochip

