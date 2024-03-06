// probably from tgmath.h
#pragma cplusplus on 

extern inline float sqrtf(float x)
{
static const double _half=.5;
static const double _three=3.0;
volatile float y;
 if(x > 0.0f)
 {


   double guess = __frsqrte((double)x);   // returns an approximation to   
   guess = _half*guess*(_three - guess*guess*x);  // now have 12 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 24 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits
   y=(float)(x*guess);
   return y ;
 }
  return x ;
}   

extern inline float sqrt(float x)
{
static const double _half=.5;
static const double _three=3.0;
volatile float y;
 if(x > 0.0f)
 {


   double guess = __frsqrte((double)x);   // returns an approximation to   
   guess = _half*guess*(_three - guess*guess*x);  // now have 12 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 24 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits

   y=(float)(x*guess);
   return y ;
 }
  return x ;
}   

extern inline float fabs(float x) {
    (*(signed int*)&x)&=0x7fffffff;
    return  x;
}

#pragma cplusplus reset

#include <dolphin.h>

// definitions
typedef struct PerfSample {
    /* 0x00 */ u8 id;
    /* 0x04 */ u32 cpuTimeStampStart;
    /* 0x08 */ u32 cpuTimeStampEnd;
    /* 0x0C */ u32 gpTimeStampStart;
    /* 0x10 */ u32 gpTimeStampEnd;
    /* 0x14 */ int interrupted;
    /* 0x18 */ u32 origcpuStart;
    /* 0x1C */ u32 origgpStart;
    /* 0x20 */ u32 cacheMisses[4];
    /* 0x30 */ u32 instructions[4];
    /* 0x40 */ u32 cpReq[2];
    /* 0x48 */ u32 tcReq[2];
    /* 0x50 */ u32 cpuRdReq[2];
    /* 0x58 */ u32 cpuWrReq[2];
    /* 0x60 */ u32 dspReq[2];
    /* 0x68 */ u32 ioReq[2];
    /* 0x70 */ u32 viReq[2];
    /* 0x78 */ u32 peReq[2];
    /* 0x80 */ u32 rfReq[2];
    /* 0x88 */ u32 fiReq[2];
    /* 0x90 */ u32 xfWaitIn[2];
    /* 0x98 */ u32 xfWaitOut[2];
    /* 0xA0 */ u32 rasBusy[2];
    /* 0xA8 */ u32 rasClocks[2];
} PerfSample;

// internal macro for Perfdraw.
#define DRAW_RECT(x1, x2, y1, y2, color)       \
    do {                                       \
        GXSetChanMatColor(GX_COLOR0A0, color); \
        GXBegin(GX_QUADS, GX_VTXFMT0, 4U);     \
        GXPosition3f32((x1), (y1), -1.0f);     \
        GXPosition3f32((x1), (y2), -1.0f);     \
        GXPosition3f32((x2), (y2), -1.0f);     \
        GXPosition3f32((x2), (y1), -1.0f);     \
        GXEnd();                               \
    } while(0)

// externs
extern unsigned long PERFNumEvents; // size: 0x4, address: 0x0;

