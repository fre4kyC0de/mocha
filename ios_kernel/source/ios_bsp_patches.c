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
#include "ios_bsp_patches.h"
#include "../../ios_bsp/ios_bsp_syms.h"
#include "fsa.h"
#include "utils.h"
#include "imports.h"

#define disable_interrupts							IOSKERNEL_disable_interrupts
#define enable_interrupts							IOSKERNEL_enable_interrupts

#define	BSP_PHYS_DIFF		(-0xE6000000 + 0x13CC0000)

extern const patch_table_t fs_patches_table[];
extern const patch_table_t fs_patches_table_end[];

u32 bsp_get_phys_code_base(void)
{
	return (ios_bsp__text_start + BSP_PHYS_DIFF);
}

int bsp_init_seeprom_buffer(u32 baseSector, int dumpFound)
{
	void *tmpBuffer = (void*)0x00140000;

	if (dumpFound)
	{
		int res = FSA_SDReadRawSectors(tmpBuffer, baseSector, 1);
		if (res < 0)
			return res;
	}
	else
	{
		//! just clear out the seeprom and it will be re-initialized on BSP module
		//! TODO: maybe read in the seeprom here from SPI or BSP module
		IOSKERNEL_memset(tmpBuffer, 0, 0x200);
	}

	int level = disable_interrupts();
	unsigned int control_register = disable_mmu();

	IOSKERNEL_memcpy((void*)(ios_bsp__seeprom_buffer_start - 0xE6047000 + 0x13D07000), tmpBuffer, 0x200);

	restore_mmu(control_register);
	enable_interrupts(level);

	return 0;
}

void bsp_run_patches(u32 ios_elf_start)
{
	section_write(ios_elf_start, ios_bsp__text_start, (void*)bsp_get_phys_code_base(), ios_bsp__text_end - ios_bsp__text_start);
	section_write_bss(ios_elf_start, ios_bsp__bss_start, ios_bsp__bss_end - ios_bsp__bss_start);

	section_write(ios_elf_start, ios_bsp__seeprom_buffer_start, (void*)(ios_bsp__seeprom_buffer_start - 0xE6047000 + 0x13D07000), 0x200);

	section_write_word(ios_elf_start, 0xE600D08C, ARM_B(0xE600D08C, ios_bsp_EEPROM_SPI_ReadWord));
	section_write_word(ios_elf_start, 0xE600D010, ARM_B(0xE600D010, ios_bsp_EEPROM_SPI_WriteWord));
	section_write_word(ios_elf_start, 0xE600CF5C, ARM_B(0xE600CF5C, ios_bsp_EEPROM_WriteControl));
}
