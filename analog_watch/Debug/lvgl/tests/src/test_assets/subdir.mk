################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/tests/src/test_assets/font_1.c \
../lvgl/tests/src/test_assets/font_2.c \
../lvgl/tests/src/test_assets/font_3.c \
../lvgl/tests/src/test_assets/ubuntu_font.c 

OBJS += \
./lvgl/tests/src/test_assets/font_1.o \
./lvgl/tests/src/test_assets/font_2.o \
./lvgl/tests/src/test_assets/font_3.o \
./lvgl/tests/src/test_assets/ubuntu_font.o 

C_DEPS += \
./lvgl/tests/src/test_assets/font_1.d \
./lvgl/tests/src/test_assets/font_2.d \
./lvgl/tests/src/test_assets/font_3.d \
./lvgl/tests/src/test_assets/ubuntu_font.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/tests/src/test_assets/%.o: ../lvgl/tests/src/test_assets/%.c lvgl/tests/src/test_assets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

