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

#ifndef _FS_OFFSETS_H_
#define _FS_OFFSETS_H_

#define	FS_PHYS_DIFF								0

#define	FS_SYSLOG_OUTPUT							0x107F0C84
#define	FS_PRINTF_SYSLOG							0x107F5720
#define	CALL_FS_REGISTERMDPHYSICALDEVICE			0x107BD81C
#define	FS_GETMDDEVICEBYID							0x107187C4
#define	FS_CREATEDEVTHREAD_HOOK						0x10700294
#define	FS_USB_READ									0x1077F1C0
#define	FS_USB_WRITE								0x1077F35C
#define	FS_SLC_READ1								0x107B998C
#define	FS_SLC_READ2								0x107B98FC
#define	FS_SLC_WRITE1								0x107B9870
#define	FS_SLC_WRITE2								0x107B97E4
#define	FS_MLC_READ1								0x107DC760
#define	FS_MLC_READ2								0x107DCDE4
#define	FS_MLC_WRITE1								0x107DC0C0
#define	FS_MLC_WRITE2								0x107DC73C
#define	FS_SDCARD_READ1								0x107BDDD0
#define	FS_SDCARD_WRITE1							0x107BDD60

#endif
