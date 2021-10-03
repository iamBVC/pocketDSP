################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/FX/FIR_filter.c \
../Core/Inc/FX/allpass_filter.c \
../Core/Inc/FX/comb_filter.c \
../Core/Inc/FX/delay.c \
../Core/Inc/FX/distorsion.c \
../Core/Inc/FX/phaser.c \
../Core/Inc/FX/reverb.c 

OBJS += \
./Core/Inc/FX/FIR_filter.o \
./Core/Inc/FX/allpass_filter.o \
./Core/Inc/FX/comb_filter.o \
./Core/Inc/FX/delay.o \
./Core/Inc/FX/distorsion.o \
./Core/Inc/FX/phaser.o \
./Core/Inc/FX/reverb.o 

C_DEPS += \
./Core/Inc/FX/FIR_filter.d \
./Core/Inc/FX/allpass_filter.d \
./Core/Inc/FX/comb_filter.d \
./Core/Inc/FX/delay.d \
./Core/Inc/FX/distorsion.d \
./Core/Inc/FX/phaser.d \
./Core/Inc/FX/reverb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/FX/%.o: ../Core/Inc/FX/%.c Core/Inc/FX/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

