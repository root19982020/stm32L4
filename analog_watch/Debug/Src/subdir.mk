################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DEV_Config.c \
../Src/GUI_Paint.c \
../Src/LCD_1inch28.c \
../Src/LCD_1inch28_test.c \
../Src/adc.c \
../Src/dma.c \
../Src/font12.c \
../Src/font12CN.c \
../Src/font16.c \
../Src/font20.c \
../Src/font24.c \
../Src/font24CN.c \
../Src/font8.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/image.c \
../Src/lcd_lvgl.c \
../Src/main.c \
../Src/rtc.c \
../Src/spi.c \
../Src/stm32l4xx_hal_msp.c \
../Src/stm32l4xx_hal_timebase_tim.c \
../Src/stm32l4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32l4xx.c \
../Src/tim.c 

OBJS += \
./Src/DEV_Config.o \
./Src/GUI_Paint.o \
./Src/LCD_1inch28.o \
./Src/LCD_1inch28_test.o \
./Src/adc.o \
./Src/dma.o \
./Src/font12.o \
./Src/font12CN.o \
./Src/font16.o \
./Src/font20.o \
./Src/font24.o \
./Src/font24CN.o \
./Src/font8.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/image.o \
./Src/lcd_lvgl.o \
./Src/main.o \
./Src/rtc.o \
./Src/spi.o \
./Src/stm32l4xx_hal_msp.o \
./Src/stm32l4xx_hal_timebase_tim.o \
./Src/stm32l4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32l4xx.o \
./Src/tim.o 

C_DEPS += \
./Src/DEV_Config.d \
./Src/GUI_Paint.d \
./Src/LCD_1inch28.d \
./Src/LCD_1inch28_test.d \
./Src/adc.d \
./Src/dma.d \
./Src/font12.d \
./Src/font12CN.d \
./Src/font16.d \
./Src/font20.d \
./Src/font24.d \
./Src/font24CN.d \
./Src/font8.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/image.d \
./Src/lcd_lvgl.d \
./Src/main.d \
./Src/rtc.d \
./Src/spi.d \
./Src/stm32l4xx_hal_msp.d \
./Src/stm32l4xx_hal_timebase_tim.d \
./Src/stm32l4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32l4xx.d \
./Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

