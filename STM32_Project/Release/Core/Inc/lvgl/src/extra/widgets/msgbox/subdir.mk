################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/src/extra/widgets/msgbox/lv_msgbox.c 

OBJS += \
./Core/Inc/lvgl/src/extra/widgets/msgbox/lv_msgbox.o 

C_DEPS += \
./Core/Inc/lvgl/src/extra/widgets/msgbox/lv_msgbox.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/src/extra/widgets/msgbox/lv_msgbox.o: ../Core/Inc/lvgl/src/extra/widgets/msgbox/lv_msgbox.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Inc/lvgl/src/extra/widgets/msgbox/lv_msgbox.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

