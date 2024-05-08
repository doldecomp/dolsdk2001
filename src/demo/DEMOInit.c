#include <dolphin.h>
#include <dolphin/pad.h>
#include <dolphin/gx.h>
#include <dolphin/vi.h>
#include <dolphin/demo.h>

#include "__demo.h"
#include "__gx.h"

extern unsigned char DemoStatEnable; // size: 0x1, address: 0x0

// .bss
static struct _GXRenderModeObj rmodeobj; // size: 0x3C, address: 0x0

// .sdata
static unsigned char DemoFirstFrame = 1; // size: 0x1, address: 0x0

// .sbss
static void * DefaultFifo = NULL; // size: 0x4, address: 0x0
static GXFifoObj *DefaultFifoObj = NULL; // size: 0x4, address: 0x4
static struct _GXRenderModeObj * rmode; // size: 0x4, address: 0x8
#if DOLPHIN_REVISION >= 45
static int GPHangWorkaround;
#else
static int BypassWorkaround; // size: 0x4, address: 0xC
#endif
static unsigned long FrameCount; // size: 0x4, address: 0x10
static unsigned long FrameMissThreshold; // size: 0x4, address: 0x14
void * DemoFrameBuffer1; // size: 0x4, address: 0x20
void * DemoFrameBuffer2; // size: 0x4, address: 0x1C
void * DemoCurrentBuffer; // size: 0x4, address: 0x18

// functions
static void __DEMOInitRenderMode(struct _GXRenderModeObj * mode);
static void __DEMOInitMem();
static void __DEMOInitGX();
static void __DEMOInitVI();
static void __DEMOInitForEmu();
#if DOLPHIN_REVISION >= 45
static void __NoHangRetraceCallback();
static void __NoHangDoneRender();
void DEMOSetGPHangMetric(u8 enable);
static void __DEMODiagnoseHang();
#else
static void __BypassRetraceCallback();
static void __BypassDoneRender();
static void LoadMemInfo();
#endif

void DEMOInit(struct _GXRenderModeObj * mode) {
    OSInit();
    DVDInit();
    VIInit();
    DEMOPadInit();
    __DEMOInitRenderMode(mode);
    __DEMOInitMem();
    VIConfigure(rmode);
    DefaultFifo = OSAllocFromHeap(__OSCurrHeap, 0x40000);
    DefaultFifoObj = GXInit(DefaultFifo, 0x40000);
    __DEMOInitGX();
    __DEMOInitVI();
}

#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 14 : 0)

static void __DEMOInitRenderMode(struct _GXRenderModeObj * mode) {
    if (mode != NULL) {
        rmode = mode;
        return;
    }
    switch(VIGetTvFormat()) {
        case VI_NTSC:
            rmode = &GXNtsc480IntDf;
            break;
        case VI_PAL:
            rmode = &GXPal528IntDf;
            break;
        case VI_MPAL:
            rmode = &GXMpal480IntDf;
            break;
        default:
            OSPanic(__FILE__, 0x1A6+LINE_OFFSET, "DEMOInit: invalid TV format\n");
            break;
    }
    GXAdjustForOverscan(rmode, &rmodeobj, 0, 0x10);
    rmode = &rmodeobj;
}

static void __DEMOInitMem() {
    void * arenaLo = OSGetArenaLo();
    void * arenaHi = OSGetArenaHi();
    unsigned long fbSize = ((u16)(rmode->fbWidth + 15) & 0xFFF0) * rmode->xfbHeight * 2;

    DemoFrameBuffer1 = (void*)(((u32)arenaLo + 0x1F) & 0xFFFFFFE0);
    DemoFrameBuffer2 = (void*)(((u32)DemoFrameBuffer1 + fbSize + 0x1F) & 0xFFFFFFE0);
    DemoCurrentBuffer = DemoFrameBuffer2;
    arenaLo = (void*)(((u32)DemoFrameBuffer2 + fbSize + 0x1F) & 0xFFFFFFE0);
    OSSetArenaLo(arenaLo);
#if DOLPHIN_REVISION < 45
    if (((OSGetConsoleType() + 0xF0000000) == 4U) && ((OSGetPhysicalMemSize() + 0xFFC00000) != 0U) && (OSGetConsoleSimulatedMemSize() < 0x01800000U)) {
        LoadMemInfo();
        return;
    }
#endif
    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();
    arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
    OSSetArenaLo(arenaLo);
    arenaLo = (void*)(((u32)arenaLo + 0x1F) & 0xFFFFFFE0);
    arenaHi = (void*)((u32)arenaHi & 0xFFFFFFE0);
    OSSetCurrentHeap(OSCreateHeap((void*)(((u32)arenaLo)), arenaHi));
    OSSetArenaLo((arenaLo = arenaHi));
}

