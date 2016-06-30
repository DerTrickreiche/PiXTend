################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include raw_io/subdir.mk
-include PiXTend/modules/subdir.mk
-include PiXTend/commands/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
PiX_ng2.lss \

FLASH_IMAGE += \
PiX_ng2.hex \

EEPROM_IMAGE += \
PiX_ng2.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: PiX_ng2.elf secondary-outputs

# Tool invocations
PiX_ng2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,PiX_ng2.map -mmcu=atmega32 -o "PiX_ng2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

PiX_ng2.lss: PiX_ng2.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S PiX_ng2.elf  >"PiX_ng2.lss"
	@echo 'Finished building: $@'
	@echo ' '

PiX_ng2.hex: PiX_ng2.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex PiX_ng2.elf  "PiX_ng2.hex"
	@echo 'Finished building: $@'
	@echo ' '

PiX_ng2.eep: PiX_ng2.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex PiX_ng2.elf  "PiX_ng2.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: PiX_ng2.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 PiX_ng2.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) PiX_ng2.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
