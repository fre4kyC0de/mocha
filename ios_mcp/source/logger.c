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

#include <stdarg.h>
#include <string.h>

#include "../../src/dynamic_libs/os_types.h"
#include "imports.h"
#include "socket.h"
#include "logger.h"

#ifdef DEBUG_LOGGER
static int log_socket = 0;

int log_init(unsigned int ipAddress)
{
	log_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (log_socket < 0)
		return log_socket;

	struct sockaddr_in connect_addr;
	IOSMCPPAYLOAD_memset(&connect_addr, 0, sizeof(connect_addr));
	connect_addr.sin_family = AF_INET;
	connect_addr.sin_port = 4405;
	connect_addr.sin_addr.s_addr = ipAddress;

	if (connect(log_socket, (struct sockaddr*)&connect_addr, sizeof(connect_addr)) < 0)
	{
		closesocket(log_socket);
		log_socket = -1;
	}

	return log_socket;
}

void log_deinit()
{
	if (log_socket >= 0)
	{
		closesocket(log_socket);
		log_socket = -1;
	}
}

static void log_print(const char *str, int len)
{
	if (log_socket < 0)
		return;

	int ret;
	while (len > 0)
	{
		int block = len < 1400 ? len : 1400; // take max 1400 bytes per UDP packet
		ret = send(log_socket, str, block, 0);
		if (ret < 0)
			break;

		len -= ret;
		str += ret;
	}
}

void log_printf(const char *format, ...)
{
	if (log_socket < 0)
		return;

	va_list args;
	va_start(args, format);

	char buffer[0x100];

	int len = IOSMCP_vsnprintf(buffer, sizeof(buffer), format, args);
	log_print(buffer, len);

	va_end(args);
}
#endif // DEBUG_LOGGER
