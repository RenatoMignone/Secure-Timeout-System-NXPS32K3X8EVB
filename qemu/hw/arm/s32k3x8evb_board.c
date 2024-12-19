/*
 * S32K3X8EVB Board Initialization and Configuration
 * 
 * This file contains the initialization and configuration functions for the 
 * NXP S32K3X8EVB board. It includes functions to load firmware, initialize 
 * memory regions, and handle various hardware components.
 * 
 * The main functionalities provided by this file include:
 * - Loading firmware into the emulated flash memory.
 * - Initializing memory regions such as flash and SRAM.
 * - Setting up and configuring hardware components like NVIC, LPUART, and PIT timers.
 * - Managing system clocks and interrupts.
 *
 * Authors:
 * - Renato Mignone    (renato.mignone@studenti.polito.it)
 * - Simone Romano     (simone.romano2@studenti.polito.it)
 * - Elia Innocenti    (elia.innocenti@studenti.polito.it)
 * - Andrea Botticella (andrea.botticella@studenti.polito.it)
 * - Fabrizio Emanuel  (fabrizio.emanuel@studenti.polito.it)
 * 
 * All authors are students from the Politecnico di Torino, Italy.
 * 
 * Documentation:
 * - GitHub: https://github.com/neo-CAOS/group2.git
 * - GitLab: https://baltig.polito.it/caos2024/group2.git
 */


/* System Includes */

/* QEMU Core Includes */
#include "qemu/osdep.h"
#include "qemu/timer.h"
#include "qemu/log.h"
#include "qemu/typedefs.h"

/* Execution and Memory Management */
#include "exec/memory.h"
#include "exec/address-spaces.h"

/* Hardware Core Includes */
#include "hw/core/split-irq.h"
#include "hw/sysbus.h"
#include "hw/boards.h"
#include "hw/irq.h"
#include "hw/qdev-clock.h"

/* Specific Hardware Components */
#include "hw/sd/sd.h"
#include "hw/ssi/ssi.h"
#include "hw/arm/boot.h"
#include "hw/i2c/i2c.h"
#include "hw/timer/cmsdk-apb-timer.h"
#include "hw/arm/armv7m.h"
#include "hw/misc/unimp.h"

/* System Emulation */
#include "sysemu/sysemu.h"
#include "migration/vmstate.h"

/* QEMU Object Model */
#include "qom/object.h"

/* QEMU API */
#include "qapi/qmp/qlist.h"

/* User Interface */
#include "ui/input.h"

/* LPUART Includes */
#include "hw/char/stm32l4x5_usart.h"

/*------------------------------------------------------------------------------*/

/* Define boolean values */
typedef short int my_bool;
#define true 1
#define false 0

/* Define the verbosity of the output */
#ifdef VERBOSE
    my_bool verbose = true;
#else
    my_bool verbose = false;
#endif

/*------------------------------------------------------------------------------*/

/* Function to load an image into the target physical memory */
int load_image_targphys(const char *filename, hwaddr addr, hwaddr size);

/* Function to load the firmware */
void s32k3x8_load_firmware(ARMCPU *cpu, MachineState *ms, MemoryRegion *flash, const char *firmware_filename);

/* Function to initialize the memory regions */
void s32k3x8_initialize_memory_regions(MemoryRegion *system_memory);

/*------------------------------------------------------------------------------*/

/* Define constants for memory regions */

/* Flash memory blocks */
#define FLASH_BLOCK0_BASE_ADDR  0x00400000    // Block0 base address
#define FLASH_BLOCK0_SIZE       0x00200000    // 2 MB (Block0 size)

#define FLASH_BLOCK1_BASE_ADDR  0x00600000    // Block1 base address
#define FLASH_BLOCK1_SIZE       0x00200000    // 2 MB (Block1 size)

#define FLASH_BLOCK2_BASE_ADDR  0x00800000    // Block2 base address
#define FLASH_BLOCK2_SIZE       0x00200000    // 2 MB (Block2 size)

#define FLASH_BLOCK3_BASE_ADDR  0x00AD0000    // Block3 base address
#define FLASH_BLOCK3_SIZE       0x00200000    // 2 MB (Block3 size)

