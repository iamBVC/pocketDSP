################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/tests/src/lv_test_indev.c \
../Core/Inc/lvgl/tests/src/lv_test_init.c \
../Core/Inc/lvgl/tests/src/lv_test_main.c 

OBJS += \
./Core/Inc/lvgl/tests/src/lv_test_indev.o \
./Core/Inc/lvgl/tests/src/lv_test_init.o \
./Core/Inc/lvgl/tests/src/lv_test_main.o 

C_DEPS += \
./Core/Inc/lvgl/tests/src/lv_test_indev.d \
./Core/Inc/lvgl/tests/src/lv_test_init.d \
./Core/Inc/lvgl/tests/src/lv_test_main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/tests/src/%.o: ../Core/Inc/lvgl/tests/src/%.c Core/Inc/lvgl/tests/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

