################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/examples/styles/lv_example_style_1.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_10.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_11.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_12.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_13.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_14.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_2.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_3.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_4.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_5.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_6.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_7.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_8.c \
../Core/Inc/lvgl/examples/styles/lv_example_style_9.c 

OBJS += \
./Core/Inc/lvgl/examples/styles/lv_example_style_1.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_10.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_11.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_12.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_13.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_14.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_2.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_3.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_4.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_5.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_6.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_7.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_8.o \
./Core/Inc/lvgl/examples/styles/lv_example_style_9.o 

C_DEPS += \
./Core/Inc/lvgl/examples/styles/lv_example_style_1.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_10.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_11.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_12.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_13.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_14.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_2.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_3.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_4.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_5.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_6.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_7.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_8.d \
./Core/Inc/lvgl/examples/styles/lv_example_style_9.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/examples/styles/%.o: ../Core/Inc/lvgl/examples/styles/%.c Core/Inc/lvgl/examples/styles/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

