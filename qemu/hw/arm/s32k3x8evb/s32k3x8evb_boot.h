#ifndef HW_ARM_S32K3X8_BOOT_H
#define HW_ARM_S32K3X8_BOOT_H

#include "hw/boards.h"
#include "target/arm/cpu.h"

bool s32k3x8_load_firmware(ARMCPU *cpu, MachineState *ms,
                           MemoryRegion *mr, const char *firmware);

#endif // HW_ARM_S32K3X8_BOOT_H