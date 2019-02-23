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

#ifndef SVC_H
#define SVC_H

#include "../../src/dynamic_libs/os_types.h"

typedef struct
{
	void* ptr;
	u32 len;
	u32 unk;
} iovec_s;

#define	svcAlloc			((void *(*)(u32 heapid, u32 size))0x081234E4)
#define	svcAllocAlign		((void *(*)(u32 heapid, u32 size, u32 align))0x08123464)
#define	svcFree				((void *(*)(u32 heapid, void *ptr))0x08123830)
#define	svcOpen				((int (*)(const char* name, int mode))0x0812940C)
#define	svcClose			((int (*)(int fd))0x08129368)
#define	svcIoctl			((int (*)(int fd, u32 request, void* input_buffer, u32 input_buffer_len, void* output_buffer, u32 output_buffer_len))0x081290E0)
#define	svcIoctlv			((int (*)(int fd, u32 request, u32 vector_count_in, u32 vector_count_out, iovec_s* vector))0x0812903C)

#endif
