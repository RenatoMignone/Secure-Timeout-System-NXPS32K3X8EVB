.. _S32K3X8EVB-board:

S32K3X8EVB Board
================

This page provides documentation for the S32K3X8EVB board.

Hardware Overview
-----------------
The S32K3X8EVB board includes a variety of features such as memory, storage, and serial ports. Here is a summary of key components:

- **Flash Memory**: -- MB
- **RAM**: --- MB
- **Serial Ports**: 16 LPUARTs

Memory Layout
-------------
The memory layout of the board starts at the following addresses:

- **Flash Memory Blocks**:
  - Block 0: `0x00400000` (2 MB)
  - Block 1: `0x00600000` (2 MB)
  - Block 2: `0x00800000` (2 MB)
  - Block 3: `0x00A00000` (2 MB)
  - Block 4: `0x10000000` (128 KB)
  - Utest:   `0x1B000000` (8 KB)

- **SRAM Memory Blocks**:
  - SRAM0:   `0x20400000` (256 KB)
  - SRAM1:   `0x20440000` (256 KB)
  - SRAM2:   `0x20480000` (256 KB)

- **DTCM Memory Blocks**:
  - DTCM0:   `0x20000000` (128 KB)
  - DTCM2:   `0x21800000` (128 KB)

- **ITCM Memory Blocks**:
  - ITCM0:   `0x00000000` (64 KB)
  - ITCM2:   `0x00010000` (64 KB)

- **LPUART Memory Address**:
  - UART Base Address: `0x4006A000`

- **PIT Timer Base Addresses**:
  - PIT Timer 1: `0x40037000`
  - PIT Timer 2: `0x40038000`
  - PIT Timer 3: `0x40039000`

Initialization and Configuration
-------------------------------
The S32K3X8EVB board initialization and configuration include the following steps:

1. **Loading Firmware**:
   - The firmware is loaded into the emulated flash memory using the `s32k3x8_load_firmware` function.

2. **Initializing Memory Regions**:
   - The memory regions such as flash, SRAM, and DTCM are initialized using the `s32k3x8_initialize_memory_regions` function.

3. **Setting Up Hardware Components**:
   - Hardware components like NVIC, LPUART, and PIT timers are set up and configured.

4. **Managing System Clocks and Interrupts**:
   - System clocks and interrupts are managed to ensure proper operation of the board.

Authors
-------
- Andrea Botticella (andrea.botticella@studenti.polito.it)
- Fabrizio Emanuel  (fabrizio.emanuel@studenti.polito.it)
- Elia Innocenti    (elia.innocenti@studenti.polito.it)
- Renato Mignone    (renato.mignone@studenti.polito.it)
- Simone Romano     (simone.romano2@studenti.polito.it)

All authors are students from the Politecnico di Torino, Italy.

.. note::
   The authors are listed in alphabetical order.

Documentation
-------------
- GitHub: https://github.com/neo-CAOS/Secure-Timeout-System-NXPS32K3X8EVB.git
- GitLab: https://baltig.polito.it/caos2024/group2.git
