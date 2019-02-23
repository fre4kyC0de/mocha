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

#include <string.h>
#include <stdio.h>

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/nn_nim_functions.h"
#include "nnu.h"

void nnupatcher(void)
{
	u32 *targetfunction, *patchoffset;

	// locate function
	OSDynLoad_FindExport(nn_nim_handle, 0, "NeedsNetworkUpdate__Q2_2nn3nimFPb", &targetfunction);

	patchoffset = OSEffectiveToPhysical(targetfunction);
	patchoffset = (u32*)((u32)patchoffset - 0x31000000 + 0xA0000000);

	// now patch
	patchoffset[0] = 0x38600000;
	patchoffset[1] = 0x38800000;
	patchoffset[2] = 0x4E800020;
}
