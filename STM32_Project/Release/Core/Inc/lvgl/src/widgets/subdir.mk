################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/src/widgets/lv_arc.c \
../Core/Inc/lvgl/src/widgets/lv_bar.c \
../Core/Inc/lvgl/src/widgets/lv_btn.c \
../Core/Inc/lvgl/src/widgets/lv_btnmatrix.c \
../Core/Inc/lvgl/src/widgets/lv_canvas.c \
../Core/Inc/lvgl/src/widgets/lv_checkbox.c \
../Core/Inc/lvgl/src/widgets/lv_dropdown.c \
../Core/Inc/lvgl/src/widgets/lv_img.c \
../Core/Inc/lvgl/src/widgets/lv_label.c \
../Core/Inc/lvgl/src/widgets/lv_line.c \
../Core/Inc/lvgl/src/widgets/lv_objx_templ.c \
../Core/Inc/lvgl/src/widgets/lv_roller.c \
../Core/Inc/lvgl/src/widgets/lv_slider.c \
../Core/Inc/lvgl/src/widgets/lv_switch.c \
../Core/Inc/lvgl/src/widgets/lv_table.c \
../Core/Inc/lvgl/src/widgets/lv_textarea.c 

OBJS += \
./Core/Inc/lvgl/src/widgets/lv_arc.o \
./Core/Inc/lvgl/src/widgets/lv_bar.o \
./Core/Inc/lvgl/src/widgets/lv_btn.o \
./Core/Inc/lvgl/src/widgets/lv_btnmatrix.o \
./Core/Inc/lvgl/src/widgets/lv_canvas.o \
./Core/Inc/lvgl/src/widgets/lv_checkbox.o \
./Core/Inc/lvgl/src/widgets/lv_dropdown.o \
./Core/Inc/lvgl/src/widgets/lv_img.o \
./Core/Inc/lvgl/src/widgets/lv_label.o \
./Core/Inc/lvgl/src/widgets/lv_line.o \
./Core/Inc/lvgl/src/widgets/lv_objx_templ.o \
./Core/Inc/lvgl/src/widgets/lv_roller.o \
./Core/Inc/lvgl/src/widgets/lv_slider.o \
./Core/Inc/lvgl/src/widgets/lv_switch.o \
./Core/Inc/lvgl/src/widgets/lv_table.o \
./Core/Inc/lvgl/src/widgets/lv_textarea.o 

C_DEPS += \
./Core/Inc/lvgl/src/widgets/lv_arc.d \
./Core/Inc/lvgl/src/widgets/lv_bar.d \
./Core/Inc/lvgl/src/widgets/lv_btn.d \
./Core/Inc/lvgl/src/widgets/lv_btnmatrix.d \
./Core/Inc/lvgl/src/widgets/lv_canvas.d \
./Core/Inc/lvgl/src/widgets/lv_checkbox.d \
./Core/Inc/lvgl/src/widgets/lv_dropdown.d \
./Core/Inc/lvgl/src/widgets/lv_img.d \
./Core/Inc/lvgl/src/widgets/lv_label.d \
./Core/Inc/lvgl/src/widgets/lv_line.d \
./Core/Inc/lvgl/src/widgets/lv_objx_templ.d \
./Core/Inc/lvgl/src/widgets/lv_roller.d \
./Core/Inc/lvgl/src/widgets/lv_slider.d \
./Core/Inc/lvgl/src/widgets/lv_switch.d \
./Core/Inc/lvgl/src/widgets/lv_table.d \
./Core/Inc/lvgl/src/widgets/lv_textarea.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/src/widgets/%.o: ../Core/Inc/lvgl/src/widgets/%.c Core/Inc/lvgl/src/widgets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

