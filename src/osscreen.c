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

#include <stdio_addition.h>

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "version.h"
#include "osscreen.h"

void _console_print_pos(int x, int y, const char*format, ...);
void _console_print_header();
void _console_clear();
void _flush_log();

#define		console_x_offset		-2
#define		console_y_offset		0
#define		console_width			68	// -2 to 66 [was 79 in vanilla mocha code]
#define		console_height			17	// 0 to 16
#define		log_offset				2


int console_initialized = 0;

int current_log_position = 0;

u8 * screenBuffer;

char* log_content[console_height - log_offset];

void console_init()
{
	if (console_initialized > 0)
		return;

	OSScreenInit();
	u32 screen_buf0_size = OSScreenGetBufferSizeEx(0);
	u32 screen_buf1_size = OSScreenGetBufferSizeEx(1);
	screenBuffer = (u8*) memalign(0x100, screen_buf0_size + screen_buf1_size);
	OSScreenSetBufferEx(0, (void *)screenBuffer);
	OSScreenSetBufferEx(1, (void *)(screenBuffer + screen_buf0_size));
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);

	_console_clear();
	_flush_log();

	console_initialized = 1;

	_console_print_header();
}

void console_deinit()
{
	if (console_initialized == 0)
		return;

	console_initialized = 0;

	_flush_log(); // free "log_content[*]"

	OSScreenShutdown();
	free(screenBuffer);
	screenBuffer = NULL;
}

void console_print_line(const char*format, ...)
{
	if (console_initialized == 0)
		return;

	char* tmp = NULL;
	int tmp2_size = 0;
	char* tmp2 = NULL;

	va_list va;
	va_start(va, format);
	if ((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
		if (strlen(tmp) > console_width)
		{
			tmp2_size = strlen(tmp) - console_width;
			if (tmp2_size > 0) {
				tmp2 = (char*)malloc(tmp2_size + 1);
				memset(tmp2, '\0', (tmp2_size + 1));
				memcpy(tmp2, ((char*)(tmp + (console_width * sizeof(char)))), tmp2_size);
			}
			tmp[(console_width - 1) + 1] = '\0';
		}

		if ((log_offset + current_log_position) <= (console_height - 1)) {
			_console_print_pos(0, (log_offset + current_log_position), "%s", tmp);
			log_content[current_log_position] = tmp;
			current_log_position += 1;
		} else {
			// shift upwards
			if (log_content[0] != NULL)
				free(log_content[0]);
			for (int idx = 0; idx <= (((console_height - log_offset) - 1) - 1); idx++)
				log_content[idx] = log_content[idx + 1];

			// add new content
			log_content[(console_height - log_offset) - 1] = tmp;

			// print to screen
			_console_clear();
			_console_print_header();
			for (int idx = 0; idx <= ((console_height - log_offset) - 1); idx++)
				if (log_content[idx] != NULL)
					_console_print_pos(0, (log_offset + idx), "%s", log_content[idx]);
		}
	}
	va_end(va);
	
	if ((tmp2_size > 0) && (tmp2 != NULL))
	{
		console_print_line("%s", tmp2);
		free(tmp2);
	}
}

void _console_print_pos(int x, int y, const char*format, ...)
{
	if (console_initialized == 0)
		return;

	if ((x < 0) || (x > (console_width - 1)))
		return;
	if ((y < 0) || (y > (console_height - 1)))
		return;

	char* tmp = NULL;

	va_list va;
	va_start(va, format);
	if ((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
		if ((x + strlen(tmp)) > console_width)
			tmp[((console_width - 1) - x) + 1] = '\0';

		for (int i = 0; i <= 1; i++)
		{
			// double-buffered
			OSScreenPutFontEx(0, (console_x_offset + x), (console_y_offset + y), tmp);
			OSScreenPutFontEx(1, (console_x_offset + x), (console_y_offset + y), tmp);
			OSScreenFlipBuffersEx(0);
			OSScreenFlipBuffersEx(1);
		}
	}
	va_end(va);

	if (tmp)
		free(tmp);
}

void _console_print_header()
{
	if (console_initialized == 0)
		return;

	char* strHeader = "%s-- MOCHA CFW %s for 5.5.0 - 5.5.4 by Dimok --";

	char* indent;
	int indent_counter, indent_counter_bak;

	indent_counter = (console_width - 1); // max_length = 68 - 1 (Abstand zum Rand einhalten)
	indent_counter -= (strlen(strHeader) - (2 * strlen("%s"))); // == strlen(TITLE)
	indent_counter -= strlen(APP_VERSION);
	if ((indent_counter < 0) || (indent_counter > console_width))
		indent_counter = 0;

	indent_counter_bak = indent_counter;
	indent_counter /= 2;
	if ((indent_counter * 2) > indent_counter_bak) // just to be sure
		indent_counter -= 1;

	indent = (char*)malloc(indent_counter + 1);
	memset(indent, '\0', (indent_counter + 1));
	if ((indent_counter > 0) && ((2 * indent_counter) < console_width))
		for (int i = 0; i <= (indent_counter - 1); i++)
			indent[i] = ' ';

	_console_print_pos(0, 0, strHeader, indent, APP_VERSION);
}

void _console_clear()
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
void _flush_log() {
	current_log_position = 0;
	for (int idx = 0; idx <= ((console_height - log_offset) - 1); idx++)
	{
		if (log_content[idx] != NULL) {
			free(log_content[idx]);
			log_content[idx] = NULL;
		}
	}
}
void console_clear()
{
	if (console_initialized == 0)
		return;

	_console_clear();

	_flush_log();

	_console_print_header();
}