#define FLASH_BLOCK4_BASE_ADDR  0x10000000    // Block4 base address
#define FLASH_BLOCK4_SIZE       0x00020000    // 128 KB (Block4 size)

#define FLASH_UTEST_BASE_ADDR   0x18000000    // Utest base address
#define FLASH_UTEST_SIZE        0x00002000    // 8 KB (Utest size)

/* SRAM memory blocks */
#define SRAM0_BASE_ADDR         0x20400000    // Block0  base address
#define SRAM0_SIZE              0x00040000    // 265 KB (Block0 size)

#define SRAM1_BASE_ADDR         0x20440000    // SRAM1 base address
#define SRAM1_SIZE              0x00040000    // 256 KB (Block1 size)

#define SRAM2_BASE_ADDR         0x20480000    // SRAM2 base address
#define SRAM2_SIZE              0x00040000    // 256 KB (Block2 size)

/* DRAM memory block */
#define DRAM_BASE_ADDR          0x30000000    // DRAM base address
#define DRAM_SIZE               0x00100000    // 1 MB (DRAM size)

#define UART_BASE_ADDR          0x4006A000    // UART base address

/* PIT Timer base addresses */
#define PIT_TIMER1_BASE_ADDR    0x40037000    // PIT base address

#define PIT_TIMER2_BASE_ADDR    0x40038000    // PIT base address

/*------------------------------------------------------------------------------*/

/* Define the machine state */

#define TYPE_S32K3X8EVB_SYS "s32k3x8evb-sys"
OBJECT_DECLARE_SIMPLE_TYPE(ssys_state, S32K3X8EVB_SYS)
typedef struct S32K3X8MachineClass S32K3X8MachineClass;

/*------------------------------------------------------------------------------*/

/* Creation of the syss_state struct that represents the state of the system */

struct ssys_state {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    uint32_t rcc;
    uint32_t rcc2;
    qemu_irq irq;
    Clock *sysclk;
    Clock *refclk;
};

/*------------------------------------------------------------------------------*/

/* Creation of the S32K3X8MachineState struct that represents the state of the machine */

struct S32K3X8MachineState {
    MachineState *parent_obj;
    ssys_state sys;
    ARMv7MState nvic;
};
typedef struct S32K3X8MachineState S32K3X8MachineState;

/*------------------------------------------------------------------------------*/

/* Creation of the names of the board seen by QEMU */

#define TYPE_S32K3X8_BASE_MACHINE MACHINE_TYPE_NAME("s32k3x8")
#define TYPE_S32K3X8_MACHINE MACHINE_TYPE_NAME("s32k3x8-board")

DECLARE_INSTANCE_CHECKER(S32K3X8MachineState, S32K3X8_MACHINE, TYPE_S32K3X8_MACHINE)

/*------------------------------------------------------------------------------*/

