################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/examples/assets/animimg001.c \
../Core/Inc/lvgl/examples/assets/animimg002.c \
../Core/Inc/lvgl/examples/assets/animimg003.c \
../Core/Inc/lvgl/examples/assets/img_caret_down.c \
../Core/Inc/lvgl/examples/assets/img_cogwheel_alpha16.c \
../Core/Inc/lvgl/examples/assets/img_cogwheel_argb.c \
../Core/Inc/lvgl/examples/assets/img_cogwheel_chroma_keyed.c \
../Core/Inc/lvgl/examples/assets/img_cogwheel_indexed16.c \
../Core/Inc/lvgl/examples/assets/img_cogwheel_rgb.c \
../Core/Inc/lvgl/examples/assets/img_hand.c \
../Core/Inc/lvgl/examples/assets/img_skew_strip.c \
../Core/Inc/lvgl/examples/assets/img_star.c \
../Core/Inc/lvgl/examples/assets/imgbtn_left.c \
../Core/Inc/lvgl/examples/assets/imgbtn_mid.c \
../Core/Inc/lvgl/examples/assets/imgbtn_right.c 

OBJS += \
./Core/Inc/lvgl/examples/assets/animimg001.o \
./Core/Inc/lvgl/examples/assets/animimg002.o \
./Core/Inc/lvgl/examples/assets/animimg003.o \
./Core/Inc/lvgl/examples/assets/img_caret_down.o \
./Core/Inc/lvgl/examples/assets/img_cogwheel_alpha16.o \
./Core/Inc/lvgl/examples/assets/img_cogwheel_argb.o \
./Core/Inc/lvgl/examples/assets/img_cogwheel_chroma_keyed.o \
./Core/Inc/lvgl/examples/assets/img_cogwheel_indexed16.o \
./Core/Inc/lvgl/examples/assets/img_cogwheel_rgb.o \
./Core/Inc/lvgl/examples/assets/img_hand.o \
./Core/Inc/lvgl/examples/assets/img_skew_strip.o \
./Core/Inc/lvgl/examples/assets/img_star.o \
./Core/Inc/lvgl/examples/assets/imgbtn_left.o \
./Core/Inc/lvgl/examples/assets/imgbtn_mid.o \
./Core/Inc/lvgl/examples/assets/imgbtn_right.o 

C_DEPS += \
./Core/Inc/lvgl/examples/assets/animimg001.d \
./Core/Inc/lvgl/examples/assets/animimg002.d \
./Core/Inc/lvgl/examples/assets/animimg003.d \
./Core/Inc/lvgl/examples/assets/img_caret_down.d \
./Core/Inc/lvgl/examples/assets/img_cogwheel_alpha16.d \
./Core/Inc/lvgl/examples/assets/img_cogwheel_argb.d \
./Core/Inc/lvgl/examples/assets/img_cogwheel_chroma_keyed.d \
./Core/Inc/lvgl/examples/assets/img_cogwheel_indexed16.d \
./Core/Inc/lvgl/examples/assets/img_cogwheel_rgb.d \
./Core/Inc/lvgl/examples/assets/img_hand.d \
./Core/Inc/lvgl/examples/assets/img_skew_strip.d \
./Core/Inc/lvgl/examples/assets/img_star.d \
./Core/Inc/lvgl/examples/assets/imgbtn_left.d \
./Core/Inc/lvgl/examples/assets/imgbtn_mid.d \
./Core/Inc/lvgl/examples/assets/imgbtn_right.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/examples/assets/%.o: ../Core/Inc/lvgl/examples/assets/%.c Core/Inc/lvgl/examples/assets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

