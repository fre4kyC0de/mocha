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

void* IOSMCPPAYLOAD_memset(void* dst, int val, size_t size)
{
	char* _dst = dst;

	for (int i = 0; i < size; i++)
		_dst[i] = val;

	return dst;
}

/*int IOSMCPPAYLOAD_strlen(const char* str)
{
	unsigned int i = 0;
	while (str[i])
		i++;
	return i;
}*/

/*int IOSMCPPAYLOAD_strncmp(const char* s1, const char* s2, size_t n)
{
	while ( n && *s1 && ( *s1 == *s2 ) ) {
		++s1;
		++s2;
		--n;
	}
	if ( n == 0 ) {
		return 0;
	} else {
		return ( *(unsigned char*)s1 - *(unsigned char*)s2 );
	}
}*/

/*// Function to implement strncat() function in C
char* IOSMCPPAYLOAD_strncat(char* destination, const char* source, size_t num)
{
	// make ptr point to the end of destination string
	char* ptr = destination + strlen(destination);

	// Appends characters of source to the destination string
	while (*source != '\0' && num--)
		*ptr++ = *source++;

	// null terminate destination string
	*ptr = '\0';

	// destination string is returned by standard strncat()
	return destination;
}*/

char* IOSMCPPAYLOAD_strncpy(char* dst, const char* src, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		dst[i] = src[i];
		if (src[i] == '\0')
			return dst;
	}

	return dst;
}