// .sdata
static unsigned long DrawNumFrames = 3; // size: 0x4, address: 0x0
static float DrawFrameW = 205.33333f; // size: 0x4, address: 0x4
static GXColor DrawFrameBGColor = { 0xC8, 0xC8, 0xC8, 0xC8 }; // size: 0x4, address: 0x8 
static GXColor DrawFrameColor = { 0x19, 0x19, 0x19, 0xC8 }; // size: 0x4, address: 0xC
static GXColor DrawCPUColor = { 0xFF, 0x19, 0x00, 0xC8 }; // size: 0x4, address: 0x10
static GXColor DrawFullColor = { 0xFF, 0x00, 0xFF, 0xC8 }; // size: 0x4, address: 0x14
static GXColor DrawGPColor = { 0x00, 0x64, 0xFF, 0xC8 }; // size: 0x4, address: 0x18
static GXColor DrawCPUCacheColor = { 0x00, 0x96, 0x00, 0xC8 }; // size: 0x4, address: 0x1C
static GXColor DrawConnectColor = { 0x00, 0x00, 0x00, 0xC8 }; // size: 0x4, address: 0x20
static GXColor DrawBWBarColor = { 0x32, 0x32, 0x32, 0xC8 }; // size: 0x4, address: 0x24
static GXColor DrawIPCBarColor = { 0x00, 0x00, 0x5A, 0xAA }; // size: 0x4, address: 0x28
static GXColor DrawGPUBarColor = { 0x5A, 0x00, 0x00, 0xAA }; // size: 0x4, address: 0x2C
static GXColor DrawIPCColor = { 0xC8, 0x64, 0x00, 0xAA }; // size: 0x4, address: 0x30
static GXColor DrawCPColor = { 0xC8, 0x00, 0xC8, 0xC8 }; // size: 0x4, address: 0x34
static GXColor DrawTCColor = { 0x00, 0xC8, 0x00, 0xC8 }; // size: 0x4, address: 0x38
static GXColor DrawCPURDColor = { 0xFF, 0xFF, 0x00, 0xC8 }; // size: 0x4, address: 0x3C
static GXColor DrawCPUWRColor = { 0x00, 0x64, 0x64, 0xC8 }; // size: 0x4, address: 0x40
static GXColor DrawDSPColor = { 0xC8, 0x00, 0x00, 0xC8 }; // size: 0x4, address: 0x44
static GXColor DrawIOColor = { 0x96, 0x96, 0x32, 0xC8 }; // size: 0x4, address: 0x48
static GXColor DrawVIColor = { 0xFF, 0xFF, 0xFF, 0xC8 }; // size: 0x4, address: 0x4C
static GXColor DrawPEColor = { 0x00, 0x00, 0xC8, 0xC8 }; // size: 0x4, address: 0x50
static GXColor DrawRFColor = { 0x00, 0xFF, 0xFF, 0xC8 }; // size: 0x4, address: 0x54
static GXColor DrawFIColor = { 0xC8, 0x64, 0x64, 0xC8 }; // size: 0x4, address: 0x58
static GXColor DrawGPXFIColor = { 0x00, 0xC8, 0x00, 0xAA }; // size: 0x4, address: 0x5C
static GXColor DrawGPXFOColor = { 0x00, 0x00, 0xC8, 0xAA }; // size: 0x4, address: 0x60
static GXColor DrawGPRASIDLEColor = { 0xC8, 0xC8, 0x00, 0xAA }; // size: 0x4, address: 0x64
static int bDrawBWBar = 1; // size: 0x4, address: 0x68
static int bDrawCPUBar = 1; // size: 0x4, address: 0x6C
static int bDrawXFBars = 1; // size: 0x4, address: 0x70
static int bDrawRASBar = 1; // size: 0x4, address: 0x74

// .sbss
static unsigned long DrawFrameMax; // size: 0x4, address: 0x0
static float DrawFrameH; // size: 0x4, address: 0x4
static unsigned long MaxBusTransactions; // size: 0x4, address: 0x8
static int bDrawBWBarKey; // size: 0x4, address: 0xC
static float lastx; // size: 0x4, address: 0x10
void (* GameDrawInit)(); // size: 0x4, address: 0x14

// .data
static float FramePts[28] = {
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    0.0f,
    0.0f,
    616.0f,
    0.0f,
    616.0f,
    0.0f,
    616.0f,
    10.0f,
    0.0f,
    10.0f,
    616.0f,
    10.0f,
    205.33333f,
    0.0f,
    205.33333f,
    10.0f,
    410.66666f,
    0.0f,
    410.66666f,
    10.0f,
    616.0f,
    0.0f,
    616.0f,
    10.0f,
};

static float CPUPts[4] = {
    0.0f,  
    0.0f,
    616.0f,
    0.0f,
};

static float GPPts[4] = {
    0.0f,  
    0.0f,
    616.0f,
    0.0f,
};

// functions
void __PERFDrawInit(void (* id)());
void PERFPreDraw();
static void DrawBWBar(struct PerfSample * s);
float HEIGHT(unsigned long a, float f);
float COORD(unsigned long a /* r3 */);
static void DrawKey();
void PERFDumpScreen();
void PERFPostDraw();
void PERFSetDrawBWBarKey(int tf);
void PERFSetDrawBWBar(int tf);
void PERFSetDrawCPUBar(int tf);
void PERFSetDrawXFBars(int tf);
void PERFSetDrawRASBar(int tf);
void PERFToggleDrawBWBarKey();
void PERFToggleDrawBWBar();
void PERFToggleDrawCPUBar();
void PERFToggleDrawXFBars();
void PERFToggleDrawRASBar();

