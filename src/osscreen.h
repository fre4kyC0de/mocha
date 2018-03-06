#ifndef _OSSCREEN_H_
#define _OSSCREEN_H_

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define x_offset	-2

void console_print_pos(int x, int y, const char *format, ...);
void console_print_header();
void console_clear();

#endif
