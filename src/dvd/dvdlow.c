#include <dolphin.h>
#include <dolphin/dvd.h>

#include "__dvd.h"
#include "__os.h"

// .sbss
#if DOLPHIN_REVISION >= 37
static volatile BOOL StopAtNextInt;
static u32 LastLength;
#endif
static void (* Callback)(unsigned long); // size: 0x4, address: 0x0
static void (* ResetCoverCallback)(unsigned long); // size: 0x4, address: 0x4
static volatile long long LastResetEnd; // size: 0x8, address: 0x8
static volatile unsigned long ResetOccurred; // size: 0x4, address: 0x10
static volatile int WaitingCoverClose; // size: 0x4, address: 0x14
static volatile int Breaking; // size: 0x4, address: 0x18
#if DOLPHIN_REVISION >= 37
static volatile u32 WorkAroundType;
static s32 WorkAroundSeekLocation;
static volatile OSTime LastReadFinished;
static OSTime LastReadIssued;
static volatile BOOL LastCommandWasRead;
static volatile s32 NextCommandNumber;
static BOOL FirstRead = TRUE;
#endif

#if DOLPHIN_REVISION >= 37
static struct DVDCommand {
    s32 command;
    void *address;
    u32 length;
    u32 offset;
    void (*callback)(u32);
} CommandList[3];
static OSAlarm AlarmForWA;
static OSAlarm AlarmForTimeout;
static OSAlarm AlarmForBreak;
struct SomeStruct {
    void *addr;
    u32 length;
    u32 offset;
};
static struct SomeStruct Prev;
static struct SomeStruct Curr;
#endif

#if DOLPHIN_REVISION >= 37
static void Read(void *addr, u32 length, u32 offset, void (*callback)(u32));
void __DVDLowSetWAType(u32 type, s32 seekLoc);

void __DVDInitWA(void) {
    NextCommandNumber = 0;
    CommandList[0].command = -1;
    __DVDLowSetWAType(0, 0);
    OSInitAlarm();
}

static BOOL ProcessNextCommand(void) {
    s32 n = NextCommandNumber;

    if (CommandList[n].command == 1) {
        NextCommandNumber++;
        Read(
            CommandList[n].address,
            CommandList[n].length,
            CommandList[n].offset,
            CommandList[n].callback);
        return TRUE;
    } else if (CommandList[n].command == 2) {
        NextCommandNumber++;
        DVDLowSeek(CommandList[n].offset, CommandList[n].callback);
        return TRUE;
    }
    return FALSE;
}
#endif