static void __DEMOInitGX() {
    GXSetViewport(0.0f, 0.0f, rmode->fbWidth,
#if DOLPHIN_REVISION >= 45
        rmode->efbHeight,
#else
        rmode->xfbHeight,
#endif
        0.0f, 1.0f);
    GXSetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, rmode->xfbHeight);
    GXSetDispCopyYScale(((f32)rmode->xfbHeight / (f32)rmode->efbHeight));
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, 1, rmode->vfilter);
    if (rmode->aa != 0) {
        GXSetPixelFmt(2, 0);
    } else {
        GXSetPixelFmt(0, 0);
    }
    GXCopyDisp(DemoCurrentBuffer, 1);
#if DEBUG
    GXSetVerifyLevel(0);
#endif
    GXSetDispCopyGamma(0);
}

static void __DEMOInitVI() {
    unsigned long nin;

    VISetNextFrameBuffer(DemoFrameBuffer1);
    DemoCurrentBuffer = DemoFrameBuffer2;
    VIFlush();
    VIWaitForRetrace();
    nin = rmode->viTVmode & 1;
    if (nin != 0) {
        VIWaitForRetrace();
    }
}

static void __DEMOInitForEmu() {

}

void DEMOBeforeRender() {
#if DOLPHIN_REVISION >= 45
    if (GPHangWorkaround) {
        GXSetDrawSync(0xFEEB);
        GXClearGPMetric();
    }
#else
    if (BypassWorkaround) {
        GXSetDrawSync(0xFEEB);
    }
#endif
    if (rmode->field_rendering != 0) {
        GXSetViewportJitter(0.0f, 0.0f, rmode->fbWidth,
#if DOLPHIN_REVISION >= 45
            rmode->efbHeight,
#else
            rmode->xfbHeight,
#endif
            0.0f, 1.0f, VIGetNextField());
    } else {
        GXSetViewport(0.0f, 0.0f, rmode->fbWidth,
#if DOLPHIN_REVISION >= 45
            rmode->efbHeight,
#else
            rmode->xfbHeight,
#endif
            0.0f, 1.0f);
    }
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
}

void DEMODoneRender() {
#if DOLPHIN_REVISION >= 45
    if (GPHangWorkaround) {
        ASSERTMSGLINE(0x2C1, !DemoStatEnable, "DEMOStats and GP hang diagnosis are mutually exclusive");
        __NoHangDoneRender();
        return;
    }
#else
    if (BypassWorkaround) {
        __BypassDoneRender();
        return;
    }
#endif
    if (DemoStatEnable != 0) {
        GXDrawDone();
        DEMOUpdateStats(1);
        DEMOPrintStats();
        GXDrawDone();
        DEMOUpdateStats(0);
    }
    GXSetZMode(1, 3, 1);
    GXSetColorUpdate(1);
    GXCopyDisp(DemoCurrentBuffer, 1);
    GXDrawDone();
    DEMOSwapBuffers();
}

void DEMOSwapBuffers() {
    VISetNextFrameBuffer(DemoCurrentBuffer);
    if (DemoFirstFrame != 0) {
        VISetBlack(0);
        DemoFirstFrame = 0;
    }
    VIFlush();
    VIWaitForRetrace();
    if ((u32) DemoCurrentBuffer == (u32) DemoFrameBuffer1) {
        DemoCurrentBuffer = DemoFrameBuffer2;
        return;
    }
    DemoCurrentBuffer = DemoFrameBuffer1;
}

