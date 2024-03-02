#ifndef _DOLPHIN_CARD_INTERNAL_H_
#define _DOLPHIN_CARD_INTERNAL_H_

#include <dolphin/card.h>
#include <dolphin/exi.h>

// unsorted includes
extern DVDDiskID * __CARDDiskID;
extern DVDDiskID __CARDDiskNone;
extern struct CARDControl __CARDBlock[2];

extern long __CARDEraseSector(long chan, int size, void *callback);
extern void __CARDDefaultApiCallback();
extern void __CARDSyncCallback();
extern void __CARDUnlockedHandler();
extern void __CARDExtHandler();
extern void __CARDExiHandler(s32 chan, OSContext *context);
extern long __CARDGetControlBlock(long chan, struct CARDControl **card);
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

// CARDOpen.c
BOOL __CARDCompareFileName(CARDDir *ent, const char *fileName);
s32 __CARDAccess(CARDDir *ent);
s32 __CARDIsPublic(CARDDir *ent);
s32 __CARDGetFileNo(CARDControl* card, const char* fileName, s32* pfileNo);
BOOL __CARDIsOpened(CARDControl *card, s32 fileNo);

// CARDMount.c
void __CARDMountCallback(s32 chan, s32 result);

// CARDFormat.c
s32 CARDFormatAsync(s32 chan, CARDCallback callback);

// CARDDir.c
CARDDir *__CARDGetDirBlock(CARDControl* card);
s32 __CARDUpdateDir(s32 chan, CARDCallback callback);

// CARDCheck.c
void __CARDCheckSum(void *ptr, int length, u16 *checksum, u16 *checksumInv);
s32 __CARDVerify(CARDControl *card);

// CARDBlock.c
void *__CARDGetFatBlock(CARDControl *card);
s32 __CARDAllocBlock(s32 chan, u32 cBlock, CARDCallback callback);
s32 __CARDFreeBlock(s32 chan, u16 nBlock, CARDCallback callback);
s32 __CARDUpdateFatBlock(s32 chan, u16 *fat, CARDCallback callback);

#endif // _DOLPHIN_CARD_INTERNAL_H_
