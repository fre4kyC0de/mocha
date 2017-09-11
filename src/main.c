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
#include "fs/fs_utils.h"
#include "fs/sd_fat_devoptab.h"
#include "system/memory.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "common/common.h"
#include "menu.h"
#include "main.h"
#include "ios_exploit.h"
#include "id.h"

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

    currentTitleId = OSGetTitleID();
    shortTilteId_low = (u32)(currentTitleId & 0xFFFFFFFF);
    shortTilteId_high = (u32)((currentTitleId & 0xFFFFFFFF00000000) >> 32);

    nn_act_Initialize();
    unsigned char slot = nn_act_GetSlotNo();
    unsigned char defaultSlot = nn_act_GetDefaultAccount();
    nn_act_Finalize();

    // in case we are not in mii maker or HBL channel but in system menu or another channel we need to exit here
    if (currentTitleId != TitleId_MiiMakerEUR && // mii maker eur
        currentTitleId != TitleId_MiiMakerUSA && // mii maker usa
        currentTitleId != TitleId_MiiMakerJPN && // mii maker jpn
        currentTitleId != TitleId_HBLChannel)    // HBL channel
    {
        return EXIT_RELAUNCH_ON_LOAD;
    }
    else if(exitToHBLOnLaunch)
    {
        return 0;
    }

    VPADInit();
    int forceMenu = 0;

    {
        VPADData vpad;
        s32 vpadError = -1;
        VPADRead(0, &vpad, 1, &vpadError);

        if(vpadError == 0)
        {
            forceMenu = (vpad.btns_d | vpad.btns_h) & VPAD_BUTTON_B;
        }
    }

    mount_sd_fat("sd");

    cfw_config_t config;
    default_config(&config);
    read_config(&config);

    int launch = 1;

    if(forceMenu || config.directLaunch == 0)
    {
        launch = ShowMenu(&config);
    }

    int returnCode = 0;

    if(launch)
    {
        int res = ExecuteIOSExploit(&config);
        if(res == 0)
        {
            OSForceFullRelaunch();

            if(defaultSlot) //normal menu boot
                SYSLaunchMenu();
            else //show mii select
                _SYSLaunchMenuWithCheckingAccount(slot);

            returnCode = EXIT_RELAUNCH_ON_LOAD;
        }
    }

    unmount_sd_fat("sd");

    return returnCode;
}
