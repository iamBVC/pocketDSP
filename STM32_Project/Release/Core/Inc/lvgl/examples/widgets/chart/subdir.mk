################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_1.c \
../Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_2.c \
../Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_3.c \
../Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_4.c \
../Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_5.c \
../Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_6.c \
../Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_7.c 

OBJS += \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_1.o \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_2.o \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_3.o \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_4.o \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_5.o \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_6.o \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_7.o 

C_DEPS += \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_1.d \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_2.d \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_3.d \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_4.d \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_5.d \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_6.d \
./Core/Inc/lvgl/examples/widgets/chart/lv_example_chart_7.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/examples/widgets/chart/%.o: ../Core/Inc/lvgl/examples/widgets/chart/%.c Core/Inc/lvgl/examples/widgets/chart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

