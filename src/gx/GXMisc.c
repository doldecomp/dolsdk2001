#include <stddef.h>
#include <dolphin/base/PPCArch.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <macros.h>

#include "__gx.h"

static GXDrawSyncCallback TokenCB;
static GXDrawDoneCallback DrawDoneCB;
static u8 DrawDone;
static OSThreadQueue FinishQueue;

#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 14 : 0)

void GXSetMisc(GXMiscToken token, u32 val)
{
    switch (token) {
    case GX_MT_XF_FLUSH:
        gx->vNum = val;
#if DOLPHIN_REVISION >= 45
        gx->vNumNot = !gx->vNum;
        gx->bpSentNot = 1;
#endif
        if (gx->vNum != 0) {
            gx->dirtyState |= 8;
        }
        break;
    case GX_MT_DL_SAVE_CONTEXT:
        ASSERTMSGLINE(0xC4+LINE_OFFSET, !gx->inDispList, "GXSetMisc: Cannot change DL context setting while making a display list");
        gx->dlSaveContext = (val > 0);
        break;
    case GX_MT_NULL:
        break;
    default:
#if DEBUG
        OSReport("GXSetMisc: bad token %d (val %d)\n", token, val);
#endif
        break;
    }
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 13 : 0)

void GXFlush(void)
{
    u32 i;

    CHECK_GXBEGIN(0xF0+LINE_OFFSET, "GXFlush");
    if (gx->dirtyState) {
        __GXSetDirtyState();
    }
#if DOLPHIN_REVISION >= 45
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
#else
    for (i = 32; i > 0; i--) {
        GX_WRITE_U8(0);
    }
#endif
    PPCSync();
}

void GXResetWriteGatherPipe(void)
{
    while (PPCMfwpar() & 1) {
    }
    PPCMtwpar(OSUncachedToPhysical((void *)GXFIFO_ADDR));
}

static inline void __GXAbortWait(u32 clocks)
{
    OSTime time0;
    OSTime time1;

    time0 = OSGetTime();
    do {
        time1 = OSGetTime();
    } while (time1 - time0 <= (clocks / 4));
}

void __GXAbort(void)
{
    __piReg[6] = 1;
    __GXAbortWait(0xC8);
    __piReg[6] = 0;
    __GXAbortWait(0x14);
}

void GXAbortFrame(void)
{
#if DOLPHIN_REVISION >= 45
    __GXAbort();
#else
    __piReg[6] = 1;
    __GXAbortWait(0xC8);
    __piReg[6] = 0;
    __GXAbortWait(0x14);
#endif
    __GXCleanGPFifo();
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 25 : 0)

