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
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <stdio.h>

#include "dynamic_libs/os_types.h"
#include "dumper.h"
#include "libiosuhax/iosuhax.h"

void DoMemoryDump(uint32_t address, uint32_t size, char* target)
{
	// IOS-USB
	// Offset:	0x10100000
	// Size:	0x3D0000
	
	unsigned char* myDump = malloc(size);
	
	int iosuhaxFd = IOSUHAX_Open("/dev/iosuhax");
	
	if (iosuhaxFd == -1) {
		return;
	}
	
	FILE *pFile = fopen(target, "wb");
	if (!pFile) {
		IOSUHAX_Close();
		return;
	}
	
	IOSUHAX_memread(address, myDump, size);
	
	//fputs(myDump, pFile);
	fwrite(myDump, size, 1, pFile);

	fclose(pFile);
	
	IOSUHAX_Close();
	
}
