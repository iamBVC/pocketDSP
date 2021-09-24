################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/lvgl/src/gpu/lv_gpu_nxp_pxp.c \
../Core/Inc/lvgl/src/gpu/lv_gpu_nxp_pxp_osa.c \
../Core/Inc/lvgl/src/gpu/lv_gpu_nxp_vglite.c \
../Core/Inc/lvgl/src/gpu/lv_gpu_stm32_dma2d.c 

OBJS += \
./Core/Inc/lvgl/src/gpu/lv_gpu_nxp_pxp.o \
./Core/Inc/lvgl/src/gpu/lv_gpu_nxp_pxp_osa.o \
./Core/Inc/lvgl/src/gpu/lv_gpu_nxp_vglite.o \
./Core/Inc/lvgl/src/gpu/lv_gpu_stm32_dma2d.o 

C_DEPS += \
./Core/Inc/lvgl/src/gpu/lv_gpu_nxp_pxp.d \
./Core/Inc/lvgl/src/gpu/lv_gpu_nxp_pxp_osa.d \
./Core/Inc/lvgl/src/gpu/lv_gpu_nxp_vglite.d \
./Core/Inc/lvgl/src/gpu/lv_gpu_stm32_dma2d.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/lvgl/src/gpu/%.o: ../Core/Inc/lvgl/src/gpu/%.c Core/Inc/lvgl/src/gpu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

