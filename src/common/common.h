/***************************************************************************
 * Copyright (C) 2015
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/

#ifndef COMMON_H
#define	COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dynamic_libs/os_defs.h"

#define	CAFE_OS_SD_PATH				"/vol/external01"
#define	SD_PATH						"sd:"
#define	WIIU_PATH					"/wiiu"

/* Macros for libs */
#define	LIB_CORE_INIT				0
#define	LIB_NSYSNET					1
#define	LIB_GX2						2
#define	LIB_AOC						3
#define	LIB_AX						4
#define	LIB_FS						5
#define	LIB_OS						6
#define	LIB_PADSCORE				7
#define	LIB_SOCKET					8
#define	LIB_SYS						9
#define	LIB_VPAD					10
#define	LIB_NN_ACP					11
#define	LIB_SYSHID					12
#define	LIB_VPADBASE				13
#define	LIB_AX_OLD					14
#define	LIB_PROC_UI					15
#define	LIB_NTAG					16
#define	LIB_NFP						17
#define	LIB_SAVE					18
#define	LIB_ACT						19
#define	LIB_NIM						20

// functions types
#define	STATIC_FUNCTION				0
#define	DYNAMIC_FUNCTION			1

// none dynamic libs
#define	LIB_LOADER					0x1001

#ifndef	MEM_BASE
#define	MEM_BASE					(0x00800000)
#endif

#define	ELF_DATA_ADDR				(*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x00))
#define	ELF_DATA_SIZE				(*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x04))
#define	MAIN_ENTRY_ADDR				(*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x00))

#ifndef	EXIT_SUCCESS
#define	EXIT_SUCCESS				0
#endif
#define	EXIT_HBL_EXIT				0xFFFFFFFE
#define	EXIT_RELAUNCH_ON_LOAD		0xFFFFFFFD

#ifdef __cplusplus
}
#endif

#endif	/* COMMON_H */
