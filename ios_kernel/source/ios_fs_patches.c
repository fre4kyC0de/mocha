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

#include "../../src/dynamic_libs/os_types.h"
#include "elf_patcher.h"
#include "ios_fs_patches.h"
#include "config.h"
#include "../../ios_fs/ios_fs_syms.h"

#include "ios_fs_offsets.h"

extern const patch_table_t fs_patches_table[];
extern const patch_table_t fs_patches_table_end[];

u32 fs_get_phys_code_base(void)
{
	return (ios_fs__text_start + FS_PHYS_DIFF);
}

void fs_run_patches(u32 ios_elf_start)
{
	// write ios_fs code and bss
	section_write(ios_elf_start, ios_fs__text_start, (void*)fs_get_phys_code_base(), ios_fs__text_end - ios_fs__text_start);
	section_write_bss(ios_elf_start, ios_fs__bss_start, ios_fs__bss_end - ios_fs__bss_start);

	// patch FS logging
	section_write_word(ios_elf_start, FS_PRINTF_SYSLOG, ARM_B(FS_PRINTF_SYSLOG, FS_SYSLOG_OUTPUT));

	if (cfw_config.redNAND)
	{
		section_write_word(ios_elf_start, CALL_FS_REGISTERMDPHYSICALDEVICE, ARM_BL(CALL_FS_REGISTERMDPHYSICALDEVICE, ios_fs_registerMdDevice_hook));
		section_write_word(ios_elf_start, FS_GETMDDEVICEBYID + 8, ARM_BL((FS_GETMDDEVICEBYID + 8), ios_fs_getMdDeviceById_hook));

		section_write_word(ios_elf_start, FS_SDCARD_READ1, ARM_B(FS_SDCARD_READ1, ios_fs_sdcardRead_patch));
		section_write_word(ios_elf_start, FS_SDCARD_WRITE1, ARM_B(FS_SDCARD_WRITE1, ios_fs_sdcardWrite_patch));

		section_write_word(ios_elf_start, FS_SLC_READ1, ARM_B(FS_SLC_READ1, ios_fs_slcRead1_patch));
		section_write_word(ios_elf_start, FS_SLC_READ2, ARM_B(FS_SLC_READ2, ios_fs_slcRead2_patch));
		section_write_word(ios_elf_start, FS_SLC_WRITE1, ARM_B(FS_SLC_WRITE1, ios_fs_slcWrite1_patch));
		section_write_word(ios_elf_start, FS_SLC_WRITE2, ARM_B(FS_SLC_WRITE2, ios_fs_slcWrite2_patch));

		//section_write_word(ios_elf_start, FS_USB_READ, ARM_B(FS_USB_READ, ios_fs_usbRead_patch));
		//section_write_word(ios_elf_start, FS_USB_WRITE, ARM_B(FS_USB_WRITE, ios_fs_usbWrite_patch));
	}

	section_write_word(ios_elf_start, FS_CREATEDEVTHREAD_HOOK, ARM_B(FS_CREATEDEVTHREAD_HOOK, ios_fs_createDevThread_hook));

	u32 patch_count = (u32)(((u8*)fs_patches_table_end) - ((u8*)fs_patches_table)) / sizeof(patch_table_t);
	patch_table_entries(ios_elf_start, fs_patches_table, patch_count);
}
