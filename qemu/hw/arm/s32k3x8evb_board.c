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

// Define constants for memory regions
#define FLASH_BASE_ADDR  0x00000000  // Adjust based on datasheet specifics
#define FLASH_SIZE       0x01000000  // Example: 16 MB for maximum flash size

#define DFLASH_BASE_ADDR 0x10000000
#define DFLASH_SIZE      0x00040000  // Example: 256 KB

#define SRAM_BASE_ADDR   0x20000000
#define SRAM_SIZE        0x00240000  // Example: 2.25 MB

#define TYPE_S32K3X8EVB_SYS "s32k3x8evb-sys"
OBJECT_DECLARE_SIMPLE_TYPE(ssys_state, S32K3X8EVB_SYS)

typedef struct S32K3X8ExampleBoardMachineClass S32K3X8ExampleBoardMachineClass;

void s32k3x8_initialize_memory_regions(MemoryRegion *system_memory);


struct ssys_state {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    uint32_t rcc;
    uint32_t rcc2;
    qemu_irq irq;
    Clock *sysclk;
};

int load_image_targphys(const char *filename, hwaddr addr, hwaddr size);
void error_report(const char *fmt, ...);
void s32k3x8_load_firmware(ARMCPU *cpu, MachineState *ms, MemoryRegion *flash, const char *firmware_filename);

struct S32K3X8ExampleBoardMachineState {
    MachineState parent_obj;
    ssys_state sys;
    ARMv7MState nvic;
};

typedef struct S32K3X8ExampleBoardMachineState S32K3X8ExampleBoardMachineState;

#define TYPE_S32K3X8_EXAMPLE_BOARD_BASE_MACHINE MACHINE_TYPE_NAME("s32k3x8")
#define TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE MACHINE_TYPE_NAME("s32k3x8-example-board")

DECLARE_INSTANCE_CHECKER(S32K3X8ExampleBoardMachineState, S32K3X8_EXAMPLE_BOARD_MACHINE, TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE)

void s32k3x8_load_firmware(ARMCPU *cpu, MachineState *ms, MemoryRegion *flash, const char *firmware_filename) {
    int ret;
    hwaddr flash_size = memory_region_size(flash);

    if (!firmware_filename) {
        error_report("No firmware specified");
        exit(1);
    }

    ret = load_image_targphys(firmware_filename, FLASH_BASE_ADDR, flash_size);
    if (ret < 0) {
        error_report("Failed to load firmware image '%s'", firmware_filename);
        exit(1);
    }

    cpu_reset(CPU(cpu));
}

void s32k3x8_initialize_memory_regions(MemoryRegion *system_memory) {
    MemoryRegion *flash = g_new(MemoryRegion, 1);
    MemoryRegion *dflash = g_new(MemoryRegion, 1);
    MemoryRegion *sram = g_new(MemoryRegion, 1);

    memory_region_init_rom(flash, NULL, "s32k3x8.flash", FLASH_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BASE_ADDR, flash);

    memory_region_init_ram(dflash, NULL, "s32k3x8.dflash", DFLASH_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, DFLASH_BASE_ADDR, dflash);

    memory_region_init_ram(sram, NULL, "s32k3x8.sram", SRAM_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, SRAM_BASE_ADDR, sram);
}

static void s32k3x8_example_board_init(MachineState *machine) {
    S32K3X8ExampleBoardMachineState *m_state = S32K3X8_EXAMPLE_BOARD_MACHINE(machine);
    MemoryRegion *system_memory = get_system_memory();

    s32k3x8_initialize_memory_regions(system_memory);

    object_initialize_child(OBJECT(machine), "sys", &m_state->sys, TYPE_S32K3X8EVB_SYS);
    sysbus_realize(SYS_BUS_DEVICE(&m_state->sys), &error_abort);

    object_initialize_child(OBJECT(machine), "nvic", &m_state->nvic, TYPE_ARMV7M);
    qdev_prop_set_uint32(DEVICE(&m_state->nvic), "num-irq", 64);
    qdev_prop_set_uint8(DEVICE(&m_state->nvic), "num-prio-bits", 3);
    qdev_prop_set_string(DEVICE(&m_state->nvic), "cpu-type", "cortex-m7");
    qdev_prop_set_bit(DEVICE(&m_state->nvic), "enable-bitband", true);

    m_state->sys.sysclk = clock_new(OBJECT(DEVICE(&m_state->sys)), "sysclk");
    clock_set_ns(m_state->sys.sysclk, 40.69);
    qdev_connect_clock_in(DEVICE(&m_state->nvic), "cpuclk", m_state->sys.sysclk);

    object_property_set_link(OBJECT(&m_state->nvic), "memory", OBJECT(system_memory), &error_abort);
    sysbus_realize(SYS_BUS_DEVICE(&m_state->nvic), &error_abort);

    sysbus_mmio_map(SYS_BUS_DEVICE(&m_state->sys), 0, 0x400fe000);
    sysbus_connect_irq(SYS_BUS_DEVICE(&m_state->sys), 0, qdev_get_gpio_in(DEVICE(&m_state->nvic), 28));
}

static void s32k3x8_example_board_class_init(ObjectClass *oc, void *data) {
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->name = g_strdup("S32K3X8EVB");
    mc->desc = "S32K3X8EVB Example Board";
    mc->alias = "s32k3x8-example-board";
    mc->init = s32k3x8_example_board_init;
    mc->default_cpus = 1;
    mc->min_cpus = mc->default_cpus;
    mc->max_cpus = mc->default_cpus;
    mc->no_floppy = 1;
    mc->no_cdrom = 1;
    mc->no_parallel = 1;
}

struct S32K3X8ExampleBoardMachineClass {
    MachineClass parent_class;
};

static const TypeInfo s32k3x8_example_board_machine_types = {
    .name           = TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE,
    .parent         = TYPE_MACHINE,
    .instance_size  = sizeof(S32K3X8ExampleBoardMachineState),
    .instance_init  = (void (*)(Object *))s32k3x8_example_board_init,
    .class_size     = sizeof(S32K3X8ExampleBoardMachineClass),
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
