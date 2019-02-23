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

#ifndef _DUMPER_H_
#define _DUMPER_H_

int check_nand_type(void);
int check_nand_dump(void);
void slc_dump(int deviceId, const char* format, u32 base_sectors);
void mlc_dump(u32 base_sector, u32 mlc_end);
void dump_nand_complete();

#if 0
//! debug dumps
void dump_syslog();
void dump_data(void* data_ptr, u32 size);
void dump_lots_data(u8* addr, u32 size);
#endif

#endif // _DUMPER_H_
