#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <dolphin/hw_regs.h>
#include <macros.h>

#include "__os.h"
#include "__vi.h"

typedef struct {
    // total size: 0x26
    u8 equ; // offset 0x0, size 0x1
    u16 acv; // offset 0x2, size 0x2
    u16 prbOdd; // offset 0x4, size 0x2
    u16 prbEven; // offset 0x6, size 0x2
    u16 psbOdd; // offset 0x8, size 0x2
    u16 psbEven; // offset 0xA, size 0x2
    u8 bs1; // offset 0xC, size 0x1
    u8 bs2; // offset 0xD, size 0x1
    u8 bs3; // offset 0xE, size 0x1
    u8 bs4; // offset 0xF, size 0x1
    u16 be1; // offset 0x10, size 0x2
    u16 be2; // offset 0x12, size 0x2
    u16 be3; // offset 0x14, size 0x2
    u16 be4; // offset 0x16, size 0x2
    u16 nhlines; // offset 0x18, size 0x2
    u16 hlw; // offset 0x1A, size 0x2
    u8 hsy; // offset 0x1C, size 0x1
    u8 hcs; // offset 0x1D, size 0x1
    u8 hce; // offset 0x1E, size 0x1
    u8 hbe640; // offset 0x1F, size 0x1
    u16 hbs640; // offset 0x20, size 0x2
    u8 hbeCCIR656; // offset 0x22, size 0x1
    u16 hbsCCIR656; // offset 0x24, size 0x2
} VITiming;

static unsigned long retraceCount; // size: 0x4, address: 0x0
static unsigned long changeMode; // size: 0x4, address: 0x4
static unsigned long flushFlag; // size: 0x4, address: 0x8
static struct OSThreadQueue retraceQueue; // size: 0x8, address: 0xC
static void (*PreCB)(unsigned long); // size: 0x4, address: 0x14
static void (*PostCB)(unsigned long); // size: 0x4, address: 0x18
static unsigned long encoderType; // size: 0x4, address: 0x1C
static signed short displayOffsetH; // size: 0x2, address: 0x20
static signed short displayOffsetV; // size: 0x2, address: 0x22
static volatile u64 changed; // size: 0x8, address: 0x28
static u16 regs[59]; // size: 0x76, address: 0x0
static volatile u64 shdwChanged; // size: 0x8, address: 0x30
static u16 shdwRegs[59]; // size: 0x76, address: 0x78

static VITiming timing[7] =
{
    { 6, 240, 24, 25, 3, 2, 12, 13, 12, 13, 520, 519, 520, 519, 525, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 6, 240, 24, 24, 4, 4, 12, 12, 12, 12, 520, 520, 520, 520, 526, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 5, 287, 35, 36, 1, 0, 13, 12, 11, 10, 619, 618, 617, 620, 625, 432, 64, 75, 106, 172, 380, 133, 420 },
    { 5, 287, 35, 35, 2, 2, 13, 11, 13, 11, 619, 621, 619, 621, 626, 432, 64, 75, 106, 172, 380, 133, 420 },
    { 6, 240, 24, 25, 3, 2, 16, 15, 14, 13, 518, 517, 516, 519, 525, 429, 64, 78, 112, 162, 373, 122, 412 },
    { 6, 240, 24, 24, 4, 4, 16, 14, 16, 14, 518, 520, 518, 520, 526, 429, 64, 78, 112, 162, 373, 122, 412 },
    { 12, 480, 44, 44, 10, 10, 24, 24, 24, 24, 1038, 1038, 1038, 1038, 1050, 429, 64, 71, 105, 162, 373, 122, 496 },
};
static u16 taps[25] = {
    0x01F0, 0x01DC,
    0x01AE, 0x0174,
    0x0129, 0x00DB,
    0x008E, 0x0046,
    0x000C, 0x00E2,
    0x00CB, 0x00C0,
    0x00C4, 0x00CF,
    0x00DE, 0x00EC,
    0x00FC, 0x0008,
    0x000F, 0x0013,
    0x0013, 0x000F,
    0x000C, 0x0008,
    0x0001
};

