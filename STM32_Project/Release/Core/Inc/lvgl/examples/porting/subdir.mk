################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/examples/porting/lv_port_disp_template.c \
../Core/Inc/lvgl/examples/porting/lv_port_fs_template.c \
../Core/Inc/lvgl/examples/porting/lv_port_indev_template.c 

OBJS += \
./Core/Inc/lvgl/examples/porting/lv_port_disp_template.o \
./Core/Inc/lvgl/examples/porting/lv_port_fs_template.o \
./Core/Inc/lvgl/examples/porting/lv_port_indev_template.o 

C_DEPS += \
./Core/Inc/lvgl/examples/porting/lv_port_disp_template.d \
./Core/Inc/lvgl/examples/porting/lv_port_fs_template.d \
./Core/Inc/lvgl/examples/porting/lv_port_indev_template.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/examples/porting/%.o: ../Core/Inc/lvgl/examples/porting/%.c Core/Inc/lvgl/examples/porting/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

