TARGET = main

# change this variable when change example
EXAMPLE = ex6_usart_extend

# change this variable when change chip device
FREERTOS_PORTABLE_DEVICE = GCC_ARM_CM4F

BUILD_DIR = ./build
BIN_DIR = ./bin

# Define the linker script location and chip archtecture
LD_SCRIPT = STM32F429.ld
MCU_SPEC = cortex-m4

# Toolchain definitions (ARM bare metal)
CC = arm-none-eabi-gcc
GCC = arm-none-eabi-g++
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
CFLAGS += -mhard-float
CFLAGS += -mfloat-abi=hard
CFLAGS += -mfpu=fpv4-sp-d16

CFLAGS += -Wall
# '-g' means: generates debug information to be used by GDB debugger
CFLAGS += -g
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nosys.specs

# CPPFLAGS += -mcpu=$(MCU_SPEC)
# CPPFLAGS += -mthumb
# CPPFLAGS += -mhard-float
# CPPFLAGS += -mfloat-abi=hard
# CPPFLAGS += -mfpu=fpv4-sp-d16
# CPPFLAGS += -Wall
# CPPFLAGS += -g
# CPPFLAGS += -Os
# CPPFLAGS += -fmessage-length=0 -fno-common
# CPPFLAGS += -ffunction-sections -fdata-sections
# CPPFLAGS += -fno-exceptions
# #CPPFLAGS += --specs=nosys.specs
# CPPFLAGS += -D$(ST_MCU_DEF)
# CPPFLAGS += -DVVC_$(MCU_CLASS)
# CPPFLAGS += -DVVC_$(MCU)

# Linker directives
LSCRIPT = ./linker/$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
# LFLAGS += -nostdlib
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)

LFLAGS += -mhard-float
LFLAGS += -mfloat-abi=hard
LFLAGS += -mfpu=fpv4-sp-d16

AS_SRC += ./startup/startup.s
AS_SRC += ./startup/vector_table.s

#src freertos
C_SRC += ./freertos/portable/MemMang/heap_4.c
C_SRC += ./freertos/portable/$(FREERTOS_PORTABLE_DEVICE)/port.c
C_SRC += ./freertos/src/list.c
C_SRC += ./freertos/src/queue.c
C_SRC += ./freertos/src/tasks.c
# C_SRC += ./freertos/src/stream_buffer.c
# C_SRC += ./freertos/src/croutine.c
C_SRC += ./freertos/src/event_groups.c
C_SRC += ./freertos/src/timers.c

# main app
C_SRC += ./app/src/main.c
C_SRC += ./example/$(EXAMPLE)/$(EXAMPLE).c

# CPP_SRC   = ./example/$(EXAMPLE)/util.cpp

INCLUDE += -I./
INCLUDE += -I./app/include
INCLUDE += -I./example/$(EXAMPLE)
INCLUDE += -I./device_headers
INCLUDE += -I./drivers/include
INCLUDE += -I./freertos/include
INCLUDE += -I./freertos/config
INCLUDE += -I./freertos/portable/$(FREERTOS_PORTABLE_DEVICE)

OBJS += $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)
# OBJS += $(CPP_SRC:.cpp=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.s
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

# %.o: %.cpp
# 	$(CPP) -c $(CPPFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).bin
	rm -f $(TARGET).elf
	rm -f $(TARGET).map

flash:
	st-flash write $(TARGET).bin 0x08000000