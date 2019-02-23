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

#include "../../src/dynamic_libs/os_types.h"
#include "imports.h"
#include "devices.h"
#include "sdio.h"
#include "mlcio.h"
#include "fat32_format.h"
#include "text.h"
#include "hardware_registers.h"
#include "svc.h"
#include "../../common/kernel_commands.h"

// the IO buffer is put behind everything else because there is no access to this region from IOS-FS it seems
unsigned char io_buffer[0x40000]  __attribute__((aligned(0x40))) __attribute__((section(".io_buffer")));

//! this one is required for the read function
static void slc_read_callback(int result, int priv)
{
	int *private_data = (int*)priv;
	private_data[1] = result;
	FS_svc_releasemutex(private_data[0]);
}

static int srcRead(void* deviceHandle, void *data_ptr, u32 offset, u32 sectors, int * result_array)
{
	int readResult = slcRead1_original(deviceHandle, 0, offset, sectors, SLC_BYTES_PER_SECTOR, data_ptr, slc_read_callback, (int)result_array);
	if (readResult == 0)
	{
		// wait for process to finish
		FS_svc_acquiremutex(result_array[0], 0);
		readResult = result_array[1];
	}
	return readResult;
}

void slc_dump(void *deviceHandle, const char* device, u32 base_sectors, int y_offset)
{
	//also create a mutex for synchronization with end of operation...
	int sync_mutex = FS_svc_createmutex(1, 1);
	FS_svc_acquiremutex(sync_mutex, 0);

	int result_array[2];
	result_array[0] = sync_mutex;

	u32 offset = 0;
	int readResult = 0;
	int writeResult = 0;
	int retry = 0;
	u32 readSize = sizeof(io_buffer) / SLC_BYTES_PER_SECTOR;

	FS_sleep(1000);

	do
	{
		// don't print single steps in between, just if they have an error or every 0x80 sectors
		if ((readSize == (sizeof(io_buffer) / SLC_BYTES_PER_SECTOR)) || (retry > 0))
			_printf(20, y_offset, "%s     = %08X / 40000, read code %08X, write code %08X, retry %d", device, offset, readResult, writeResult, retry);

		//! set flash erased byte to buffer
		FS_memset(io_buffer, 0xff, sizeof(io_buffer));
		//readResult = readSlc(io_buffer, offset, (sizeof(io_buffer) / SLC_BYTES_PER_SECTOR), deviceHandle);
		readResult = srcRead(deviceHandle, io_buffer, offset, readSize, result_array);

		//! retry 2 times as there are read failures in several places
		if ((readResult != 0) && (retry < 2))
		{
			readSize = 1;
			FS_sleep(10);
			retry++;
		}
		else
		{
			retry = 0;

			while (1)
			{
				FS_sleep(10);

				writeResult = sdcard_readwrite(SDIO_WRITE, io_buffer, (readSize * (SLC_BYTES_PER_SECTOR / SDIO_BYTES_PER_SECTOR)), SDIO_BYTES_PER_SECTOR, base_sectors, NULL, DEVICE_ID_SDCARD_PATCHED);
				if ((writeResult == 0) || (retry >= 2))
				{
					retry = 0;
					base_sectors += (readSize * (SLC_BYTES_PER_SECTOR / SDIO_BYTES_PER_SECTOR));
					offset += readSize;

					// if we did single sector reads and got to a point where we can do multiple reads -> switch to multiple sector reads
					if ((offset % (sizeof(io_buffer) / SLC_BYTES_PER_SECTOR)) == 0)
						readSize = sizeof(io_buffer) / SLC_BYTES_PER_SECTOR;

					break;
				}
				else
					retry++;
			}
		}
	}
	while (offset < SLC_SECTOR_COUNT);

	FS_svc_destroymutex(sync_mutex);

	// last print to show "done"
	_printf(20, y_offset, "%s     = %08X / 40000, read code %08X, write code %08X, retry %d", device, offset, readResult, writeResult, retry);
}

