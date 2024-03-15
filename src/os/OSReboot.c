#if DOLPHIN_REVISION >= 37

// We currently do not have debug info for this file, so parameters and local
// variables are guessed.

#include <dolphin.h>
#include <dolphin/os.h>

extern void __DVDPrepareResetAsync();

#include "__os.h"

struct bb2struct
{
    u32 _00;
    u32 _04;
    s32 _08;  // size?
    u32 FSTMaxLength;
    void* FSTLocationInRam;
    u32 unk14;
    u32 unk18;
};

static struct bb2struct Header;
static volatile BOOL Prepared;

extern u32 UNK_817FFFF8 : 0x817FFFF8;
extern u32 UNK_817FFFFC : 0x817FFFFC;
extern u8 UNK_800030E2 : 0x800030E2;

#pragma dont_inline on
static void Run(register void *arg0)
{
    register void *dumb = arg0;

    OSDisableInterrupts();
    ICFlashInvalidate();
    asm {
        sync
        isync
        mtlr arg0
        blr
    }
}
#pragma dont_inline reset

static void Callback(void) {

    Prepared = TRUE;
}

#pragma peephole off

static void someinline(void *ptr, u32 size, u32 arg2)
{
    DVDCommandBlock sp40;
    u32 unused;

    while (!Prepared) {
    }
    DVDReadAbsAsyncForBS(&sp40, ptr, size, arg2, NULL);
    while(1)
    {
        switch (sp40.state) {
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
        case 0:
            return;
        default:
        case 1:
            break;
        }
    }
}

void __OSReboot(unsigned long resetCode, int forceMenu /* unused */)
{
    OSContext sp70;
    u32 r30;
    u32 r4;

    OSDisableInterrupts();
    UNK_817FFFFC = resetCode;
    UNK_817FFFF8 = 0;
    UNK_800030E2 = 1;
    OSClearContext(&sp70);
    OSSetCurrentContext(&sp70);
    DVDInit();
    __DVDPrepareResetAsync(Callback);
    if (!DVDCheckDisk()) {
        __OSDoHotReset(UNK_817FFFFC);
    }
    __OSMaskInterrupts(~0x1F);
    __OSUnmaskInterrupts(0x400);
    OSEnableInterrupts();
    someinline(&Header, 32, 0x2440);
  
    
    r4 = Header.unk14 + 32;
    r30 = OSRoundUp32B(Header.unk18);
    someinline((void *)0x81300000, r30, (u32)(r4 + 0x2440));
    
    ICInvalidateRange((void *)0x81300000, r30);
    Run((void *)0x81300000);
}

#endif  // DOLPHIN_REVISION >= 37
