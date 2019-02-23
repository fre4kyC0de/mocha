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

#ifndef IMPORTS_H
#define IMPORTS_H

#include <stdlib.h>
#include <stdarg.h>

#include "../../src/dynamic_libs/os_types.h"

#define	MCP_SVC_BASE						((void*)0x050567EC)

#define	MCP_usleep							((void (*)(u32 time))0x050564E4)
#define	MCP_memcpy							((void* (*)(void*, const void*, u32))0x05054E54)
#define	MCP_vsnprintf						((int (*)(char * s, u32 n, const char * format, va_list arg))0x05055C40)

#endif
