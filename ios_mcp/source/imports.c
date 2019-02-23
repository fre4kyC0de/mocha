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

#include "imports.h"

void* memset(void* dst, int val, size_t size)
{
	char* _dst = dst;

	for (int i = 0; i < size; i++)
		_dst[i] = val;

	return dst;
}

int strlen(const char* str)
{
	unsigned int i = 0;
	while (str[i])
		i++;
	return i;
}

char* strncpy(char* dst, const char* src, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		dst[i] = src[i];
		if (src[i] == '\0')
			return dst;
	}

	return dst;
}
