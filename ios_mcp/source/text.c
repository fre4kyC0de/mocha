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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "imports.h"
#include "../../src/dynamic_libs/os_types.h"
#include "font_bin.h"

#define	FRAMEBUFFER_ADDRESS			(0x14000000+0x38C0000)
#define	FRAMEBUFFER_STRIDE			(0xE00)
#define	FRAMEBUFFER_STRIDE_WORDS	(FRAMEBUFFER_STRIDE >> 2)

#define	CHAR_SIZE_X					(8)
#define	CHAR_SIZE_Y					(8)

static const u8 *launch_image_tga = (const u8*)0x27000000;

u32* const framebuffer = (u32*)FRAMEBUFFER_ADDRESS;

void drawSplashScreen(void)
{
	// check if it is an unmapped RGB tga
	if (*(u32*)launch_image_tga != 0x00000200)
		return;

	for (int i = 0; i < (896 * 504); i++)
	{
		u32 pixel;
		u32 pixelOffset = 0x12 + i * 2;
		// access only 4 byte aligned data as the file is in code section
		u32 dualPixel = *(u32*)(launch_image_tga + (pixelOffset & ~3));

		if ((pixelOffset & 3) == 0)
			pixel = ((dualPixel >> 24) & 0xFF) | (((dualPixel >> 16) & 0xFF) << 8);
		else
			pixel = ((dualPixel >> 8) & 0xFF) | ((dualPixel & 0xFF) << 8);

		framebuffer[i] = (((pixel >> 10) & 0x1F) << (3 + 24)) | (((pixel >> 5) & 0x1F) << (3 + 16)) | ((pixel & 0x1F) << (3 + 8)) | 0xFF;
	}
}

void clearScreen(u32 color)
{
	for (int i = 0; i < ((FRAMEBUFFER_STRIDE * 504)/4); i++)
		framebuffer[i] = color;
}

void drawCharacter(char c, int x, int y)
{
	if (c < 32)
		return;

	c -= 32;
	u8* charData = (u8*)&font_bin[(CHAR_SIZE_X * CHAR_SIZE_Y * c) / 8];
	u32* fb = &framebuffer[x + y * FRAMEBUFFER_STRIDE_WORDS];
	for (int i = 0; i < CHAR_SIZE_Y; i++)
	{
		u8 v = *(charData++);
		for (int j = 0; j < CHAR_SIZE_X; j++)
		{
			if (v & 1)
				*fb = 0x00000000;
			else
				*fb = 0xFFFFFFFF;
			v >>= 1;
			fb++;
		}
		fb += FRAMEBUFFER_STRIDE_WORDS - CHAR_SIZE_X;
	}
}

void drawString(char* str, int x, int y)
{
	if (!str)
		return;

	int dx = 0, dy = 0;
	for (int k = 0; str[k]; k++)
	{
		if ((str[k] >= 32) && (str[k] < 128))
			drawCharacter(str[k], x + dx, y + dy);

		dx += 8;

		if (str[k] == '\n')
		{
			dx = 0;
			dy -= 8;
		}
	}
}

void _printf(int x, int y, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	static char buffer[0x100];

	IOSMCP_vsnprintf(buffer, 0xFF, format, args);
	drawString(buffer, x, y);

	va_end(args);
}