static struct {
    // total size: 0x58
    unsigned short DispPosX; // offset 0x0, size 0x2
    unsigned short DispPosY; // offset 0x2, size 0x2
    unsigned short DispSizeX; // offset 0x4, size 0x2
    unsigned short DispSizeY; // offset 0x6, size 0x2
    unsigned short AdjustedDispPosX; // offset 0x8, size 0x2
    unsigned short AdjustedDispPosY; // offset 0xA, size 0x2
    unsigned short AdjustedDispSizeY; // offset 0xC, size 0x2
    unsigned short AdjustedPanPosY; // offset 0xE, size 0x2
    unsigned short AdjustedPanSizeY; // offset 0x10, size 0x2
    unsigned short FBSizeX; // offset 0x12, size 0x2
    unsigned short FBSizeY; // offset 0x14, size 0x2
    unsigned short PanPosX; // offset 0x16, size 0x2
    unsigned short PanPosY; // offset 0x18, size 0x2
    unsigned short PanSizeX; // offset 0x1A, size 0x2
    unsigned short PanSizeY; // offset 0x1C, size 0x2
    VIXFBMode FBMode; // offset 0x20, size 0x4
    unsigned long nonInter; // offset 0x24, size 0x4
    unsigned long tv; // offset 0x28, size 0x4
    unsigned char wordPerLine; // offset 0x2C, size 0x1
    unsigned char std; // offset 0x2D, size 0x1
    unsigned char wpl; // offset 0x2E, size 0x1
    unsigned long bufAddr; // offset 0x30, size 0x4
    unsigned long tfbb; // offset 0x34, size 0x4
    unsigned long bfbb; // offset 0x38, size 0x4
    unsigned char xof; // offset 0x3C, size 0x1
    int black; // offset 0x40, size 0x4
    int threeD; // offset 0x44, size 0x4
    unsigned long rbufAddr; // offset 0x48, size 0x4
    unsigned long rtfbb; // offset 0x4C, size 0x4
    unsigned long rbfbb; // offset 0x50, size 0x4
    VITiming *timing; // offset 0x54, size 0x4
} HorVer; // size: 0x58, address: 0xF0

static u32 getCurrentFieldEvenOdd(void);

static u32 getEncoderType(void)
{
    return 1;
}

static s32 cntlzd(u64 bit)
{
    u32 hi;
    u32 lo;
    s32 value;

    hi = bit >> 32;
    lo = bit & 0xFFFFFFFF;
    value = __cntlzw(hi);
    if (value < 32) {
        return value;
    }
    return __cntlzw(lo) + 32;
}

static int VISetRegs(void)
{
    s32 regIndex;

    if (changeMode != 1 || getCurrentFieldEvenOdd() != 0) {
        while (shdwChanged != 0) {
            regIndex = cntlzd(shdwChanged);
            __VIRegs[regIndex] = shdwRegs[regIndex];
            shdwChanged &= ~((u64)1 << (63 - regIndex));
        }
        changeMode = 0;
        return 1;
    }
    return 0;
}

