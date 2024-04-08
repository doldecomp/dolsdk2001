#ifndef _DOLPHIN_MCC_H_
#define _DOLPHIN_MCC_H_

enum MCC_CHANNEL {
    MCC_CHANNEL_SYSTEM = 0,
    MCC_CHANNEL_1 = 1,
    MCC_CHANNEL_2 = 2,
    MCC_CHANNEL_3 = 3,
    MCC_CHANNEL_4 = 4,
    MCC_CHANNEL_5 = 5,
    MCC_CHANNEL_6 = 6,
    MCC_CHANNEL_7 = 7,
    MCC_CHANNEL_8 = 8,
    MCC_CHANNEL_9 = 9,
    MCC_CHANNEL_10 = 10,
    MCC_CHANNEL_11 = 11,
    MCC_CHANNEL_12 = 12,
    MCC_CHANNEL_13 = 13,
    MCC_CHANNEL_14 = 14,
    MCC_CHANNEL_15 = 15,
};

typedef struct {
    /* 0x00 */ u8 firstBlock;
    /* 0x01 */ u8 blockLength;
    /* 0x02 */ u8 connect;
    /* 0x03 */ u8 isLocked;
} MCC_Info;

enum MCC_EXI {
    MCC_EXI_0 = 0,
    MCC_EXI_1 = 1,
    MCC_EXI_2 = 2,
};

enum MCC_SYSEVENT {
    MCC_SYSEVENT_UNKNOWN = 0,
    MCC_SYSEVENT_REBOOT = 1,
    MCC_SYSEVENT_INITIALIZED = 2,
    MCC_SYSEVENT_PING = 3,
    MCC_SYSEVENT_PING_RESULT = 4,
    MCC_SYSEVENT_FLUSH = 5,
};

typedef void (* MCC_CBEvent)(enum MCC_CHANNEL channel, u32, u32);
typedef void (* MCC_CBSysEvent)(enum MCC_SYSEVENT sysEvent);

typedef struct {
    /* 0x00 */ u32 length;
    /* 0x04 */ u16 rsvd;
    /* 0x06 */ u16 protocol;
} MCC_Hdr;

// unsorted externs
extern int MCCGetChannelInfo(enum MCC_CHANNEL chID, MCC_Info *info);
extern int MCCInit(enum MCC_EXI exiChannel, u8 timeout, MCC_CBSysEvent callbackSysEvent);
extern int MCCOpen(enum MCC_CHANNEL chID, u8 blockSize, MCC_CBEvent callbackEvent);
extern u32 MCCSetChannelEventMask(enum MCC_CHANNEL chID, int unk);
extern int MCCClose(enum MCC_CHANNEL chID);
extern int MCCNotify(enum MCC_CHANNEL chID, int unk);
extern int MCCWrite(enum MCC_CHANNEL chID, u32 offset, void *data, long size, int unk);

#endif // _DOLPHIN_MCC_H_