// .bss
float mID[3][4]; // size: 0x30, address: 0x0

void __PERFDrawInit(void (* id)()) {
    C_MTXIdentity(mID);
    GameDrawInit = id;
    DrawFrameMax = (OS_CORE_CLOCK/60U) * 3;
    DrawFrameH = (PERFNumEvents + 1) * 7;
    MaxBusTransactions = (OS_BUS_CLOCK / 120);
    FramePts[3] = FramePts[11] = FramePts[13] = FramePts[15] = FramePts[19] = FramePts[23] = DrawFrameH;
    FramePts[6] = FramePts[8] = FramePts[10] = FramePts[14] = 616.0f;
    FramePts[33] = (PERFNumEvents + 2) * 19;
    FramePts[35] = FramePts[33];
}

static float mProj[4][4]; // size: 0x40, address: 0x30
float pSave[7]; // size: 0x1C, address: 0x70

void PERFPreDraw() {
    unsigned long i;
    unsigned long j;

    GXGetProjectionv(pSave);
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            mID[i][j] = 0.0f;
        }
    }
    mProj[0][0] = 0.003125f;
    mProj[1][1] = 0.004166667f; 
    mProj[2][2] = 1.f;
    mProj[3][3] = 1.f;
    mProj[0][3] = -0.95f;
    mProj[1][3] = -0.87500005f;
    GXSetProjection(mProj, GX_ORTHOGRAPHIC);
    GXClearVtxDesc();
    GXInvalidateVtxCache();
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(0U);
    GXSetNumChans(1U);
    GXSetChanCtrl(GX_COLOR0A0, 0U, GX_SRC_REG, GX_SRC_REG, 0U, GX_DF_NONE, GX_AF_NONE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0U);
    GXSetNumTevStages(1U);
    GXSetZMode(0U, GX_ALWAYS, 0U);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0U);
}

static void DrawBWBar(struct PerfSample * s) {
    unsigned long delta;
    unsigned long interval;
    float bwscale;
    float lastY;
    float x1;
    float x2;
    float height;
    unsigned long rasclocks;
    unsigned long rasBusy;
    unsigned long xfI;
    unsigned long xfO;
    unsigned long instructions;
    float ipc;
    float ipcscale;
    unsigned long misses;

    interval = s->gpTimeStampEnd - s->gpTimeStampStart;
    bwscale = (f32)interval / (OS_CORE_CLOCK / 60);
    lastY = 7.0f + DrawFrameH;
    x1 = COORD(s->gpTimeStampStart);
    x2 = COORD(s->gpTimeStampEnd);
    if (fabs(lastx - x1) < 1.0f) {
        x1 = lastx;
    }
    lastx = x2;

    // Draw BW Bars if toggled
    if (bDrawBWBar != 0) {
        delta = s->cpReq[1] - s->cpReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPColor);
            lastY += height;
        }
        delta = s->tcReq[1] - s->tcReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawTCColor);
            lastY += height;
        }
        delta = s->cpuRdReq[1] - s->cpuRdReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPURDColor);
            lastY += height;
        }
        delta = s->cpuWrReq[1] - s->cpuWrReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPUWRColor);
            lastY += height;
        }
        delta = s->dspReq[1] - s->dspReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawDSPColor);
            lastY += height;
        }
        delta = s->ioReq[1] - s->ioReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawIOColor);
            lastY += height;
        }
        delta = s->viReq[1] - s->viReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawVIColor);
            lastY += height;
        }
        delta = s->peReq[1] - s->peReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawPEColor);
            lastY += height;
        }
        delta = s->rfReq[1] - s->rfReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawRFColor);
            lastY += height;
        }
        s->fiReq[0] /= 2;
        s->fiReq[1] /= 2;
        delta = s->fiReq[1] - s->fiReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawFIColor);
        }
    }
    if (bDrawCPUBar != 0) {
        instructions = s->instructions[1] - s->instructions[0];
        ipc = (f32)instructions / (f32) interval;
        ipcscale = ipc / 2.0f;
        misses = s->cacheMisses[1] - s->cacheMisses[0];
        lastY = 7.0f + (140.0f + (7.0f + DrawFrameH));
        height = ipcscale * 0.5f;
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawIPCColor);
        }
        height = (50.0f * (f32) misses) / (f32) interval;
        DRAW_RECT(x1, x2, (50.0f + lastY) - height, (50.0f + lastY), DrawCPUCacheColor);
    }
    rasclocks = s->rasClocks[1] - s->rasClocks[0];
    if (bDrawXFBars != 0) {
        lastY = 14.0f + (50.0f + (140.0f + (7.0f + DrawFrameH)));
        xfI = s->xfWaitIn[1] - s->xfWaitIn[0];
        xfO = s->xfWaitOut[1] - s->xfWaitOut[0];
        if (rasclocks >= (u32) (xfO + xfI)) {
            xfI = rasclocks - (xfO + xfI);
            height = (50.0f * xfI) / rasclocks;
            if (height > 1.0f) {
                DRAW_RECT(x1, x2, lastY, lastY + height, DrawGPXFIColor);
            }
        }
    }
    if (bDrawRASBar != 0) {
        lastY = 50.0f + (21.0f + (50.0f + (140.0f + (7.0f + DrawFrameH))));
        rasBusy = s->rasBusy[1] - s->rasBusy[0];
        height = (50.0f * (f32)rasBusy) / rasclocks;
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawGPRASIDLEColor);
        }
    }
}

