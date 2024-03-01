#ifndef _DOLPHIN_CARD_INTERNAL_H_
#define _DOLPHIN_CARD_INTERNAL_H_

#include <dolphin/card.h>

// unsorted includes
extern DVDDiskID * __CARDDiskID;
extern struct CARDControl __CARDBlock[2];

extern CARDDir *__CARDGetDirBlock(struct CARDControl *card);
extern long __CARDUpdateDir(long chan, void (*callback)(long, long));
extern unsigned short *__CARDGetFatBlock(struct CARDControl *card);
extern long __CARDEraseSector(long chan, int size, void *callback);
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

// CARDUnlock.c
s32 __CARDUnlock(s32 chan, u8 flashID[12]);

// CARDRead.c
s32 __CARDSeek(CARDFileInfo *fileInfo, s32 length, s32 offset, CARDControl **pcard);

// CARDRdwr.c
long __CARDRead(long chan, unsigned long addr, long length, void * dst, void (* callback)(long, long));
long __CARDWrite(long chan, unsigned long addr, long length, void * dst, void (* callback)(long, long));

// CARDRaw.c
long __CARDRawReadAsync(long chan, void * buf, long length, long offset, void (* callback)(long, long));
long __CARDRawRead(long chan, void * buf, long length, long offset);

#endif // _DOLPHIN_CARD_INTERNAL_H_
