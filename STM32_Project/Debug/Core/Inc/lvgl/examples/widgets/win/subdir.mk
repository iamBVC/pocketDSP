################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/examples/widgets/win/lv_example_win_1.c 

OBJS += \
./Core/Inc/lvgl/examples/widgets/win/lv_example_win_1.o 

C_DEPS += \
./Core/Inc/lvgl/examples/widgets/win/lv_example_win_1.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/examples/widgets/win/lv_example_win_1.o: ../Core/Inc/lvgl/examples/widgets/win/lv_example_win_1.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Inc/lvgl/examples/widgets/win/lv_example_win_1.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

