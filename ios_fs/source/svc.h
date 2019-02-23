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

#define	SHUTDOWN_TYPE_POWER_OFF		0
#define	SHUTDOWN_TYPE_REBOOT		1

typedef struct
{
	void* ptr;
	u32 len;
	u32 unk;
} iovec_s;

void* svcAlloc(u32 heapid, u32 size);
void* svcAllocAlign(u32 heapid, u32 size, u32 align);
void svcFree(u32 heapid, void* ptr);
int svcOpen(char* name, int mode);
int svcClose(int fd);
int svcIoctl(int fd, u32 request, void* input_buffer, u32 input_buffer_len, void* output_buffer, u32 output_buffer_len);
int svcIoctlv(int fd, u32 request, u32 vector_count_in, u32 vector_count_out, iovec_s* vector);
int svcInvalidateDCache(void* address, u32 size);
int svcFlushDCache(void* address, u32 size);

void svcShutdown(int shutdown_type);
int svcCustomKernelCommand(u32 command, ...);

#endif
