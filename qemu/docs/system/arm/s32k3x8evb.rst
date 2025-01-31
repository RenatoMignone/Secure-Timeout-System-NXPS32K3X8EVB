.. _s32k3x8evb:

S32K3X8EVB Board Implementation in QEMU
=======================================

This document outlines the implementation of the NXP S32K3X8EVB board in the QEMU emulator. The S32K3X8EVB is an evaluation board featuring the S32K358 microcontroller. This RST file provides details on the board's memory layout, peripheral mappings, and initialization steps as they have been implemented in QEMU.

Board Specifications
--------------------

Memory Layout
~~~~~~~~~~~~~

Flash Memory
^^^^^^^^^^^^
The board has multiple flash memory blocks:

- Block 0: 2 MB at 0x00400000  
- Block 1: 2 MB at 0x00600000  
- Block 2: 2 MB at 0x00800000  
- Block 3: 2 MB at 0x00A00000  
- Block 4: 128 KB at 0x10000000  
- Utest: 8 KB at 0x1B000000  

SRAM, DTCM, ITCM
^^^^^^^^^^^^^^^^
- SRAM: 3 blocks, 256 KB each  

  - Block 0: 0x20400000  
  - Block 1: 0x20440000  
  - Block 2: 0x20480000  

- DTCM: 2 blocks, 128 KB each  

  - DTCM0: 0x20000000  
  - DTCM2: 0x21800000  

- ITCM: 2 blocks, 64 KB each  

  - ITCM0: 0x00000000  
  - ITCM2: 0x00010000  

Peripheral Mapping
~~~~~~~~~~~~~~~~~~

- UART Base Address: 0x4006A000  
- PIT Timer Base Addresses:  

  - Timer 1: 0x40037000  
  - Timer 2: 0x40038000  
  - Timer 3: 0x40039000  

- 16 LPUART peripherals mapped from the UART base address  
- LPUART 0, 1, and 8 are clocked by AIPS_PLAT_CLK  
- The remaining LPUARTs are clocked by AIPS_SLOW_CLK  

Note:
~~~~~
Refer to NXP S32K3X8EVB docs for comprehensive information.

Initialization and Configuration
--------------------------------

Memory Initialization
~~~~~~~~~~~~~~~~~~~~~
- Flash: 2 MB blocks at 0x00400000, 0x00600000, 0x00800000, 0x00A00000; 128 KB at 0x10000000; 8 KB at 0x1B000000  
- SRAM: 64 KB standby at 0x20400000; 192 KB at 0x20410000; 256 KB at 0x20440000 and 0x20480000  
- ITCM and DTCM blocks  

Peripheral Initialization
~~~~~~~~~~~~~~~~~~~~~~~~~
- 16 LPUART devices mapped from 0x4006A000  
- PIT Timers at 0x40037000, 0x40038000, 0x40039000  

Clock Initialization
~~~~~~~~~~~~~~~~~~~~
- 240 MHz system clock  
- 1 MHz reference clock  
- 80 MHz AIPS_PLAT_CLK  
- 40 MHz AIPS_SLOW_CLK  

Firmware Loading
~~~~~~~~~~~~~~~~
- Firmware loaded into flash memory at 0x00400000  

NVIC Initialization
~~~~~~~~~~~~~~~~~~~
- 32 IRQs, 4 priority bits  
- Connected to system and reference clocks  
- Linked to system memory  

Once initialized, the S32K3X8EVB board is ready to run the loaded firmware.

Authors
-------

- Andrea Botticella (andrea.botticella@studenti.polito.it)
- Fabrizio Emanuel  (fabrizio.emanuel@studenti.polito.it)
- Elia Innocenti    (elia.innocenti@studenti.polito.it)
- Renato Mignone    (renato.mignone@studenti.polito.it)
- Simone Romano     (simone.romano2@studenti.polito.it)

All authors are students from the Politecnico di Torino, Italy.

Note:
~~~~~
The authors are listed in alphabetical order.

Documentation
-------------

- GitHub: https://github.com/neo-CAOS/Secure-Timeout-System-NXPS32K3X8EVB.git
- GitLab: https://baltig.polito.it/caos2024/group2.git
