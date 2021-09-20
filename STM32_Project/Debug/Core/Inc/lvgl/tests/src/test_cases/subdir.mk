################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/tests/src/test_cases/_test_template.c \
../Core/Inc/lvgl/tests/src/test_cases/test_config.c \
../Core/Inc/lvgl/tests/src/test_cases/test_dropdown.c \
../Core/Inc/lvgl/tests/src/test_cases/test_font_loader.c \
../Core/Inc/lvgl/tests/src/test_cases/test_obj_tree.c \
../Core/Inc/lvgl/tests/src/test_cases/test_style.c 

OBJS += \
./Core/Inc/lvgl/tests/src/test_cases/_test_template.o \
./Core/Inc/lvgl/tests/src/test_cases/test_config.o \
./Core/Inc/lvgl/tests/src/test_cases/test_dropdown.o \
./Core/Inc/lvgl/tests/src/test_cases/test_font_loader.o \
./Core/Inc/lvgl/tests/src/test_cases/test_obj_tree.o \
./Core/Inc/lvgl/tests/src/test_cases/test_style.o 

C_DEPS += \
./Core/Inc/lvgl/tests/src/test_cases/_test_template.d \
./Core/Inc/lvgl/tests/src/test_cases/test_config.d \
./Core/Inc/lvgl/tests/src/test_cases/test_dropdown.d \
./Core/Inc/lvgl/tests/src/test_cases/test_font_loader.d \
./Core/Inc/lvgl/tests/src/test_cases/test_obj_tree.d \
./Core/Inc/lvgl/tests/src/test_cases/test_style.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/tests/src/test_cases/%.o: ../Core/Inc/lvgl/tests/src/test_cases/%.c Core/Inc/lvgl/tests/src/test_cases/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