float HEIGHT(unsigned long a, float f) {
    return 140.0f * ((f32) a / ((f32) MaxBusTransactions * f));
}

float COORD(unsigned long a) {
    return 616.0f * ((f32) a / (f32) DrawFrameMax);
}

static void DrawKey() {
    unsigned long delta;
    unsigned long foo[2];
    float bwscale;
    float lastY;
    float x1;
    float x2;
    float height;

    x1 = 595.4667f;
    x2 = 616.0f;
    lastY = 7.0f + DrawFrameH;
    bwscale = -1.0f;
    foo[0] = 0;
    foo[1] = MaxBusTransactions / 10;

    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) {
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPColor);
        lastY += height; 
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) {
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawTCColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPURDColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPUWRColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawDSPColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawIOColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawVIColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawPEColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawRFColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawFIColor);
        lastY += height;
    }
}

struct Frame {
    // total size: 0x10
    struct PerfSample * samples; // offset 0x0, size 0x4
    long lastSample; // offset 0x4, size 0x4
    unsigned long end; // offset 0x8, size 0x4
    unsigned long cachemisscycles; // offset 0xC, size 0x4
};
extern struct Frame * PERFFrames; // size: 0x4, address: 0x0
extern unsigned long PERFCurrFrame; // size: 0x4, address: 0x0

struct PerfEvent {
    // total size: 0x10
    char * name; // offset 0x0, size 0x4
    PerfType type; // offset 0x4, size 0x4
    long currSample; // offset 0x8, size 0x4
    struct _GXColor color; // offset 0xC, size 0x4
};
extern struct PerfEvent * PERFEvents; // size: 0x4, address: 0x0

