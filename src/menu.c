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
#include "osscreen.h"
#include "menu.h"

#define MAX_CONFIG_SETTINGS_EXPERT          8
#define MAX_CONFIG_SETTINGS_DEFAULT         (MAX_CONFIG_SETTINGS_EXPERT - 3)

#define TEXT_SEL(x, text1, text2)           ((x) ? (text1) : (text2))

struct {
    const char *option;
    const char *enabled;
    const char *disabled;
    int *pSetting;
} selection_options[] =
{
    /* default options */
    { "Config view mode", "expert", "default", NULL },
    { "Display this menu on launch", "hide", "show", NULL },
    { "Unrestrict SD access", "on", "off", NULL },
    { "redNAND", "on", "off", NULL },
    { "Start wupserver", "yes", "no", NULL },
    /* expert options */
    { "SEEPROM redirection", "on", "off", NULL },
    { "OTP redirection", "on", "off", NULL },
    { "Use syshax.xml (coldboothax)", "on", "off", NULL },
};

int ShowMenu(cfw_config_t * currentConfig)
{
    VPADData vpad;
    s32 vpadError;
    int refreshScreen = 1;
    int selected = 0;
    int launch = 0;
    cfw_config_t config;
    memcpy(&config, currentConfig, sizeof(cfw_config_t));

    // declare pointers [fre4kyC0de-style]
    selection_options[0].pSetting = &config.viewMode;
    selection_options[1].pSetting = &config.directLaunch;
    selection_options[2].pSetting = &config.sd_access;
    selection_options[3].pSetting = &config.redNAND;
    selection_options[4].pSetting = &config.wupserver;
    selection_options[5].pSetting = &config.seeprom_red;
    selection_options[6].pSetting = &config.otp_red;
    selection_options[7].pSetting = &config.syshaxXml;

    int option_count = sizeof(selection_options) / sizeof(selection_options[0]);
    int max_config_item = config.viewMode ? MAX_CONFIG_SETTINGS_EXPERT : MAX_CONFIG_SETTINGS_DEFAULT;

    while(1)
    {
        //! update only at 50 Hz, thats more than enough
        vpadError = -1;
        VPADRead(0, &vpad, 1, &vpadError);

        if(vpadError == 0)
        {
            if(vpad.btns_d & VPAD_BUTTON_HOME)
            {
                break;
            }
            else if(vpad.btns_d & VPAD_BUTTON_A)
            {
                launch = 1;
                break;
            }
            else if(vpad.btns_d & VPAD_BUTTON_DOWN)
            {
                selected++;
                if(selected >= max_config_item)
                    selected = 0;

                refreshScreen = 1;
            }
            else if(vpad.btns_d & VPAD_BUTTON_UP)
            {
                selected--;
                if(selected < 0)
                    selected = max_config_item - 1;

                refreshScreen = 1;
            }
            else if(vpad.btns_d & (VPAD_BUTTON_LEFT | VPAD_BUTTON_RIGHT))
            {
                if (selected >= 0 && selected <= (option_count - 1)) { // should never cause problems, but just to be sure
                    if (selection_options[selected].pSetting != NULL) {
                        *selection_options[selected].pSetting = !(*selection_options[selected].pSetting);
                    }
                }

                max_config_item = config.viewMode ? MAX_CONFIG_SETTINGS_EXPERT : MAX_CONFIG_SETTINGS_DEFAULT;

                if(!config.viewMode)
                {
                    config.syshaxXml = 0;

                    if(config.redNAND)
                    {
                        config.seeprom_red = 1;
                        config.otp_red = 1;
                    }
                }

                if(config.redNAND == 0)
                {
                    config.seeprom_red = 0;
                    config.otp_red = 0;
                }

                refreshScreen = 1;
            }
        }

        if(refreshScreen)
        {
            console_clear();

            console_print_header();

            console_print_pos(x_offset, 3, "Select your options and press A to launch.");
            console_print_pos(x_offset, 4, "Press HOME to exit back to HBL.");
            console_print_pos(x_offset, 5, "Hold B on start to force enter this menu");

            int y_offset = 6;
            int idx;

            for(idx = 0; idx < option_count && idx < max_config_item; idx++)
            {
                if (selection_options[idx].pSetting != NULL) {
                    console_print_pos(x_offset, y_offset++, "%s %-29s : %s%s%s %s%s%s", TEXT_SEL((selected == idx), "--->", "    "), selection_options[idx].option,
                                      TEXT_SEL(*selection_options[idx].pSetting, "<", " "), selection_options[idx].enabled,  TEXT_SEL(*selection_options[idx].pSetting, ">", " "),
                                      TEXT_SEL(*selection_options[idx].pSetting, " ", "<"), selection_options[idx].disabled, TEXT_SEL(*selection_options[idx].pSetting, " ", ">"));
                }
            }

			y_offset += 1;
			console_print_pos(x_offset, y_offset++, "Credits go to everyone who contributed to Wii U scene publicly");
            console_print_pos(x_offset, y_offset++, "Special thanks to smealum, plutoo, yellows8, naehrwert and derrek");
            console_print_pos(x_offset, y_offset++, "FSHax by Zarklord1 and Maschell; multiple small changes by fre4kyC0de");

            // Flip buffers
            OSScreenFlipBuffersEx(0);
            OSScreenFlipBuffersEx(1);

            refreshScreen = 0;
        }

        os_usleep(20000);
    }

    if(memcmp(currentConfig, &config, sizeof(cfw_config_t)) != 0)
    {
        memcpy(currentConfig, &config, sizeof(cfw_config_t));
        write_config(currentConfig);
    }

    return launch;
}
