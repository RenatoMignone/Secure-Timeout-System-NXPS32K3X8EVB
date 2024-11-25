#ifndef HW_ARM_S32K3X8_MCU_H
#define HW_ARM_S32K3X8_MCU_H

#include "target/arm/cpu.h" // Include Cortex-M7 CPU definitions
#include "qom/object.h"
#include "hw/sysbus.h"

#define TYPE_S32K3X8_MCU "S32K3X8EVB"       // Microcontroller (MCU) type
#define TYPE_S32K3X8_SOC "S32K3X8EVB-SOC"   // System-on-chip (SoC) type

typedef struct S32K3X8MCUState S32K3X8MCUState;

DECLARE_INSTANCE_CHECKER(S32K3X8MCUState, S32K3X8_MCU, TYPE_S32K3X8_MCU)

// MCU state definition
struct S32K3X8MCUState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    // Reference to the Cortex-M7 CPU
    ARMCPU cpu;

    // Flash memory for firmware
    MemoryRegion flash;

    // RAM memory region
    MemoryRegion ram;
};

#endif // HW_ARM_S32K3X8_MCU_H

