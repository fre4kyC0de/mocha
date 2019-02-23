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

#include "imports.h"

void _main()
{
	int saved_handle = *(volatile int*)0x0012F000;
	int myret = IOSUSB_reply(saved_handle, 0);
	if (myret != 0)
		IOSUSB_ios_shutdown(1);

	// stack pointer will be 0x1016AE30
	// link register will be 0x1012EACC
	asm("LDR SP, newsp\n"
		"LDR R0, newr0\n"
		"LDR LR, newlr\n"
		"LDR PC, newpc\n"
		"newsp: .word 0x1016AE30\n"
		"newlr: .word 0x1012EACC\n"
		"newr0: .word 0x10146080\n"
		"newpc: .word 0x10111164\n");
}