void GXSetDrawSync(u16 token)
{
    BOOL enabled;
    u32 reg;

    CHECK_GXBEGIN(0x162+LINE_OFFSET, "GXSetDrawSync");

    enabled = OSDisableInterrupts();
    reg = token | 0x48000000;
    GX_WRITE_RAS_REG(reg);
    SET_REG_FIELD(0x16F+LINE_OFFSET, reg, 16, 0, token);
    SET_REG_FIELD(0x170+LINE_OFFSET, reg, 8, 24, 0x47);
    GX_WRITE_RAS_REG(reg);
    GXFlush();
    OSRestoreInterrupts(enabled);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

u16 GXReadDrawSync(void)
{
    u16 token = __peReg[7];
    return token;
}

void GXSetDrawDone(void)
{
    u32 reg;
    BOOL enabled;

    CHECK_GXBEGIN(0x19C+LINE_OFFSET, "GXSetDrawDone");
    enabled = OSDisableInterrupts();
    reg = 0x45000002;
    GX_WRITE_RAS_REG(reg);
    GXFlush();
    DrawDone = 0;
    OSRestoreInterrupts(enabled);
}

void GXWaitDrawDone(void)
{
    BOOL enabled;

    CHECK_GXBEGIN(0x1CA+LINE_OFFSET, "GXWaitDrawDone");

    enabled = OSDisableInterrupts();
    while (!DrawDone) {
        OSSleepThread(&FinishQueue);
    }
    OSRestoreInterrupts(enabled);
}

void GXDrawDone(void)
{
    CHECK_GXBEGIN(0x1EA+LINE_OFFSET, "GXDrawDone");
    GXSetDrawDone();
    GXWaitDrawDone();
}

void GXPixModeSync(void)
{
    CHECK_GXBEGIN(0x20D+LINE_OFFSET, "GXPixModeSync");
    GX_WRITE_RAS_REG(gx->peCtrl);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

void GXTexModeSync(void)
{
    u32 reg;

    CHECK_GXBEGIN(0x225+LINE_OFFSET, "GXTexModeSync");
    reg = 0x63000000;
    GX_WRITE_RAS_REG(reg);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

#if DEBUG
void __GXBypass(u32 reg)
{
    CHECK_GXBEGIN(0x23B+LINE_OFFSET, "__GXBypass");
    GX_WRITE_RAS_REG(reg);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

u16 __GXReadPEReg(u32 reg)
{
    return __peReg[reg];
}
#endif

void GXPokeAlphaMode(GXCompare func, u8 threshold)
{
    u32 reg;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x25F, "GXPokeAlphaMode");
#endif
    reg = (func << 8) | threshold;
    __peReg[3] = reg;
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 21 : 0)

void GXPokeAlphaRead(GXAlphaReadMode mode)
{
    u32 reg;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x26A, "GXPokeAlphaRead");
#endif
    reg = 0;
    SET_REG_FIELD(0x26D+LINE_OFFSET, reg, 2, 0, mode);
    SET_REG_FIELD(0x26E+LINE_OFFSET, reg, 1, 2, 1);
    __peReg[4] = reg;
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 19 : 0)

void GXPokeAlphaUpdate(GXBool update_enable)
{
    u32 reg;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x277, "GXPokeAlphaUpdate");
#endif
    reg = __peReg[1];
    SET_REG_FIELD(0x27A+LINE_OFFSET, reg, 1, 4, update_enable);
    __peReg[1] = reg;
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 17 : 0)

void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op)
{
    u32 reg;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x284, "GXPokeBlendUpdate");
#endif
    reg = __peReg[1];
    SET_REG_FIELD(0x28C+LINE_OFFSET, reg, 1, 0, (type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT));
    SET_REG_FIELD(0x28D+LINE_OFFSET, reg, 1, 11, (type == GX_BM_SUBTRACT));
    SET_REG_FIELD(0x28F+LINE_OFFSET, reg, 1, 1, (type == GX_BM_LOGIC));
    SET_REG_FIELD(0x290+LINE_OFFSET, reg, 4, 12, op);
    SET_REG_FIELD(0x291+LINE_OFFSET, reg, 3, 8, src_factor);
    SET_REG_FIELD(0x292+LINE_OFFSET, reg, 3, 5, dst_factor);
    SET_REG_FIELD(0x293+LINE_OFFSET, reg, 8, 24, 0x41);
    __peReg[1] = reg;
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 15 : 0)

void GXPokeColorUpdate(GXBool update_enable)
{
    u32 reg;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x29D, "GXPokeColorUpdate");
#endif
    reg = __peReg[1];
    SET_REG_FIELD(0x2A0+LINE_OFFSET, reg, 1, 3, update_enable);
    __peReg[1] = reg;
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 13 : 0)

void GXPokeDstAlpha(GXBool enable, u8 alpha)
{
    u32 reg = 0;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x2A9, "GXPokeDstAlpha");
#endif
    SET_REG_FIELD(0x2AB+LINE_OFFSET, reg, 8, 0, alpha);
    SET_REG_FIELD(0x2AC+LINE_OFFSET, reg, 1, 8, enable);
    __peReg[2] = reg;
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 11 : 0)

