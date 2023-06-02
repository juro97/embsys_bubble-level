################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MikroeSDK_WiFiBLE_Click/main.c \
../Core/MikroeSDK_WiFiBLE_Click/wifible.c 

OBJS += \
./Core/MikroeSDK_WiFiBLE_Click/main.o \
./Core/MikroeSDK_WiFiBLE_Click/wifible.o 

C_DEPS += \
./Core/MikroeSDK_WiFiBLE_Click/main.d \
./Core/MikroeSDK_WiFiBLE_Click/wifible.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MikroeSDK_WiFiBLE_Click/%.o Core/MikroeSDK_WiFiBLE_Click/%.su Core/MikroeSDK_WiFiBLE_Click/%.cyclo: ../Core/MikroeSDK_WiFiBLE_Click/%.c Core/MikroeSDK_WiFiBLE_Click/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-MikroeSDK_WiFiBLE_Click

clean-Core-2f-MikroeSDK_WiFiBLE_Click:
	-$(RM) ./Core/MikroeSDK_WiFiBLE_Click/main.cyclo ./Core/MikroeSDK_WiFiBLE_Click/main.d ./Core/MikroeSDK_WiFiBLE_Click/main.o ./Core/MikroeSDK_WiFiBLE_Click/main.su ./Core/MikroeSDK_WiFiBLE_Click/wifible.cyclo ./Core/MikroeSDK_WiFiBLE_Click/wifible.d ./Core/MikroeSDK_WiFiBLE_Click/wifible.o ./Core/MikroeSDK_WiFiBLE_Click/wifible.su

.PHONY: clean-Core-2f-MikroeSDK_WiFiBLE_Click

