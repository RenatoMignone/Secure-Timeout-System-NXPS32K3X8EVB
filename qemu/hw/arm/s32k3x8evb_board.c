#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/core/split-irq.h"
#include "hw/sysbus.h"
#include "hw/sd/sd.h"
#include "hw/ssi/ssi.h"
#include "hw/arm/boot.h"
#include "qemu/timer.h"
#include "exec/memory.h"
#include "hw/i2c/i2c.h"
#include "net/net.h"
#include "hw/boards.h"
#include "qemu/log.h"
#include "exec/address-spaces.h"
#include "sysemu/sysemu.h"
#include "hw/arm/armv7m.h"
#include "hw/char/pl011.h"
#include "hw/irq.h"
#include "hw/watchdog/cmsdk-apb-watchdog.h"
#include "migration/vmstate.h"
#include "hw/misc/unimp.h"
#include "hw/qdev-clock.h"
#include "qom/object.h"
#include "qapi/qmp/qlist.h"
#include "ui/input.h"
#include "qemu/typedefs.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define some functions useful in the code.
int load_image_targphys(const char *filename, hwaddr addr, hwaddr size);
// Function for error reporting
void error_report(const char *fmt, ...);
// Function to load the firmware
void s32k3x8_load_firmware(ARMCPU *cpu, MachineState *ms, MemoryRegion *flash, const char *firmware_filename);
// Function to initialize the memory regions
void s32k3x8_initialize_memory_regions(MemoryRegion *system_memory);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define constants for memory regions
#define FLASH_BASE_ADDR  0x00000000  // Flash memory base address
#define FLASH_SIZE       0x00C00000  // 12 MB program flash