void __DVDInterruptHandler(short unused, struct OSContext * context) {
    struct OSContext exceptionContext;
    unsigned long cause;
    unsigned long reg;
    unsigned long intr;
    unsigned long mask;

    cause = 0;
#if DOLPHIN_REVISION >= 37
    OSCancelAlarm(&AlarmForTimeout);
    if (LastCommandWasRead) {
        LastReadFinished = __OSGetSystemTime();
        FirstRead = FALSE;
        Prev.addr   = Curr.addr;
        Prev.length = Curr.length;
        Prev.offset = Curr.offset;
        if (StopAtNextInt == TRUE) {
            cause |= 8;
        }
    }
    LastCommandWasRead = FALSE;
    StopAtNextInt = FALSE;
#endif
    reg = __DIRegs[0];
    mask = (reg & 0x0000002A);
    intr = (reg & 0x00000054) & (mask << 1);
    if (intr & 0x40) {
        cause |= 8;
    }
    if (intr & 0x10) {
        cause |= 1;
    }
    if (intr & 4) {
        cause |= 2;
    }
    if (cause != 0) {
        ResetOccurred = 0U;
    }
    __DIRegs[0] = (intr | mask);

    if ((ResetOccurred != 0) && ((__OSGetSystemTime() - LastResetEnd)) < OSMillisecondsToTicks(200)) {
        reg = __DIRegs[1];
        mask = (reg & 2);
        intr = (reg & 4) & (mask << 1);
        if (intr & 4) {
            if (ResetCoverCallback) {
                ResetCoverCallback(4);
            }
            ResetCoverCallback = NULL;
        }
        __DIRegs[1] = __DIRegs[1];
    } else if (WaitingCoverClose) {
        reg = __DIRegs[1];
        mask = (reg & 2);
        intr = (reg & 4) & (mask << 1);
        if (intr & 4) {
            cause |= 4;
        }
        __DIRegs[1] = (intr | mask);
        WaitingCoverClose = 0;
    } else {
        __DIRegs[1] = 0;
    }

    if ((cause & 8) && (Breaking == 0)) {
        cause &= 0xFFFFFFF7;
    }
#if DOLPHIN_REVISION >= 37
    if (cause & 1) {
        if (ProcessNextCommand()) {
            return;
        }
    }
    else {
        CommandList[0].command = -1;
        NextCommandNumber = 0;
    }
#endif
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (cause != 0) {
#if DOLPHIN_REVISION >= 37
        void (*cb)() = Callback;
        Callback = NULL;
        if (cb) {
            cb(cause);
        }
#else
        if (Callback) {
            Callback(cause);
        }
#endif
        Breaking = 0;
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

#if DOLPHIN_REVISION >= 37
static void AlarmHandler(OSAlarm *alarm, OSContext *context) {
    ProcessNextCommand();
}

static void AlarmHandlerForTimeout(OSAlarm *alarm, OSContext *context) {
    void (*cb)(u32);
    OSContext exceptionContext;

    __OSMaskInterrupts(0x400);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    cb = Callback;
    Callback = NULL;
    if (cb) {
        cb(0x10);
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

static void SetTimeoutAlarm(s64 timeout) {
    OSCreateAlarm(&AlarmForTimeout);
    OSSetAlarm(&AlarmForTimeout, timeout, AlarmHandlerForTimeout);
}

static void Read(void *addr, u32 length, u32 offset, void (*callback)(u32)) {
    u32 r30;
    StopAtNextInt = FALSE;
    Callback = callback;
    LastCommandWasRead = TRUE;
    LastReadIssued = __OSGetSystemTime();
    __DIRegs[2] = 0xA8000000;
    __DIRegs[3] = (offset >> 2);
    __DIRegs[4] = (length);
    __DIRegs[5] = (u32)addr;
    __DIRegs[6] = (length);
    LastLength = length;
    __DIRegs[7] = 3;
    if (length > 0xA00000) {
        r30 = OS_TIMER_CLOCK * 20;
        OSCreateAlarm(&AlarmForTimeout);
        OSSetAlarm(&AlarmForTimeout, r30, AlarmHandlerForTimeout);
    } else {
        r30 = OS_TIMER_CLOCK * 10;
        OSCreateAlarm(&AlarmForTimeout);
        OSSetAlarm(&AlarmForTimeout, r30, AlarmHandlerForTimeout);
    }
}

static void DoJustRead(void *addr, u32 length, u32 offset, void (*callback)(u32)) {
    CommandList[0].command = -1;
    NextCommandNumber = 0;
    Read(addr, length, offset, callback);
}

static void SeekTwiceBeforeRead(void *addr, u32 length, u32 offset, void (*callback)(u32)) {
    u32 offsetToSeek;

    if ((offset & ~0x7FFF) == 0) {
        offsetToSeek = 0;
    } else {
        offsetToSeek = (offset & ~0x7FFF) + WorkAroundSeekLocation;
    }

    CommandList[0].command = 2;
    CommandList[0].offset = offsetToSeek;
    CommandList[0].callback = callback;

    CommandList[1].command = 1;
    CommandList[1].address = addr;
    CommandList[1].length = length;
    CommandList[1].offset = offset;
    CommandList[1].callback = callback;

    CommandList[2].command = -1;

    NextCommandNumber = 0;
    DVDLowSeek(offsetToSeek, callback);
}

static void WaitBeforeRead(void* addr, u32 length, u32 offset, void (* callback)(unsigned long),
                           OSTime timeout) {
    CommandList[0].command = 1;
    CommandList[0].address = addr;
    CommandList[0].length = length;
    CommandList[0].offset = offset;
    CommandList[0].callback = callback;
    CommandList[1].command = -1;
    NextCommandNumber = 0;
    OSCreateAlarm(&AlarmForWA);
    OSSetAlarm(&AlarmForWA, timeout, AlarmHandler);
}

static BOOL AudioBufferOn(void) {
    struct DVDDiskID *id = DVDGetCurrentDiskID();

    if (id->streaming)
        return TRUE;
    else
        return FALSE;
}

static BOOL HitCache(struct SomeStruct *cur, struct SomeStruct *prev) {
    u32 blockNumOfPrevEnd = (prev->offset + prev->length - 1) >> 15;
    u32 blockNumOfCurrStart = (cur->offset >> 15);
    u32 cacheBlockSize = AudioBufferOn() ? 5 : 15;

    if (blockNumOfCurrStart > blockNumOfPrevEnd - 2
     || blockNumOfCurrStart < blockNumOfPrevEnd + cacheBlockSize + 3) {
        return TRUE;
    } else {
        return FALSE;
    }
}

#endif

int DVDLowRead(void * addr, unsigned long length, unsigned long offset, void (* callback)(unsigned long)) {
#if DOLPHIN_REVISION >= 37
    unsigned long blockNumOfPrevEnd; // r25
    long long diff; // r22
#endif

    ASSERTMSGLINE(0x143, !OFFSET(addr, 32), "DVDLowRead(): address must be aligned with 32 byte boundary.");
    ASSERTMSGLINE(0x144, !(length % 32), "DVDLowRead(): length must be a multiple of 32.");
    ASSERTMSGLINE(0x145, !(offset % 4), "DVDLowRead(): offset must be a multiple of 4.");
    ASSERTMSGLINE(0x147, (length != 0), "DVD read: 0 was specified to length of the read\n");
#if DOLPHIN_REVISION >= 37
    __DIRegs[6] = length;
    Curr.addr = addr;
    Curr.length = length;
    Curr.offset = offset;

    if (WorkAroundType == 0) {
        DoJustRead(addr, length, offset, callback);
    } else if (WorkAroundType == 1) {
        if (FirstRead) {
            SeekTwiceBeforeRead(addr, length, offset, callback);
        } else if (!HitCache(&Curr, &Prev)) {
            DoJustRead(addr, length, offset, callback);
        } else {
        
            blockNumOfPrevEnd = (Prev.offset + Prev.length - 1) >> 15;
            if (blockNumOfPrevEnd == (Curr.offset >> 15)
             || blockNumOfPrevEnd + 1 == (Curr.offset >> 15)) {
                 diff = __OSGetSystemTime() - LastReadFinished;
                 if (OSMillisecondsToTicks(5) < diff) {
                     DoJustRead(addr, length, offset, callback);
                 } else {
                    WaitBeforeRead(addr, length, offset, callback, OSMillisecondsToTicks(5) - diff + OSMicrosecondsToTicks(500));
                }
            } else {
                SeekTwiceBeforeRead(addr, length, offset, callback);
            }
        }
    }
    return 1;
#else
    Callback = callback;
    __DIRegs[2] = 0xA8000000;
    __DIRegs[3] = (offset >> 2);
    __DIRegs[4] = (length);
    __DIRegs[5] = (u32)addr;
    __DIRegs[6] = (length);
    __DIRegs[7] = 3;
    return 1;
#endif
}

int DVDLowSeek(unsigned long offset, void (* callback)(unsigned long)) {
    ASSERTMSGLINE(0x166, !(offset % 4), "DVDLowSeek(): offset must be a multiple of 4.");
    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[2] = 0xAB000000;
    __DIRegs[3] = (offset >> 2);
    __DIRegs[7] = 1;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

int DVDLowWaitCoverClose(void (* callback)(unsigned long)) {
    Callback = callback;
    WaitingCoverClose = 1;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[1] = 2;
    return 1;
}

int DVDLowReadDiskID(struct DVDDiskID * diskID, void (* callback)(unsigned long)) {
    ASSERTMSGLINE(0x19B, !OFFSET(diskID, 32), "DVDLowReadID(): id must be aligned with 32 byte boundary.");
    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[2] = 0xA8000040;
    __DIRegs[3] = 0;
    __DIRegs[4] = 0x20;
    __DIRegs[5] = (u32)diskID;
    __DIRegs[6] = 0x20;
    __DIRegs[7] = 3;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

int DVDLowStopMotor(void (* callback)(unsigned long)) {
    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[2] = 0xE3000000;
    __DIRegs[7] = 1;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

int DVDLowRequestError(void (* callback)(unsigned long)) {
    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[2] = 0xE0000000;
    __DIRegs[7] = 1;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

int DVDLowInquiry(struct DVDDriveInfo * info, void (* callback)(unsigned long)) {
    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[2] = 0x12000000;
    __DIRegs[4] = 0x20;
    __DIRegs[5] = (u32)info;
    __DIRegs[6] = 0x20;
    __DIRegs[7] = 3;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

int DVDLowAudioStream(unsigned long subcmd, unsigned long length, unsigned long offset, void (* callback)(unsigned long)) {
    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[2] = (subcmd | 0xE1000000);
    __DIRegs[3] = (offset >> 2);
    __DIRegs[4] = length;
    __DIRegs[7] = 1;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

int DVDLowRequestAudioStatus(unsigned long subcmd, void (* callback)(unsigned long)) {
    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#endif
    __DIRegs[2] = (subcmd | 0xE2000000);
    __DIRegs[7] = 1;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

int DVDLowAudioBufferConfig(int enable, unsigned long size, void (* callback)(unsigned long)) {
#if DOLPHIN_REVISION < 37
    unsigned long bufSize;
    unsigned long trigger;
#endif

    Callback = callback;
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = FALSE;
#else
    bufSize = size & 0xF;
    trigger = (size >> 4) & 0xF;
    ASSERTLINE(0x25B, bufSize < 16);
    ASSERTLINE(0x25C, trigger <= 2);
#endif
    __DIRegs[2] = ((enable ? 0x10000 : 0) | 0xE4000000 | size);
    __DIRegs[7] = 1;
#if DOLPHIN_REVISION >= 37
    SetTimeoutAlarm(OSSecondsToTicks(10));
#endif
    return 1;
}

void DVDLowReset() {
    unsigned long reg;
    long long resetStart;

    __DIRegs[1] = 2;
    reg = __PIRegs[9];
    __PIRegs[9] = (reg & 0xFFFFFFFB) | 1;
    resetStart = __OSGetSystemTime();
    while((__OSGetSystemTime() - resetStart) < OSMicrosecondsToTicks(12))
        ;
    __PIRegs[9] = (reg | 4 | 1);
    ResetOccurred = 1;
    LastResetEnd = __OSGetSystemTime();
}

void (* DVDLowSetResetCoverCallback(void (* callback)(unsigned long)))(unsigned long) {
    void (* old)(unsigned long);
    int enabled;

    enabled = OSDisableInterrupts();
    old = ResetCoverCallback;
    ResetCoverCallback = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

int DVDLowBreak() {
#if DOLPHIN_REVISION >= 37
    StopAtNextInt = TRUE;
#else
    unsigned long statusReg;

    statusReg = __DIRegs[0];
    statusReg |= 0x41;

    __DIRegs[0] = statusReg;
#endif
    Breaking = 1;
    return 1;
}

void (* DVDLowClearCallback())(unsigned long) {
    void (* old)(unsigned long);

    __DIRegs[1] = 0;
    old = Callback;
    Callback = NULL;
    return old;
}

unsigned long DVDLowGetCoverStatus() {
    if (((__OSGetSystemTime() - LastResetEnd)) < OSMillisecondsToTicks(100)) {
        return 0;
    }
    if (__DIRegs[1] & 1) {
        return 1;
    }
    return 2;
}

#if DOLPHIN_REVISION >= 37
void __DVDLowSetWAType(u32 type, s32 seekLoc)
{
    BOOL enabled = OSDisableInterrupts();
    WorkAroundType = type;
    WorkAroundSeekLocation = seekLoc;
    OSRestoreInterrupts(enabled);
}
#endif
