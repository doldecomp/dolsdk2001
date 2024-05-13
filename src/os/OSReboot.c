#if DOLPHIN_REVISION >= 37

// We currently do not have debug info for this file, so parameters and local
// variables are guessed.

#include <dolphin.h>
#include <dolphin/os.h>

extern void __DVDPrepareResetAsync();

#include "__os.h"

static struct {
    char date[16];
    u32 entry;
    u32 size;
    u32 rebootSize;
    u32 reserved2;
} Header;

#if DOLPHIN_REVISION >= 45
static void *SaveStart;
static void *SaveEnd;
#endif
static volatile BOOL Prepared;

#pragma dont_inline on
static void Run(register void *entryPoint)
{
    entryPoint;  // needed because the compiler is stupid

    OSDisableInterrupts();
    ICFlashInvalidate();
    asm {
        sync
        isync
        mtlr entryPoint
        blr
    }
}
#pragma dont_inline reset

#pragma peephole off

static void ReadApploader(void *addr, long length, long offset)
{
    DVDCommandBlock block;

    while (!Prepared) {
    }
    DVDReadAbsAsyncForBS(&block, addr, length, offset + 0x2440, NULL);
    while(1)
    {
        switch (block.state) {
        case 0:
            return;
        case 1:
            break;
        case -1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            __OSDoHotReset(UNK_817FFFFC);
            break;
        default:
            ASSERTLINE(0xB4, FALSE);
            break;
        }
    }
}

static void Callback(void) {

    Prepared = TRUE;
}

void __OSReboot(unsigned long resetCode, int forceMenu)
{
    u32 offset;
    u32 length;
    OSContext exceptionContext;

#if DOLPHIN_REVISION >= 45 && DEBUG
    if (forceMenu == 1) {
        OSReport("OSResetSystem(): You can't specify TRUE to forceMenu if you Restart. Ignored\n");
    }
#endif

    OSDisableInterrupts();
    UNK_817FFFFC = resetCode;
    UNK_817FFFF8 = 0;
    UNK_800030E2 = 1;
#if DOLPHIN_REVISION >= 45
    UNK_812FDFF0 = SaveStart;
    UNK_812FDFEC = SaveEnd;
#endif
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    DVDInit();
#if DOLPHIN_REVISION >= 45
    DVDSetAutoInvalidation(1);
#endif
    __DVDPrepareResetAsync(Callback);
    if (!DVDCheckDisk()) {
        __OSDoHotReset(UNK_817FFFFC);
    }
    __OSMaskInterrupts(~0x1F);
    __OSUnmaskInterrupts(0x400);
    OSEnableInterrupts();
    ReadApploader(&Header, 32, 0);
#if DOLPHIN_REVISION >= 45
    ASSERTMSGLINE(0x101, Header.rebootSize != 0, "OSResetSystem(): old apploader");
#endif
    offset = Header.size + 32;
    length = OSRoundUp32B(Header.rebootSize);
    ReadApploader((void *)0x81300000, length, offset);
    
    ICInvalidateRange((void *)0x81300000, length);
    Run((void *)0x81300000);
}

#if DOLPHIN_REVISION >= 45
void OSSetSaveRegion(void *start, void *end)
{
    ASSERTMSGLINE(0x112, (u32)start >= 0x80700000 || start == NULL, "OSSetSaveRegion(): start address should be NULL or higher than 0x80700000\n");
    ASSERTMSGLINE(0x113, 0x81200000 >= (u32)end || end == NULL, "OSSetSaveRegion(): end address should be NULL or lower than 0x81200000\n");
    ASSERTMSGLINE(0x114, !((start == NULL) ^ (end == NULL)), "OSSetSaveRegion(): if either start or end is NULL, both should be NULL\n");
    SaveStart = start;
    SaveEnd = end;
}

void OSGetSaveRegion(void **start, void **end)
{
    *start = SaveStart;
    *end = SaveEnd;
}
#endif

#endif  // DOLPHIN_REVISION >= 37