void PERFDumpScreen() {
    struct PerfSample * samples; // r30
    unsigned long s; // r31
    unsigned long id; // r28
    unsigned long i; // r26
    unsigned long delta; // r27
    unsigned long e; // r25
    float c; // f29
    float lastY; // f31
    float allX; // f30
    float xcoord; // f28

    if (GameDrawInit) {
        samples = PERFFrames[PERFCurrFrame].samples;
        DrawNumFrames = PERFFrames[PERFCurrFrame].end / (OS_CORE_CLOCK/60U) + 1;
        DrawFrameMax = (OS_CORE_CLOCK/60U) * DrawNumFrames;
        DrawFrameW = 616.0f / DrawNumFrames;
        allX = COORD(PERFFrames[PERFCurrFrame].end);
        GXLoadPosMtxImm(mID, 0);
        DRAW_RECT(0.0f, 616.0f, 0.0f, DrawFrameH, DrawFrameBGColor);
        GXSetChanMatColor(GX_COLOR0A0, DrawFrameColor);
        GXSetLineWidth(0xCU, GX_TO_ZERO);
        // different draw shape? Consider other forms of this draw macro that may work. If anything will work.
        GXBegin(GX_LINES, GX_VTXFMT0, 8U);
        GXPosition3f32(FramePts[0], FramePts[1], -1.0f);
        GXPosition3f32(FramePts[2], FramePts[3], -1.0f);
        GXPosition3f32(FramePts[4], FramePts[5], -1.0f);
        GXPosition3f32(FramePts[6], FramePts[7], -1.0f);
        GXPosition3f32(FramePts[8], FramePts[9], -1.0f);
        GXPosition3f32(FramePts[10], FramePts[11], -1.0f);
        GXPosition3f32(FramePts[12], FramePts[13], -1.0f);
        GXPosition3f32(FramePts[14], FramePts[15], -1.0f);
        GXEnd();
        if (DrawNumFrames > 1U) {
            GXBegin(GX_LINES, GX_VTXFMT0, (DrawNumFrames - 1) * 2);
            for(i = 1; i < DrawNumFrames; i++) {
                xcoord = COORD(i * (OS_CORE_CLOCK/60U));
                GXPosition3f32(xcoord, FramePts[17], -1.0f);
                GXPosition3f32(xcoord, FramePts[19], -1.0f);
            }
            GXEnd();
        }
        GXSetChanMatColor(GX_COLOR0A0, DrawFullColor);
        GXSetLineWidth(0x20U, GX_TO_ZERO);
        GXBegin(GX_LINES, GX_VTXFMT0, 2U);
        GXPosition3f32(0.0f, 0.0f, -1.0f);
        GXPosition3f32(allX, 0.0f, -1.0f);
        GXEnd();
        GXSetChanMatColor(GX_COLOR0A0, DrawCPUCacheColor);
        GXBegin(GX_LINES, GX_VTXFMT0, 2U);
        GXPosition3f32(0.0f, 0.0f, -1.0f);
        GXPosition3f32(COORD(PERFFrames[PERFCurrFrame].cachemisscycles), 0.0f, -1.0f); 
        GXEnd();
        if (bDrawBWBar != 0) {
            lastY = 7.0f + DrawFrameH;
            GXSetChanMatColor(GX_COLOR0A0, DrawBWBarColor);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4U);
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 140.0f + lastY, -1.0f);
            GXPosition3f32(allX, 140.0f + lastY, -1.0f); 
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
        }
        if (bDrawCPUBar != 0) {
            lastY = 7.0f + (140.0f + (7.0f + DrawFrameH));
            GXSetChanMatColor(GX_COLOR0A0, DrawIPCBarColor);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4U);
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
            GXSetChanMatColor(GX_COLOR0A0, DrawConnectColor);
            GXSetLineWidth(6U, GX_TO_ZERO);
            GXBegin(GX_LINES, GX_VTXFMT0, 2U);
            GXPosition3f32(0.0f, 25.0f + lastY, -1.0f);
            GXPosition3f32(allX, 25.0f + lastY, -1.0f);
            GXEnd();
        }
        if (bDrawXFBars != 0) {
            lastY = 14.0f + (50.0f + (140.0f + (7.0f + DrawFrameH)));
            GXSetChanMatColor(GX_COLOR0A0, DrawGPUBarColor);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4U);
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
        }
        if (bDrawRASBar != 0) {
            lastY = 50.0f + (21.0f + (50.0f + (140.0f + (7.0f + DrawFrameH))));
            GXSetChanMatColor(GX_COLOR0A0, DrawGPUBarColor);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4U); 
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
        }
        for(s = 0; s < PERFFrames[PERFCurrFrame].lastSample; s++) {
            id = samples[s].id;
            if (id == 0xFF) {
                DrawBWBar(&samples[s]);
            } else {
                switch(PERFEvents[id].type) {
                    case PERF_CPU_GP_EVENT:
                        GXSetChanMatColor(GX_COLOR0A0, PERFEvents[id].color);
                        GXSetLineWidth(0x20U, GX_TO_ZERO);
                        GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                        GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                        GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f); 
                        GXEnd();
                        delta = samples[s].cacheMisses[3] - samples[s].cacheMisses[2];
                        if (delta) {
                            e = delta + samples[s].cpuTimeStampStart;
                            c = COORD(e);
                            GXSetChanMatColor(GX_COLOR0A0, DrawCPUCacheColor);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                            GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                            GXPosition3f32(c, (f32) ((id + 1) * 7), -1.0f);
                            GXEnd();
                        }

                        if (samples[s].gpTimeStampEnd != 0) {
                            if (!samples[s].interrupted) {
                                GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                                GXSetLineWidth(0x20U, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                                GXPosition3f32(COORD(samples[s].gpTimeStampStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXEnd();

                                DrawBWBar(&samples[s]);
                                
                                GXSetChanMatColor(GX_COLOR0A0, DrawConnectColor);
                                GXSetLineWidth(0x6U, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 4U);
                                GXPosition3f32(COORD(samples[s].gpTimeStampStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f);
                                GXEnd();
                            } else {
                                GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                                GXSetLineWidth(0x20U, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                                GXPosition3f32(COORD(samples[s].origgpStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXEnd();

                                DrawBWBar(&samples[s]);
                                
                                GXSetChanMatColor(GX_COLOR0A0, DrawConnectColor);
                                GXSetLineWidth(0x6U, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 4U);
                                GXPosition3f32(COORD(samples[s].origgpStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].origcpuStart), (f32) ((id + 1) * 7), -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f);
                                GXEnd();
                            }
                        }
                        break;
                    case PERF_CPU_EVENT:
                        GXSetChanMatColor(GX_COLOR0A0, PERFEvents[id].color);
                        GXSetLineWidth(0x20U, GX_TO_ZERO);
                        GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                        GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                        GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f);
                        GXEnd();

                        delta = samples[s].cacheMisses[3] - samples[s].cacheMisses[2];
                        if (delta != 0U) {
                            e = delta + samples[s].cpuTimeStampStart;
                            c = COORD(e);
                            GXSetChanMatColor(GX_COLOR0A0, DrawCPUCacheColor);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                            GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                            GXPosition3f32(c, (f32) ((id + 1) * 7), -1.0f);
                            GXEnd();
                        }
                        break;
                    case PERF_GP_EVENT:
                        if (samples[s].interrupted == 0) {
                            GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                            GXSetLineWidth(0x20U, GX_TO_ZERO);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                            GXPosition3f32(COORD(samples[s].gpTimeStampStart), DrawFrameH, -1.0f);
                            GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                            GXEnd();
                            DrawBWBar(&samples[s]);
                        } else {
                            GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                            GXSetLineWidth(0x20U, GX_TO_ZERO);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2U);
                            GXPosition3f32(COORD(samples[s].origgpStart), DrawFrameH, -1.0f);
                            GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                            GXEnd();
                            DrawBWBar(&samples[s]);
                        }
                        break;
                }
            }
        }
        if (bDrawBWBarKey != 0) {
            DrawKey();
        }
    }
}

void PERFPostDraw() {
    unsigned long i;
    unsigned long j;

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            mProj[i][j] = 0.0f;
        }
    }
    mProj[0][0] = pSave[1];
    mProj[0][2] = pSave[2];
    mProj[1][1] = pSave[3];
    mProj[1][2] = pSave[4];
    mProj[2][2] = pSave[5];
    mProj[2][3] = pSave[6];
    mProj[3][2] = -1.0f;
    GXSetProjection(mProj, GX_PERSPECTIVE);
    GameDrawInit();
}

void PERFSetDrawBWBarKey(int tf) {
    bDrawBWBarKey = tf;
}

void PERFSetDrawBWBar(int tf) {
    bDrawBWBar = tf;
}

void PERFSetDrawCPUBar(int tf) {
    bDrawCPUBar = tf;
}

void PERFSetDrawXFBars(int tf) {
    bDrawXFBars = tf;
}

void PERFSetDrawRASBar(int tf) {
    bDrawRASBar = tf;
}

void PERFToggleDrawBWBarKey() {
    bDrawBWBarKey = (bDrawBWBarKey) ? 0 : 1; 
}

void PERFToggleDrawBWBar() {
    bDrawBWBar = (bDrawBWBar) ? 0 : 1; 
}

void PERFToggleDrawCPUBar() {
    bDrawCPUBar = (bDrawCPUBar) ? 0 : 1; 
}

void PERFToggleDrawXFBars() {
    bDrawXFBars = (bDrawXFBars) ? 0 : 1; 
}

void PERFToggleDrawRASBar() {
    bDrawRASBar = (bDrawRASBar) ? 0 : 1; 
}
