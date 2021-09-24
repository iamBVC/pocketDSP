################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/src/misc/lv_anim.c \
../Core/Inc/lvgl/src/misc/lv_anim_timeline.c \
../Core/Inc/lvgl/src/misc/lv_area.c \
../Core/Inc/lvgl/src/misc/lv_async.c \
../Core/Inc/lvgl/src/misc/lv_bidi.c \
../Core/Inc/lvgl/src/misc/lv_color.c \
../Core/Inc/lvgl/src/misc/lv_fs.c \
../Core/Inc/lvgl/src/misc/lv_gc.c \
../Core/Inc/lvgl/src/misc/lv_ll.c \
../Core/Inc/lvgl/src/misc/lv_log.c \
../Core/Inc/lvgl/src/misc/lv_math.c \
../Core/Inc/lvgl/src/misc/lv_mem.c \
../Core/Inc/lvgl/src/misc/lv_printf.c \
../Core/Inc/lvgl/src/misc/lv_style.c \
../Core/Inc/lvgl/src/misc/lv_style_gen.c \
../Core/Inc/lvgl/src/misc/lv_templ.c \
../Core/Inc/lvgl/src/misc/lv_timer.c \
../Core/Inc/lvgl/src/misc/lv_tlsf.c \
../Core/Inc/lvgl/src/misc/lv_txt.c \
../Core/Inc/lvgl/src/misc/lv_txt_ap.c \
../Core/Inc/lvgl/src/misc/lv_utils.c 

OBJS += \
./Core/Inc/lvgl/src/misc/lv_anim.o \
./Core/Inc/lvgl/src/misc/lv_anim_timeline.o \
./Core/Inc/lvgl/src/misc/lv_area.o \
./Core/Inc/lvgl/src/misc/lv_async.o \
./Core/Inc/lvgl/src/misc/lv_bidi.o \
./Core/Inc/lvgl/src/misc/lv_color.o \
./Core/Inc/lvgl/src/misc/lv_fs.o \
./Core/Inc/lvgl/src/misc/lv_gc.o \
./Core/Inc/lvgl/src/misc/lv_ll.o \
./Core/Inc/lvgl/src/misc/lv_log.o \
./Core/Inc/lvgl/src/misc/lv_math.o \
./Core/Inc/lvgl/src/misc/lv_mem.o \
./Core/Inc/lvgl/src/misc/lv_printf.o \
./Core/Inc/lvgl/src/misc/lv_style.o \
./Core/Inc/lvgl/src/misc/lv_style_gen.o \
./Core/Inc/lvgl/src/misc/lv_templ.o \
./Core/Inc/lvgl/src/misc/lv_timer.o \
./Core/Inc/lvgl/src/misc/lv_tlsf.o \
./Core/Inc/lvgl/src/misc/lv_txt.o \
./Core/Inc/lvgl/src/misc/lv_txt_ap.o \
./Core/Inc/lvgl/src/misc/lv_utils.o 

C_DEPS += \
./Core/Inc/lvgl/src/misc/lv_anim.d \
./Core/Inc/lvgl/src/misc/lv_anim_timeline.d \
./Core/Inc/lvgl/src/misc/lv_area.d \
./Core/Inc/lvgl/src/misc/lv_async.d \
./Core/Inc/lvgl/src/misc/lv_bidi.d \
./Core/Inc/lvgl/src/misc/lv_color.d \
./Core/Inc/lvgl/src/misc/lv_fs.d \
./Core/Inc/lvgl/src/misc/lv_gc.d \
./Core/Inc/lvgl/src/misc/lv_ll.d \
./Core/Inc/lvgl/src/misc/lv_log.d \
./Core/Inc/lvgl/src/misc/lv_math.d \
./Core/Inc/lvgl/src/misc/lv_mem.d \
./Core/Inc/lvgl/src/misc/lv_printf.d \
./Core/Inc/lvgl/src/misc/lv_style.d \
./Core/Inc/lvgl/src/misc/lv_style_gen.d \
./Core/Inc/lvgl/src/misc/lv_templ.d \
./Core/Inc/lvgl/src/misc/lv_timer.d \
./Core/Inc/lvgl/src/misc/lv_tlsf.d \
./Core/Inc/lvgl/src/misc/lv_txt.d \
./Core/Inc/lvgl/src/misc/lv_txt_ap.d \
./Core/Inc/lvgl/src/misc/lv_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/src/misc/%.o: ../Core/Inc/lvgl/src/misc/%.c Core/Inc/lvgl/src/misc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

