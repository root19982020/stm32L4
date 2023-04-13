################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/examples/others/msg/lv_example_msg_1.c \
../lvgl/examples/others/msg/lv_example_msg_2.c \
../lvgl/examples/others/msg/lv_example_msg_3.c 

OBJS += \
./lvgl/examples/others/msg/lv_example_msg_1.o \
./lvgl/examples/others/msg/lv_example_msg_2.o \
./lvgl/examples/others/msg/lv_example_msg_3.o 

C_DEPS += \
./lvgl/examples/others/msg/lv_example_msg_1.d \
./lvgl/examples/others/msg/lv_example_msg_2.d \
./lvgl/examples/others/msg/lv_example_msg_3.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/examples/others/msg/%.o: ../lvgl/examples/others/msg/%.c lvgl/examples/others/msg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

