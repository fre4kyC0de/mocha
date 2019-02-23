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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "imports.h"
#include "wupserver.h"
#include "ipc.h"
#include "svc.h"
#include "text.h"
#include "../../common/config_types.h"
#include "../../common/kernel_commands.h"

static int threadsStarted = 0;

int _startMainThread(void)
{
	if (threadsStarted == 0)
	{
		threadsStarted = 1;
		cfw_config_t cfw_config;
		IOSMCPPAYLOAD_memset(&cfw_config, 0, sizeof(cfw_config));
		svcCustomKernelCommand(KERNEL_COMMAND_GET_CFW_CONFIG, &cfw_config);

		if (cfw_config.launchImage)
			drawSplashScreen();

		if (cfw_config.wupserver)
			wupserver_init();

		ipc_init();
	}
	return 0;
}
