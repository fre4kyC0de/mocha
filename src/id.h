//id.h
#ifndef _ID_H_
#define _ID_H_

#include "dynamic_libs/os_types.h"

u64 currentTitleId;
u32 currentTilteId_low;
u32 currentTilteId_high;

u64 systemTitleId;
u32 systemTitleId_low;
u32 systemTitleId_high;

#define TitleId_SysMenuEUR	0x0005001010040200ULL
#define TitleId_SysMenuUSA	0x0005001010040100ULL
#define TitleId_SysMenuJPN	0x0005001010040000ULL

#define	TitleId_MiiMakerEUR	0x000500101004A200ULL
#define	TitleId_MiiMakerUSA	0x000500101004A100ULL
#define	TitleId_MiiMakerJPN	0x000500101004A000ULL
#define	TitleId_HBLChannel	0x0005000013374842ULL

#endif
