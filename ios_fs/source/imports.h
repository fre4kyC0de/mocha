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

#define	IOSFS_ios_shutdown					((void (*)(int))0x107F6C94)

#define	IOSFS_svcCreateMutex				((int (*)(int, int))0x107F6BBC)
#define	IOSFS_svcAcquireMutex				((int (*)(int, int))0x107F6BC4)
#define	IOSFS_svcReleaseMutex				((int (*)(int))0x107F6BCC)
#define	IOSFS_svcDestroyMutex				((int (*)(int))0x107F6BD4)

#define	IOSFS_sleep							((void (*)(int))0x1071D668)
#define	IOSFS_memcmp						((int (*)(const void*, const void*, u32))0x107F4E94)
#define	IOSFS_memcpy						((void* (*)(void*, const void*, u32))0x107F4F7C)
#define	IOSFS_memset						((void* (*)(void*, int, u32))0x107F5018)
#define	IOSFS_vsnprintf						((int (*)(char * s, u32 n, const char * format, va_list arg))0x107F5F68)
#define	IOSFS_snprintf						((int (*)(char * s, u32 n, const char * format, ...))0x107F5FB4)
#define	IOSFS_strncmp						((int (*)(const char *s1, const char *s2, u32 size))0x107F6138)
#define	IOSFS_strncpy						((char* (*)(char *s1, const char *s2, u32 size))0x107F60DC)
#define	IOSFS_strlen_s						((int (*)(const char*, u32))0x107F61BC)
#define	IOSFS_strlen(x)						IOSFS_strlen_s(x, 0x1000)
#define	IOSFS_syslog_output					((void (*)(const char *format, ...))0x107F0C84)
#define	IOSFS_Raw_Read1						((int (*)(int handle, u32 offset_high, u32 offset_low, u32 size, void* buf, void *callback, int callback_arg))0x10732BC0)
#define	IOSFS_SDIO_DoReadWriteCommand		((int (*)(int, void*, u32, void*, void*))0x10718A8C)

#define	IOSFS_RegisterMDPhysicalDevice		((int (*)(void*, int, int))0x10718860)

#define	IOSFS_mmc_sdcard_struct				((vu32*)0x1089B9F8)
#define	IOSFS_mmc_mlc_struct				((vu32*)0x1089B948)

#define	IOSFS_mlc_phys_dev_struct			((void*)0x11C3A14C)
#define	IOSFS_slc_phys_dev_struct			((void*)0x11C381CC)
#define	IOSFS_slccmpt_phys_dev_struct		((void*)0x11C37668)

#define	le16(i)								((((u16) ((i) & 0xFF)) << 8) | ((u16) (((i) & 0xFF00) >> 8)))
#define	le32(i)								((((u32)le16((i) & 0xFFFF)) << 16) | ((u32)le16(((i) & 0xFFFF0000) >> 16)))
#define	le64(i)								((((u64)le32((i) & 0xFFFFFFFFLL)) << 32) | ((u64)le32(((i) & 0xFFFFFFFF00000000LL) >> 32)))

#endif // IMPORTS_H_