static void __VIRetraceHandler(__OSInterrupt unused, OSContext *context)
{
    OSContext exceptionContext;
    u16 reg;
    u32 inter;
#if DEBUG
    static u32 dbgCount;
#endif

    inter = 0;
    reg = __VIRegs[0x18];
    if (reg & 0x8000) {
        __VIRegs[0x18] = reg & ~0x8000;
        inter |= 1;
    }
    reg = __VIRegs[0x1A];
    if (reg & 0x8000) {
        __VIRegs[0x1A] = reg & ~0x8000;
        inter |= 2;
    }
    reg = __VIRegs[0x1C];
    if (reg & 0x8000) {
        __VIRegs[0x1C] = reg & ~0x8000;
        inter |= 4;
    }
    reg = __VIRegs[0x1E];
    if (reg & 0x8000) {
        __VIRegs[0x1E] = reg & ~0x8000;
        inter |= 8;
    }
    reg = __VIRegs[0x1E];
    if ((inter & 4) || (inter & 8)) {
        OSSetCurrentContext(context);
        return;
    }
    if (inter == 0) {
        ASSERTLINE(0x2BA, FALSE);
    }
    retraceCount += 1;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (PreCB) {
        PreCB(retraceCount);
    }
    if (flushFlag != 0) {
#if DEBUG
        dbgCount = 0;
#endif
        if (VISetRegs() != 0) {
            flushFlag = 0;
        }
    }
#if DEBUG
    else if (changed != 0) {
        dbgCount++;
        if (dbgCount > 60) {
            OSReport("Warning: VIFlush() was not called for 60 frames although VI settings were changed\n");
            dbgCount = 0;
        }
    }
#endif
    if (PostCB) {
        OSClearContext(&exceptionContext);
        PostCB(retraceCount);
    }
    OSWakeupThread(&retraceQueue);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb)
{
    BOOL enabled;
    VIRetraceCallback oldcb;

    oldcb = PreCB;
    enabled = OSDisableInterrupts();
    PreCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb)
{
    BOOL enabled;
    VIRetraceCallback oldcb;

    oldcb = PostCB;
    enabled = OSDisableInterrupts();
    PostCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

#pragma dont_inline on
static VITiming *getTiming(VITVMode mode)
{
    switch (mode) {
    case VI_TVMODE_NTSC_INT:  return &timing[0];
    case VI_TVMODE_NTSC_DS:   return &timing[1];
    case VI_TVMODE_PAL_INT:   return &timing[2];
    case VI_TVMODE_PAL_DS:    return &timing[3];
    case VI_TVMODE_MPAL_INT:  return &timing[4];
    case VI_TVMODE_MPAL_DS:   return &timing[5];
    case VI_TVMODE_NTSC_PROG: return &timing[6];
    default: return NULL;
    }
}
#pragma dont_inline reset

void __VIInit(VITVMode mode)
{
    VITiming *tm;
    u32 nonInter;
    u32 tv;
    volatile u32 a;
    u16 hct;
    u16 vct;
    u32 encoderType;

    encoderType = getEncoderType();
    if (encoderType == 0) {
        __VIInitPhilips();
    }
    nonInter = mode & 2;
    tv = (u32)mode >> 2;
    *(u32 *)OSPhysicalToCached(0xCC) = tv;
    if (encoderType == 0) {
        tv = 3;
    }
    tm = getTiming(mode);
    __VIRegs[1] = 2;

    // why?
    for (a = 0; a < 1000; a++) {
    }

    __VIRegs[1] = 0;
    __VIRegs[3] = (u32)tm->hlw;
    __VIRegs[2] = tm->hce | (tm->hcs << 8);
    __VIRegs[5] = tm->hsy | ((tm->hbe640 & 0x1FF) << 7);
    __VIRegs[4] = (tm->hbe640 >> 9) | ((tm->hbs640 & 0xFFFF) << 1);
    if (encoderType == 0) {
        __VIRegs[0x39] = tm->hbeCCIR656 | 0x8000;
        __VIRegs[0x3A] = (u32)tm->hbsCCIR656;
    }
    __VIRegs[0] = (u32)tm->equ;
    __VIRegs[7] = (u32)(tm->prbOdd + (tm->acv * 2) - 2);
    __VIRegs[6] = (u32)(tm->psbOdd + 2);
    __VIRegs[9] = (u32)(tm->prbEven + (tm->acv * 2) - 2);
    __VIRegs[8] = (u32)(tm->psbEven + 2);
    __VIRegs[11] = tm->bs1 | (tm->be1 << 5);
    __VIRegs[10] = tm->bs3 | (tm->be3 << 5);
    __VIRegs[13] = tm->bs2 | (tm->be2 << 5);
    __VIRegs[12] = tm->bs4 | (tm->be4 << 5);
    __VIRegs[36] = 0x2828;
    __VIRegs[27] = 1;
    __VIRegs[26] = 0x1001;
    hct = tm->hlw + 1;
    vct = (tm->nhlines / 2) + 1;
    __VIRegs[25] = (u16)(u32)hct;
    __VIRegs[24] = vct | 0x1000;
    if (mode != VI_TVMODE_NTSC_PROG) {
        __VIRegs[1] = (nonInter << 2) | 1 | (tv << 8);
        __VIRegs[54] = 0;
        return;
    }
    __VIRegs[1] = (tv << 8) | 5;
    __VIRegs[54] = 1;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(val, min, max) ((val) > (max) ? (max) : (val) < (min) ? (min) : (val))

static void AdjustPosition(u16 acv)
{
    s32 coeff;
    s32 frac;

    HorVer.AdjustedDispPosX = CLAMP((s16)HorVer.DispPosX + displayOffsetH, 0, 0x2D0 - HorVer.DispSizeX);
    coeff = (HorVer.FBMode == VI_XFBMODE_SF) ? 2 : 1;
    frac = HorVer.DispPosY & 1;
    HorVer.AdjustedDispPosY = MAX((s16)HorVer.DispPosY + displayOffsetV, frac);
    HorVer.AdjustedDispSizeY = HorVer.DispSizeY
                             + MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0)
                             - MAX((s16)HorVer.DispPosY + (s16)HorVer.DispSizeY + displayOffsetV - (((s16)acv * 2) - frac), 0);
    HorVer.AdjustedPanPosY = HorVer.PanPosY
                           - (MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0) / coeff);
    HorVer.AdjustedPanSizeY = HorVer.PanSizeY
                            + (MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0) / coeff)
                            - (MAX((s16)HorVer.DispPosY + (s16)HorVer.DispSizeY + displayOffsetV - (((s16)acv * 2) - frac), 0) / coeff);
}

