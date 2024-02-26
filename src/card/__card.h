#ifndef _DOLPHIN_CARD_INTERNAL_H_
#define _DOLPHIN_CARD_INTERNAL_H_

#include <dolphin/card.h>

// unsorted includes
extern struct CARDDir *__CARDGetDirBlock(struct CARDControl *card);
extern long __CARDUpdateDir(long chan, void (*callback)(long, long));
extern unsigned short *__CARDGetFatBlock(struct CARDControl *card);
extern long __CARDEraseSector(long chan, int size, void *callback);
extern long __CARDSeek(struct CARDFileInfo *, long, long, struct CARDControl **);
extern void __CARDDefaultApiCallback();
extern void __CARDSyncCallback();
extern long __CARDGetControlBlock(long chan, struct CARDControl **card);
extern long __CARDAccess(struct CARDDir *dir);
extern long __CARDIsPublic(struct CARDDir *dir);
extern long __CARDPutControlBlock(struct CARDControl *card, long result);

// CARDStatEx.c
long __CARDGetStatusEx(long chan, long fileNo, struct CARDDir * dirent);
long __CARDSetStatusExAsync(long chan, long fileNo, struct CARDDir * dirent, void (* callback)(long, long));
long __CARDSetStatusEx(long chan, long fileNo, struct CARDDir * dirent);

#endif // _DOLPHIN_CARD_INTERNAL_H_
