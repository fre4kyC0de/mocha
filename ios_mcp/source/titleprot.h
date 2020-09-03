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

// THUMB_BL
static const unsigned char titleprot_bin[] /*__attribute__((section(".text")))*/ = {
  0x48, 0x05, // ldr r0, =#0x00050000
  0x42, 0x88, // cmp r0, r1
  0xd1, 0x02, // bne _nomatch
  0x48, 0x06, // ldr r0, =#0xffffffff [target]
  0x42, 0x90, // cmp r0, r2
  0xd0, 0x02, // beq _match
  // _nomatch:
  // original code
  0x20, 0x9f, // movs r0, #0x9f
  0x23, 0x9f, // movs r6, #0x9f
  0x47, 0x70, // bx lr
  // _match:
  // return delete error
  0x20, 0x00, // movs r0, #0
  0x4b, 0x01, // ldr r3, =#0x05108237
  0x47, 0x18, // bx r3
  // ------------------------
  0x00, 0x05, 0x00, 0x00, // dcd #0x00050000
  0x05, 0x10, 0x82, 0x37, // dcd #0x05108237
  0xff, 0xff, 0xff, 0xff // dcd #0xffffffff [target]
};
static const unsigned int titleprot_bin_len = 36;

// THUMB_BL
static const unsigned char titleprot2_bin[] /*__attribute__((section(".text")))*/ = {
  0x69, 0x20, // ldr r0, [r4,#0x10]
  0x49, 0x06, // ldr r1, =#0x00050000
  0x42, 0x88, // cmp r0, r1
  0xd1, 0x06, // bne _exit_ok
  0x69, 0x60, // ldr r0, [r4,#0x14]
  0x49, 0x07, // ldr r1, =#0xffffffff [target]
  0x42, 0x88, // cmp r0, r1
  0xd1, 0x02, // bne _exit_ok
  // return our error message
  0x48, 0x03, // ldr r0, =#0x99999999
  0x4b, 0x04, // ldr r3, =#0x050146A5
  0x47, 0x18, // bx r3
  // _exit_ok:
  // original code
  0x0c, 0x1a, // lsrs r2, r3, #0x10
  0x04, 0x13, // lsls r3, r2, #0x10
  0x47, 0x70, // bx lr
  // ------------------------
  0x00, 0x05, 0x00, 0x00, // dcd #0x00050000
  0x99, 0x99, 0x99, 0x99, // dcd #0x99999999
  0x05, 0x01, 0x46, 0xa5, // dcd #0x050146A5
  0xff, 0xff, 0xff, 0xff // dcd #0xffffffff [target]
};
static const unsigned int titleprot2_bin_len = 44;