static void ImportAdjustingValues(void)
{
    OSSram *sram = __OSLockSram();

    ASSERTLINE(0x3E2, sram);
    displayOffsetH = sram->displayOffsetH;
    displayOffsetV = 0;
    __OSUnlockSram(0);
}

void VIInit(void)
{
    u16 dspCfg;
    u32 value;
    u32 tv;

    encoderType = getEncoderType();
    if (!(__VIRegs[1] & 1)) {
        __VIInit(VI_TVMODE_NTSC_INT);
    }
    retraceCount = 0;
    changed = 0;
    shdwChanged = 0;
    changeMode = 0;
    flushFlag = 0;
    __VIRegs[39] = taps[0] | ((taps[1] & 0x3F) << 10);
    __VIRegs[38] = (taps[1] >> 6) | (taps[2] << 4);
    __VIRegs[41] = taps[3] | ((taps[4] & 0x3F) << 10);
    __VIRegs[40] = (taps[4] >> 6) | (taps[5] << 4);
    __VIRegs[43] = taps[6] | ((taps[7] & 0x3F) << 10);
    __VIRegs[42] = (taps[7] >> 6) | (taps[8] << 4);
    __VIRegs[45] = taps[9] | (taps[10] << 8);
    __VIRegs[44] = taps[11] | (taps[12] << 8);
    __VIRegs[47] = taps[13] | (taps[14] << 8);
    __VIRegs[46] = taps[15] | (taps[16] << 8);
    __VIRegs[49] = taps[17] | (taps[18] << 8);
    __VIRegs[48] = taps[19] | (taps[20] << 8);
    __VIRegs[51] = taps[21] | (taps[22] << 8);
    __VIRegs[50] = taps[23] | (taps[24] << 8);
    __VIRegs[56] = 0x280;
    ImportAdjustingValues();
    HorVer.DispSizeX = 0x280U;
    HorVer.DispSizeY = 0x1E0U;
    HorVer.DispPosX = (0x2D0 - HorVer.DispSizeX) / 2;
    HorVer.DispPosY = (0x1E0 - HorVer.DispSizeY) / 2;
    AdjustPosition(0xF0U);
    HorVer.FBSizeX = 0x280;
    HorVer.FBSizeY = 0x1E0;
    HorVer.PanPosX = 0;
    HorVer.PanPosY = 0;
    HorVer.PanSizeX = 0x280;
    HorVer.PanSizeY = 0x1E0;
    HorVer.FBMode = 0;
    dspCfg = __VIRegs[1];
    HorVer.nonInter = (s32) ((dspCfg >> 2U) & 1);
    HorVer.tv = (u32) ((dspCfg >> 8U) & 3);
    tv = (HorVer.tv == 3) ? 0 : HorVer.tv;
    HorVer.timing = getTiming((tv << 2) + HorVer.nonInter);
    regs[1] = dspCfg;
    HorVer.wordPerLine = 0x28;
    HorVer.std = 0x28;
    HorVer.wpl = 0x28;
    HorVer.xof = 0;
    HorVer.black = 1;
    HorVer.threeD = 0;
    OSInitThreadQueue(&retraceQueue);
    value = __VIRegs[24];
    value &= ~0x8000;
#if !DEBUG
    value = (u16)value;
#endif
    __VIRegs[24] = value;
    value = __VIRegs[26];
    value = value & ~0x8000;
#if !DEBUG
    value = (u16)value;
#endif
    __VIRegs[26] = value;
    PreCB = NULL;
    PostCB = NULL;
    __OSSetInterruptHandler(0x18, __VIRetraceHandler);
    __OSUnmaskInterrupts(0x80);
}

