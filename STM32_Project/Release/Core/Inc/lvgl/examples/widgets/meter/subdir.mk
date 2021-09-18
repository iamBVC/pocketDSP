################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_1.c \
../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_2.c \
../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_3.c \
../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_4.c 

OBJS += \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_1.o \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_2.o \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_3.o \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_4.o 

C_DEPS += \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_1.d \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_2.d \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_3.d \
./Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_4.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_1.o: ../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_1.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_1.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_2.o: ../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_2.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_2.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_3.o: ../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_3.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_3.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_4.o: ../Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_4.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Inc/lvgl/examples/widgets/meter/lv_example_meter_4.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

