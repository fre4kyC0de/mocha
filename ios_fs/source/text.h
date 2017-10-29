#ifndef TEXT_H
#define TEXT_H

#include "../../src/dynamic_libs/os_types.h"

void clearScreen(u32 color);
void clearLine(int y, u32 color);
void drawString(char* str, int x, int y);
void _printf(int x, int y, const char *format, ...);

#endif