void VIWaitForRetrace(void)
{
    BOOL enabled;
    u32 count;

    enabled = OSDisableInterrupts();
    count = retraceCount;
    do {
        OSSleepThread(&retraceQueue);
    } while (count == retraceCount);
    OSRestoreInterrupts(enabled);
}

static void setInterruptRegs(VITiming *tm)
{
    u16 vct;
    u16 hct;
    u16 borrow;

    vct = tm->nhlines / 2;
    borrow = tm->nhlines % 2;
    if (borrow != 0) {
        hct = tm->hlw;
    } else {
        hct = 0;
    }
    vct++;
    hct++;
    regs[0x19] = (u16)(u32)hct;
    changed |= 0x4000000000;
    regs[0x18] = vct | 0x1000;
    changed |= 0x8000000000;

    vct;
}

static void setPicConfig(u16 fbSizeX, VIXFBMode xfbMode, u16 panPosX, u16 panSizeX, u8 *wordPerLine, u8 *std, u8 *wpl, u8 *xof)
{
    *wordPerLine = (fbSizeX + 15) / 16;
    *std = (xfbMode == VI_XFBMODE_SF) ? *wordPerLine : (u8)(*wordPerLine << 1);
    *xof = panPosX % 16;
    *wpl = (*xof + panSizeX + 15) / 16;
    regs[0x24] = *std | (*wpl << 8);
    changed |= 0x8000000;
}

static void setBBIntervalRegs(VITiming *tm)
{
    u16 val;

    val = tm->bs1 | (tm->be1 << 5);
    regs[11] = val;
    changed |= 0x10000000000000;

    val = tm->bs3 | (tm->be3 << 5);
    regs[10] = val;
    changed |= 0x20000000000000;

    val = tm->bs2 | (tm->be2 << 5);
    regs[13] = val;
    changed |= 0x4000000000000;

    val = tm->bs4 | (tm->be4 << 5);
    regs[12] = val;
    changed |= 0x8000000000000;
}

static void setScalingRegs(u16 panSizeX, u16 dispSizeX, int threeD)
{
    u32 scale;

    panSizeX = threeD ? (panSizeX << 1) : panSizeX;
    if (panSizeX < dispSizeX) {
        scale = (u32)(dispSizeX + (panSizeX << 8) - 1) / dispSizeX;
        regs[37] = scale | 0x1000;
        changed |= 0x04000000;
        regs[56] = (u32)panSizeX;
        changed |= 0x80;
    } else {
        regs[37] = 0x100;
        changed |= 0x04000000;
    }
}
