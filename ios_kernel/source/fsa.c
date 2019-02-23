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

#include "svc.h"
#include "imports.h"
#include "fsa.h"

static void* allocIobuf()
{
	void* ptr = svcAlloc(0xCAFF, 0x828);

	KERNEL_memset(ptr, 0x00, 0x828);

	return ptr;
}

static void freeIobuf(void* ptr)
{
	svcFree(0xCAFF, ptr);
}

static int IOS_Open(const char * dev, int mode)
{
	// put string into a good location
	char* devStr = (char*)svcAlloc(0xCAFF, 0x20);
	if (!devStr)
		return -3;

	KERNEL_strncpy(devStr, dev, 0x20);

	int res = svcOpen(devStr, 0);

	svcFree(0xCAFF, devStr);

	return res;
}

static int FSA_Open(void)
{
	return IOS_Open("/dev/fsa", 0);
}

static int FSA_Close(int fd)
{
	return svcClose(fd);
}

static int FSA_RawOpen(int fd, const char* device_path, int* outHandle)
{
	u8* iobuf = allocIobuf();
	u32* inbuf = (u32*)iobuf;
	u32* outbuf = (u32*)&iobuf[0x520];

	KERNEL_strncpy((char*)&inbuf[0x01], device_path, 0x27F);

	int ret = svcIoctl(fd, 0x6A, inbuf, 0x520, outbuf, 0x293);

	if (outHandle)
		*outHandle = outbuf[1];

	freeIobuf(iobuf);
	return ret;
}

static int FSA_RawClose(int fd, int device_handle)
{
	u8* iobuf = allocIobuf();
	u32* inbuf = (u32*)iobuf;
	u32* outbuf = (u32*)&iobuf[0x520];

	inbuf[1] = device_handle;

	int ret = svcIoctl(fd, 0x6D, inbuf, 0x520, outbuf, 0x293);

	freeIobuf(iobuf);
	return ret;
}

// offset in blocks of 0x1000 bytes
static int FSA_RawRead(int fd, void* data, u32 size_bytes, u32 cnt, u64 blocks_offset, int device_handle)
{
	u8* iobuf = allocIobuf();
	u8* inbuf8 = iobuf;
	u8* outbuf8 = &iobuf[0x520];
	iovec_s* iovec = (iovec_s*)&iobuf[0x7C0];
	u32* inbuf = (u32*)inbuf8;
	u32* outbuf = (u32*)outbuf8;

	// note : offset_bytes = blocks_offset * size_bytes
	inbuf[0x08 / 4] = (blocks_offset >> 32);
	inbuf[0x0C / 4] = (blocks_offset & 0xFFFFFFFF);
	inbuf[0x10 / 4] = cnt;
	inbuf[0x14 / 4] = size_bytes;
	inbuf[0x18 / 4] = device_handle;

	iovec[0].ptr = inbuf;
	iovec[0].len = 0x520;

	iovec[1].ptr = data;
	iovec[1].len = size_bytes * cnt;

	iovec[2].ptr = outbuf;
	iovec[2].len = 0x293;

	int ret = svcIoctlv(fd, 0x6B, 1, 2, iovec);

	freeIobuf(iobuf);
	return ret;
}

static int FSA_RawWrite(int fd, void* data, u32 size_bytes, u32 cnt, u64 blocks_offset, int device_handle)
{
	u8* iobuf = allocIobuf();
	u8* inbuf8 = iobuf;
	u8* outbuf8 = &iobuf[0x520];
	iovec_s* iovec = (iovec_s*)&iobuf[0x7C0];
	u32* inbuf = (u32*)inbuf8;
	u32* outbuf = (u32*)outbuf8;

	inbuf[0x08 / 4] = (blocks_offset >> 32);
	inbuf[0x0C / 4] = (blocks_offset & 0xFFFFFFFF);
	inbuf[0x10 / 4] = cnt;
	inbuf[0x14 / 4] = size_bytes;
	inbuf[0x18 / 4] = device_handle;

	iovec[0].ptr = inbuf;
	iovec[0].len = 0x520;

	iovec[1].ptr = data;
	iovec[1].len = size_bytes * cnt;

	iovec[2].ptr = outbuf;
	iovec[2].len = 0x293;

	int ret = svcIoctlv(fd, 0x6C, 2, 1, iovec);

	freeIobuf(iobuf);
	return ret;
}

int FSA_SDReadRawSectors(void *buffer, u32 sector, u32 num_sectors)
{
	int fsa = FSA_Open();
	if (fsa < 0)
		return fsa;

	int fd;
	int res = FSA_RawOpen(fsa, "/dev/sdcard01", &fd);
	if (res < 0)
	{
		FSA_Close(fsa);
		return res;
	}

	void *buf = svcAllocAlign(0xCAFF, num_sectors << 9, 0x40);
	if (!buf)
	{
		FSA_RawClose(fsa, fd);
		FSA_Close(fsa);
		return -2;
	}

	res = FSA_RawRead(fsa, buf, 0x200, num_sectors, sector, fd);

	KERNEL_memcpy(buffer, buf, num_sectors << 9);

	svcFree(0xCAFF, buf);
	FSA_RawClose(fsa, fd);
	FSA_Close(fsa);

	return res;
}

int FSA_SDWriteRawSectors(const void *buffer, u32 sector, u32 num_sectors)
{
	int fsa = FSA_Open();
	if (fsa < 0)
		return fsa;

	int fd;
	int res = FSA_RawOpen(fsa, "/dev/sdcard01", &fd);
	if (res < 0)
	{
		FSA_Close(fsa);
		return res;
	}

	void *buf = svcAllocAlign(0xCAFF, num_sectors << 9, 0x40);
	if (!buf)
	{
		FSA_RawClose(fsa, fd);
		FSA_Close(fsa);
		return -2;
	}

	KERNEL_memcpy(buf, buffer, num_sectors << 9);

	res = FSA_RawWrite(fsa, buf, 0x200, num_sectors, sector, fd);

	svcFree(0xCAFF, buf);
	FSA_RawClose(fsa, fd);
	FSA_Close(fsa);

	return res;
}

