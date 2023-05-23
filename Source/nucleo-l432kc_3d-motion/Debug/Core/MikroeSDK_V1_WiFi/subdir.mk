################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MikroeSDK_V1_WiFi/main.c \
../Core/MikroeSDK_V1_WiFi/wifible.c 

OBJS += \
./Core/MikroeSDK_V1_WiFi/main.o \
./Core/MikroeSDK_V1_WiFi/wifible.o 

C_DEPS += \
./Core/MikroeSDK_V1_WiFi/main.d \
./Core/MikroeSDK_V1_WiFi/wifible.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MikroeSDK_V1_WiFi/%.o Core/MikroeSDK_V1_WiFi/%.su Core/MikroeSDK_V1_WiFi/%.cyclo: ../Core/MikroeSDK_V1_WiFi/%.c Core/MikroeSDK_V1_WiFi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-MikroeSDK_V1_WiFi

clean-Core-2f-MikroeSDK_V1_WiFi:
	-$(RM) ./Core/MikroeSDK_V1_WiFi/main.cyclo ./Core/MikroeSDK_V1_WiFi/main.d ./Core/MikroeSDK_V1_WiFi/main.o ./Core/MikroeSDK_V1_WiFi/main.su ./Core/MikroeSDK_V1_WiFi/wifible.cyclo ./Core/MikroeSDK_V1_WiFi/wifible.d ./Core/MikroeSDK_V1_WiFi/wifible.o ./Core/MikroeSDK_V1_WiFi/wifible.su

.PHONY: clean-Core-2f-MikroeSDK_V1_WiFi

