# QEMU and FreeRTOS on NXP S32K3X8EVB

<details closed>
<summary><b>Table of Contents</b></summary>
 
&nbsp;• [Introduction](#introduction) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Requirements](#requirements) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Setting Up the Project Environment](#setting-up-the-project-environment) <br>
&nbsp;• [Part 1 - QEMU Board Emulation](#part-1---qemu-board-emulation) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Setting Up QEMU](#setting-up-qemu) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Creating the Board](#creating-the-board) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Adding the S32K3X8EVB Board](#adding-the-s32k3x8evb-board) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Compiling QEMU](#compiling-qemu) <br>
&nbsp;• [Part 2 - FreeRTOS Porting](#part-2---freertos-porting) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Setting Up FreeRTOS](#setting-up-freertos) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Running FreeRTOS on QEMU](#running-freertos-on-qemu) <br>
&nbsp;• [Part 3 - Simple Application](#part-3---simple-application) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Creating all the files](#creating-all-the-files) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Compiling and Running the Application](#compiling-and-running-the-application) <br>
&nbsp;• [Conclusion](#conclusion) <br>

</details>

## Introduction

This guide provides a comprehensive walkthrough of our project, which involves:

1. Emulating the NXP S32K3X8EVB board using QEMU.
2. Porting FreeRTOS to run on the emulated board.
3. Creating a simple application to demonstrate the setup.

#### Prerequisites

- The project was developed on **Ubuntu 22.04 LTS** and should work with most **Debian-based distributions**.
- The commands provided in this guide use **`apt`** as the package manager. If you are using a non-Debian distribution, equivalent packages may be required, which might differ depending on the package manager used by your system.

### Requirements

To successfully complete this project, you will need the following:
- A Linux-based operating system (e.g., Ubuntu)
- Git for version control
- QEMU for hardware emulation
- FreeRTOS for the real-time operating system
- ARM GCC toolchain for compiling the code
- Ninja build system for building QEMU

You can verify if you have all the necessary tools installed by running the following commands:

```sh
# Check for Git
git --version

# Check for QEMU
qemu-system-arm --version

# Check for ARM GCC toolchain
arm-none-eabi-gcc --version

# Check for Ninja build system
ninja --version
```

If any of these commands return an error or indicate the tool is not installed, follow the detailed configuration steps below.

<details closed>
<summary><b>Configuration Details</b></summary>

### Strictly Needed Dependencies

The following packages are essential for building and running QEMU, the ARM GCC toolchain, and the rest of the project:
    
- **Core Build Tools**:
  - `build-essential` (includes `gcc`, `g++`, `make`)
  - `python3` (needed for QEMU build scripts)
  - `python3-pip` (for installing Python-related dependencies, if needed)

- **QEMU-Specific Build Dependencies**:
  - `git` (version control to clone repositories)
  - `libglib2.0-dev` (core library for QEMU)
  - `libfdt-dev` (for working with device tree files)
  - `libpixman-1-dev` (required for graphics emulation)
  - `zlib1g-dev` (for compression support)
  - `ninja-build` (for faster builds compared to Makefiles)
  -  `flex` and `bison` (required for QEMU's device emulation parsing logic)

- **ARM Toolchain**:
  - `gcc-arm-none-eabi` (the ARM GCC compiler)
  - `gdb-multiarch`  (a versatile debugger for cross-architecture debugging, including ARM)

### Optional Dependencies

These dependencies provide additional QEMU functionality or extend its features. They are not strictly required but are recommended if you want advanced capabilities:
    
- **Virtualization and I/O Support**:
  - `libaio-dev`, `libcap-ng-dev`, `libudev-dev`

- **Networking and Filesystem Support**:
  - `libslirp-dev`, `libattr1-dev`, `libnfs-dev`

- **Graphical and Display Support**:
  - `libdrm-dev`, `libsdl2-dev`, `libspice-protocol-dev`, `libspice-server-dev`, `libgbm-dev`, `libepoxy-dev`, `libvirglrenderer-dev`, `libgtk-3-dev`

- **Peripheral and USB Support**:
  - `libusb-1.0-0-dev`

- **Sound Support**:
  - `libpulse-dev`

- **Terminal and UI Support**:
  - `libncurses5-dev`, `libncursesw5-dev`, `libvte-2.91-dev`

- **Multimedia and File Format Support**:
  - `libjpeg8-dev`, `libpng-dev`

### Installation Command

To install all necessary dependencies (strictly needed and optional), run the following commands:

```sh
sudo apt update && sudo apt upgrade -y
sudo apt install git bison flex libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev \
ninja-build build-essential python3 python3-pip libaio-dev libcap-ng-dev \
libiscsi-dev libattr1-dev libnfs-dev libudev-dev libxen-dev libepoxy-dev \
libdrm-dev libgbm-dev libvirglrenderer-dev libgtk-3-dev libspice-protocol-dev \
libspice-server-dev libusb-1.0-0-dev libpulse-dev libsdl2-dev libslirp-dev \
libncurses5-dev libncursesw5-dev libx11-dev libxext-dev libxt-dev \
libpng-dev libjpeg8-dev libvte-2.91-dev libfuse-dev gcc-arm-none-eabi gdb-multiarch
```

Once these dependencies are installed, re-run the verification commands listed above to ensure everything is set up correctly.

</details>

### Setting Up the Project Environment

1. Create a directory for the project:
    ```sh
    mkdir project_name
    cd project_name
    ```

2. The final structure will be like this:
    ```
    project_name
    ├── qemu
    ├── FreeRTOS
    └── App
        ├── main.c
        ├── FreeRTOSConfig.c
        ├── Makefile
        ├── s32_linker.ld
        ├── s32_startup.c
        └── ...
    ```
## Part 1 - QEMU Board Emulation

### Setting Up QEMU

1. Clone the QEMU repository:
    ```sh
    git clone https://github.com/qemu/qemu.git
    cd qemu
    ```

2. Create a new branch for your custom board (optional):
    ```sh
    git checkout -b s32k3x8evb-support
    ```

### Creating the Board

The `s32k3x8evb_board.c` file is the core of the QEMU board emulation. It includes several key sections:

1. **File Header and Includes**:
    - The file starts with a header that includes the authors and a brief description of the functionalities provided.
    - It includes necessary QEMU headers and other system headers required for memory management, hardware components, and system emulation.
  
    <!-- TODO: write -->

2. **Memory Regions**:
    - Defines constants for memory regions such as flash and SRAM.
    - Functions to initialize these memory regions, including `s32k3x8_initialize_memory_regions()`, which sets up the flash and SRAM memory blocks.

    <!-- TODO: write -->

3. **LPUART Initialization**:
    - The `initialize_lpuarts()` function sets up the LPUART devices.
    - It configures each LPUART, assigns base addresses, and connects interrupts to the NVIC.

    <!-- TODO: write -->

4. **Interrupts and NVIC**:
    - The Nested Vectored Interrupt Controller (NVIC) is initialized in the `s32k3x8_init()` function.
    - Configures the NVIC with the number of IRQs and priority bits, and connects it to the system clock.

    <!-- TODO: write -->

5. **Timers**:
    - Initializes PIT timers in the `s32k3x8_init()` function.
    - Each timer is configured with a base address and connected to the NVIC.

    <!-- TODO: write -->

6. **Firmware Loading**:
    - The `s32k3x8_load_firmware()` function loads the firmware into the emulated flash memory.
    - Uses the `armv7m_load_kernel()` function to load the kernel into the flash memory.

    <!-- TODO: write -->

7. **Machine Initialization**:
    - The `s32k3x8_init()` function is the main initialization function for the board.
    - It sets up the system memory, SoC container, system controller, clocks, NVIC, LPUARTs, and timers.
    - Logs the initialization process if verbose mode is enabled.

    <!-- TODO: write -->

8. **Class Initialization**:
    - The `s32k3x8_class_init()` function sets up the machine class, including the default CPU type and number of CPUs.
    - Registers the machine type with QEMU.

    <!-- TODO: write -->

For the full content of the `s32k3x8evb_board.c` file, refer to the file located at: [qemu/hw/arm/s32k3x8evb_board.c](qemu/hw/arm/s32k3x8evb_board.c).

### Adding the S32K3X8EVB Board

1. Insert the file `s32k3x8evb_board.c` in the appropriate directory:
    ```plaintext
    // filepath: /project_name/qemu/hw/arm/s32k3x8evb_board.c
    // ...existing code...
    ```

2. Update the `meson.build` file to include the new board:
    ```plaintext
    // filepath: /project_name/qemu/hw/arm/meson.build
    arm_ss.add(when: 'CONFIG_S32K3X8EVB', if_true: files('s32k3x8evb_board.c'))
    ```

3. Add the configuration in `Kconfig`:
    ```plaintext
    // filepath: /project_name/qemu/hw/arm/Kconfig
    config S32K3X8EVB
        bool
        default y
        depends on TCG && ARM
        select ARM_V7M
        select ARM_TIMER
    ```

4. Update the `default.mak` file:
    ```plaintext
    // filepath: /project_name/qemu/configs/devices/s32k3x8evb-softmmu/default.mak
    CONFIG_S32K3X8EVB=y
    ```

### Compiling QEMU

1. Configure and compile QEMU:
    ```sh
    cd qemu && ./configure
    cd qemu && ninja -C build qemu-system-arm
    ```

2. Verify the build and display the machine:
    ```sh
    ./build/qemu-system-arm --machine help | grep s32k3x8evb
    ```

    This command should list the `s32k3x8evb` machine if the build was successful.

## Part 2 - FreeRTOS Porting

To test the FreeRTOS porting on QEMU, we're going to run FreeRTOS with a very simple task that prints every second.

### Setting Up FreeRTOS

1. Clone the FreeRTOS repository:
    ```sh
    cd project_name
    git clone https://github.com/FreeRTOS/FreeRTOS.git
    ```

2. Create the directory structure:
    ```sh
    mkdir App
    mkdir App/Peripherals
    ```

    For now, the `Peripherals` directory is only for UART/print functionality.

3. Create the following files in the `App` directory:
    - `main.c`
    - `s32_linker.ld`
    - `s32_startup.c`
    - `FreeRTOSConfig.h`
    - `Makefile`
    - `Peripherals/uart.c`
    - `Peripherals/uart.h`
    - `Peripherals/printf-stdarg.c`
    - `Peripherals/printf-stdarg.h`

4. Now the structure should look like this:
    ```
    project_name/
    ├── qemu/
    ├── FreeRTOS/
    └── App/
        ├── main.c
        ├── s32_linker.ld
        ├── s32_startup.c
        ├── FreeRTOSConfig.h
        ├── Makefile
        └── Peripherals/
            ├── uart.c
            ├── uart.h
            ├── printf-stdarg.c
            └── printf-stdarg.h
    ```

### Running FreeRTOS on QEMU

1. The `Peripherals` files can be copied from the files in [App/Peripherals](App/Peripherals).

2. Implement the following files: `s32_startup.c`, `s32_linker.ld`, `FreeRTOSConfig.h`, and `Makefile`.

    <details closed>
    <summary><b>s32_startup.c</b></summary>
    ```c

    TODO: insert the code

    ```
    <details>

    <details closed>
    <summary><b>s32_linker.ld</b></summary>
    ```

    TODO: insert the code

    ```
    <details>

    <details closed>
    <summary><b>FreeRTOSConfig.h</b></summary>
    ```c
    /*
    * FreeRTOS V202212.00
    * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
    *
    * Permission is hereby granted, free of charge, to any person obtaining a copy of
    * this software and associated documentation files (the "Software"), to deal in
    * the Software without restriction, including without limitation the rights to
    * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    * the Software, and to permit persons to whom the Software is furnished to do so,
    * subject to the following conditions:
    *
    * The above copyright notice and this permission notice shall be included in all
    * copies or substantial portions of the Software.
    *
    * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    * IN AN ACTION OF CONTRACT, TORT or OTHERWISE, ARISING FROM, OUT OF OR IN
    * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    *
    * https://www.FreeRTOS.org
    * https://github.com/FreeRTOS
    *
    */

    #ifndef FREERTOS_CONFIG_H
    #define FREERTOS_CONFIG_H

    /*-----------------------------------------------------------
    * Application specific definitions.
    * Adjusted for balanced priorities and stable timer operation.
    *----------------------------------------------------------*/

    #ifndef __NVIC_PRIO_BITS
    #define __NVIC_PRIO_BITS 4  /* Cortex-M7 uses 4 priority bits */
    #endif

    #define configUSE_TRACE_FACILITY                 0
    #define configGENERATE_RUN_TIME_STATS            0

    #define configUSE_PREEMPTION                     1
    #define configUSE_IDLE_HOOK                      0
    #define configUSE_TICK_HOOK                      0
    #define configCPU_CLOCK_HZ                       ( ( unsigned long ) 143000000 )
    #define configTICK_RATE_HZ                       ( ( TickType_t ) 1000 )
    #define configMINIMAL_STACK_SIZE                 ( ( unsigned short ) 160 )
    #define configTOTAL_HEAP_SIZE                    ( ( size_t ) ( 100 * 1024 ) )
    #define configMAX_TASK_NAME_LEN                  ( 12 )
    #define configUSE_16_BIT_TICKS                   0
    #define configIDLE_SHOULD_YIELD                  0
    #define configUSE_CO_ROUTINES                    0
    #define configUSE_MUTEXES                        1
    #define configUSE_RECURSIVE_MUTEXES              1
    #define configCHECK_FOR_STACK_OVERFLOW           0
    #define configUSE_MALLOC_FAILED_HOOK             0
    #define configUSE_QUEUE_SETS                     1
    #define configUSE_COUNTING_SEMAPHORES            1

    #define configMAX_PRIORITIES                     ( 9UL )
    #define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )
    #define configQUEUE_REGISTRY_SIZE                10
    #define configSUPPORT_STATIC_ALLOCATION          0

    /* Timer-related defines: Balanced priorities for stable operation. */
    #define configUSE_TIMERS                         0
    #define configTIMER_TASK_PRIORITY                (configMAX_PRIORITIES - 4 )
    #define configTIMER_QUEUE_LENGTH                 20
    #define configTIMER_TASK_STACK_DEPTH             (configMINIMAL_STACK_SIZE * 2)

    #define configUSE_TASK_NOTIFICATIONS             1
    #define configTASK_NOTIFICATION_ARRAY_ENTRIES    3

    /* Include API functions for required functionality. */
    #define INCLUDE_vTaskPrioritySet                  1
    #define INCLUDE_uxTaskPriorityGet                 1
    #define INCLUDE_vTaskDelete                       1
    #define INCLUDE_vTaskCleanUpResources             0
    #define INCLUDE_vTaskSuspend                      1
    #define INCLUDE_vTaskDelayUntil                   1
    #define INCLUDE_vTaskDelay                        1
    #define INCLUDE_uxTaskGetStackHighWaterMark       1
    #define INCLUDE_xTaskGetSchedulerState            1
    #define INCLUDE_xTimerGetTimerDaemonTaskHandle    1
    #define INCLUDE_xTaskGetIdleTaskHandle            1
    #define INCLUDE_xSemaphoreGetMutexHolder          1
    #define INCLUDE_eTaskGetState                     1
    #define INCLUDE_xTimerPendFunctionCall            1
    #define INCLUDE_xTaskAbortDelay                   1
    #define INCLUDE_xTaskGetHandle                    1

    #define configUSE_STATS_FORMATTING_FUNCTIONS      0

    #define configKERNEL_INTERRUPT_PRIORITY           ( 255 )  /* Lowest priority for kernel interrupt */
    #define configMAX_SYSCALL_INTERRUPT_PRIORITY      ( 5 << (8 - __NVIC_PRIO_BITS) )  /* NVIC priority level */

    #ifndef __IASMARM__
        #define configASSERT( x ) if( ( x ) == 0 ) while(1);
    #endif

    #define configUSE_PORT_OPTIMISED_TASK_SELECTION   1
    #define configRUN_ADDITIONAL_TESTS                1
    #define configSTREAM_BUFFER_TRIGGER_LEVEL_TEST_MARGIN    4

    #define configENABLE_BACKWARD_COMPATIBILITY 0

    #endif /* FREERTOS_CONFIG_H */
    ```
    <details>

    <details closed>
    <summary><b>Makefile</b></summary>
    ```Makefile
    # The directory that contains FreeRTOS source code
    FREERTOS_ROOT := ../../group2/FreeRTOS/FreeRTOS/

    # Demo code
    DEMO_PROJECT := .

    # FreeRTOS kernel
    KERNEL_DIR := $(FREERTOS_ROOT)Source
    KERNEL_PORT_DIR := $(KERNEL_DIR)/portable/GCC/ARM_CM7/r0p1

    # Where to store all the generated files (objects, elf and map)
    OUTPUT_DIR := ./Output

    # Demo project name and output files
    DEMO_NAME := Test
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
    INCLUDE_DIRS += -I$(DEMO_PROJECT)/Peripherals 

    # Source file search paths
    VPATH += $(KERNEL_DIR)
    VPATH += $(KERNEL_PORT_DIR)
    VPATH += $(KERNEL_DIR)/portable/MemMang
    VPATH += $(DEMO_PROJECT)
    VPATH += $(DEMO_PROJECT)/Peripherals

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
    SOURCE_FILES += $(DEMO_PROJECT)/Peripherals/uart.c
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
    #-------------- Section Dedicated to Application ----------------------#

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
    ```
    <details>

3. Implement the `main.c` file:
    ```c
    /* FreeRTOS includes */
    #include "FreeRTOS.h"
    #include "task.h"

    /* Peripheral includes */
    #include "uart.h"
    #include "IntTimer.h"
    #include "printf-stdarg.h"

    /* Task priorities */
    #define mainTASK_PRIORITY (tskIDLE_PRIORITY + 2)

    /* Task prototypes */
    void vTask1(void *pvParameters);

    int main(int argc, char **argv) 
    {
        (void) argc;
        (void) argv;

        /* Hardware initialisation. */
        UART_init();

        /* Create the tasks. */
        xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, mainTASK_PRIORITY, NULL);

        printf("Ready to run the scheduler...\n");
        vTaskStartScheduler();

        for (;;);
    }

    void vTask1(void *pvParameters) 
    {
        (void) pvParameters;

        for (;;) 
        {
            printf("Task1 is running...\n");
            vTaskDelay(1000);
        }
    }
    ```

4. Test everything:
    ```sh
    cd App
    make run
    ```

    If everything works correctly, it means that the **FreeRTOS porting** has been successfully implemented.

## Part 3 - Simple Application

### Implement all the files

To create a simple application, you need to set up several files, including the Makefile, startup code, linker script, FreeRTOS configuration, and the main application code.

Most of the files have already been implemented to test the FreeRTOS porting. Now, we need to create and implement all the other files needed and update the existing files.

1. **Startup Code (`s32_startup.c`)**: This file contains the startup code for the S32K3X8EVB board.

    ```c
    // filepath: /project_name/App/s32_startup.c
    // ...existing code...

    /* Peripheral includes */
    #include "IntTimer.h"

    // ...existing code...

    const uint32_t* isr_vector[] __attribute__((section(".isr_vector"), used)) = {
        // ...existing code...
        0, /* 7 */
        ( uint32_t * ) TIMER0_IRQHandler,  // Timer 0
        ( uint32_t * ) TIMER1_IRQHandler,  // Timer 1
        0, /* 10 */
        // ...existing code...
    };

    // ...existing code...
    ```

2. **Linker Script (`s32_linker.ld`)**: This file defines the memory layout for the application.

    The linker file doesn't need to be updated. The one used for the FreeRTOS porting phase is correct. However, it is possible to check the [App/s32_linker.ld](App/s32_linker.ld) file.

3. **FreeRTOS Configuration (`FreeRTOSConfig.h`)**: This file contains the FreeRTOS configuration settings.

    Even the FreeRTOS config is already well-implemented. However, it's possible to define some task priorities that can be useful in the app. You can still check the [App/FreeRTOSConfig.h](App/FreeRTOSConfig.h) file.

    ```c
    // filepath: /project_name/App/FreeRTOSConfig.h
    // ...existing code...

    /* Define task priorities */
    #define mainTASK_PRIORITY         ( tskIDLE_PRIORITY + 1 ) /* Main task priority */
    #define highTASK_PRIORITY         ( tskIDLE_PRIORITY + 2 ) /* High priority task */
    #define lowTASK_PRIORITY          ( tskIDLE_PRIORITY + 3 ) /* Low priority task */
    ```

4. **Makefile**: This file contains the build configuration and rules.

    Now it's possible to refer to the [App/Makefile](App/Makefile) file in order to have the full list of commands to build, compile, make, and run.

5. **Timer Interrupt Handlers (`IntTimer.c` and `IntTimer.h`)**: These files contain the implementation and declarations for the timer interrupt handlers.

    The `IntTimer.c` and `IntTimer.h` files can be copied from [App/Peripherals/IntTimer.c](App/Peripherals/IntTimer.c) and [App/Peripherals/IntTimer.h](App/Peripherals/IntTimer.h).

    These files represent the initialization and handling of hardware timers. The timers are used to generate periodic interrupts, which can be used to perform regular tasks such as checking for user activities or suspicious activities.

6. **Main Application Code**: These files contain the main application code.

    The app we've implemented includes these files. They can all be copied and checked from the existing files in the [App](App) directory. Here is a very short description of what they do:

    - `main.c`: Contains the main application entry point and initializes the secure timeout system.
    - `secure_timeout_system.c/.h`: Implements the secure timeout system with tasks for monitoring user activity, handling alerts, and simulating events.
    - `globals.h`: Declares global variables used across the application.

### Compiling and Running the Application

1. Compile the application using the FreeRTOS build system:
    ```sh
    make clean all
    ```

2. Run the compiled application on QEMU:
    ```sh
    make qemu_start
    ```

    It is equivalent to:
    ```sh
    ../qemu/build/qemu-system-arm -machine s32k3x8evb -cpu cortex-m7 -kernel ./Output/app_name.elf -monitor none -nographic -serial stdio
    ```

3. It is possible to use a single command:
    ```sh
    make run
    ```

## Conclusion

This guide provides a detailed walkthrough of our project, from setting up QEMU to running a FreeRTOS application on the emulated NXP S32K3X8EVB board. By following these steps, you should be able to recreate our project and understand the process of emulating hardware and running an RTOS on it.

If you encounter any errors or bugs, please refer to or contact the [authors](README.md#authors).
