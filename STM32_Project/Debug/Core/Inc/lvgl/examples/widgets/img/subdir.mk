################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/examples/widgets/img/lv_example_img_1.c \
../Core/Inc/lvgl/examples/widgets/img/lv_example_img_2.c \
../Core/Inc/lvgl/examples/widgets/img/lv_example_img_3.c \
../Core/Inc/lvgl/examples/widgets/img/lv_example_img_4.c 

OBJS += \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_1.o \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_2.o \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_3.o \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_4.o 

C_DEPS += \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_1.d \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_2.d \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_3.d \
./Core/Inc/lvgl/examples/widgets/img/lv_example_img_4.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/examples/widgets/img/%.o: ../Core/Inc/lvgl/examples/widgets/img/%.c Core/Inc/lvgl/examples/widgets/img/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

