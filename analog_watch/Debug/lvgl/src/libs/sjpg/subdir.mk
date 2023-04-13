################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/libs/sjpg/lv_sjpg.c \
../lvgl/src/libs/sjpg/tjpgd.c 

OBJS += \
./lvgl/src/libs/sjpg/lv_sjpg.o \
./lvgl/src/libs/sjpg/tjpgd.o 

C_DEPS += \
./lvgl/src/libs/sjpg/lv_sjpg.d \
./lvgl/src/libs/sjpg/tjpgd.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/libs/sjpg/%.o: ../lvgl/src/libs/sjpg/%.c lvgl/src/libs/sjpg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

