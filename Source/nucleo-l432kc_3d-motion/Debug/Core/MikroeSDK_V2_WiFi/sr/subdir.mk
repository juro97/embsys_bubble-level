################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MikroeSDK_V2_WiFi/sr/main.c \
../Core/MikroeSDK_V2_WiFi/sr/wifiesp.c 

OBJS += \
./Core/MikroeSDK_V2_WiFi/sr/main.o \
./Core/MikroeSDK_V2_WiFi/sr/wifiesp.o 

C_DEPS += \
./Core/MikroeSDK_V2_WiFi/sr/main.d \
./Core/MikroeSDK_V2_WiFi/sr/wifiesp.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MikroeSDK_V2_WiFi/sr/%.o Core/MikroeSDK_V2_WiFi/sr/%.su Core/MikroeSDK_V2_WiFi/sr/%.cyclo: ../Core/MikroeSDK_V2_WiFi/sr/%.c Core/MikroeSDK_V2_WiFi/sr/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-MikroeSDK_V2_WiFi-2f-sr

clean-Core-2f-MikroeSDK_V2_WiFi-2f-sr:
	-$(RM) ./Core/MikroeSDK_V2_WiFi/sr/main.cyclo ./Core/MikroeSDK_V2_WiFi/sr/main.d ./Core/MikroeSDK_V2_WiFi/sr/main.o ./Core/MikroeSDK_V2_WiFi/sr/main.su ./Core/MikroeSDK_V2_WiFi/sr/wifiesp.cyclo ./Core/MikroeSDK_V2_WiFi/sr/wifiesp.d ./Core/MikroeSDK_V2_WiFi/sr/wifiesp.o ./Core/MikroeSDK_V2_WiFi/sr/wifiesp.su

.PHONY: clean-Core-2f-MikroeSDK_V2_WiFi-2f-sr

