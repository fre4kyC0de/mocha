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

#include "text.h"
#include "sdio.h"
#include "dumper.h"
#include "imports.h"
#include "devices.h"
#include "svc.h"
#include "../../common/config_types.h"
#include "../../common/kernel_commands.h"

void createDevThread_entry(int node_type, u32 *dev_handles)
{
	IOSFS_syslog_output("FSA: %s thread created\n", (char*)dev_handles[0]);

	if (node_type == NODE_TYPE_DEV_MMC)
	{
		cfw_config_t cfw_config;
		IOSFS_memset(&cfw_config, 0, sizeof(cfw_config));
		svcCustomKernelCommand(KERNEL_COMMAND_GET_CFW_CONFIG, &cfw_config);

		if (cfw_config.redNAND)
			sdcard_init();
	}
	else if (node_type == NODE_TYPE_DEV_ATFS) // ATFS is started right before ISFS for slc/slccmpt
	{
		cfw_config_t cfw_config;
		IOSFS_memset(&cfw_config, 0, sizeof(cfw_config));
		svcCustomKernelCommand(KERNEL_COMMAND_GET_CFW_CONFIG, &cfw_config);

		if (cfw_config.redNAND && (check_nand_dump() == 0))
		{
			clearScreen(0x000000FF);
			_printf(20, 20, "welcome to redNAND!");

			dump_nand_complete();
		}
	}

	/*if (initialization_type == INITIALIZING_FLA)
		dump_nand_complete();*/
}
