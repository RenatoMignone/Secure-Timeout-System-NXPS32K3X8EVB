#include "qemu/osdep.h"
#include "qemu/datadir.h"
#include "hw/loader.h"
#include "hw/arm/s32k3x8evb_boot.h"
#include "qemu/error-report.h"

bool s32k3x8_load_firmware(ARMCPU *cpu, MachineState *ms,
                           MemoryRegion *program_mr, const char *firmware)
{
    g_autofree char *filename = NULL;
    int bytes_loaded;

    // Locate the firmware file specified as 'bios' during QEMU startup
    filename = qemu_find_file(QEMU_FILE_TYPE_BIOS, firmware);
    if (filename == NULL) {
        error_report("Cannot find firmware image '%s'", firmware);
        return false;
    }

    // Load the firmware image into the specified memory region
    bytes_loaded = load_image_mr(filename, program_mr);

    if (bytes_loaded < 0) {
        error_report("Unable to load firmware image %s as ELF or raw binary",
                     firmware);
        return false;
    }

    // Ensure the CPU starts executing from the vector table base address
    cpu->env.v7m.pc = 0x00000000; // Typical ARM Cortex-M vector table base

    return true;
}