/* Implementation of the function to initialize the memory regions */
void s32k3x8_initialize_memory_regions(MemoryRegion *system_memory) {

	if (verbose) fprintf(stdout, "\n------------------ Initialization of the memory regions ------------------\n");

    /* Initialize the memory regions for the flash, SRAM, and DRAM */

    MemoryRegion *flash0 = g_new(MemoryRegion, 1);
    MemoryRegion *flash1 = g_new(MemoryRegion, 1);
    MemoryRegion *flash2 = g_new(MemoryRegion, 1);
    MemoryRegion *flash3 = g_new(MemoryRegion, 1);
    MemoryRegion *flash4 = g_new(MemoryRegion, 1);

    MemoryRegion *utest = g_new(MemoryRegion, 1);

    MemoryRegion *flash0_alias = g_new(MemoryRegion, 1);

    MemoryRegion *sram0 = g_new(MemoryRegion, 1);
    MemoryRegion *sram1 = g_new(MemoryRegion, 1);
    MemoryRegion *sram2 = g_new(MemoryRegion, 1);

    MemoryRegion *dram = g_new(MemoryRegion, 1);
	
    /* Flash memory initialization (Read-Only) */

	if (verbose) fprintf(stdout, "\nInitializing flash memory...\n\n");

    memory_region_init_rom(flash0, NULL, "s32k3x8.flash0", FLASH_BLOCK0_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BLOCK0_BASE_ADDR, flash0);

    memory_region_init_rom(flash1, NULL, "s32k3x8.flash1", FLASH_BLOCK1_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BLOCK1_BASE_ADDR, flash1);

    memory_region_init_rom(flash2, NULL, "s32k3x8.flash2", FLASH_BLOCK2_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BLOCK2_BASE_ADDR, flash2);

    memory_region_init_rom(flash3, NULL, "s32k3x8.flash3", FLASH_BLOCK3_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BLOCK3_BASE_ADDR, flash3);

    memory_region_init_rom(flash4, NULL, "s32k3x8.flash4", FLASH_BLOCK4_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BLOCK4_BASE_ADDR, flash4);

    memory_region_init_rom(utest, NULL, "s32k3x8.utest", FLASH_UTEST_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_UTEST_BASE_ADDR, utest);
    
    memory_region_init_alias(flash0_alias, NULL, "s32k3x8.flash0.alias", flash0, 0, FLASH_BLOCK0_SIZE);
    memory_region_add_subregion(system_memory, 0, flash0_alias);

    /* SRAM memory initialization (RAM - Read-Write) */

	if (verbose) fprintf(stdout, "Initializing SRAM memory...\n\n");

	memory_region_init_ram(sram0, NULL, "s32k3x8.sram0", SRAM0_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, SRAM0_BASE_ADDR, sram0, 0);

    memory_region_init_ram(sram1, NULL, "s32k3x8.sram1", SRAM1_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, SRAM1_BASE_ADDR, sram1, 0);

    memory_region_init_ram(sram2, NULL, "s32k3x8.sram2", SRAM2_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, SRAM2_BASE_ADDR, sram2, 0);

    /* DRAM memory initialization (RAM - Read-Write) */

    if (verbose) fprintf(stdout, "Initializing DRAM memory...\n\n");

    memory_region_init_ram(dram, NULL, "s32k3x8.dram", DRAM_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, DRAM_BASE_ADDR, dram, 0);
    
    if (verbose) fprintf(stdout, "Memory regions initialized successfully.\n");
}

/*------------------------------------------------------------------------------*/

/* Function to initialize the S32K3X8 board for QEMU */

