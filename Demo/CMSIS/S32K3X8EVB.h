/*
* Copyright (c) 2023 Your Company
* SPDX-License-Identifier: BSD-3-Clause
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************
* File:     S32K3X8EVB.h
* Release:  Version 1.0
*******************************************************************************/

#ifndef __S32K3X8EVB_H
#define __S32K3X8EVB_H

#include "CMSDK_CM7.h"                         /* Device specific header file    */
#include "core_cm7.h"                          /* Core specific header file      */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/******************************************************************************/
/*                          Timer Register declaration                        */
/******************************************************************************/

typedef struct
{
  __IO uint32_t CTRL;             // Offset: 0x000 (R/W)  Control Register
  __IO uint32_t VALUE;            // Offset: 0x004 (R/W)  Current Value Register
  __IO uint32_t RELOAD;           // Offset: 0x008 (R/W)  Reload Value Register
  __IO uint32_t INTCLR;           // Offset: 0x00C ( /W)  Interrupt Clear Register
  __I  uint32_t RIS;              // Offset: 0x010 (R/ )  Raw Interrupt Status Register
  __I  uint32_t MIS;              // Offset: 0x014 (R/ )  Masked Interrupt Status Register
  __IO uint32_t BGLOAD;           // Offset: 0x018 (R/W)  Background Load Register
} S32K3X8_TIMER_TypeDef;

/******************************************************************************/
/*                          Peripheral memory map                             */
/******************************************************************************/
#define S32K3X8_TIMER0_BASE       (0x40037000UL)  // Timer 0 base address
#define S32K3X8_TIMER1_BASE       (0x40038000UL)  // Timer 1 base address

/******************************************************************************/
/*                          Peripheral declaration                            */
/******************************************************************************/
#define S32K3X8_TIMER0            ((S32K3X8_TIMER_TypeDef *) S32K3X8_TIMER0_BASE)
#define S32K3X8_TIMER1            ((S32K3X8_TIMER_TypeDef *) S32K3X8_TIMER1_BASE)

/******************************************************************************/
/*                          Timer Control Register Definitions                */
/******************************************************************************/
#define TIMER_CTRL_ENABLE_Pos     0
#define TIMER_CTRL_ENABLE_Msk     (1UL << TIMER_CTRL_ENABLE_Pos)

#define TIMER_CTRL_MODE_Pos       1
#define TIMER_CTRL_MODE_Msk       (1UL << TIMER_CTRL_MODE_Pos)

#define TIMER_CTRL_INTEN_Pos      2
#define TIMER_CTRL_INTEN_Msk      (1UL << TIMER_CTRL_INTEN_Pos)

/******************************************************************************/
/*                          Timer Interrupt Clear Register Definitions        */
/******************************************************************************/
#define TIMER_INTCLR_Pos          0
#define TIMER_INTCLR_Msk          (1UL << TIMER_INTCLR_Pos)

/******************************************************************************/
/*                          Timer Raw Interrupt Status Register Definitions   */
/******************************************************************************/
#define TIMER_RIS_Pos             0
#define TIMER_RIS_Msk             (1UL << TIMER_RIS_Pos)

/******************************************************************************/
/*                          Timer Masked Interrupt Status Register Definitions*/
/******************************************************************************/
#define TIMER_MIS_Pos             0
#define TIMER_MIS_Msk             (1UL << TIMER_MIS_Pos)

#endif /* __S32K3X8EVB_H */