void GXPokeDither(GXBool dither)
{
    u32 reg;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x2B5, "GXPokeDither");
#endif
    reg = __peReg[1];
    SET_REG_FIELD(0x2B8+LINE_OFFSET, reg, 1, 2, dither);
    __peReg[1] = reg;
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 9 : 0)

void GXPokeZMode(GXBool compare_enable, GXCompare func, GXBool update_enable)
{
    u32 reg = 0;

#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x2C1, "GXPokeZMode");
#endif
    SET_REG_FIELD(0x2C3+LINE_OFFSET, reg, 1, 0, compare_enable);
    SET_REG_FIELD(0x2C4+LINE_OFFSET, reg, 3, 1, func);
    SET_REG_FIELD(0x2C5+LINE_OFFSET, reg, 1, 4, update_enable);
    __peReg[0] = reg;
}

void GXPeekARGB(u16 x, u16 y, u32 *color)
{
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SET_REG_FIELD(0x2DC+LINE_OFFSET, addr, 10, 2, x);
    SET_REG_FIELD(0x2DD+LINE_OFFSET, addr, 10, 12, y);
    SET_REG_FIELD(0x2DE+LINE_OFFSET, addr, 2, 22, 0);
    *color = *(u32 *)addr;
}

void GXPokeARGB(u16 x, u16 y, u32 color)
{
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SET_REG_FIELD(0x2E6+LINE_OFFSET, addr, 10, 2, x);
    SET_REG_FIELD(0x2E7+LINE_OFFSET, addr, 10, 12, y);
    SET_REG_FIELD(0x2E8+LINE_OFFSET, addr, 2, 22, 0);
    *(u32 *)addr = color;
}

void GXPeekZ(u16 x, u16 y, u32 *z)
{
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SET_REG_FIELD(0x2F0+LINE_OFFSET, addr, 10, 2, x);
    SET_REG_FIELD(0x2F1+LINE_OFFSET, addr, 10, 12, y);
    SET_REG_FIELD(0x2F2+LINE_OFFSET, addr, 2, 22, 1);
    *z = *(u32 *)addr;
}

