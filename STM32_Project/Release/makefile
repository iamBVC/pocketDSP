################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Inc/lvgl/tests/unity/subdir.mk
-include Core/Inc/lvgl/tests/src/test_fonts/subdir.mk
-include Core/Inc/lvgl/tests/src/test_cases/subdir.mk
-include Core/Inc/lvgl/tests/src/subdir.mk
-include Core/Inc/lvgl/src/widgets/subdir.mk
-include Core/Inc/lvgl/src/misc/subdir.mk
-include Core/Inc/lvgl/src/hal/subdir.mk
-include Core/Inc/lvgl/src/gpu/subdir.mk
-include Core/Inc/lvgl/src/font/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/win/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/tileview/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/tabview/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/spinner/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/spinbox/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/span/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/msgbox/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/meter/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/list/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/led/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/keyboard/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/imgbtn/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/colorwheel/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/chart/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/calendar/subdir.mk
-include Core/Inc/lvgl/src/extra/widgets/animimg/subdir.mk
-include Core/Inc/lvgl/src/extra/themes/mono/subdir.mk
-include Core/Inc/lvgl/src/extra/themes/default/subdir.mk
-include Core/Inc/lvgl/src/extra/themes/basic/subdir.mk
-include Core/Inc/lvgl/src/extra/others/snapshot/subdir.mk
-include Core/Inc/lvgl/src/extra/layouts/grid/subdir.mk
-include Core/Inc/lvgl/src/extra/layouts/flex/subdir.mk
-include Core/Inc/lvgl/src/extra/subdir.mk
-include Core/Inc/lvgl/src/draw/subdir.mk
-include Core/Inc/lvgl/src/core/subdir.mk
-include Core/Inc/lvgl/examples/widgets/win/subdir.mk
-include Core/Inc/lvgl/examples/widgets/tileview/subdir.mk
-include Core/Inc/lvgl/examples/widgets/textarea/subdir.mk
-include Core/Inc/lvgl/examples/widgets/tabview/subdir.mk
-include Core/Inc/lvgl/examples/widgets/table/subdir.mk
-include Core/Inc/lvgl/examples/widgets/switch/subdir.mk
-include Core/Inc/lvgl/examples/widgets/spinner/subdir.mk
-include Core/Inc/lvgl/examples/widgets/spinbox/subdir.mk
-include Core/Inc/lvgl/examples/widgets/span/subdir.mk
-include Core/Inc/lvgl/examples/widgets/slider/subdir.mk
-include Core/Inc/lvgl/examples/widgets/roller/subdir.mk
-include Core/Inc/lvgl/examples/widgets/obj/subdir.mk
-include Core/Inc/lvgl/examples/widgets/msgbox/subdir.mk
-include Core/Inc/lvgl/examples/widgets/meter/subdir.mk
-include Core/Inc/lvgl/examples/widgets/list/subdir.mk
-include Core/Inc/lvgl/examples/widgets/line/subdir.mk
-include Core/Inc/lvgl/examples/widgets/led/subdir.mk
-include Core/Inc/lvgl/examples/widgets/label/subdir.mk
-include Core/Inc/lvgl/examples/widgets/keyboard/subdir.mk
-include Core/Inc/lvgl/examples/widgets/imgbtn/subdir.mk
-include Core/Inc/lvgl/examples/widgets/img/subdir.mk
-include Core/Inc/lvgl/examples/widgets/dropdown/subdir.mk
-include Core/Inc/lvgl/examples/widgets/colorwheel/subdir.mk
-include Core/Inc/lvgl/examples/widgets/checkbox/subdir.mk
-include Core/Inc/lvgl/examples/widgets/chart/subdir.mk
-include Core/Inc/lvgl/examples/widgets/canvas/subdir.mk
-include Core/Inc/lvgl/examples/widgets/calendar/subdir.mk
-include Core/Inc/lvgl/examples/widgets/btnmatrix/subdir.mk
-include Core/Inc/lvgl/examples/widgets/btn/subdir.mk
-include Core/Inc/lvgl/examples/widgets/bar/subdir.mk
-include Core/Inc/lvgl/examples/widgets/arc/subdir.mk
-include Core/Inc/lvgl/examples/widgets/animimg/subdir.mk
-include Core/Inc/lvgl/examples/styles/subdir.mk
-include Core/Inc/lvgl/examples/scroll/subdir.mk
-include Core/Inc/lvgl/examples/porting/subdir.mk
-include Core/Inc/lvgl/examples/others/snapshot/subdir.mk
-include Core/Inc/lvgl/examples/layouts/grid/subdir.mk
-include Core/Inc/lvgl/examples/layouts/flex/subdir.mk
-include Core/Inc/lvgl/examples/get_started/subdir.mk
-include Core/Inc/lvgl/examples/event/subdir.mk
-include Core/Inc/lvgl/examples/assets/subdir.mk
-include Core/Inc/lvgl/examples/anim/subdir.mk
-include Core/Inc/FX/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := pocketDSP
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
pocketDSP.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
pocketDSP.list \

OBJCOPY_BIN += \
pocketDSP.bin \


# All Target
all: main-build

# Main-build Target
main-build: pocketDSP.elf secondary-outputs

# Tool invocations
pocketDSP.elf: $(OBJS) $(USER_OBJS) D:\Documenti\Github\pocketDSP_repo\STM32_Project\STM32H743VITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "pocketDSP.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\Documenti\Github\pocketDSP_repo\STM32_Project\STM32H743VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="pocketDSP.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

pocketDSP.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "pocketDSP.list"
	@echo 'Finished building: $@'
	@echo ' '

pocketDSP.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "pocketDSP.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SIZE_OUTPUT)$(OBJDUMP_LIST)$(EXECUTABLES)$(OBJS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(OBJCOPY_BIN) pocketDSP.elf
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
