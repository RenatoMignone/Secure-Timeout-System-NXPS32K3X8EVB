/*---------------------------------------------------------
 * port_mpu.c - Cortex-M7 MPU Port Layer for FreeRTOS
 *--------------------------------------------------------*/

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

/* Cortex-M7 MPU Register Addresses (CMSIS standard) */
#define MPU_TYPE         (*(volatile uint32_t*)0xE000ED90)
#define MPU_CTRL         (*(volatile uint32_t*)0xE000ED94)
#define MPU_RNR          (*(volatile uint32_t*)0xE000ED98)
#define MPU_RBAR         (*(volatile uint32_t*)0xE000ED9C)
#define MPU_RASR         (*(volatile uint32_t*)0xE000EDA0)

/* MPU Region Numbers */
#define MAIN_FLASH_REGION    0
#define SRAM_REGION          1
#define PERIPHERALS_REGION   2

/* MPU Settings */
#define MPU_ENABLE              (1UL << 0)
#define MPU_PRIVILEGED_DEFAULT  (1UL << 2)

/*---------------------------------------------------------------------------*/

void vPortSetupMPU(void) {
    /* Disable MPU during configuration */
    MPU_CTRL = 0;

    /*---------------------------------------------------------
     * Region 0: Flash Memory (Execute + Read-Only)
     * Base: 0x00400000 (PFLASH), Size: 8MB
     *--------------------------------------------------------*/
    MPU_RNR = MAIN_FLASH_REGION;
    MPU_RBAR = 0x00400000 | (MAIN_FLASH_REGION << 8) | (1UL << 4);  // VALID=1
    MPU_RASR = (0x16UL << 1)  |  // SIZE=8MB (log2(8M)-1=23 → TEX=0, S=1, C=1, B=0)
               (0x03UL << 24) |  // XN=0, AP=011 (Privileged RW, Unprivileged RO)
               (1UL << 0);       // ENABLE

               

    /*---------------------------------------------------------
     * Region 1: DTCM/SRAM (Full Access)
     * Base: 0x20000000 (DTCM0), Size: 128KB
     *--------------------------------------------------------*/
    MPU_RNR = SRAM_REGION;
    MPU_RBAR = 0x20000000 | (SRAM_REGION << 8) | (1UL << 4);  // VALID=1
    MPU_RASR = (0x10UL << 1)  |  // SIZE=128KB (log2(128K)-1=17)
               (0x03UL << 24) |  // AP=011 (RW for privileged, RO for unprivileged)
               (1UL << 0);       // ENABLE

    /*---------------------------------------------------------
     * Region 2: Peripherals (Privileged Access Only)
     * Base: 0x40000000, Size: 512MB
     *--------------------------------------------------------*/
    MPU_RNR = PERIPHERALS_REGION;
    MPU_RBAR = 0x40000000 | (PERIPHERALS_REGION << 8) | (1UL << 4);  // VALID=1
    MPU_RASR = (0x1CUL << 1)  |  // SIZE=512MB (log2(512M)-1=28)
               (0x01UL << 24) |  // AP=001 (Privileged RW only)
               (1UL << 0);       // ENABLE

    /* Enable MPU with default privileged access */
    MPU_CTRL = MPU_ENABLE | MPU_PRIVILEGED_DEFAULT;
    __asm volatile("dsb");
    __asm volatile("isb");
}


/*---------------------------------------------------------------------------*/

/* Enable MPU in privileged mode before starting scheduler */
void vPortEnterPrivileged(void) {
    __asm volatile("mrs r0, control  \n"
                   "bic r0, #1       \n"
                   "msr control, r0  \n"
                   "dsb             \n"
                   "isb             \n");
}