void GXPokeZ(u16 x, u16 y, u32 z)
{
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SET_REG_FIELD(0x2FA+LINE_OFFSET, addr, 10, 2, x);
    SET_REG_FIELD(0x2FB+LINE_OFFSET, addr, 10, 12, y);
    SET_REG_FIELD(0x2FC+LINE_OFFSET, addr, 2, 22, 1);
    *(u32 *)addr = z;
}

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb)
{
    GXDrawSyncCallback oldcb;
    BOOL enabled;

    oldcb = TokenCB;
    enabled = OSDisableInterrupts();
    TokenCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

static void GXTokenInterruptHandler(__OSInterrupt interrupt, OSContext *context)
{
    u16 token;
    OSContext exceptionContext;
    u32 reg;

    token = __peReg[7];
    if (TokenCB != NULL) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        TokenCB(token);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
    reg = __peReg[5];
    SET_REG_FIELD(0, reg, 1, 2, 1);
    __peReg[5] = reg;
}

GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb)
{
    GXDrawDoneCallback oldcb;
    BOOL enabled;

    oldcb = DrawDoneCB;
    enabled = OSDisableInterrupts();
    DrawDoneCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

static void GXFinishInterruptHandler(__OSInterrupt interrupt, OSContext *context)
{
    OSContext exceptionContext;
    u32 reg;

    reg = __peReg[5];
    SET_REG_FIELD(0, reg, 1, 3, 1);
    __peReg[5] = reg;
    DrawDone = 1;
    if (DrawDoneCB != NULL) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        DrawDoneCB();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
    OSWakeupThread(&FinishQueue);
}

void __GXPEInit(void)
{
    u32 reg;
    __OSSetInterruptHandler(0x12, GXTokenInterruptHandler);
    __OSSetInterruptHandler(0x13, GXFinishInterruptHandler);
    OSInitThreadQueue(&FinishQueue);
    __OSUnmaskInterrupts(0x2000);
    __OSUnmaskInterrupts(0x1000);
    reg = __peReg[5];
    SET_REG_FIELD(0, reg, 1, 2, 1);
    SET_REG_FIELD(0, reg, 1, 3, 1);
    SET_REG_FIELD(0, reg, 1, 0, 1);
    SET_REG_FIELD(0, reg, 1, 1, 1);
    __peReg[5] = reg;
}

u32 GXCompressZ16(u32 z24, GXZFmt16 zfmt)
{
    u32 z16;
    u32 z24n;
    s32 exp;
    s32 shift;
#if DEBUG
#define temp exp
#else
    s32 temp;
    u8 unused[4];
#endif

    z24n = ~(z24 << 8);
    temp = __cntlzw(z24n);
    switch (zfmt) {
    case GX_ZC_LINEAR:
        z16 = (z24 >> 8) & 0xFFFF;
        break;
    case GX_ZC_NEAR:
        if (temp > 3) {
            exp = 3;
        } else {
            exp = temp;
        }
        if (exp == 3) {
            shift = 7;
        } else {
            shift = 9 - exp;
        }
        z16 = ((z24 >> shift) & 0x3FFF & ~0xFFFFC000) | (exp << 14);
        break;
    case GX_ZC_MID:
        if (temp > 7) {
            exp = 7;
        } else {
            exp = temp;
        }
        if (exp == 7) {
            shift = 4;
        } else {
            shift = 10 - exp;
        }
        z16 = ((z24 >> shift) & 0x1FFF & ~0xFFFFE000) | (exp << 13);
        break;
    case GX_ZC_FAR:
        if (temp > 12) {
            exp = 12;
        } else {
            exp = temp;
        }
        if (exp == 12) {
            shift = 0;
        } else {
            shift = 11 - exp;
        }
        z16 = ((z24 >> shift) & 0xFFF & ~0xFFFFF000) | (exp << 12);
        break;
    default:
        OSPanic(__FILE__, 0x3B0+LINE_OFFSET, "GXCompressZ16: Invalid Z format\n");
        break;
    }
    return z16;
}

u32 GXDecompressZ16(u32 z16, GXZFmt16 zfmt)
{
    u32 z24;
    u32 cb1;
    long exp;
    long shift;

    cb1; cb1; cb1; z16; z16; z16;  // needed to match

    switch (zfmt) {
    case GX_ZC_LINEAR:
        z24 = (z16 << 8) & 0xFFFF00;
        break;
    case GX_ZC_NEAR:
        exp = (z16 >> 14) & 3;
        if (exp == 3) {
            shift = 7;
        } else {
            shift = 9 - exp;
        }
        cb1 = -1 << (24 - exp);
        z24 = (cb1 | ((z16 & 0x3FFF) << shift)) & 0xFFFFFF;
        break;
    case GX_ZC_MID:
        exp = (z16 >> 13) & 7;
        if (exp == 7) {
            shift = 4;
        } else {
            shift = 10 - exp;
        }
        cb1 = -1 << (24 - exp);
        z24 = (cb1 | ((z16 & 0x1FFF) << shift)) & 0xFFFFFF;
        break;
    case GX_ZC_FAR:
        exp = (z16 >> 12) & 0xF;
        if (exp == 12) {
            shift = 0;
        } else {
            shift = 11 - exp;
        }
        cb1 = -1 << (24 - exp);
        z24 = (cb1 | ((z16 & 0xFFF) << shift)) & 0xFFFFFF;
        break;
    default:
        OSPanic(__FILE__, 0x3E2+LINE_OFFSET, "GXDecompressZ16: Invalid Z format\n");
        break;
    }
    return z24;
}
