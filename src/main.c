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

#define	usleep_TimeOut	-1

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
	shortTilteId_low = (u32)(currentTitleId & 0xFFFFFFFF);
	shortTilteId_high = (u32)((currentTitleId & 0xFFFFFFFF00000000) >> 32);

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

	int launch = 1;

	if (forceMenu || (config.directLaunch == 0))
	{
		console_print_pos(x_offset, y_offset, "Displaying menu...");
		y_offset += 1;

		launch = ShowMenu(&config);

		console_clear();
		console_print_header();
		y_offset = 3;
		if (usleep_TimeOut > 0)
			os_usleep(usleep_TimeOut);
	}

	int returnCode = 0;

	if (launch)
	{
		console_print_pos(x_offset, y_offset, "Handing over control to IOSUHAX...");
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
		}
	}
	else
	{
		OSScreenShutdown();
		free(screenBuffer);
	}

	//nnupatcher();

	unmount_sd_fat("sd");

	return returnCode;
}
