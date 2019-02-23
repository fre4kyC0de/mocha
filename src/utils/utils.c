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
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>

#include "utils.h"
#include "logger.h"

// https://gist.github.com/ccbrown/9722406
void dumpHex(const void* data, size_t size)
{
	char ascii[17];
	ascii[16] = '\0';
	DEBUG_FUNCTION_LINE("0x%08X (0x0000): ", data);
	for (size_t i = 0; i < size; ++i)
	{
		log_printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~')
			ascii[i % 16] = ((unsigned char*)data)[i];
		else
			ascii[i % 16] = '.';
		if ((((i + 1) % 8) == 0) || ((i + 1) == size))
		{
			log_printf(" ");
			if (((i + 1) % 16) == 0)
			{
				log_printf("|  %s \n", ascii);
				if ((i + 1) < size) {
					DEBUG_FUNCTION_LINE("0x%08X (0x%04X); ", (data + i + 1), (i + 1));
				}
			}
			else if ((i + 1) == size)
			{
				ascii[(i + 1) % 16] = '\0';
				if (((i + 1) % 16) <= 8) {
					log_printf(" ");
				}
				for (size_t j = (i + 1) % 16; j < 16; ++j) {
					log_printf("   ");
				}
				log_printf("|  %s \n", ascii);
			}
		}
	}
}
