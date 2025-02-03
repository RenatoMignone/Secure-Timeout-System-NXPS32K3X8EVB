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
 * - Andrea Botticella (andrea.botticella@studenti.polito.it)
 * - Fabrizio Emanuel  (fabrizio.emanuel@studenti.polito.it)
 * - Elia Innocenti    (elia.innocenti@studenti.polito.it)
 * - Renato Mignone    (renato.mignone@studenti.polito.it)
 * - Simone Romano     (simone.romano2@studenti.polito.it)
 * 
 * All authors are students from the Politecnico di Torino, Italy.
 * 
 * Note: The authors are listed in alphabetical order.
 * 
 * Documentation:
 * - GitHub: https://github.com/neo-CAOS/Secure-Timeout-System-NXPS32K3X8EVB.git
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

/* Macro to print verbose output */
#define fprintf_v(stream, format, ...) \
    do { if (verbose) fprintf(stream, format, ##__VA_ARGS__); } while(0)

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

#define FLASH_BLOCK3_BASE_ADDR  0x00A00000    // Block3 base address
#define FLASH_BLOCK3_SIZE       0x00200000    // 2 MB (Block3 size)

#define FLASH_BLOCK4_BASE_ADDR  0x10000000    // Block4 base address
#define FLASH_BLOCK4_SIZE       0x00020000    // 128 KB (Block4 size)

#define FLASH_UTEST_BASE_ADDR   0x1B000000    // Utest base address
#define FLASH_UTEST_SIZE        0x00002000    // 8 KB (Utest size)

/* SRAM memory blocks */

#define SRAM_STDBY_BASE_ADDR    0x20400000    // SRAM standby base address
#define SRAM_STDBY_SIZE         0x00010000    // 64 KB

#define SRAM0_BASE_ADDR         0x20410000    // Block0  base address
#define SRAM0_SIZE              0x00030000    // 192 KB (Block0 size) Total = 256 KB (standby + Sram0)

#define SRAM1_BASE_ADDR         0x20440000    // SRAM1 base address
#define SRAM1_SIZE              0x00040000    // 256 KB (Block1 size)

#define SRAM2_BASE_ADDR         0x20480000    // SRAM2 base address
#define SRAM2_SIZE              0x00040000    // 256 KB (Block2 size)

/* DTCM memory block*/
#define DTCM0_BASE_ADDR         0x20000000    // DTCM0 base address
#define DTCM0_SIZE              0x00020000    // 128 KB 

#define DTCM2_BASE_ADDR         0x21800000    // DTCM2 base address
#define DTCM2_SIZE              0x00020000    // 128 KB

/* ITCM memory block*/
#define ITCM0_BASE_ADDR         0x00000000    // ITCM0 base address
#define ITCM0_SIZE              0x00010000    // 64KB

#define ITCM2_BASE_ADDR         0x00010000    // ITCM2 base address
#define ITCM2_SIZE              0x00010000    // 64KB

/*LPUART memory address*/
#define UART_BASE_ADDR          0x4006A000    // UART base address

/* PIT Timer base addresses */
#define PIT_TIMER1_BASE_ADDR    0x40037000    // PIT base address
#define PIT_TIMER2_BASE_ADDR    0x40038000    // PIT base address
#define PIT_TIMER3_BASE_ADDR    0x40039000    // PIT base address

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
    Clock *aips_plat_clk;
    Clock *aips_slow_clk;
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

    fprintf_v(stdout, "\n------------------ Initialization of the memory regions ------------------\n");

    /* Initialize the memory regions for the flash, SRAM, and DRAM */

    MemoryRegion *flash0 = g_new(MemoryRegion, 1);
    MemoryRegion *flash1 = g_new(MemoryRegion, 1);
    MemoryRegion *flash2 = g_new(MemoryRegion, 1);
    MemoryRegion *flash3 = g_new(MemoryRegion, 1);
    MemoryRegion *flash4 = g_new(MemoryRegion, 1);

    MemoryRegion *utest = g_new(MemoryRegion, 1);

    MemoryRegion *sram_standby = g_new(MemoryRegion, 1);
    MemoryRegion *sram0 = g_new(MemoryRegion, 1);
    MemoryRegion *sram1 = g_new(MemoryRegion, 1);
    MemoryRegion *sram2 = g_new(MemoryRegion, 1);
		
    MemoryRegion *itcm0 = g_new(MemoryRegion, 1);
    MemoryRegion *itcm2 = g_new(MemoryRegion, 1);
    MemoryRegion *dtcm0 = g_new(MemoryRegion, 1);
    MemoryRegion *dtcm2 = g_new(MemoryRegion, 1);

    /* Flash memory initialization (Read-Only) */

    fprintf_v(stdout, "\nInitializing flash memory...\n\n");

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

    /* SRAM memory initialization (RAM - Read-Write) */

    fprintf_v(stdout, "Initializing SRAM memory...\n\n");

    /* SRAM Standby */
    memory_region_init_ram(sram_standby, NULL, "s32k3x8.sram_standby", SRAM_STDBY_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, SRAM_STDBY_BASE_ADDR, sram_standby, 0);

	memory_region_init_ram(sram0, NULL, "s32k3x8.sram0", SRAM0_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, SRAM0_BASE_ADDR, sram0, 0);

    memory_region_init_ram(sram1, NULL, "s32k3x8.sram1", SRAM1_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, SRAM1_BASE_ADDR, sram1, 0);

    memory_region_init_ram(sram2, NULL, "s32k3x8.sram2", SRAM2_SIZE, &error_fatal);
    memory_region_add_subregion_overlap(system_memory, SRAM2_BASE_ADDR, sram2, 0);
    
    /* ITCM memory inizialization */

    fprintf_v(stdout, "Initializing ITCM memory...\n\n");

    memory_region_init_ram(itcm0, NULL, "s32k3x8.itcm0", ITCM0_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, ITCM0_BASE_ADDR, itcm0);

    memory_region_init_ram(itcm2, NULL, "s32k3x8.itcm2", ITCM2_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, ITCM2_BASE_ADDR, itcm2);
 
    /* DTCM memory inizialization */

    fprintf_v(stdout, "Initializing DTCM memory...\n\n");

    memory_region_init_ram(dtcm0, NULL, "s32k3x8.dtcm0", DTCM0_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, DTCM0_BASE_ADDR, dtcm0);

    memory_region_init_ram(dtcm2, NULL, "s32k3x8.dtcm2", DTCM2_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, DTCM2_BASE_ADDR, dtcm2);

    fprintf_v(stdout, "Memory regions initialized successfully.\n");
}

/*------------------------------------------------------------------------------*/

/* Function to initialize LPUART devices */

static void initialize_lpuarts(S32K3X8MachineState *m_state, DeviceState *nvic, int num_lpuarts) {

    fprintf_v(stdout, "\n---------------------- Initializing LPUART Devices -----------------------\n\n");

    for (int i = 0; i < num_lpuarts; i++) {
        char device_name[32];
        snprintf(device_name, sizeof(device_name), "s32k3x8.lpuart%d", i);

        DeviceState *lpuart = qdev_new(TYPE_STM32L4X5_LPUART);
        qdev_prop_set_chr(lpuart, "chardev", serial_hd(i));

	    if(i==0 || i==1 || i==8) {
            qdev_connect_clock_in(lpuart, "clk", m_state->sys.aips_plat_clk);
        } else {
            qdev_connect_clock_in(lpuart, "clk", m_state->sys.aips_slow_clk);
        }

        sysbus_realize_and_unref(SYS_BUS_DEVICE(lpuart), &error_fatal);

        /* Calculate base address for each LPUART */
        hwaddr base_addr = UART_BASE_ADDR + (i * 0x1000); // Assuming 0x1000 offset between LPUARTs
        sysbus_mmio_map(SYS_BUS_DEVICE(lpuart), 0, base_addr);

        /* Connect LPUART interrupt to NVIC */
        sysbus_connect_irq(SYS_BUS_DEVICE(lpuart), 0, qdev_get_gpio_in(nvic, i));

        fprintf_v(stdout, "Initialized LPUART %2d at base address 0x%08lx\n", i, base_addr);
    }

    fprintf_v(stdout, "\nAll LPUART devices initialized and connected to NVIC.\n");
}

/*------------------------------------------------------------------------------*/

/* Function to initialize the S32K3X8 board for QEMU */

static void s32k3x8_init(MachineState *ms) {

    fprintf_v(stdout, "\n\n======================== Initializing the System =========================\n");

    /*--------------------------------------------------------------------------------------*/    
    /*------------Declaration of pointers for various QEMU device models--------------------*/
    /*--------------------------------------------------------------------------------------*/

    DeviceState *nvic;                                  // Device models for NVIC and PIT timer
    Object *soc_container;                              // Container object for the System-on-Chip (SoC)
    DeviceState *syss_dev;                              // Device state for the system controller
    DeviceState *pit_timer1,*pit_timer2,*pit_timer3;    // DeviceState for the PIT timers
    MemoryRegion *system_memory;                        // Initialize the pointer to the system memory

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
    /*------------------------ Initialize the clocks  for the system----------------------------*/
    /*--------------------------------------------------------------------------------------*/

    fprintf_v(stdout, "\n--------------------- Initialization of the Clocks -----------------------\n");

    m_state->sys.sysclk = clock_new(OBJECT(DEVICE(&m_state->sys)), "sysclk"); // Create clock object
    
    /* Set the clock period to 4.16ns (equivalent to 240MHz frequency) */
    clock_set_ns(m_state->sys.sysclk, 4.16);

    m_state->sys.refclk = clock_new(OBJECT(DEVICE(&m_state->sys)), "refclk");
    clock_set_hz(m_state->sys.refclk, 1000000);

    m_state->sys.aips_plat_clk = clock_new(OBJECT(DEVICE(&m_state->sys)), "aips_plat_clk");
    clock_set_hz(m_state->sys.aips_plat_clk, 80000000);

    m_state->sys.aips_slow_clk = clock_new(OBJECT(DEVICE(&m_state->sys)), "aips_slow_clk");
    clock_set_hz(m_state->sys.aips_slow_clk, 40000000);

    /* Log the successful clock initialization */
    fprintf_v(stdout, "\nClock initialized.\n");

    /*--------------------------------------------------------------------------------------*/
    /*-------------Initialize the Nested Vectored Interrupt Controller (NVIC)---------------*/
    /*--------------------------------------------------------------------------------------*/

    fprintf_v(stdout, "\n------------------- Initialization of the system NVIC --------------------\n");

    nvic = qdev_new(TYPE_ARMV7M); // Create a new NVIC device model

    /* Add the NVIC to the SoC container */
    object_property_add_child(soc_container, "v7m", OBJECT(nvic));

    /* Configure the NVIC with the number of IRQs (256 for S32K3X8) */
    qdev_prop_set_uint32(nvic, "num-irq", 256);

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
    fprintf_v(stdout, "\nNVIC realized.\n");

    /*--------------------------------------------------------------------------------------*/
    /*--------------------------Initialize the LPUART device--------------------------------*/
    /*--------------------------------------------------------------------------------------*/

    initialize_lpuarts(m_state, nvic, 16);

    /*--------------------------------------------------------------------------------------*/
    /*-------------------------- Initialize the PIT timer-----------------------------------*/
    /*--------------------------------------------------------------------------------------*/

    fprintf_v(stdout, "\n---------------------- Initialization of the Timers ----------------------\n");

    /* First Timer */
    pit_timer1 = qdev_new(TYPE_CMSDK_APB_TIMER);
    SysBusDevice *sbd1;
    sbd1 = SYS_BUS_DEVICE(pit_timer1);
    qdev_connect_clock_in(DEVICE(pit_timer1), "pclk", m_state->sys.sysclk);
    sysbus_realize_and_unref(sbd1, &error_fatal);
    sysbus_mmio_map(sbd1, 0, PIT_TIMER1_BASE_ADDR);
    sysbus_connect_irq(sbd1, 0, qdev_get_gpio_in(nvic, 8));

    fprintf_v(stdout,"\nFirst Timer Initialized Correctly\n");

    /* Second Timer */
    pit_timer2 = qdev_new(TYPE_CMSDK_APB_TIMER);
    SysBusDevice *sbd2;
    sbd2 = SYS_BUS_DEVICE(pit_timer2);
    qdev_connect_clock_in(DEVICE(pit_timer2), "pclk", m_state->sys.sysclk);
    sysbus_realize_and_unref(sbd2, &error_fatal);
    sysbus_mmio_map(sbd2, 0, PIT_TIMER2_BASE_ADDR);
    sysbus_connect_irq(sbd2, 0, qdev_get_gpio_in(nvic, 9));

    fprintf_v(stdout,"\nSecond Timer Initialized Correctly\n");

    /* Third Timer */
    pit_timer3 = qdev_new(TYPE_CMSDK_APB_TIMER);
    SysBusDevice *sbd3;
    sbd3 = SYS_BUS_DEVICE(pit_timer3);
    qdev_connect_clock_in(DEVICE(pit_timer3), "pclk", m_state->sys.sysclk);
    sysbus_realize_and_unref(sbd3, &error_fatal);
    sysbus_mmio_map(sbd3, 0, PIT_TIMER3_BASE_ADDR);
    sysbus_connect_irq(sbd3, 0, qdev_get_gpio_in(nvic, 10));

    fprintf_v(stdout,"\nThird Timer Initialized Correctly\n");

    /*--------------------------------------------------------------------------------------*/
    /*--------------------Load firmware into the emulated flash memory----------------------*/
    /*--------------------------------------------------------------------------------------*/

    fprintf_v(stdout, "\n---------------- Loading the Kernel into the flash memory ----------------\n");

    /* The firmware file is specified in the machine state (ms->kernel_filename) */
    armv7m_load_kernel(ARM_CPU(first_cpu), ms->kernel_filename, FLASH_BLOCK0_BASE_ADDR, FLASH_BLOCK0_SIZE);

    /* Log the successful loading of the firmware */
    fprintf_v(stdout, "\nKernel loaded into flash memory.\n\n");

    /* Log the completion of the board initialization */
    fprintf_v(stdout, "System initialized.\n\n");

    fprintf_v(stdout, "Starting to run...\n");

    fprintf_v(stdout, "\n==========================================================================\n");
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

/* Section of code to define the infos of the board, where we do specify the functions to run for the init and class init */

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
