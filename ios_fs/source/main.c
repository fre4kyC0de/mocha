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
	FS_SYSLOG_OUTPUT("FSA: %s thread created\n", (char*)dev_handles[0]);

	if (node_type == NODE_TYPE_DEV_MMC)
	{
		cfw_config_t cfw_config;
		FS_MEMSET(&cfw_config, 0, sizeof(cfw_config));
		svcCustomKernelCommand(KERNEL_GET_CFW_CONFIG, &cfw_config);

		if (cfw_config.redNAND)
			sdcard_init();
	}
	else if (node_type == NODE_TYPE_DEV_ATFS) // ATFS is started right before ISFS for slc/slccmpt
	{
		cfw_config_t cfw_config;
		FS_MEMSET(&cfw_config, 0, sizeof(cfw_config));
		svcCustomKernelCommand(KERNEL_GET_CFW_CONFIG, &cfw_config);

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