void DEMOSetTevColorIn(enum _GXTevStageID stage, enum _GXTevColorArg a, enum _GXTevColorArg b, enum _GXTevColorArg c, enum _GXTevColorArg d) {
    unsigned long swap;

    swap = 0;
    if (a == GX_CC_TEXC) {
        swap = 0xF;
    } else if (a >= GX_CC_TEXRRR) {
        swap = a;
        a = GX_CC_TEXC;
    }
    if (b == GX_CC_TEXC) {
        swap = 0xF;
    } else if (b >= GX_CC_TEXRRR) {
        swap = b;
        b = GX_CC_TEXC;
    }
    if (c == GX_CC_TEXC) {
        swap = 0xF;
    } else if (c >= GX_CC_TEXRRR) {
        swap = c;
        c = GX_CC_TEXC;
    }
    if (d == GX_CC_TEXC) {
        swap = 0xF;
    } else if (d >= GX_CC_TEXRRR) {
        swap = d;
        d = GX_CC_TEXC;
    }
    GXSetTevColorIn(stage, a, b, c, d);
    if (swap != 0U) {
        GXSetTevSwapMode(stage, 0, swap - 0xF);
    }
}

#undef LINE_OFFSET
#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? -1 : 0)

void DEMOSetTevOp(enum _GXTevStageID id, enum _GXTevMode mode) {
    enum _GXTevColorArg carg;
    enum _GXTevAlphaArg aarg;

    carg = GX_CC_RASC;
    aarg = GX_TEVSTAGE5;
    if (id != 0) {
        carg = 0;
        aarg = 0;
    }
    switch(mode) {
    case 0:
        DEMOSetTevColorIn(id, 0xF, 8, carg, 0xF);
        GXSetTevAlphaIn(id, 7, 4, aarg, 7);
        break;
    case 1:
        DEMOSetTevColorIn(id, carg, 8, 9, 0xF);
        GXSetTevAlphaIn(id, 7, 7, 7, aarg);
        break;
    case 2:
        DEMOSetTevColorIn(id, carg, 0xC, 8, 0xF);
        GXSetTevAlphaIn(id, 7, 4, aarg, 7);
        break;
    case 3:
        DEMOSetTevColorIn(id, 0xF, 0xF, 0xF, 8);
        GXSetTevAlphaIn(id, 7, 7, 7, 4);
        break;
    case 4:
#if DOLPHIN_REVISION >= 45
        DEMOSetTevColorIn(id, 0xF, 0xF, 0xF, carg);
#else
        GXSetTevColorIn(id, 0xF, 0xF, 0xF, carg);
#endif
        GXSetTevAlphaIn(id, 7, 7, 7, aarg);
        break;
    default:
        ASSERTMSGLINE(0x367+LINE_OFFSET, FALSE, "DEMOSetTevOp: Invalid Tev Mode");
        break;
    }
    GXSetTevColorOp(id, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(id, 0, 0, 0, 1, 0);
}

struct _GXRenderModeObj * DEMOGetRenderModeObj() {
    return rmode;
}

u32 DEMOGetCurrentBuffer(void) {
    return (u32)DemoCurrentBuffer;
}

#if DOLPHIN_REVISION >= 45
void DEMOEnableGPHangWorkaround(unsigned long timeoutFrames) {
    if (timeoutFrames) {
        ASSERTMSGLINE(0x3B1, !DemoStatEnable, "DEMOStats and GP hang diagnosis are mutually exclusive");
        GPHangWorkaround = 1;
        FrameMissThreshold = timeoutFrames;
        VISetPreRetraceCallback(__NoHangRetraceCallback);
        DEMOSetGPHangMetric(1);
    } else {
        GPHangWorkaround = 0;
        FrameMissThreshold = 0;
        DEMOSetGPHangMetric(0);
        VISetPreRetraceCallback(0);
    }
}

static void __NoHangRetraceCallback() {
    u32 ovc;
    u8 overhi;
    u8 junk;
    u32 unused;
    static u32 ovFrameCount;
    static u32 lastOvc;

    FrameCount++;
    GXGetGPStatus(&overhi, &junk, &junk, &junk, &junk);
    ovc = GXGetOverflowCount();
    if (overhi && ovc == lastOvc) {
        ovFrameCount++;
        if (ovFrameCount >= FrameMissThreshold) {
            OSReport("---------WARNING : HANG AT HIGH WATERMARK----------\n");
            __DEMODiagnoseHang();
            OSPanic(__FILE__, 0x3EC, "Halting program");
        }
    } else {
        lastOvc = ovc;
        ovFrameCount = 0;
    }
}

static void __NoHangDoneRender() {
    int abort;

    abort = 0;
    GXCopyDisp(DemoCurrentBuffer, 1);
    GXSetDrawSync(0xB00B);
    FrameCount = 0;
    while ((GXReadDrawSync() != 0xB00B) && (abort == 0)) {
        if (FrameCount >= FrameMissThreshold) {
            OSReport("---------WARNING : ABORTING FRAME----------\n");
            abort = 1;
            __DEMODiagnoseHang();
            DEMOReInit(rmode);
            DEMOSetGPHangMetric(1);
        }
    }
    DEMOSwapBuffers();
}

void DEMOSetGPHangMetric(u8 enable) {
    if (enable) {
        GXSetGPMetric(0x23, 0x16);
        GX_WRITE_U8(0x61);
        GX_WRITE_U32(0x2402C004);
        GX_WRITE_U8(0x61);
        GX_WRITE_U32(0x23000020);
        GX_WRITE_U8(0x10);
        GX_WRITE_U16(0);
        GX_WRITE_U16(0x1006);
        GX_WRITE_U32(0x84400);
    } else {
        GX_WRITE_U8(0x61);
        GX_WRITE_U32(0x24000000);
        GX_WRITE_U8(0x61);
        GX_WRITE_U32(0x23000000);
        GX_WRITE_U8(0x10);
        GX_WRITE_U16(0);
        GX_WRITE_U16(0x1006);
        GX_WRITE_U32(0);
    }
}

static void __DEMODiagnoseHang() {
    u32 xfTop0;
    u32 xfBot0;
    u32 suRdy0;
    u32 r0Rdy0;
    u32 xfTop1;
    u32 xfBot1;
    u32 suRdy1;
    u32 r0Rdy1;
    u32 xfTopD;
    u32 xfBotD;
    u32 suRdyD;
    u32 r0RdyD;
    u8 readIdle;
    u8 cmdIdle;
    u8 junk;

    GXReadXfRasMetric(&xfBot0, &xfTop0, &r0Rdy0, &suRdy0);
    GXReadXfRasMetric(&xfBot1, &xfTop1, &r0Rdy1, &suRdy1);
    xfTopD = (xfTop1 - xfTop0) == 0;
    xfBotD = (xfBot1 - xfBot0) == 0;
    suRdyD = (suRdy1 - suRdy0) != 0;
    r0RdyD = (r0Rdy1 - r0Rdy0) != 0;
    GXGetGPStatus(&junk, &junk, &readIdle, &cmdIdle, &junk);
    OSReport("GP status %d%d%d%d%d%d --> ", readIdle, cmdIdle, xfTopD, xfBotD, suRdyD, r0RdyD);
    if (!xfBotD && suRdyD) {
        OSReport("GP hang due to XF stall bug.\n");
        return;
    }
    if (!xfTopD && xfBotD && suRdyD) {
        OSReport("GP hang due to unterminated primitive.\n");
        return;
    }
    if (!cmdIdle && xfTopD && xfBotD && suRdyD) {
        OSReport("GP hang due to illegal instruction.\n");
        return;
    }
    if (readIdle && cmdIdle && xfTopD && xfBotD && suRdyD && r0RdyD) {
        OSReport("GP appears to be not hung (waiting for input).\n");
        return;
    }
    OSReport("GP is in unknown state.\n");
}
#else
void DEMOEnableBypassWorkaround(unsigned long timeoutFrames) {
    BypassWorkaround = 1;
    FrameMissThreshold = timeoutFrames;
    VISetPreRetraceCallback(__BypassRetraceCallback);
}

static void __BypassRetraceCallback() {
    FrameCount += 1;
}

static void __BypassDoneRender() {
    int abort;

    abort = 0;
    GXCopyDisp(DemoCurrentBuffer, 1);
    GXSetDrawSync(0xB00B);
    FrameCount = 0U;
    while ((GXReadDrawSync() != 0xB00B) && (abort == 0)) {
        if (FrameCount >= FrameMissThreshold) {
            OSReport("---------WARNING : ABORTING FRAME----------\n");
            abort = 1;
            DEMOReInit(rmode);
        }
    }
    DEMOSwapBuffers();
}
#endif

void DEMOReInit(struct _GXRenderModeObj * mode) {
    GXFifoObj tmpobj;
    void * tmpFifo;
    GXFifoObj * realFifoObj;
    void * realFifoBase;
    unsigned long realFifoSize;

    tmpFifo = OSAllocFromHeap(__OSCurrHeap, 0x10000);
    realFifoObj = GXGetCPUFifo();
    realFifoBase = GXGetFifoBase(realFifoObj);
    realFifoSize = GXGetFifoSize(realFifoObj);
    GXAbortFrame();
    GXInitFifoBase(&tmpobj, tmpFifo, 0x10000);
    GXSetCPUFifo(&tmpobj);
    GXSetGPFifo(&tmpobj);
    __DEMOInitRenderMode(mode);
    DefaultFifoObj = GXInit(realFifoBase, realFifoSize);
    __DEMOInitGX();
    VIConfigure(rmode);
    __DEMOInitVI();
    OSFreeToHeap(__OSCurrHeap, tmpFifo);
}

#if DOLPHIN_REVISION < 45
static void LoadMemInfo() {
    void * arenaHiOld;
    void * arenaLo;
    void * arenaHi;
    void * simMemEnd;
    struct DVDFileInfo fileInfo;
    unsigned long length;
    unsigned long transferLength;
    long offset;
    unsigned long i;
    unsigned long indexMax;
    char * buf[63];
    struct {
        void * start;
        void * end;
    } * memEntry;

    OSReport("\nNow, try to find memory info file...\n\n");
    if (!DVDOpen("/meminfo.bin", &fileInfo)) {
        OSReport("\nCan't find memory info file. Use /XXX toolname/ to maximize available\n");
        OSReport("memory space. For now, we only use the first %dMB.\n", OSGetConsoleSimulatedMemSize() >> 0x14);
        arenaLo = OSGetArenaLo();
        arenaHi = OSGetArenaHi();
        arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
        OSSetArenaLo(arenaLo);
        arenaLo = (void*)(((u32)arenaLo + 0x1F) & 0xFFFFFFE0);
        arenaHi = (void*)((u32)arenaHi & 0xFFFFFFE0);
        OSSetCurrentHeap(OSCreateHeap((void*)(((u32)arenaLo)), arenaHi));
        OSSetArenaLo((arenaLo = arenaHi));
        return;
    }
    memEntry = (void*)((u32)buf + 0x1F & 0xFFFFFFE0);
    arenaHiOld = OSGetArenaHi();
    simMemEnd = OSPhysicalToCached(OSGetConsoleSimulatedMemSize());
    OSSetArenaHi(OSPhysicalToCached(OSGetPhysicalMemSize()));
    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();
    arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
    OSSetArenaLo(arenaLo);
    arenaLo = (void*)(((u32)arenaLo + 0x1F) & 0xFFFFFFE0);
    arenaHi = (void*)((u32)arenaHi & 0xFFFFFFE0);
    OSSetCurrentHeap(OSCreateHeap((void*)(arenaLo), arenaHi));
    OSSetArenaLo((arenaLo = arenaHi));
    OSAllocFixed(&arenaHiOld, &simMemEnd);
    length = fileInfo.length;
    offset = 0;
    while(length) {
        OSReport("loop\n");
        transferLength = (length < 0x20) ? length : 0x20;
        if (DVDReadPrio(&fileInfo, memEntry, (transferLength + 0x1F) & 0xFFFFFFE0, offset, 2) < 0) {
            OSPanic(__FILE__, 0x49F, "An error occurred when issuing read to /meminfo.bin\n");
        }
        indexMax = (transferLength / 8);
        for(i = 0; i < indexMax; i++) {
            OSReport("start: 0x%08x, end: 0x%08x\n", memEntry[i].start, memEntry[i].end);
            OSAllocFixed(&memEntry[i].start, &memEntry[i].end);
            OSReport("Removed 0x%08x - 0x%08x from the current heap\n", memEntry[i].start, (char*)memEntry[i].end-1);
        }
        length -= transferLength;
        offset += transferLength;
    }
    DVDClose(&fileInfo);
    OSDumpHeap(__OSCurrHeap);
}
#endif
