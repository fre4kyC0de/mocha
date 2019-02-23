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
#include <string.h>

#include "net_ifmgr_ncl.h"
#include "imports.h"
#include "svc.h"

static int ifmgrncl_handle = 0;

int ifmgrnclInit()
{
	if (ifmgrncl_handle)
		return ifmgrncl_handle;

	int ret = svcOpen("/dev/net/ifmgr/ncl", 0);

	if (ret > 0)
	{
		ifmgrncl_handle = ret;
		return ifmgrncl_handle;
	}

	return ret;
}

int ifmgrnclExit()
{
	int ret = svcClose(ifmgrncl_handle);

	ifmgrncl_handle = 0;

	return ret;
}

static void* allocIobuf(u32 size)
{
	void* ptr = svcAlloc(0xCAFF, size);

	if (ptr)
		memset(ptr, 0x00, size);

	return ptr;
}

static void freeIobuf(void* ptr)
{
	svcFree(0xCAFF, ptr);
}

int	IFMGRNCL_GetInterfaceStatus(u16 interface_id, u16* out_status)
{
	u8* iobuf1 = allocIobuf(0x2);
	u16* inbuf = (u16*)iobuf1;
	u8* iobuf2 = allocIobuf(0x8);
	u16* outbuf = (u16*)iobuf2;

	inbuf[0] = interface_id;

	int ret = svcIoctl(ifmgrncl_handle, 0x14, inbuf, 0x2, outbuf, 0x8);

	if (!ret && out_status)
		*out_status = outbuf[2];

	freeIobuf(iobuf1);
	freeIobuf(iobuf2);
	return ret;
}
