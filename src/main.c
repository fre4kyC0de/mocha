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

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/nn_act_functions.h"
#include "dynamic_libs/nn_nim_functions.h"
#include "fs/fs_utils.h"
#include "fs/sd_fat_devoptab.h"
#include "system/memory.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "common/common.h"
#include "osscreen.h"
#include "menu.h"
#include "main.h"
#include "ios_exploit.h"
#include "id.h"
#include "nnu.h"
#include "dumper.h"

#define	usleep_TimeOut	0
//#define	usleep_TimeOut	1000000

static int exitToHBLOnLaunch = 0;

int Menu_Main(void)
{
	//!---------INIT---------
	InitOSFunctionPointers();
	InitSysFunctionPointers();
	InitFSFunctionPointers();
	InitSocketFunctionPointers();
	InitVPadFunctionPointers();
	InitACTFunctionPointers();
	InitNimFunctionPointers();

	currentTitleId = OSGetTitleID();
	currentTilteId_low = (u32)(currentTitleId & 0xFFFFFFFF);
	currentTilteId_high = (u32)((currentTitleId & 0xFFFFFFFF00000000) >> 32);
	systemTitleId = _SYSGetSystemApplicationTitleId(0);
	systemTitleId_low = (u32)(systemTitleId & 0xFFFFFFFF);
	systemTitleId_high = (u32)((systemTitleId & 0xFFFFFFFF00000000) >> 32);

	nn_act_Initialize();
	unsigned char slot = nn_act_GetSlotNo();
	unsigned char defaultSlot = nn_act_GetDefaultAccount();
	nn_act_Finalize();

	// in case we are not in mii maker or HBL channel but in system menu or another channel we need to exit here
	if ((currentTitleId != TitleId_MiiMakerEUR) && (currentTitleId != TitleId_MiiMakerUSA) && (currentTitleId != TitleId_MiiMakerJPN) && (currentTitleId != TitleId_HBLChannel))
		return EXIT_RELAUNCH_ON_LOAD;
	else if (exitToHBLOnLaunch)
		return 0;

	// Init screen and screen buffers
	int y_offset = 3;
	OSScreenInit();
	u32 screen_buf0_size = OSScreenGetBufferSizeEx(0);
	u32 screen_buf1_size = OSScreenGetBufferSizeEx(1);
	u8 * screenBuffer = (u8*) memalign(0x100, screen_buf0_size + screen_buf1_size);
	OSScreenSetBufferEx(0, (void *)screenBuffer);
	OSScreenSetBufferEx(1, (void *)(screenBuffer + screen_buf0_size));
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
	// Clear screens
	console_clear();

	console_print_header();

	/*console_print_pos(x_offset, y_offset, "systemTitleId: %08X%08X", systemTitleId_high, systemTitleId_low);
	y_offset += 1;
	if (usleep_TimeOut > 0)
		os_usleep(usleep_TimeOut);*/

	console_print_pos(x_offset, y_offset, "Initializing VPAD...");
	y_offset += 1;
	if (usleep_TimeOut > 0)
		os_usleep(usleep_TimeOut);

	VPADInit();
	int forceMenu = 0;

	{
		VPADData vpad;
		s32 vpadError = -1;
		VPADRead(0, &vpad, 1, &vpadError);

		if (vpadError == 0)
			forceMenu = (vpad.btns_d | vpad.btns_h) & VPAD_BUTTON_B;
	}

	console_print_pos(x_offset, y_offset, "Mounting SD...");
	y_offset += 1;
	if (usleep_TimeOut > 0)
		os_usleep(usleep_TimeOut);

	mount_sd_fat("sd");

	console_print_pos(x_offset, y_offset, "Reading config...");
	y_offset += 1;
	if (usleep_TimeOut > 0)
		os_usleep(usleep_TimeOut);

	cfw_config_t config;
	default_config(&config);
	read_config(&config);

	int launch, launchable;
	
	if (OS_FIRMWARE == 550)
		launchable = 1;
	else
		launchable = 0;
	
	if (launchable && (forceMenu || (config.directLaunch == 0)))
	{
		console_print_pos(x_offset, y_offset, "Displaying menu...");
		y_offset += 1;

		launch = ShowMenu(&config);

		console_clear();
		console_print_header();
		y_offset = 3;
		if (usleep_TimeOut > 0)
			os_usleep(usleep_TimeOut);
	} else
		launch = 1;

	if (!launchable)
		launch = 0;

	int returnCode = 0;

	if (launch)
	{
		console_print_pos(x_offset, y_offset, "Handing over control to uhshax...");
		y_offset += 1;
		if (usleep_TimeOut > 0)
			os_usleep(usleep_TimeOut);

		int res = ExecuteIOSExploit(&config);
		if (res == 0)
		{
			console_print_pos(x_offset, y_offset, "OSForceFullRelaunch()");
			y_offset += 1;
			if (usleep_TimeOut > 0)
				os_usleep(usleep_TimeOut);

			OSScreenShutdown();
			free(screenBuffer);

			OSForceFullRelaunch();

			if (defaultSlot) //normal menu boot
				SYSLaunchMenu();
			else //show mii select
				_SYSLaunchMenuWithCheckingAccount(slot);

			returnCode = EXIT_RELAUNCH_ON_LOAD;
			
			//nnupatcher();
		}
	}
	else
	{
		OSScreenShutdown();
		free(screenBuffer);
	}

	/*DoMemoryDump(0x04000000, 0x30000, "sd:/IOS-CRYPTO.bin");
	DoMemoryDump(0x05000000, 0xC0000, "sd:/IOS-MCP.bin");
	DoMemoryDump(0x05100000, 0x20000, "sd:/IOS-MCP_D_R.bin");
	DoMemoryDump(0x08120000, 0xA0000, "sd:/IOS-KERNEL.bin");
	DoMemoryDump(0x0D400000, 0x10000, "sd:/BOOT1.bin");
	DoMemoryDump(0x0D410000, 0x10000, "sd:/BOOT0.bin");
	DoMemoryDump(0x10100000, 0x3D0000, "sd:/IOS-USB.bin");
	DoMemoryDump(0x10700000, 0x1540000, "sd:/IOS-FS.bin");
	DoMemoryDump(0x11F00000, 0x260000, "sd:/IOS-PAD.bin");
	DoMemoryDump(0x12300000, 0x590000, "sd:/IOS-NET.bin");
	DoMemoryDump(0xE0000000, 0x270000, "sd:/IOS-ACP.bin");
	DoMemoryDump(0xE1000000, 0x2F0000, "sd:/IOS-NSEC.bin");
	DoMemoryDump(0xE2000000, 0x6D0000, "sd:/IOS-NIM-BOSS.bin");
	DoMemoryDump(0xE3000000, 0x300000, "sd:/IOS-FPD.bin");
	DoMemoryDump(0xE4000000, 0x160000, "sd:/IOS-TEST.bin");
	DoMemoryDump(0xE5000000, 0x70000, "sd:/IOS-AUXIL.bin");
	DoMemoryDump(0xE6000000, 0x50000, "sd:/IOS-BSP.bin");*/

	unmount_sd_fat("sd");

	return returnCode;
}
