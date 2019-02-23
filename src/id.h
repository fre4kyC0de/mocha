/***************************************************************************
 * Copyright (C) 2016
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

#ifndef _ID_H_
#define _ID_H_

#include "dynamic_libs/os_types.h"

u64 currentTitleId;
u32 currentTilteId_low;
u32 currentTilteId_high;

u64 systemTitleId;
u32 systemTitleId_low;
u32 systemTitleId_high;

#define TitleId_SysMenuEUR	0x0005001010040200ULL
#define TitleId_SysMenuUSA	0x0005001010040100ULL
#define TitleId_SysMenuJPN	0x0005001010040000ULL

#define	TitleId_MiiMakerEUR	0x000500101004A200ULL
#define	TitleId_MiiMakerUSA	0x000500101004A100ULL
#define	TitleId_MiiMakerJPN	0x000500101004A000ULL
#define	TitleId_HBLChannel	0x0005000013374842ULL

#endif