void mlc_dump(u32 base_sector, u32 mlc_end)
{
	u32 offset = 0;

	int retry = 0;
	int mlc_result = 0;
	int callback_result = 0;
	int write_result = 0;
	int print_counter = 0;

	do
	{
		//! print only every 4th time
		if (print_counter == 0)
		{
			print_counter = 4;
			_printf(20, 70, "mlc         = %08X / %08X, mlc res %08X, sd res %08X, retry %d", offset, mlc_end, mlc_result, write_result, retry);
		}
		else
			--print_counter;

		//! set flash erased byte to buffer
		FS_memset(io_buffer, 0xff, sizeof(io_buffer));
		mlc_result = sdcard_readwrite(SDIO_READ, io_buffer, (sizeof(io_buffer) / MLC_BYTES_PER_SECTOR), MLC_BYTES_PER_SECTOR, offset, &callback_result, DEVICE_ID_MLC);

		if ((mlc_result == 0) && (callback_result != 0))
			mlc_result = callback_result;

		//! retry 5 times as there are read failures in several places
		if ((mlc_result != 0) && (retry < 5))
		{
			FS_sleep(100);
			retry++;
			print_counter = 0; // print errors directly
		}
		else
		{
			write_result = sdcard_readwrite(SDIO_WRITE, io_buffer, (sizeof(io_buffer) / MLC_BYTES_PER_SECTOR), SDIO_BYTES_PER_SECTOR, base_sector + offset, NULL, DEVICE_ID_SDCARD_PATCHED);
			if ((write_result == 0) || (retry >= 5))
			{
				retry = 0;
				offset += (sizeof(io_buffer) / MLC_BYTES_PER_SECTOR);
			}
			else
			{
				FS_sleep(100);
				retry++;
				print_counter = 0; // print errors directly
			}
		}
	}
	while (offset < mlc_end); //! TODO: make define MLC32_SECTOR_COUNT

	// last print to show "done"
	_printf(20, 70, "mlc     = %08X / %08X, mlc res %08X, sd res %08X, retry %d", offset, mlc_end, mlc_result, write_result, retry);
}

int check_nand_type(void)
{
	//! check if MLC size is > 8GB
	if ( FS_MMC_MLC_struct[0x30/4] > 0x1000000)
		return MLC_NAND_TYPE_32GB;
	else
		return MLC_NAND_TYPE_8GB;
}

int check_nand_dump(void)
{
	u32 mlc_sector_count = FS_MMC_MLC_struct[0x30/4];

	int signature_correct = 0;
	sdio_nand_signature_sector_t * sign_sect = (sdio_nand_signature_sector_t*)io_buffer;
	memset(sign_sect, 0, SDIO_BYTES_PER_SECTOR);
	sdcard_readwrite(SDIO_READ, sign_sect, 1, SDIO_BYTES_PER_SECTOR, NAND_DUMP_SIGNATURE_SECTOR, NULL, DEVICE_ID_SDCARD_PATCHED);

	signature_correct = (sign_sect->signature == NAND_DUMP_SIGNATURE);

	memset(io_buffer, 0, SDIO_BYTES_PER_SECTOR);
	sdcard_readwrite(SDIO_READ, io_buffer, 1, SDIO_BYTES_PER_SECTOR, 0, NULL, DEVICE_ID_SDCARD_PATCHED);

	return (signature_correct && CheckFAT32PartitionOffset(io_buffer, MLC_BASE_SECTORS + mlc_sector_count));
}

static void wait_format_confirmation(void)
{
	int timeout = 600;
	//"Press the POWER button SD then , else the console will reboot in %u seconds."
	while (1)
	{
		_printf(20, 30, "No NAND dump detected. SD Format and complete NAND dump required.");
		_printf(20, 40, "Press the POWER button to format SD card otherwise the console will reboot in %d seconds.", timeout/10);

		if (svcCustomKernelCommand(KERNEL_COMMAND_READ32, LT_GPIO_IN) & GPIO_IN_POWER_BUTTON)
			break;

		if (--timeout == 0)
		{
			FS_sleep(1000);
			svcShutdown(SHUTDOWN_TYPE_REBOOT);
		}

		FS_sleep(100);
	}

	// clear the lines
	clearLine(30, 0x000000FF);
	clearLine(40, 0x000000FF);
}

