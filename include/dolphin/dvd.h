#ifndef _DOLPHIN_DVD_H_
#define _DOLPHIN_DVD_H_

#include <dolphin/types.h>

typedef struct DVDDiskID
{
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize;
    u8 padding[22];
} DVDDiskID;

typedef struct DVDCommandBlock DVDCommandBlock;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock *block);
struct DVDCommandBlock
{
    /*0x00*/ DVDCommandBlock *next;
    /*0x04*/ DVDCommandBlock *prev;
    /*0x08*/ u32 command;
    /*0x0C*/ s32 state;
    /*0x10*/ u32 offset;
    /*0x14*/ u32 length;
    /*0x18*/ void *addr;
    /*0x1C*/ u32 currTransferSize;
    /*0x20*/ u32 transferredSize;
    /*0x24*/ DVDDiskID *id;
    /*0x28*/ DVDCBCallback callback;
    /*0x2C*/ void *userData;
};

typedef struct DVDFileInfo  DVDFileInfo;
typedef void (*DVDCallback)(s32 result, DVDFileInfo *fileInfo);
struct DVDFileInfo
{
	/*0x00*/ DVDCommandBlock cb;
    /*0x30*/ u32 startAddr;
    /*0x34*/ u32 length;
    /*0x38*/ DVDCallback callback;
};

typedef struct
{
    u32 entryNum;
    u32 location;
    u32 next;
} DVDDir;

typedef struct 
{
    u32 entryNum;
    BOOL isDir;
    char *name;
} DVDDirEntry;

typedef struct DVDBB2 {
    /* 0x00 */ u32 bootFilePosition;
    /* 0x04 */ u32 FSTPosition;
    /* 0x08 */ u32 FSTLength;
    /* 0x0C */ u32 FSTMaxLength;
    /* 0x10 */ void * FSTAddress;
    /* 0x14 */ u32 userPosition;
    /* 0x18 */ u32 userLength;
    /* 0x1C */ u32 padding0;
} DVDBB2;

void DVDInit(void);
s32 DVDCancel(DVDCommandBlock *block);
s32 DVDGetDriveStatus(void);
void DVDDumpWaitingQueue(void);
int DVDLowRead(void * addr, unsigned long length, unsigned long offset, void (* callback)(unsigned long));
int DVDLowSeek(unsigned long offset, void (* callback)(unsigned long));
int DVDLowWaitCoverClose(void (* callback)(unsigned long));
int DVDLowReadDiskID(struct DVDDiskID * diskID, void (* callback)(unsigned long));
int DVDLowStopMotor(void (* callback)(unsigned long));
int DVDLowRequestError(void (* callback)(unsigned long));
int DVDLowInquiry(struct DVDDriveInfo * info, void (* callback)(unsigned long));
int DVDLowAudioStream(unsigned long subcmd, unsigned long length, unsigned long offset, void (* callback)(unsigned long));
int DVDLowRequestAudioStatus(unsigned long subcmd, void (* callback)(unsigned long));
int DVDLowAudioBufferConfig(int enable, unsigned long size, void (* callback)(unsigned long));
void DVDLowReset();
void (* DVDLowSetResetCoverCallback(void (* callback)(unsigned long)))(unsigned long);
int DVDLowBreak();
void (* DVDLowClearCallback())(unsigned long);
unsigned long DVDLowGetCoverStatus();

// dvdfs.c
s32 DVDConvertPathToEntrynum(char* pathPtr);
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
BOOL DVDOpen(char* fileName, DVDFileInfo* fileInfo);
BOOL DVDClose(DVDFileInfo* fileInfo);
BOOL DVDGetCurrentDir(char* path, u32 maxlen);
BOOL DVDChangeDir(char* dirName);
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio);
long DVDReadPrio(struct DVDFileInfo * fileInfo, void * addr, long length, long offset, long prio);
int DVDSeekAsyncPrio(struct DVDFileInfo * fileInfo, long offset, void (* callback)(long, struct DVDFileInfo *), long prio);
long DVDSeekPrio(struct DVDFileInfo * fileInfo, long offset, long prio);
long DVDGetFileInfoStatus(struct DVDFileInfo * fileInfo);
int DVDOpenDir(char * dirName, DVDDir * dir);
int DVDReadDir(DVDDir * dir, DVDDirEntry* dirent);
int DVDCloseDir(DVDDir* dir);
void * DVDGetFSTLocation();
BOOL DVDPrepareStreamAsync(DVDFileInfo* fileInfo, u32 length, u32 offset, DVDCallback callback);
s32 DVDPrepareStream(DVDFileInfo* fileInfo, u32 length, u32 offset);
s32 DVDGetTransferredSize(DVDFileInfo* fileinfo);

#define DVDReadAsync(fileInfo, addr, length, offset, callback) \
    DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)

#define DVD_RESULT_GOOD        0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED     -2
#define DVD_RESULT_CANCELED    -6

#define DVD_STATE_FATAL_ERROR   -1
#define DVD_STATE_END            0
#define DVD_STATE_BUSY           1
#define DVD_STATE_WAITING        2
#define DVD_STATE_COVER_CLOSED   3
#define DVD_STATE_NO_DISK        4
#define DVD_STATE_COVER_OPEN     5
#define DVD_STATE_WRONG_DISK     6
#define DVD_STATE_MOTOR_STOPPED  7
#define DVD_STATE_PAUSING        8
#define DVD_STATE_IGNORED        9
#define DVD_STATE_CANCELED       10
#define DVD_STATE_RETRY          11

#define DVD_MIN_TRANSFER_SIZE    32

// unidentified externs
extern int DVDReadAbsAsyncForBS(struct DVDCommandBlock * block, void * addr, long length, long offset, void (* callback)(long, struct DVDCommandBlock *));
extern int DVDReadDiskID(struct DVDCommandBlock * block, struct DVDDiskID * diskID, void (* callback)(long, struct DVDCommandBlock *));

#endif
