################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/src/draw/lv_draw_arc.c \
../Core/Inc/lvgl/src/draw/lv_draw_blend.c \
../Core/Inc/lvgl/src/draw/lv_draw_img.c \
../Core/Inc/lvgl/src/draw/lv_draw_label.c \
../Core/Inc/lvgl/src/draw/lv_draw_line.c \
../Core/Inc/lvgl/src/draw/lv_draw_mask.c \
../Core/Inc/lvgl/src/draw/lv_draw_rect.c \
../Core/Inc/lvgl/src/draw/lv_draw_triangle.c \
../Core/Inc/lvgl/src/draw/lv_img_buf.c \
../Core/Inc/lvgl/src/draw/lv_img_cache.c \
../Core/Inc/lvgl/src/draw/lv_img_decoder.c 

OBJS += \
./Core/Inc/lvgl/src/draw/lv_draw_arc.o \
./Core/Inc/lvgl/src/draw/lv_draw_blend.o \
./Core/Inc/lvgl/src/draw/lv_draw_img.o \
./Core/Inc/lvgl/src/draw/lv_draw_label.o \
./Core/Inc/lvgl/src/draw/lv_draw_line.o \
./Core/Inc/lvgl/src/draw/lv_draw_mask.o \
./Core/Inc/lvgl/src/draw/lv_draw_rect.o \
./Core/Inc/lvgl/src/draw/lv_draw_triangle.o \
./Core/Inc/lvgl/src/draw/lv_img_buf.o \
./Core/Inc/lvgl/src/draw/lv_img_cache.o \
./Core/Inc/lvgl/src/draw/lv_img_decoder.o 

C_DEPS += \
./Core/Inc/lvgl/src/draw/lv_draw_arc.d \
./Core/Inc/lvgl/src/draw/lv_draw_blend.d \
./Core/Inc/lvgl/src/draw/lv_draw_img.d \
./Core/Inc/lvgl/src/draw/lv_draw_label.d \
./Core/Inc/lvgl/src/draw/lv_draw_line.d \
./Core/Inc/lvgl/src/draw/lv_draw_mask.d \
./Core/Inc/lvgl/src/draw/lv_draw_rect.d \
./Core/Inc/lvgl/src/draw/lv_draw_triangle.d \
./Core/Inc/lvgl/src/draw/lv_img_buf.d \
./Core/Inc/lvgl/src/draw/lv_img_cache.d \
./Core/Inc/lvgl/src/draw/lv_img_decoder.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/src/draw/%.o: ../Core/Inc/lvgl/src/draw/%.c Core/Inc/lvgl/src/draw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