#define SRAM_BASE_ADDR   0x20400000  // SRAM base address
#define SRAM_SIZE        0x00240000  // 2.25 MB SRAM


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define the machine state
#define TYPE_S32K3X8EVB_SYS "s32k3x8evb-sys"
OBJECT_DECLARE_SIMPLE_TYPE(ssys_state, S32K3X8EVB_SYS)
typedef struct S32K3X8ExampleBoardMachineClass S32K3X8ExampleBoardMachineClass;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creation of the syss_state struct that represents the state of the system
struct ssys_state {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    uint32_t rcc;
    uint32_t rcc2;
    qemu_irq irq;
    Clock *sysclk;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creation of the S32K3X8ExampleBoardMachineState struct that represents the state of the machine
struct S32K3X8ExampleBoardMachineState {
    MachineState *parent_obj;
    ssys_state sys;
    ARMv7MState nvic;
};
typedef struct S32K3X8ExampleBoardMachineState S32K3X8ExampleBoardMachineState;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creation of the names of the board seen by QEMU.
#define TYPE_S32K3X8_EXAMPLE_BOARD_BASE_MACHINE MACHINE_TYPE_NAME("s32k3x8")
#define TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE MACHINE_TYPE_NAME("s32k3x8-example-board")

DECLARE_INSTANCE_CHECKER(S32K3X8ExampleBoardMachineState, S32K3X8_EXAMPLE_BOARD_MACHINE, TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of the function to initialize the memory regions
void s32k3x8_initialize_memory_regions(MemoryRegion *system_memory) {
    MemoryRegion *flash = g_new(MemoryRegion, 1);
    MemoryRegion *sram = g_new(MemoryRegion, 1);

    // Validate system memory
    if (!system_memory) {
        error_report("System memory region is NULL");
        exit(1);
    }

	// Flash memory initialization (Read-Only)
	fprintf(stdout, "Initializing flash memory...\n\n");
	memory_region_init_rom(flash, NULL, "s32k3x8.flash", FLASH_SIZE, &error_fatal);
	memory_region_add_subregion(system_memory, FLASH_BASE_ADDR, flash);

	// SRAM memory initialization (RAM - Read-Write)
	fprintf(stdout, "Initializing SRAM memory...\n\n");
	memory_region_init_ram(sram, NULL, "s32k3x8.sram", SRAM_SIZE, &error_fatal);
	memory_region_add_subregion_overlap(system_memory, SRAM_BASE_ADDR, sram, 0);

	fprintf(stdout, "Memory regions initialized successfully.\n\n");


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//implementation of the function to initialize the board
static void s32k3x8_example_board_init(MachineState *ms) {

    DeviceState *nvic, *uart_dev, *pit_timer;
    Object *soc_container;
    DeviceState *syss_dev;
    SysBusDevice *uart;

    // Process logging
    fprintf(stdout, "Instantiating the Machine\n\n");

    // Machine state initialization
    S32K3X8ExampleBoardMachineState *m_state = g_new0(S32K3X8ExampleBoardMachineState, 1);
    m_state->parent_obj = ms;
    MemoryRegion *system_memory = get_system_memory();

    // Initialize memory regions (flash, SRAM, etc.)
    fprintf(stdout, "Initializing memory regions...\n\n");
    s32k3x8_initialize_memory_regions(system_memory);

    // Create container for the SoC
    soc_container = object_new("container");
    object_property_add_child(OBJECT(ms), "soc", soc_container);

    // Initialize and add the system controller (sys state)
    object_initialize_child(OBJECT(ms), "sys", &m_state->sys, TYPE_S32K3X8EVB_SYS);
    sysbus_realize(SYS_BUS_DEVICE(&m_state->sys), &error_abort);
    syss_dev = qdev_new(TYPE_S32K3X8EVB_SYS);
    object_property_add_child(soc_container, "sys", OBJECT(syss_dev));
    sysbus_realize_and_unref(SYS_BUS_DEVICE(syss_dev), &error_fatal);
    fprintf(stdout, "System controller realized.\n\n");


    // CLOCK INITIALIZATION
    m_state->sys.sysclk = clock_new(OBJECT(DEVICE(&m_state->sys)), "sysclk");
    clock_set_ns(m_state->sys.sysclk, 40.69); // Example: 40.69ns (24.5MHz)
    fprintf(stdout, "Clock initialized.\n\n");


    // Initialize NVIC
    nvic = qdev_new(TYPE_ARMV7M);
    object_property_add_child(soc_container, "v7m", OBJECT(nvic));
    qdev_prop_set_uint32(nvic, "num-irq", 128); // Adjusted for S32K3X8
    qdev_prop_set_uint8(nvic, "num-prio-bits", 4); // Adjusted priority bits
    qdev_connect_clock_in(nvic, "cpuclk", m_state->sys.sysclk);
    qdev_prop_set_string(nvic, "cpu-type", m_state->parent_obj->cpu_type);
    qdev_prop_set_bit(nvic, "enable-bitband", true);

    // Connect the NVIC to the system memory
    object_property_set_link(OBJECT(nvic), "memory", OBJECT(get_system_memory()), &error_abort);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(nvic), &error_fatal);
    fprintf(stdout, "NVIC realized.\n\n");



    // Initialize UART
    uart_dev = qdev_new("pl011"); // Using Arm PL011 UART model
    uart = SYS_BUS_DEVICE(uart_dev);
    object_property_add_child(soc_container, "uart[*]", OBJECT(uart_dev));
    qdev_prop_set_chr(uart_dev, "chardev", serial_hd(0));
    sysbus_realize_and_unref(uart, &error_fatal);
    sysbus_mmio_map(uart, 0, 0x4006A000); // Base address for UART
    sysbus_connect_irq(uart, 0, qdev_get_gpio_in(nvic, 32)); // Connect to NVIC IRQ 32
    fprintf(stdout, "UART initialized and connected to NVIC.\n\n");

    // Initialize PIT Timer
    pit_timer = qdev_new("sp804"); // Using Dual-Timer model as an example
    sysbus_mmio_map(SYS_BUS_DEVICE(pit_timer), 0, 0x40037000); // Base address for PIT timer
    sysbus_realize_and_unref(SYS_BUS_DEVICE(pit_timer), &error_fatal);
    sysbus_connect_irq(SYS_BUS_DEVICE(pit_timer), 0, qdev_get_gpio_in(nvic, 48)); // Connect to NVIC IRQ 48
    fprintf(stdout, "PIT timer initialized and connected to NVIC.\n\n");



    // Load firmware into flash memory
    armv7m_load_kernel(ARM_CPU(first_cpu), ms->kernel_filename, 0, FLASH_SIZE);
    fprintf(stdout, "Firmware loaded into flash memory.\n\n");

    // Final initialization logging
    fprintf(stdout, "System initialized.\n");
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//implementation of the class init function
static void s32k3x8_example_board_class_init(ObjectClass *oc, void *data) {
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->name = g_strdup("s32k3x8evb");
    mc->desc = "NXP S32K3X8 EVB (Cortex-M7)";
    mc->init = s32k3x8_example_board_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-m7");
    mc->default_cpus = 1;
    mc->min_cpus = mc->default_cpus;
    mc->max_cpus = mc->default_cpus;
    mc->no_floppy = 1;
    mc->no_cdrom = 1;
    mc->no_parallel = 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//implementation of the machine class
//struct S32K3X8ExampleBoardMachineClass {
//    MachineClass parent_class;
//};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Section of code to define the infos of the board, wehere we do specify the functions to run for the init and class init
static const TypeInfo s32k3x8_example_board_machine_types = {
    .name           = TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE,
    .parent         = TYPE_MACHINE,
//    .class_size     = sizeof(S32K3X8ExampleBoardMachineClass),
    .class_init     = s32k3x8_example_board_class_init,
};

static const TypeInfo s32k3x8evb_sys_info = {
    .name          = TYPE_S32K3X8EVB_SYS,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(ssys_state),
};

static void s32k3x8evb_machine_init(void) {
    type_register_static(&s32k3x8_example_board_machine_types);
    type_register_static(&s32k3x8evb_sys_info);
}

type_init(s32k3x8evb_machine_init);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
