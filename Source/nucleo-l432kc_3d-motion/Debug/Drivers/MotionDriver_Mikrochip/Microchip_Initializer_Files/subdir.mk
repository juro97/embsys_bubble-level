################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/ext_int.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/i2c1.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/interrupt_manager.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/mcc.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/pin_manager.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr1.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr2.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/traps.c \
../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/uart2.c 

OBJS += \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/ext_int.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/i2c1.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/interrupt_manager.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/mcc.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/pin_manager.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr1.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr2.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/traps.o \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/uart2.o 

C_DEPS += \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/ext_int.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/i2c1.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/interrupt_manager.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/mcc.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/pin_manager.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr1.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr2.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/traps.d \
./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/uart2.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/%.o Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/%.su Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/%.cyclo: ../Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/%.c Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MotionDriver_Mikrochip-2f-Microchip_Initializer_Files

clean-Drivers-2f-MotionDriver_Mikrochip-2f-Microchip_Initializer_Files:
	-$(RM) ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/ext_int.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/ext_int.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/ext_int.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/ext_int.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/i2c1.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/i2c1.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/i2c1.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/i2c1.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/interrupt_manager.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/interrupt_manager.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/interrupt_manager.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/interrupt_manager.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/mcc.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/mcc.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/mcc.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/mcc.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/pin_manager.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/pin_manager.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/pin_manager.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/pin_manager.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr1.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr1.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr1.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr1.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr2.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr2.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr2.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/tmr2.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/traps.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/traps.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/traps.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/traps.su ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/uart2.cyclo ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/uart2.d ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/uart2.o ./Drivers/MotionDriver_Mikrochip/Microchip_Initializer_Files/uart2.su

.PHONY: clean-Drivers-2f-MotionDriver_Mikrochip-2f-Microchip_Initializer_Files

