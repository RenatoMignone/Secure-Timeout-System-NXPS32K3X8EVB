//#include "qemu/osdep.h"
//#include "hw/sysbus.h"
//#include "hw/arm/arm.h"
//#include "hw/irq.h"
//#include "qemu/log.h"
//#include "qemu/module.h"
//
///* Define the state of the S32K3 MCU */
//typedef struct {
//    /* Base device state */
//    ARMCPU parent_obj;
//    /* Memory regions */
//    MemoryRegion flash;
//    MemoryRegion ram;
//    /* Peripheral state */
//    uint32_t uart_status;
//    /* IRQs */
//    qemu_irq irq_lines[16];
//} S32K3State;
//
///* Reset function */
//static void s32k3_reset(DeviceState *dev)
//{
//    S32K3State *s = (S32K3State *)dev;
//    /* Reset MCU state */
//    s->uart_status = 0;
//    /* Reset other peripherals as needed */
//}
//
///* Initialize function */
//static void s32k3_init(Object *obj)
//{
//    S32K3State *s = (S32K3State *)obj;
//
//    /* Initialize flash memory */
//    memory_region_init_ram(&s->flash, obj, "s32k3.flash", 0x100000, &error_fatal);
//    memory_region_add_subregion(get_system_memory(), 0x00000000, &s->flash);
//
//    /* Initialize RAM */
//    memory_region_init_ram(&s->ram, obj, "s32k3.ram", 0x20000, &error_fatal);
//    memory_region_add_subregion(get_system_memory(), 0x20000000, &s->ram);
//
//    /* Initialize peripherals */
//    /* Example: UART */
//    s->uart_status = 0;
//}
//
///* Type information */
//static const TypeInfo s32k3_info = {
//    .name          = TYPE_S32K3,
//    .parent        = TYPE_ARM_CPU,
//    .instance_size = sizeof(S32K3State),
//    .instance_init = s32k3_init,
//    .class_init    = NULL,
//};
//
//static void s32k3_register_types(void)
//{
//    type_register_static(&s32k3_info);
//}
//
//type_init(s32k3_register_types);

#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "hw/arm/arm.h"
#include "hw/irq.h"
#include "qemu/log.h"
#include "qemu/module.h"

/* Define the state of the S32K3 MCU */
typedef struct {
    /* Base device state */
    ARMCPU parent_obj;
    /* Memory regions */
    MemoryRegion flash;
    MemoryRegion ram;
    /* Peripheral state */
    uint32_t uart_status;
    /* IRQs */
    qemu_irq irq_lines[16];
} S32K3State;

/* Reset function */
static void s32k3_reset(DeviceState *dev)
{
    S32K3State *s = (S32K3State *)dev;
    /* Reset MCU state */
    s->uart_status = 0;
    /* Reset other peripherals as needed */
}

/* Initialize function */
static void s32k3_init(Object *obj)
{
    S32K3State *s = (S32K3State *)obj;

    /* Initialize flash memory */
    memory_region_init_ram(&s->flash, obj, "s32k3.flash", 0x100000, &error_fatal);
    memory_region_add_subregion(get_system_memory(), 0x00000000, &s->flash);

    /* Initialize RAM */
    memory_region_init_ram(&s->ram, obj, "s32k3.ram", 0x20000, &error_fatal);
    memory_region_add_subregion(get_system_memory(), 0x20000000, &s->ram);

    /* Initialize peripherals */
    /* Example: UART */
    s->uart_status = 0;
}

/* Type information */
static const TypeInfo s32k3_info = {
    .name          = TYPE_S32K3,
    .parent        = TYPE_ARM_CPU,
    .instance_size = sizeof(S32K3State),
    .instance_init = s32k3_init,
    .class_init    = NULL,
};

static void s32k3_register_types(void)
{
    type_register_static(&s32k3_info);
}

type_init(s32k3_register_types);

