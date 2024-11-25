#include "s32k3x8evb_mcu.h"
#include "s32k3x8evb_boot.h"

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/core/split-irq.h"
#include "hw/sysbus.h"
#include "hw/sd/sd.h"
#include "hw/ssi/ssi.h"
#include "qemu/timer.h"
#include "hw/i2c/i2c.h"
#include "net/net.h"
#include "hw/boards.h"
#include "qemu/log.h"
#include "exec/address-spaces.h"
#include "sysemu/sysemu.h"
#include "hw/arm/armv7m.h"
#include "hw/char/pl011.h"
#include "hw/input/stellaris_gamepad.h"
#include "hw/irq.h"
#include "hw/watchdog/cmsdk-apb-watchdog.h"
#include "migration/vmstate.h"
#include "hw/misc/unimp.h"
#include "hw/timer/stellaris-gptm.h"
#include "hw/qdev-clock.h"
#include "qom/object.h"
#include "qapi/qmp/qlist.h"
#include "ui/input.h"

struct S32K3X8ExampleBoardMachineState {
    MachineState parent_obj;
    S32K3X8MCUState mcu;
};

typedef struct S32K3X8ExampleBoardMachineState S32K3X8ExampleBoardMachineState;

struct S32K3X8ExampleBoardMachineClass {
    MachineClass parent_class;
};

#define TYPE_S32K3X8_EXAMPLE_BOARD_BASE_MACHINE MACHINE_TYPE_NAME("s32k3x8")
#define TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE MACHINE_TYPE_NAME("s32k3x8-example-board")

DECLARE_INSTANCE_CHECKER(S32K3X8ExampleBoardMachineState, S32K3X8_EXAMPLE_BOARD_MACHINE, TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE)

// The generic MachineState is passed by QEMU
static void s32k3x8_example_board_init(MachineState *machine)
{
    // Create a specific MachineState instance from the generic one
    S32K3X8ExampleBoardMachineState *m_state = S32K3X8_EXAMPLE_BOARD_MACHINE(machine);

    // Initialize the microcontroller that is part of the board
    object_initialize_child(OBJECT(machine), "mcu", &m_state->mcu, TYPE_S32K3X8_SOC);
    // Connect it via QEMU's SYSBUS
    sysbus_realize(SYS_BUS_DEVICE(&m_state->mcu), &error_abort);

    // Load the firmware file using the boot function
    if (machine->firmware) {
        if (!s32k3x8_load_firmware(&m_state->mcu.cpu, machine,
                                   &m_state->mcu.flash, machine->firmware)) {
            exit(1);
        }
    }
}

// Class initialization function
static void s32k3x8_example_board_class_init(ObjectClass *oc, void *data)
{
    // Generic MachineClass object
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->name = "S32K3X8EVB";
    mc->desc = "S32K3X8EVB Example Board";
    mc->alias = "s32k3x8Eevbexample-board";

    // Set the initialization function for this board
    mc->init = s32k3x8_example_board_init;

    // Specify the CPU and board constraints
    mc->default_cpus = 1;  // Single Cortex-M7 CPU
    mc->min_cpus = mc->default_cpus;
    mc->max_cpus = mc->default_cpus;

    // Disable unnecessary components for this board
    mc->no_floppy = 1;
    mc->no_cdrom = 1;
    mc->no_parallel = 1;
}

// Register the board type with QEMU
static const TypeInfo s32k3x8_example_board_machine_types = {
    {
        .name           = TYPE_S32K3X8_EXAMPLE_BOARD_MACHINE,
        .parent         = TYPE_MACHINE,  // Direct child of the generic Machine class
        .instance_size  = sizeof(S32K3X8ExampleBoardMachineState),
        .instance_init  = s32k3x8_example_board_init,  // Register instance initialization
        .class_size     = sizeof(S32K3X8ExampleBoardMachineClass),
        .class_init     = s32k3x8_example_board_class_init,  // Register class initialization
    }
};

static void s32k3x8evb_machine_init(void)
{
    type_register_static(&s32k3x8_example_board_machine_types);
}

type_init(s32k3x8evb_machine_init);


//DEFINE_TYPES(s32k3x8_example_board_machine_types)
//DEFINE_MACHINE("S32K3X8", s32k3x8_example_board_class_init)
armv7m