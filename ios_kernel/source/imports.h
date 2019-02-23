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

#ifndef IMPORTS_H_
#define IMPORTS_H_

#define	IOSKERNEL_vsnprintf							((int (*)(char * s, u32 n, const char * format, va_list arg))0x0813293C)
#define	IOSKERNEL_memcpy							((void * (*)(void*, const void*, int))0x08131D04)
#define	IOSKERNEL_memset							((void *(*)(void*, int, unsigned int))0x08131DA0)
#define	IOSKERNEL_strncpy							((char *(*)(char*, const char*, unsigned int))0x081329B8)
#define	IOSKERNEL_disable_interrupts				((int(*)())0x0812E778)
#define	IOSKERNEL_enable_interrupts					((int(*)(int))0x0812E78C)
#define	IOSKERNEL_bsp_command_5						((int (*)(const char*, int offset, const char*, int size, void *buffer))0x0812EC40)

#endif // IMPORTS_H_
