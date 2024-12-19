# The directory that contains FreeRTOS source code
FREERTOS_ROOT := ./../FreeRTOS/FreeRTOS/

# Demo code
DEMO_PROJECT := .

# FreeRTOS kernel
KERNEL_DIR := $(FREERTOS_ROOT)Source
KERNEL_PORT_DIR := $(KERNEL_DIR)/portable/GCC/ARM_CM7/r0p1

# Where to store all the generated files (objects, elf and map)
OUTPUT_DIR := ./Output

# Demo project name and output files
DEMO_NAME := SecureTimeoutSystem
ELF := $(OUTPUT_DIR)/$(DEMO_NAME).elf
MAP := $(OUTPUT_DIR)/$(DEMO_NAME).map

# Compiler toolchain
CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
SIZE := arm-none-eabi-size

# Emulator used for ARM systems
QEMU := ../qemu/build/qemu-system-arm

# Target embedded board and CPU
MACHINE := s32k3x8evb 
CPU := cortex-m7

# QEMU flags for debugging
QEMU_FLAGS_DBG = -s -S 

# Include directories
INCLUDE_DIRS = -I$(KERNEL_DIR)/include -I$(KERNEL_PORT_DIR)
INCLUDE_DIRS += -I$(DEMO_PROJECT) 
INCLUDE_DIRS += -I$(DEMO_PROJECT)/CMSIS 
INCLUDE_DIRS += -I$(DEMO_PROJECT)/Peripherals 
INCLUDE_DIRS += -I$(DEMO_PROJECT)/SecureTimeoutSystem

# Source file search paths
VPATH += $(KERNEL_DIR)
VPATH += $(KERNEL_PORT_DIR)
VPATH += $(KERNEL_DIR)/portable/MemMang
VPATH += $(DEMO_PROJECT)
VPATH += $(DEMO_PROJECT)/CMSIS
VPATH += $(DEMO_PROJECT)/Peripherals
VPATH += $(DEMO_PROJECT)/SecureTimeoutSystem

# Compiler flags
CFLAGS = $(INCLUDE_DIRS)
CFLAGS += -ffreestanding
CFLAGS += -mcpu=$(CPU)
CFLAGS += -mthumb
CFLAGS += -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wshadow
CFLAGS += -g3
CFLAGS += -Os
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -DCMSDK_CM7

# Linker flags
LDFLAGS = -T ./s32_linker.ld
LDFLAGS += -nostartfiles
LDFLAGS += -specs=nano.specs
LDFLAGS += -specs=nosys.specs
LDFLAGS += -Xlinker -Map=$(MAP)
LDFLAGS += -Xlinker --gc-sections
LDFLAGS += -mcpu=$(CPU)
LDFLAGS += -mthumb
LDFLAGS += -mfpu=fpv5-d16 -mfloat-abi=hard

# Kernel source files
SOURCE_FILES += $(KERNEL_DIR)/list.c
SOURCE_FILES += $(KERNEL_DIR)/tasks.c
SOURCE_FILES += $(KERNEL_DIR)/queue.c
SOURCE_FILES += $(KERNEL_DIR)/event_groups.c
SOURCE_FILES += $(KERNEL_DIR)/stream_buffer.c
SOURCE_FILES += $(KERNEL_DIR)/portable/MemMang/heap_4.c
SOURCE_FILES += $(KERNEL_PORT_DIR)/port.c

# Demo source files
SOURCE_FILES += $(DEMO_PROJECT)/main.c
SOURCE_FILES += $(DEMO_PROJECT)/CMSIS/system_CMSDK_CM7.c
SOURCE_FILES += $(DEMO_PROJECT)/Peripherals/uart.c
SOURCE_FILES += $(DEMO_PROJECT)/Peripherals/IntTimer.c
SOURCE_FILES += $(DEMO_PROJECT)/SecureTimeoutSystem/secure_timeout_system.c
SOURCE_FILES += $(DEMO_PROJECT)/Peripherals/printf-stdarg.c

# Start-up code
SOURCE_FILES += ./s32_startup.c

# Create list of object files with the same names of the sources
OBJS = $(SOURCE_FILES:%.c=%.o)

# Remove path from object filename
OBJS_NOPATH = $(notdir $(OBJS))

# Prepend output dir to object filenames
OBJS_OUTPUT = $(patsubst %.o, $(OUTPUT_DIR)/%.o, $(OBJS_NOPATH))


#----------------------------------------------------------------------#
#-------------- Section Dedicated to APplication ----------------------#

# Link the final executable
$(ELF): $(OBJS_OUTPUT) ./s32_linker.ld Makefile
	echo "\n\n--- Final linking ---\n"
	$(LD) $(LDFLAGS) $(OBJS_OUTPUT) -o $(ELF)
	$(SIZE) $(ELF)

# Compile source files to object files
$(OUTPUT_DIR)/%.o : %.c Makefile $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create output directory if it doesn't exist
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Clean all generated files
clean:
	rm -rf $(ELF) $(MAP) $(OUTPUT_DIR)/*.o $(OUTPUT_DIR)

# Default target
all: $(ELF)

# Run QEMU emulator
qemu_start:
	$(QEMU) -machine $(MACHINE) -cpu $(CPU) -kernel $(ELF) -monitor none -nographic -serial stdio

# New run command: clean, build, and start QEMU
run: clean all qemu_start


#----------------------------------------------------------------------#
#------------------ Section Dedicated to qemu -------------------------#

# Run QEMU emulator in debug mode
qemu_debug:
	$(QEMU) -machine $(MACHINE) -cpu $(CPU) -kernel $(ELF) -monitor none -nographic -serial stdio $(QEMU_FLAGS_DBG)

# Start GDB
gdb_start:
	gdb-multiarch $(ELF)

# Run QEMU emulator with monitor
qemu_monitor:
	$(QEMU) -machine $(MACHINE) -cpu $(CPU) -kernel $(ELF) -monitor stdio

# Configure QEMU
qemu_configure:
	cd ../qemu && ./configure

# Build QEMU using Ninja
qemu_ninja:
	cd ../qemu && ninja -C build qemu-system-arm 

#----------------------------------------------------------------------#
#-------------------------- Final command -----------------------------#

# Run the full build and start QEMU
jesus: qemu_configure qemu_ninja clean all qemu_start