static void s32k3x8_init(MachineState *ms) {

    if (verbose) fprintf(stdout, "\n\n======================== Initializing the System =========================\n");

    /*--------------------------------------------------------------------------------------*/    
    /*------------Declaration of pointers for various QEMU device models--------------------*/
    /*--------------------------------------------------------------------------------------*/

    DeviceState *nvic;                    // Device models for NVIC and PIT timer
    Object *soc_container;                // Container object for the System-on-Chip (SoC)
    DeviceState *syss_dev;                // Device state for the system controller
    DeviceState *pit_timer1,*pit_timer2;  // DeviceState for the PIT timers
    MemoryRegion *system_memory;          // Initialize the pointer to the system memory

    /*--------------------------------------------------------------------------------------*/
    /*------------Allocate memory and initialize the machine state structure----------------*/
    /*--------------------------------------------------------------------------------------*/

    S32K3X8MachineState *m_state = g_new0(S32K3X8MachineState, 1);

    /* The machine state in qemu represents the state of the machine at runtime */
    m_state->parent_obj = ms;  // Link the machine state to the parent machine state object

    /*--------------------------------------------------------------------------------------*/
    /*---------------Obtain a reference to the global system memory region------------------*/
    /*--------------------------------------------------------------------------------------*/

    system_memory = get_system_memory();

    /* Initialize memory regions for flash, SRAM, etc. */
    s32k3x8_initialize_memory_regions(system_memory);

    /*--------------------------------------------------------------------------------------*/
    /*------------------------Create a container object for the SoC-------------------------*/
    /*--------------------------------------------------------------------------------------*/

    /* This organizes all devices under the "soc" hierarchy */
    soc_container = object_new("container");

    /* Add the SoC container as a child of the machine state object */
    object_property_add_child(OBJECT(ms), "soc", soc_container);

    /* Initialize the system controller (represents core system functionality) */
    object_initialize_child(OBJECT(ms), "sys", &m_state->sys, TYPE_S32K3X8EVB_SYS);
    sysbus_realize(SYS_BUS_DEVICE(&m_state->sys), &error_abort); // Realize the system controller

    /* Create and add another instance of the system controller for the SoC */
    syss_dev = qdev_new(TYPE_S32K3X8EVB_SYS);
    object_property_add_child(soc_container, "sys", OBJECT(syss_dev));

    /* Realize and activate the system controller device model */
    sysbus_realize_and_unref(SYS_BUS_DEVICE(syss_dev), &error_fatal);

    /*--------------------------------------------------------------------------------------*/
    /*------------------------ Initialize a clock for the system----------------------------*/
    /*--------------------------------------------------------------------------------------*/

    if (verbose) fprintf(stdout, "\n------------------- Initialization of the system Clock -------------------\n");

    m_state->sys.sysclk = clock_new(OBJECT(DEVICE(&m_state->sys)), "sysclk"); // Create clock object
    
    /* Set the clock period to 7.14ns (equivalent to 140MHz frequency) */
    clock_set_ns(m_state->sys.sysclk, 7.14);

    m_state->sys.refclk = clock_new(OBJECT(DEVICE(&m_state->sys)), "refclk");
    clock_set_hz(m_state->sys.refclk, 1000000);

    /* Log the successful clock initialization */
    if (verbose) fprintf(stdout, "\nClock initialized.\n");

    /*--------------------------------------------------------------------------------------*/
    /*-------------Initialize the Nested Vectored Interrupt Controller (NVIC)---------------*/
    /*--------------------------------------------------------------------------------------*/

    if (verbose) fprintf(stdout, "\n------------------- Initialization of the system NVIC --------------------\n");

    nvic = qdev_new(TYPE_ARMV7M); // Create a new NVIC device model

    /* Add the NVIC to the SoC container */
    object_property_add_child(soc_container, "v7m", OBJECT(nvic));

    /* Configure the NVIC with the number of IRQs (128 for S32K3X8) */
    qdev_prop_set_uint32(nvic, "num-irq", 32);

    /* Configure the number of priority bits for the NVIC */
    qdev_prop_set_uint8(nvic, "num-prio-bits", 4);

    /* Connect the NVIC to the system clock */
    qdev_connect_clock_in(nvic, "cpuclk", m_state->sys.sysclk);
    qdev_connect_clock_in(nvic, "refclk", m_state->sys.refclk);

    /* Set the CPU type for the NVIC (retrieved from the machine state) */
    /* In particular we are setting the cortex-m7 cpu type */
    qdev_prop_set_string(nvic, "cpu-type", m_state->parent_obj->cpu_type);

    /* Enable bit-band support for the NVIC */
    qdev_prop_set_bit(nvic, "enable-bitband", true);

    /* Link the NVIC's memory access to the system memory */
    object_property_set_link(OBJECT(nvic), "memory", OBJECT(get_system_memory()), &error_abort);

    /* Realize and activate the NVIC model */
    sysbus_realize_and_unref(SYS_BUS_DEVICE(nvic), &error_fatal);

    /* Log the successful realization of the NVIC */
    if (verbose) fprintf(stdout, "\nNVIC realized.\n");

    /*--------------------------------------------------------------------------------------*/
    /*--------------------------Initialize the LPUART device--------------------------------*/
    /*--------------------------------------------------------------------------------------*/

    if (verbose) fprintf(stdout, "\n---------------------- Initialization of the LPUART ----------------------\n");

    DeviceState *lpuart = qdev_new(TYPE_STM32L4X5_LPUART);
    qdev_prop_set_chr(lpuart, "chardev", serial_hd(0));
    qdev_connect_clock_in(lpuart, "clk", m_state->sys.sysclk);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(lpuart), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(lpuart), 0, UART_BASE_ADDR);
    sysbus_connect_irq(SYS_BUS_DEVICE(lpuart), 0, qdev_get_gpio_in(nvic, 0));

    if (verbose) fprintf(stdout, "\nLPUART initialized and connected to NVIC.\n");

    /*--------------------------------------------------------------------------------------*/
    /*-------------------------- Initialize the PIT timer-----------------------------------*/
    /*--------------------------------------------------------------------------------------*/

    if (verbose) fprintf(stdout, "\n-------------------- Initialization of the Two timers --------------------\n");

    /* First Timer */
    pit_timer1 = qdev_new(TYPE_CMSDK_APB_TIMER);
    SysBusDevice *sbd1;
    sbd1 = SYS_BUS_DEVICE(pit_timer1);
    qdev_connect_clock_in(DEVICE(pit_timer1), "pclk", m_state->sys.sysclk);
    sysbus_realize_and_unref(sbd1, &error_fatal);
    sysbus_mmio_map(sbd1, 0, PIT_TIMER1_BASE_ADDR);
    sysbus_connect_irq(sbd1, 0, qdev_get_gpio_in(nvic, 8));

    if (verbose) fprintf(stdout,"\nFirst Timer Initialized Correctly\n");

    /* Second Timer */
    pit_timer2 = qdev_new(TYPE_CMSDK_APB_TIMER);
    SysBusDevice *sbd2;
    sbd2 = SYS_BUS_DEVICE(pit_timer2);
    qdev_connect_clock_in(DEVICE(pit_timer2), "pclk", m_state->sys.sysclk);
    sysbus_realize_and_unref(sbd2, &error_fatal);
    sysbus_mmio_map(sbd2, 0, PIT_TIMER2_BASE_ADDR);
    sysbus_connect_irq(sbd2, 0, qdev_get_gpio_in(nvic, 9));

    if (verbose) fprintf(stdout,"\nSecond Timer Initialized Correctly\n");

    /*--------------------------------------------------------------------------------------*/
    /*--------------------Load firmware into the emulated flash memory----------------------*/
    /*--------------------------------------------------------------------------------------*/

    if (verbose) fprintf(stdout, "\n---------------- Loading the Kernel into the flash memory ----------------\n");

    /* The firmware file is specified in the machine state (ms->kernel_filename) */
    armv7m_load_kernel(ARM_CPU(first_cpu), ms->kernel_filename, FLASH_BLOCK0_BASE_ADDR, FLASH_BLOCK0_SIZE);

    /* Log the successful loading of the firmware */
    if (verbose) fprintf(stdout, "\nKernel loaded into flash memory.\n\n");

    /* Log the completion of the board initialization */
    if (verbose) fprintf(stdout, "System initialized.\n\n");

    if (verbose) fprintf(stdout, "Starting to run...\n");

    if (verbose) fprintf(stdout, "\n==========================================================================\n");
}

