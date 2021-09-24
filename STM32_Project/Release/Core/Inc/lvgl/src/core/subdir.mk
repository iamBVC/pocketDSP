################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/src/core/lv_disp.c \
../Core/Inc/lvgl/src/core/lv_event.c \
../Core/Inc/lvgl/src/core/lv_group.c \
../Core/Inc/lvgl/src/core/lv_indev.c \
../Core/Inc/lvgl/src/core/lv_indev_scroll.c \
../Core/Inc/lvgl/src/core/lv_obj.c \
../Core/Inc/lvgl/src/core/lv_obj_class.c \
../Core/Inc/lvgl/src/core/lv_obj_draw.c \
../Core/Inc/lvgl/src/core/lv_obj_pos.c \
../Core/Inc/lvgl/src/core/lv_obj_scroll.c \
../Core/Inc/lvgl/src/core/lv_obj_style.c \
../Core/Inc/lvgl/src/core/lv_obj_style_gen.c \
../Core/Inc/lvgl/src/core/lv_obj_tree.c \
../Core/Inc/lvgl/src/core/lv_refr.c \
../Core/Inc/lvgl/src/core/lv_theme.c 

OBJS += \
./Core/Inc/lvgl/src/core/lv_disp.o \
./Core/Inc/lvgl/src/core/lv_event.o \
./Core/Inc/lvgl/src/core/lv_group.o \
./Core/Inc/lvgl/src/core/lv_indev.o \
./Core/Inc/lvgl/src/core/lv_indev_scroll.o \
./Core/Inc/lvgl/src/core/lv_obj.o \
./Core/Inc/lvgl/src/core/lv_obj_class.o \
./Core/Inc/lvgl/src/core/lv_obj_draw.o \
./Core/Inc/lvgl/src/core/lv_obj_pos.o \
./Core/Inc/lvgl/src/core/lv_obj_scroll.o \
./Core/Inc/lvgl/src/core/lv_obj_style.o \
./Core/Inc/lvgl/src/core/lv_obj_style_gen.o \
./Core/Inc/lvgl/src/core/lv_obj_tree.o \
./Core/Inc/lvgl/src/core/lv_refr.o \
./Core/Inc/lvgl/src/core/lv_theme.o 

C_DEPS += \
./Core/Inc/lvgl/src/core/lv_disp.d \
./Core/Inc/lvgl/src/core/lv_event.d \
./Core/Inc/lvgl/src/core/lv_group.d \
./Core/Inc/lvgl/src/core/lv_indev.d \
./Core/Inc/lvgl/src/core/lv_indev_scroll.d \
./Core/Inc/lvgl/src/core/lv_obj.d \
./Core/Inc/lvgl/src/core/lv_obj_class.d \
./Core/Inc/lvgl/src/core/lv_obj_draw.d \
./Core/Inc/lvgl/src/core/lv_obj_pos.d \
./Core/Inc/lvgl/src/core/lv_obj_scroll.d \
./Core/Inc/lvgl/src/core/lv_obj_style.d \
./Core/Inc/lvgl/src/core/lv_obj_style_gen.d \
./Core/Inc/lvgl/src/core/lv_obj_tree.d \
./Core/Inc/lvgl/src/core/lv_refr.d \
./Core/Inc/lvgl/src/core/lv_theme.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/src/core/%.o: ../Core/Inc/lvgl/src/core/%.c Core/Inc/lvgl/src/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

