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
#include <fcntl.h>

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "../libs/libc.h"
#include "version.h"
#include "id.h"
#include "osscreen.h"

void console_print_pos(int x, int y, const char *format, ...)
{
	char * tmp = NULL;

	va_list va;
	va_start(va, format);
	if ((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
		if (strlen(tmp) > 79)
			tmp[79] = 0;

		for (int i = 0; i <= 1; i++)
		{
			// double-buffered
			OSScreenPutFontEx(0, x, y, tmp);
			OSScreenPutFontEx(1, x, y, tmp);
			OSScreenFlipBuffersEx(0);
			OSScreenFlipBuffersEx(1);
		}
	}
	va_end(va);

	if (tmp)
		free(tmp);
}

void console_print_header()
{
	char* indent;
	int indent_counter, indent_counter_bak;
	
	indent_counter = 68; // max_length = 69 - 1 (Abstand zum Rand einhalten)
	indent_counter -= 25; // == strlen(TITLE)
	indent_counter -= strlen(APP_VERSION);
	if ((indent_counter < 0) || (indent_counter > 68))
		indent_counter = 0;

	indent_counter_bak = indent_counter;
	indent_counter /= 2;
	if ((indent_counter * 2) > indent_counter_bak) // just to be sure
		indent_counter -= 1;

	indent = (char*)malloc(35); // 34 [== 68 / 2] + 1 ('\0')
	memset(indent, '\0', 35);
	if ((indent_counter > 0) && (indent_counter < 35))
		for (int i = 0; i <= (indent_counter - 1); i++)
			indent[i] = ' ';

	console_print_pos(x_offset, 1, "%s-- MOCHA CFW %s by Dimok --", indent, APP_VERSION);
}

void console_clear()
{
	for (int i = 0; i <= 1; i++)
	{
		// double-buffered
		OSScreenClearBufferEx(0, 0);
		OSScreenClearBufferEx(1, 0);
		OSScreenFlipBuffersEx(0);
		OSScreenFlipBuffersEx(1);
	}
}