/*------------------------------------------------------------------------------*/

/* Implementation of the class init function */

static void s32k3x8_class_init(ObjectClass *oc, void *data) {
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->name = g_strdup("s32k3x8evb");
    mc->desc = "NXP S32K3X8 EVB (Cortex-M7)";
    mc->init = s32k3x8_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-m7");
    mc->default_cpus = 1;
    mc->min_cpus = mc->default_cpus;
    mc->max_cpus = mc->default_cpus;
    mc->no_floppy = 1;
    mc->no_cdrom = 1;
    mc->no_parallel = 1;
}

/*------------------------------------------------------------------------------*/

/* Section of code to define the infos of the board, wehere we do specify the functions to run for the init and class init */

static const TypeInfo s32k3x8_machine_types = {
    .name           = TYPE_S32K3X8_MACHINE,
    .parent         = TYPE_MACHINE,
    .class_init     = s32k3x8_class_init,
};

static const TypeInfo s32k3x8evb_sys_info = {
    .name          = TYPE_S32K3X8EVB_SYS,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(ssys_state),
};

static void s32k3x8evb_machine_init(void) {
    type_register_static(&s32k3x8_machine_types);
    type_register_static(&s32k3x8evb_sys_info);
}

type_init(s32k3x8evb_machine_init);

/*------------------------------------------------------------------------------*/
