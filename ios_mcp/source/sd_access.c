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

void patch_SD_access_check(void)
{
	__asm__ volatile(
		".thumb\n"
		//clobbered instructions
		"add r0, r7, r2\n"
		//app.permissions.r2.mask seems to be 0xFFFFFFFFFFFFFFFF for every application
		"ldr r1, =0x32\n"
		"sub r3, r3, #7\n"
		"strb r1, [r3]\n"
		//this instruction was also clobbered but we use r1 so we do it after our patch stuff
		"movs r1, #0\n"
		"bx lr");
}
