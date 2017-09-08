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
#include <stdio.h>
#include "types.h"
#include "devices.h"
#include "imports.h"
#include "fsa.h"

/*
int FSMakeQuota(int fsa, int client, const char *path, int mode, int unkn, unsigned int size)
{
    int (*callFunc)(int fsa, int client, const char *path, int mode, int unkn, unsigned int size) = (void*)0x1070BC9C;

    if(FS_STRNCMP(path, "/vol/mcp_devmgr", 15) == 0)
    {
        mode = 0x666;
    }
    int res = callFunc(fsa, client, path, mode, unkn, size);
    FS_SYSLOG_OUTPUT("FSMakeQuota: res %d %08X %08X %20s %08X %08X %08X\n", res, fsa, client, path, mode, unkn, size);
    return res;
}

int FSCreateDir(int fsa, int client, const char *path, int mode)
{
    int (*callFunc)(int fsa, int client, const char *path, int mode) = (void*)0x1070BEBC;

    if(FS_STRNCMP(path, "/vol/mcp_devmgr", 15) == 0)
    {
        mode = 0x666;
    }
    int res = callFunc(fsa, client, path, mode);
    FS_SYSLOG_OUTPUT("FSCreateDir: res %d %08X %08X %s %08X\n", res, fsa, client, path, mode);
    return res;
}

int FSChangeDir(int a1, char *dir)
{
    int (*callFunc)(int a1, char* a2) = (void*)0x1070EB7C;
    int res = callFunc(a1, dir);

    FS_SYSLOG_OUTPUT("FSChangeDir: res %d %s\n", res, dir);

    return res;
}

int FSOpenFile(int a1, int a2, char *dir, char *mode, int a3, int a4, int a5, int a6)
{
    int (*callFunc)(int a1, int a2, char *dir, char *mode, int a3, int a4, int a5, int a6) = (void*)0x1070AF08;

    if(FS_STRNCMP(dir, "/vol/mcp_devmgr", 15) == 0)
    {
        a4 = 0x666;
    }

    int res = callFunc(a1, a2, dir, mode, a3, a4, a5, a6);

        FS_SYSLOG_OUTPUT("FSOpenFile: res %d %s %s %08X %08X %08X\n", res, dir, mode, a4, a5, a6);

    return res;
}

int FSWriteFileIssueCommand(int a1, int a2, int a3, int a4, signed int a5, int a6, int a7, int a8)
{
    int (*callFunc)(int a1, int a2, int a3, int a4, signed int a5, int a6, int a7, int a8) = (void*)0x1070A7A4;

    int res = callFunc(a1, a2, a3, a4, a5, a6, a7, a8);

    FS_SYSLOG_OUTPUT("FSWriteFile: res %d %08X %08X %08X %08X %08X %08X\n", res, a3, a4, a5, a6, a7, a8);

    return res;
}
*/
