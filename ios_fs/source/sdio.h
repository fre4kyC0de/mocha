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

#ifndef _SDIO_H_
#define _SDIO_H_

#define	SDIO_WRITE		0
#define	SDIO_READ		1

void sdcard_init(void);
void sdcard_lock_mutex(void);
void sdcard_unlock_mutex(void);
int sdcard_readwrite(int is_read, void *data, u32 cnt, u32 block_size, u32 offset_blocks, int * out_callback_arg, int device_id);

#endif // _SDIO_H_
