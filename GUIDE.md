# QEMU and FreeRTOS on NXP S32K3X8EVB

## Table of Contents

<details closed>
<summary><b>Table of Contents</b></summary>
 
&nbsp;• [Introduction](#introduction)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Requirements](#requirements)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Setting Up the Project Environment](#setting-up-the-project-environment)
&nbsp;• [Part 1 - QEMU Board Emulation](#part-1---qemu-board-emulation)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Setting Up QEMU](#setting-up-qemu)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Creating the Board](#creating-the-board)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Adding the S32K3X8EVB Board](#adding-the-s32k3x8evb-board)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Compiling QEMU](#compiling-qemu)
&nbsp;• [Part 2 - FreeRTOS Porting](#part-2---freertos-porting)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Setting Up FreeRTOS](#setting-up-freertos)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Running FreeRTOS on QEMU](#running-freertos-on-qemu)
&nbsp;• [Part 3 - Simple Application](#part-3---simple-application)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Creating all the files](#creating-all-the-files)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Compiling and Running the Application](#compiling-and-running-the-application)
&nbsp;• [Conclusion](#conclusion)

</details>

## Introduction

This guide provides a comprehensive walkthrough of our project, which involves emulating the NXP S32K3X8EVB board using QEMU, porting FreeRTOS to run on the emulated board, and creating a simple application to demonstrate the setup.

### Requirements

To successfully complete this project, you will need the following:
- A Linux-based operating system (e.g., Ubuntu)
- Git for version control
- QEMU for hardware emulation
- FreeRTOS for the real-time operating system
- ARM GCC toolchain for compiling the code
- Ninja build system for building QEMU

You can check if you have all the necessary tools installed by running the following commands:

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

If any of these commands return an error or are not found, you will need to install the corresponding tool.

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

### Setting Up FreeRTOS

1. Clone the FreeRTOS repository:
    ```sh
    git clone https://github.com/FreeRTOS/FreeRTOS.git
    ```

<!-- TODO: is there anything else? -->

### Running FreeRTOS on QEMU

<!-- TODO: add the test with a kernel -->



## Part 3 - Simple Application

### Creating all the files

To create a simple application, you need to set up several files, including the Makefile, startup code, linker script, FreeRTOS configuration, and the main application code.

1. **Makefile**: This file contains the build configuration and rules.

    <!-- TODO: add some details -->

2. **Startup Code (`s32_startup.c`)**: This file contains the startup code for the S32K3X8EVB board.

    <!-- TODO: add some details -->

3. **Linker Script (`s32_linker.ld`)**: This file defines the memory layout for the application.

    <!-- TODO: add some details -->

4. **FreeRTOS Configuration (`FreeRTOSConfig.h`)**: This file contains the FreeRTOS configuration settings.

    <!-- TODO: add some details -->

5. **Main Application Code (`main.c`)**: This file contains the main application code.

    <!-- TODO: add some details -->

### Compiling and Running the Application

1. Compile the application using the FreeRTOS build system:
    ```sh
    make
    ```

2. Run the compiled application on QEMU:
    ```sh
    ../qemu/build/qemu-system-arm -machine s32k3x8evb -cpu cortex-m7 -kernel ./Output/app_name.elf -monitor none -nographic -serial stdio
    ```

## Conclusion

This guide provides a detailed walkthrough of our project, from setting up QEMU to running a FreeRTOS application on the emulated NXP S32K3X8EVB board. By following these steps, you should be able to recreate our project and understand the process of emulating hardware and running an RTOS on it.

If you encounter any errors or bugs, please refer to or contact the [authors](README.md#authors).
