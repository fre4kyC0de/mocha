#ifndef IMPORTS_H
#define IMPORTS_H

#include <stdlib.h>
#include <stdarg.h>
#include "../../src/dynamic_libs/os_types.h"

#define MCP_SVC_BASE ((void*)0x050567EC)

void usleep(u32 time);

#endif