void dump_nand_complete()
{
	wait_format_confirmation();

	mlc_init();
	FS_sleep(1000);

	int nand_type = check_nand_type();
	u32 sdio_sector_count = FS_MMC_SDCard_struct[0x30/4];
	u32 mlc_sector_count = FS_MMC_MLC_struct[0x30/4];
	u32 fat32_partition_offset = (MLC_BASE_SECTORS + mlc_sector_count);

	_printf(20, 30, "Detected %d GB MLC NAND type.", (nand_type == MLC_NAND_TYPE_8GB) ? 8 : 32);

	if (sdio_sector_count < fat32_partition_offset)
	{
		_printf(20, 40, "SD card too small! Required sectors %u > available %u.", fat32_partition_offset, sdio_sector_count);
		FS_sleep(3000);
		svcShutdown(SHUTDOWN_TYPE_REBOOT);
	}

	if (FormatSDCard(fat32_partition_offset, sdio_sector_count) < 0)
	{
		FS_sleep(3000);
		svcShutdown(SHUTDOWN_TYPE_REBOOT);
	}

	slc_dump(FS_SLC_phys_dev_struct,     "slc    ", SLC_BASE_SECTORS, 50);
	slc_dump(FS_SLCCMPT_phys_dev_struct, "slccmpt", SLCCMPT_BASE_SECTORS, 60);
	mlc_dump(MLC_BASE_SECTORS, mlc_sector_count);

	//! write marker to SD card from which we can auto detect NAND dump
	//! we can actually use that for settings
	sdio_nand_signature_sector_t * sign_sect = (sdio_nand_signature_sector_t*)io_buffer;
	memset(sign_sect, 0, SDIO_BYTES_PER_SECTOR);
	sign_sect->signature = NAND_DUMP_SIGNATURE;
	sign_sect->nand_descriptions[0].nand_type = NAND_DESC_TYPE_SLC;
	sign_sect->nand_descriptions[0].base_sector = SLC_BASE_SECTORS;
	sign_sect->nand_descriptions[0].sector_count = SLC_SECTOR_COUNT * (SLC_BYTES_PER_SECTOR / SDIO_BYTES_PER_SECTOR);
	sign_sect->nand_descriptions[1].nand_type = NAND_DESC_TYPE_SLCCMPT;
	sign_sect->nand_descriptions[1].base_sector = SLCCMPT_BASE_SECTORS;
	sign_sect->nand_descriptions[1].sector_count = SLC_SECTOR_COUNT * (SLC_BYTES_PER_SECTOR / SDIO_BYTES_PER_SECTOR);
	sign_sect->nand_descriptions[2].nand_type = NAND_DESC_TYPE_MLC;
	sign_sect->nand_descriptions[2].base_sector = MLC_BASE_SECTORS;
	sign_sect->nand_descriptions[2].sector_count = mlc_sector_count * (MLC_BYTES_PER_SECTOR / SDIO_BYTES_PER_SECTOR);

	sdcard_readwrite(SDIO_WRITE, io_buffer, 1, SDIO_BYTES_PER_SECTOR, NAND_DUMP_SIGNATURE_SECTOR, NULL, DEVICE_ID_SDCARD_PATCHED);

	_printf(20, 80, "Complete! -> rebooting into sysNAND...");

	FS_sleep(3000);
	svcShutdown(SHUTDOWN_TYPE_REBOOT);
}

#if 0
// debug and not used at the moment
void dump_data(void* data_ptr, u32 size)
{
	static u32 dumpdata_offset = 0;

	u32 num_sectors = size >> 9; // size / SDIO_BYTES_PER_SECTOR but faster ;)
	if (num_sectors == 0)
		num_sectors = 1;

	sdcard_readwrite(SDIO_WRITE, data_ptr, num_sectors, SDIO_BYTES_PER_SECTOR, DUMPDATA_BASE_SECTORS + dumpdata_offset, NULL, DEVICE_ID_SDCARD_PATCHED);
	dumpdata_offset += num_sectors;
}

void dump_lots_data(u8* addr, u32 size)
{
	u32 cur_size;
	u32 size_remaining = size;
	u8* cur_addr = addr;
	do
	{
		cur_size = sizeof(io_buffer);
		if (cur_size > size_remaining)
			cur_size = size_remaining;

		FS_memcpy(io_buffer, cur_addr, cur_size);
		dump_data(io_buffer, cur_size);

		cur_addr += cur_size;
		size_remaining -= cur_size;
	}
	while (cur_size != 0);
}

void dump_syslog()
{
	FS_memcpy(io_buffer, *(void**)0x05095ECC, sizeof(io_buffer));
	sdcard_readwrite(SDIO_WRITE, io_buffer, sizeof(io_buffer) / SDIO_BYTES_PER_SECTOR, SDIO_BYTES_PER_SECTOR, SYSLOG_BASE_SECTORS, NULL, DEVICE_ID_SDCARD_PATCHED);
}
#endif
