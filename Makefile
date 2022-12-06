TARGET = main

EXAMPLE = ex3_sysclk

# Define the linker script location and chip archtecture
LD_SCRIPT = STM32F429.ld
MCU_SPEC = cortex-m4

# Toolchain definitions (ARM bare metal)
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OD = arm-none-eabi-size

# Assembly directives
# '-c' means: compile source without linking
ASFLAGS += -c
# '-O0' means: set the compiler's optimization level (0 mean default)
ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
# '-Wall' means: enable all compiler's warning messages.
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
# '-g' means: generates debug information to be used by GDB debugger
CFLAGS += -g
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nosys.specs

# Linker directives
LSCRIPT = ./linker/$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += -nostdlib
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)

AS_SRC = ./startup/startup.s
AS_SRC += ./startup/vector_table.s
C_SRC = ./app/src/main.c
C_SRC += ./example/$(EXAMPLE)/$(EXAMPLE).c

INCLUDE = -I./
INCLUDE += -I./app/include
INCLUDE += -I./device_headers
INCLUDE += -I./drivers/include
INCLUDE += -I./example/$(EXAMPLE)

OBJS += $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.s
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@


$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).bin
	rm -f $(TARGET).elf