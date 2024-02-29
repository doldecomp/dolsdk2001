#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <macros.h>

#define GX_WRITE_U8(ub)	    \
    GXWGFifo.u8 = (u8)(ub)

#define GX_WRITE_U16(us)   \
   GXWGFifo.u16 = (u16)(us)

#define GX_WRITE_U32(ui)   \
   GXWGFifo.u32 = (u32)(ui)

#define GX_WRITE_F32(f)	 	\
   GXWGFifo.f32 = (f32)(f);

struct __GXVerifyData {
    // total size: 0x13F8
    void (* cb)(GXAttr, unsigned long, char *); // offset 0x0, size 0x4
    GXAttr verifyLevel; // offset 0x4, size 0x4
    unsigned long xfRegs[80]; // offset 0x8, size 0x140
    unsigned long xfMtx[256]; // offset 0x148, size 0x400
    unsigned long xfNrm[96]; // offset 0x548, size 0x180
    unsigned long xfDMtx[256]; // offset 0x6C8, size 0x400
    unsigned long xfLight[128]; // offset 0xAC8, size 0x200
    unsigned long rasRegs[256]; // offset 0xCC8, size 0x400
    unsigned char xfRegsDirty[80]; // offset 0x10C8, size 0x50
    unsigned char xfMtxDirty[256]; // offset 0x1118, size 0x100
    unsigned char xfNrmDirty[96]; // offset 0x1218, size 0x60
    unsigned char xfDMtxDirty[256]; // offset 0x1278, size 0x100
    unsigned char xfLightDirty[128]; // offset 0x1378, size 0x80
};

extern struct __GXVerifyData *__gxVerif;

extern u8 __GXinBegin;

extern struct __GXData_struct {
    // total size: 0x4F4
    unsigned short vNum; // offset 0x0, size 0x2
    unsigned short bpSent; // offset 0x2, size 0x2
    unsigned long vLim; // offset 0x4, size 0x4
    unsigned long cpEnable; // offset 0x8, size 0x4
    unsigned long cpStatus; // offset 0xC, size 0x4
    unsigned long cpClr; // offset 0x10, size 0x4
    unsigned long vcdLo; // offset 0x14, size 0x4
    unsigned long vcdHi; // offset 0x18, size 0x4
    unsigned long vatA[8]; // offset 0x1C, size 0x20
    unsigned long vatB[8]; // offset 0x3C, size 0x20
    unsigned long vatC[8]; // offset 0x5C, size 0x20
    unsigned long lpSize; // offset 0x7C, size 0x4
    unsigned long matIdxA; // offset 0x80, size 0x4
    unsigned long matIdxB; // offset 0x84, size 0x4
    unsigned long indexBase[4]; // offset 0x88, size 0x10
    unsigned long indexStride[4]; // offset 0x98, size 0x10
    unsigned long ambColor[2]; // offset 0xA8, size 0x8
    unsigned long matColor[2]; // offset 0xB0, size 0x8
    unsigned long suTs0[8]; // offset 0xB8, size 0x20
    unsigned long suTs1[8]; // offset 0xD8, size 0x20
    unsigned long suScis0; // offset 0xF8, size 0x4
    unsigned long suScis1; // offset 0xFC, size 0x4
    unsigned long tref[8]; // offset 0x100, size 0x20
    unsigned long iref; // offset 0x120, size 0x4
    unsigned long bpMask; // offset 0x124, size 0x4
    unsigned long IndTexScale0; // offset 0x128, size 0x4
    unsigned long IndTexScale1; // offset 0x12C, size 0x4
    unsigned long tevc[16]; // offset 0x130, size 0x40
    unsigned long teva[16]; // offset 0x170, size 0x40
    unsigned long tevKsel[8]; // offset 0x1B0, size 0x20
    unsigned long cmode0; // offset 0x1D0, size 0x4
    unsigned long cmode1; // offset 0x1D4, size 0x4
    unsigned long zmode; // offset 0x1D8, size 0x4
    unsigned long peCtrl; // offset 0x1DC, size 0x4
    unsigned long cpDispSrc; // offset 0x1E0, size 0x4
    unsigned long cpDispSize; // offset 0x1E4, size 0x4
    unsigned long cpDispStride; // offset 0x1E8, size 0x4
    unsigned long cpDisp; // offset 0x1EC, size 0x4
    unsigned long cpTexSrc; // offset 0x1F0, size 0x4
    unsigned long cpTexSize; // offset 0x1F4, size 0x4
    unsigned long cpTexStride; // offset 0x1F8, size 0x4
    unsigned long cpTex; // offset 0x1FC, size 0x4
    unsigned char cpTexZ; // offset 0x200, size 0x1
    unsigned long genMode; // offset 0x204, size 0x4
    GXTexRegion TexRegions[8]; // offset 0x208, size 0x80
    GXTexRegion TexRegionsCI[4]; // offset 0x288, size 0x40
    unsigned long nextTexRgn; // offset 0x2C8, size 0x4
    unsigned long nextTexRgnCI; // offset 0x2CC, size 0x4
    GXTlutRegion TlutRegions[20]; // offset 0x2D0, size 0x140
    GXTexRegion * (* texRegionCallback)(GXTexObj *, GXTexMapID); // offset 0x410, size 0x4
    GXTlutRegion * (* tlutRegionCallback)(unsigned long); // offset 0x414, size 0x4
    GXAttrType nrmType; // offset 0x418, size 0x4
    unsigned char hasNrms; // offset 0x41C, size 0x1
    unsigned char hasBiNrms; // offset 0x41D, size 0x1
    unsigned long projType; // offset 0x420, size 0x4
    float projMtx[6]; // offset 0x424, size 0x18
    float vpLeft; // offset 0x43C, size 0x4
    float vpTop; // offset 0x440, size 0x4
    float vpWd; // offset 0x444, size 0x4
    float vpHt; // offset 0x448, size 0x4
    float vpNearz; // offset 0x44C, size 0x4
    float vpFarz; // offset 0x450, size 0x4
    unsigned char fgRange; // offset 0x454, size 0x1
    float fgSideX; // offset 0x458, size 0x4
    unsigned long tImage0[8]; // offset 0x45C, size 0x20
    unsigned long tMode0[8]; // offset 0x47C, size 0x20
    unsigned long texmapId[16]; // offset 0x49C, size 0x40
    unsigned long tcsManEnab; // offset 0x4DC, size 0x4
    GXPerf0 perf0; // offset 0x4E0, size 0x4
    GXPerf1 perf1; // offset 0x4E4, size 0x4
    unsigned long perfSel; // offset 0x4E8, size 0x4
    unsigned char inDispList; // offset 0x4EC, size 0x1
    unsigned char dlSaveContext; // offset 0x4ED, size 0x1
    unsigned char dirtyVAT; // offset 0x4EE, size 0x1
    unsigned long dirtyState; // offset 0x4F0, size 0x4
} *gx;  // size = 0x4F4

#define CHECK_GXBEGIN(line, name)    ASSERTMSGLINE(__FILE__, line, !__GXinBegin, "'" name "' is not allowed between GXBegin/GXEnd")
#define CHECK_ATTRPTR(line, attrPtr) ASSERTMSGLINE(__FILE__, line, (attrPtr) != NULL, "GXSetVtxDescv: attrPtr is NULL")
#define CHECK_ATTRNAME(line, attr)   ASSERTMSGLINE(__FILE__, line, (attr) >= 0 && (attr) < 0x1A, "GXSetVtxDesc: Invalid vertex attribute name")
#define CHECK_ATTRNAME2(line, attr)  ASSERTMSGLINE(__FILE__, line, (attr) >= 9 && (attr) <= 0x1A, "GXSetVtxAttrFmt: Invalid vertex attribute name")
#define CHECK_ATTRNAME3(line, attr)  ASSERTMSGLINE(__FILE__, line, (attr) >= 9 && (attr) <= 0x18, "GXSetArray: Invalid vertex attribute name")
#define CHECK_ATTRTYPE(line, type)   ASSERTMSGLINE(__FILE__, line, (type) >= 0 && (type) <= 3, "GXSetVtxDesc: Invalid vertex attribute type")
#define CHECK_VTXFMT(line, vtxfmt)   ASSERTMSGLINE(__FILE__, line, (vtxfmt) < 8, "GXSetVtxAttrFmt: Format Index is out of range")
#define CHECK_FRAC(line, frac)       ASSERTMSGLINE(__FILE__, line, (frac) < 32, "GXSetVtxAttrFmt: Frac value is >= 32")
#define CHECK_LISTPTR(line, list)    ASSERTMSGLINE(__FILE__, line, (list) != NULL, "GXSetVtxAttrFmt: list pointer is NULL")

#define SET_REG(line, reg, size, shift, val) \
do { \
    ASSERTMSGLINE("GXAttr.c", line, ((val) & ~((1 << (size)) - 1)) == 0, "GX Internal: Register field out of range"); \
    (reg) = ((reg) & ~(((1 << (size)) - 1) << (shift))) | ((val) << (shift)); \
} while (0)

#define SET_REG2(line, reg, size, shift, val) \
do { \
    if (((val) & ~((1 << (size)) - 1))) \
        OSPanic("GXAttr.c", line, "GX Internal: Register field out of range"); \
    (reg) = ((reg) & ~(((1 << (size)) - 1) << (shift))) | ((val) << (shift)); \
} while (0)

static void SETVCDATTR(GXAttr Attr, GXAttrType Type);

static void __GXXfVtxSpecs(void)
{
    u32 nCols = 0;
    u32 nNrm;
    u32 nTex;
    u32 reg;
    long regAddr;

    nCols = ((int)(gx->vcdLo >> 13) & 3) ? 1 : 0;
    nCols += ((int)(gx->vcdLo >> 15) & 3) ? 1 : 0;
    nNrm = gx->hasBiNrms ? 2 : gx->hasNrms ? 1 : 0;
    nTex = 0;
    nTex += (gx->vcdHi & 3) ? 1 : 0;
    nTex += ((int)(gx->vcdHi >> 2) & 3) ? 1 : 0;
    nTex += ((int)(gx->vcdHi >> 4) & 3) ? 1 : 0;
    nTex += ((int)(gx->vcdHi >> 6) & 3) ? 1 : 0;
    nTex += ((int)(gx->vcdHi >> 8) & 3) ? 1 : 0;
    nTex += ((int)(gx->vcdHi >> 10) & 3) ? 1 : 0;
    nTex += ((int)(gx->vcdHi >> 12) & 3) ? 1 : 0;
    nTex += ((int)(gx->vcdHi >> 14) & 3) ? 1 : 0;
    reg = (nCols) | (nNrm << 2) | (nTex << 4);
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(0x1008);
    GX_WRITE_U32(reg);
    regAddr = 8;
    if ((regAddr >= 0) && (regAddr < 0x50)) {
		__gxVerif->xfRegs[regAddr] = reg;
		__gxVerif->xfRegsDirty[regAddr] = TRUE;
    }
    gx->bpSent = 0;
}

void GXSetVtxDesc(GXAttr attr, GXAttrType type)
{
	CHECK_GXBEGIN(0xCC, "GXSetVtxDesc");
	CHECK_ATTRNAME(0xCF, attr);
	CHECK_ATTRTYPE(0xD1, type);

    SETVCDATTR(attr, type);
    if ((gx->hasNrms != 0) || (gx->hasBiNrms != 0)) {
        if (gx->nrmType & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xD7, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFE7FF) | (gx->nrmType << 0xB);
    } else {
        gx->vcdLo = (gx->vcdLo & 0xFFFFE7FF);
    }
    gx->dirtyState |= 8;
}

static void SETVCDATTR(GXAttr Attr, GXAttrType Type)
{
    switch (Attr) {
    case 0:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xA8, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFFFE) | Type;
        break;
    case 1:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xA9, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFFFD) | (Type * 2);
        break;
    case 2:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xAA, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFFFB) | (Type * 4);
        break;
    case 3:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xAB, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFFF7) | (Type * 8);
        break;
    case 4:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xAC, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFFEF) | (Type * 0x10);
        break;
    case 5:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xAD, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFFDF) | (Type << 5);
        break;
    case 6:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xAE, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFFBF) | (Type << 6);
        break;
    case 7:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xAF, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFF7F) | (Type << 7);
        break;
    case 8:
        if (Type & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0xB0, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFFEFF) | (Type << 8);
        break;
    case 9:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xB1, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFF9FF) | (Type << 9);
        break;
    case 10:
        gx->hasNrms = (Type != 0);
        if (Type != GX_NONE) {
            gx->nrmType = Type;
            break;
        }
    default:
        break;
    case 25:
        gx->hasBiNrms = (Type != 0);
        if (Type != GX_NONE) {
            gx->nrmType = Type;
            break;
        }
        break;
    case 11:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xBA, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFF9FFF) | (Type << 0xD);
        break;
    case 12:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xBB, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFE7FFF) | (Type << 0xF);
        break;
    case 13:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xBC, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFFFFFC) | Type;
        break;
    case 14:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xBD, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFFFFF3) | (Type * 4);
        break;
    case 15:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xBE, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFFFFCF) | (Type * 0x10);
        break;
    case 16:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xBF, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFFFF3F) | (Type << 6);
        break;
    case 17:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xC0, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFFFCFF) | (Type << 8);
        break;
    case 18:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xC1, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFFF3FF) | (Type << 0xA);
        break;
    case 19:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xC2, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFFCFFF) | (Type << 0xC);
        break;
    case 20:
        if (Type & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0xC3, "GX Internal: Register field out of range");
        }
        gx->vcdHi = (gx->vcdHi & 0xFFFF3FFF) | (Type << 0xE);
        break;
    }
}

void GXSetVtxDescv(struct _GXVtxDescList * attrPtr)
{
	CHECK_GXBEGIN(0xF5, "GXSetVtxDescv");
	CHECK_ATTRPTR(0xF6, attrPtr);
    while (attrPtr->attr != 0xFF) {
		CHECK_ATTRNAME(0xFB, attrPtr->attr);
		CHECK_ATTRTYPE(0xFE, attrPtr->type);
        SETVCDATTR(attrPtr->attr, attrPtr->type);
        attrPtr++;
    }
    if (gx->hasNrms || gx->hasBiNrms) {
        if (gx->nrmType & 0xFFFFFFFC) {
            OSPanic("GXAttr.c", 0x107, "GX Internal: Register field out of range");
        }
        gx->vcdLo = (gx->vcdLo & 0xFFFFE7FF) | (gx->nrmType << 0xB);
    } else {
        gx->vcdLo = gx->vcdLo & 0xFFFFE7FF;
    }
    gx->dirtyState |= 8;
}

void __GXSetVCD(void)
{
	static u8 tbl1[] = { 0, 4, 1, 2 };
	static u8 tbl2[] = { 0, 8, 1, 2 };
	static u8 tbl3[] = { 0, 12, 1, 2 };
    unsigned long vlm;
    unsigned long b;
    unsigned long vl;
    unsigned long vh;

	GX_WRITE_U8(8);
	GX_WRITE_U8(0x50);
	GX_WRITE_U32(gx->vcdLo);
    { volatile long regAddr = -12; }
    GX_WRITE_U8(8);
	GX_WRITE_U8(0x60);
	GX_WRITE_U32(gx->vcdHi);
    { volatile long regAddr = -12; }
    __GXXfVtxSpecs();
    if (gx->vNum != 0) {
        vl = gx->vcdLo;
        vh = gx->vcdHi;
        vlm  = (vl >> 0) & 1;
        vlm += (vl >> 1) & 1;
        vlm += (vl >> 2) & 1;
        vlm += (vl >> 3) & 1;
        vlm += (vl >> 4) & 1;
        vlm += (vl >> 5) & 1;
        vlm += (vl >> 6) & 1;
        vlm += (vl >> 7) & 1;
        vlm += (vl >> 8) & 1;
        vlm += tbl3[(vl >> 9) & 3];
        b = (gx->hasBiNrms << 1) + 1;
        vlm += tbl3[(vl >> 11) & 3] * b;
        vlm += tbl1[(vl >> 13) & 3];
        vlm += tbl1[(vl >> 15) & 3];
        vlm += tbl2[(vh >> 0) & 3];
        vlm += tbl2[(vh >> 2) & 3];
        vlm += tbl2[(vh >> 4) & 3];
        vlm += tbl2[(vh >> 6) & 3];
        vlm += tbl2[(vh >> 8) & 3];
        vlm += tbl2[(vh >> 10) & 3];
        vlm += tbl2[(vh >> 12) & 3];
        vlm += tbl2[(vh >> 14) & 3];
        gx->vLim = vlm;
    }
}

void GXGetVtxDesc(GXAttr attr, GXAttrType *type)
{
    u32 cpType;

	CHECK_GXBEGIN(0x185, "GXGetVtxDesc");
	CHECK_ATTRNAME(0x187, attr);

    switch (attr) {
    case 0:
        cpType = ((int)gx->vcdLo) & 1;
        break;
    case 1:
        cpType = (int)(gx->vcdLo >> 1) & 1;
        break;
    case 2:
        cpType = (int)(gx->vcdLo >> 2) & 1;
        break;
    case 3:
        cpType = (int)(gx->vcdLo >> 3) & 1;
        break;
    case 4:
        cpType = (int)(gx->vcdLo >> 4) & 1;
        break;
    case 5:
        cpType = (int)(gx->vcdLo >> 5) & 1;
        break;
    case 6:
        cpType = (int)(gx->vcdLo >> 6) & 1;
        break;
    case 7:
        cpType = (int)(gx->vcdLo >> 7) & 1;
        break;
    case 8:
        cpType = (int)(gx->vcdLo >> 8) & 1;
        break;
    case 9:
        cpType = (int)(gx->vcdLo >> 9) & 3;
        break;
    case 10:
		cpType = gx->hasNrms ? (int)(gx->vcdLo >> 11) & 3 : 0;
        break;
    case 25:
        cpType = gx->hasBiNrms ? (int)(gx->vcdLo >> 11) & 3 : 0;
        break;
    case 11:
        cpType =(int) (gx->vcdLo >> 13) & 3;
        break;
    case 12:
        cpType = (int)(gx->vcdLo >> 15) & 3;
        break;
    case 13:
        cpType = gx->vcdHi & 3;
        break;
    case 14:
        cpType = (int)(gx->vcdHi >> 2) & 3;
        break;
    case 15:
        cpType = (int)(gx->vcdHi >> 4) & 3;
        break;
    case 16:
        cpType = (int)(gx->vcdHi >> 6) & 3;
        break;
    case 17:
        cpType = (int)(gx->vcdHi >> 8) & 3;
        break;
    case 18:
        cpType = (int)(gx->vcdHi >> 10) & 3;
        break;
    case 19:
        cpType = (int)(gx->vcdHi >> 12) & 3;
        break;
    case 20:
        cpType = (int)(gx->vcdHi >> 14) & 3;
        break;
    default:
        cpType = 0;
        break;
    }
    *type = cpType;
}

void GXGetVtxDescv(GXVtxDescList *vcd)
{
    GXAttr attr;

	CHECK_GXBEGIN(0x1BA, "GXGetVtxDescv");
	CHECK_ATTRPTR(0x1BC, vcd);
    for (attr = 0; attr < 0x1A; attr++) {
		vcd[attr].attr = attr;
		GXGetVtxDesc(attr, &vcd[attr].type);
	}
	vcd[attr].attr = 0xFF;
}

void GXClearVtxDesc(void)
{
	CHECK_GXBEGIN(0x1D3, "GXClearVtxDesc");
    gx->vcdLo = 0;
    gx->vcdLo = (gx->vcdLo & 0xFFFFF9FF) | 0x200;
    gx->vcdHi = 0;
    gx->hasNrms = 0;
    gx->hasBiNrms = 0;
    gx->dirtyState |= 8;
}

static void SETVAT(u32 *va, u32 *vb, u32 *vc, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 shft);

void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac)
{
    u32 *va;
    u32 *vb;
    u32 *vc;

	CHECK_GXBEGIN(0x252, "GXSetVtxAttrFmt");
	CHECK_VTXFMT(0x253, vtxfmt);
	CHECK_ATTRNAME2(0x255, attr);
	CHECK_FRAC(0x256, frac);
    va = &gx->vatA[vtxfmt];
    vb = &gx->vatB[vtxfmt];
    vc = &gx->vatC[vtxfmt];
    SETVAT(va, vb, vc, attr, cnt, type, frac);
    gx->dirtyState |= 0x10;
    gx->dirtyVAT |= (u8)(1 << (u8)vtxfmt);
}

#define GX_VAT_REG_A_UNK_SIZE 1
#define GX_VAT_REG_A_UNK_SHIFT 30
#define GX_VAT_REG_A_UNK_MASK 0x40000000

#define GX_VAT_REG_B_UNK_SIZE 1
#define GX_VAT_REG_B_UNK_SHIFT 31
#define GX_VAT_REG_B_UNK_MASK 0x80000000

#define GX_UNK_REG_FIELD_SIZE 3
#define GX_UNK_REG_FIELD_SHIFT 1
#define GX_UNK_REG_FIELD_MASK (~0xFFFFFFF1)

#define GX_REG_ASSERT(file, line, c) ASSERTMSGLINE(file, line, c, "GX Internal: Register field out of range")

#define GX_FLAG_SET(file, line, regOrg, newFlag, regName)                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        GX_REG_ASSERT(file, line, !((newFlag) & ~((1 << (regName##_SIZE)) - 1)));                                                  \
        (regOrg) = (((u32)(regOrg) & ~(regName##_MASK)) | (((u32)(newFlag) << (regName##_SHIFT)) & (regName##_MASK))); \
    } while (0)

static void SETVAT(unsigned long * va /* r30 */, unsigned long * vb /* r27 */, unsigned long * vc /* r26 */, enum _GXAttr attr /* r1+0x14 */, enum _GXCompCnt cnt /* r29 */, enum _GXCompType type /* r28 */, unsigned char shft /* r25 */)
{
    switch (attr - 9) {
    case 0:
        if ((cnt & ~1) != 0) {
            OSPanic("GXAttr.c", 0x1FF, "GX Internal: Register field out of range");
        }
        *va = (*va & ~1) | cnt;
        if (type & ~7) {
            OSPanic("GXAttr.c", 0x200, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFFFFFF1) | (type << 1);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x201, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFFFFE0F) | ((shft << 4) & 0xFF0);
        return;
    case 1:
    case 16:
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x209, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFFFE3FF) | (type << 0xA);
        if (cnt == GX_NRM_NBT3) {
            *va = (*va & 0xFFFFFDFF) | 0x200;
            *va = (*va & 0x7FFFFFFF) | 0x80000000;
            return;
        }
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x20F, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFFFFDFF) | (cnt << 9);
        *va &= 0x7FFFFFFF;
        return;
    case 2:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x215, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFFFDFFF) | (cnt << 0xD);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x216, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFFE3FFF) | (type << 0xE);
        return;
    case 3:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x219, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFFDFFFF) | (cnt << 0x11);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x21A, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFE3FFFF) | (type << 0x12);
        return;
    case 4:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x21D, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFFDFFFFF) | (cnt << 0x15);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x21E, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xFE3FFFFF) | (type << 0x16);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x21F, "GX Internal: Register field out of range");
        }
        *va = (*va & 0xC1FFFFFF) | (shft << 0x19);
        return;
    case 5:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x222, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFFFFFFE) | cnt;
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x223, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFFFFFF1) | (type * 2);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x224, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFFFFE0F) | ((shft * 0x10) & 0xFF0);
        return;
    case 6:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x227, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFFFFDFF) | (cnt << 9);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x228, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFFFE3FF) | (type << 0xA);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x229, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFFC1FFF) | ((shft << 0xD) & 0x1FE000);
        return;
    case 7:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x22C, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFFBFFFF) | (cnt << 0x12);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x22D, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xFFC7FFFF) | (type << 0x13);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x22E, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xF83FFFFF) | ((shft << 0x16) & 0x3FC00000);
        return;
    case 8:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x231, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0xF7FFFFFF) | (cnt << 0x1B);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x232, "GX Internal: Register field out of range");
        }
        *vb = (*vb & 0x8FFFFFFF) | (type << 0x1C);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x233, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFFFFFFE0) | shft;
        return;
    case 9:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x236, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFFFFFFDF) | (cnt << 5);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x237, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFFFFFE3F) | (type << 6);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x238, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFFFFC1FF) | ((shft << 9) & 0x1FE00);
        return;
    case 10:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x23B, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFFFFBFFF) | (cnt << 0xE);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x23C, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFFFC7FFF) | (type << 0xF);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x23D, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFF83FFFF) | ((shft << 0x12) & 0x03FC0000);
        return;
    case 11:
        if (cnt & 0xFFFFFFFE) {
            OSPanic("GXAttr.c", 0x240, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xFF7FFFFF) | (cnt << 0x17);
        if (type & 0xFFFFFFF8) {
            OSPanic("GXAttr.c", 0x241, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0xF8FFFFFF) | (type << 0x18);
        if (shft & 0xE0) {
            OSPanic("GXAttr.c", 0x242, "GX Internal: Register field out of range");
        }
        *vc = (*vc & 0x07FFFFFF) | ((shft << 0x1B) & ~0x07FFFFFF);
        /* fallthrough */
    default:
        return;
    }
}

void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList *list)
{
    u32 *va;
    u32 *vb;
    u32 *vc;

	CHECK_GXBEGIN(0x27B, "GXSetVtxAttrFmtv");
	CHECK_LISTPTR(0x27C, list);
    CHECK_VTXFMT(0x27D, vtxfmt);
    va = &gx->vatA[vtxfmt];
    vb = &gx->vatB[vtxfmt];
    vc = &gx->vatC[vtxfmt];
    while (list->attr != GX_VA_NULL) {
		CHECK_ATTRNAME2(0x286, list->attr);
		CHECK_FRAC(0x287, list->frac);
        SETVAT(va, vb, vc, list->attr, list->cnt, list->type, list->frac);
        list++;
    }
    gx->dirtyState |= 0x10;
    gx->dirtyVAT |= (u8)(1 << (u8)vtxfmt);
}

void __GXSetVAT(void)
{
    u8 i;

    for (i = 0; i < 8; i++) {
        if (gx->dirtyVAT & (1 << i)) {
			GX_WRITE_U8(8);
			GX_WRITE_U8(i | 0x70);
			GX_WRITE_U32(gx->vatA[i]);
			{ volatile long regAddr = i - 12; }

			GX_WRITE_U8(8);
			GX_WRITE_U8(i | 0x80);
			GX_WRITE_U32(gx->vatB[i]);
			{ volatile long regAddr = i - 12; }

			GX_WRITE_U8(8);
			GX_WRITE_U8(i | 0x90);
			GX_WRITE_U32(gx->vatC[i]);
			{ volatile long regAddr = i - 12; }
        }
    }
    gx->dirtyVAT = 0;
}

void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt *cnt, GXCompType *type, u8 *frac)
{
    u32 *va;
    u32 *vb;
    u32 *vc;

	CHECK_GXBEGIN(0x2CF, "GXGetVtxAttrFmt");
	CHECK_VTXFMT(0x2D0, fmt);
    va = &gx->vatA[fmt];
    vb = &gx->vatB[fmt];
    vc = &gx->vatC[fmt];
    switch (attr - 9) {
    case 0:
        *cnt = (int)*va & 1;
        *type = (int)(*va >> 1) & 7;
        *frac = (u8)(*va >> 4) & 0x1F;
        return;
    case 1:
    case 16:
        *cnt = (int)(*va >> 9) & 1;
        if (*cnt == GX_TEX_ST && (u8)(*va >> 0x1F) != 0) {
            *cnt = GX_NRM_NBT3;
        }
        *type = (int)(*va >> 0xAU) & 7;
        *frac = 0;
        return;
    case 2:
        *cnt = (int)(*va >> 0xDU) & 1;
        *type = (int)(*va >> 0xEU) & 7;
        *frac = 0;
        return;
    case 3:
        *cnt = (int)(*va >> 0x11U) & 1;
        *type = (int)(*va >> 0x12U) & 7;
        *frac = 0;
        return;
    case 4:
        *cnt = (int)(*va >> 0x15U) & 1;
        *type = (int)(*va >> 0x16U) & 7;
        *frac = (u8)(*va >> 0x19U) & 0x1F;
        return;
    case 5:
        *cnt = *vb & 1;
        *type = (int)(*vb >> 1U) & 7;
        *frac = (u8)(*vb >> 4U) & 0x1F;
        return;
    case 6:
        *cnt = (int)(*vb >> 9U) & 1;
        *type = (int)(*vb >> 0xAU) & 7;
        *frac = (u8)(*vb >> 0xDU) & 0x1F;
        return;
    case 7:
        *cnt = (int)(*vb >> 0x12U) & 1;
        *type = (int)(*vb >> 0x13U) & 7;
        *frac = (u8)(*vb >> 0x16U) & 0x1F;
        return;
    case 8:
        *cnt = (int)(*vb >> 0x1BU) & 1;
        *type = (int)(*vb >> 0x1CU) & 7;
        *frac = *vc & 0x1F;
        return;
    case 9:
        *cnt = (int)(*vc >> 5U) & 1;
        *type = (int)(*vc >> 6U) & 7;
        *frac = (u8)(*vc >> 9U) & 0x1F;
        return;
    case 10:
        *cnt = (int)(*vc >> 0xE) & 1;
        *type = (int)(*vc >> 0xF) & 7;
        *frac = (u8)(*vc >> 0x12) & 0x1F;
        return;
    case 11:
        *cnt = (int)(*vc >> 0x17U) & 1;
        *type = (int)(*vc >> 0x18U) & 7;
        *frac = (int)(*vc >> 0x1BU);
        return;
    default:
        *cnt = GX_TEX_ST;
        *type = GX_RGB565;
        *frac = 0;
        return;
    }
}

void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList *vat)
{
	GXAttr attr;

	CHECK_GXBEGIN(0x330, "GXGetVtxAttrFmtv");
	CHECK_LISTPTR(0x331, vat);
    CHECK_VTXFMT(0x332, fmt);
    for (attr = GX_VA_POS; attr < 0x1A; attr++) {
        vat->attr = attr;
        GXGetVtxAttrFmt(fmt, attr, &vat->cnt, &vat->type, &vat->frac);
        vat++;
    }
    vat->attr = GX_VA_NULL;
}

void GXSetArray(s32 attr, void *base_ptr, u8 stride)
{
    enum _GXAttr cpAttr;
    unsigned long phyAddr;

	attr;  // needed to match

    CHECK_GXBEGIN(0x34F, "GXSetArray");
    if (attr == 0x19) {
        attr = 0xA;
    }
    CHECK_ATTRNAME3(0x352, attr);
    cpAttr = attr - 9;
    phyAddr = (u32)base_ptr & 0x3FFFFFFF;
    GX_WRITE_U8(8);
    GX_WRITE_U8(cpAttr | 0xA0);
    GX_WRITE_U32(phyAddr);
    {
		long regAddr = cpAttr - 12;
		if ((regAddr >= 0) && (regAddr < 4)) {
			gx->indexBase[regAddr] = phyAddr;
		}
	}
    GX_WRITE_U8(8);
    GX_WRITE_U8(cpAttr | 0xB0);
    GX_WRITE_U32(stride);
    {
		long regAddr = cpAttr - 12;
		if ((regAddr >= 0) && (regAddr < 4)) {
			gx->indexStride[regAddr] = stride;
		}
	}
}

void GXInvalidateVtxCache(void)
{
	CHECK_GXBEGIN(0x368, "GXInvalidateVtxCache");
	GX_WRITE_U8(0x48);
}

void GXSetTexCoordGen2(GXTexCoordID dst_coord /* r25 */, GXTexGenType func /* r21 */, GXTexGenSrc src_param /* r24 */, unsigned long mtx /* r28 */, u8 normalize /* r18 */, u32 pt_texmtx /* r19 */)
{
    unsigned long reg = 0; // r30
    unsigned long row; // r29
    unsigned long bumprow; // r26
    unsigned long form; // r27
    GXAttr mtxIdAttr; // r20
    //long regAddr; // r23
    //long regAddr; // r22

    //reg = 0;
    CHECK_GXBEGIN(0x392, "GXSetTexCoordGen");

    (dst_coord < 8) || (OSPanic(__FILE__, 0x393, "GXSetTexCoordGen: Invalid coordinate Id"), 0);
    form = 0;
    row = 5;
    switch (src_param) {                            /* switch 1 */
    case GX_TG_POS:                                 /* switch 1 */
        row = 0;
        form = 1;
        break;
    case GX_TG_NRM:                                 /* switch 1 */
        row = 1;
        form = 1;
        break;
    case GX_TG_BINRM:                               /* switch 1 */
        row = 3;
        form = 1;
        break;
    case GX_TG_TANGENT:                             /* switch 1 */
        row = 4;
        form = 1;
        break;
    case GX_TG_COLOR0:                              /* switch 1 */
        row = 2;
        break;
    case GX_TG_COLOR1:                              /* switch 1 */
        row = 2;
        break;
    case GX_TG_TEX0:                                /* switch 1 */
        row = 5;
        break;
    case GX_TG_TEX1:                                /* switch 1 */
        row = 6;
        break;
    case GX_TG_TEX2:                                /* switch 1 */
        row = 7;
        break;
    case GX_TG_TEX3:                                /* switch 1 */
        row = 8;
        break;
    case GX_TG_TEX4:                                /* switch 1 */
        row = 9;
        break;
    case GX_TG_TEX5:                                /* switch 1 */
        row = 0xA;
        break;
    case GX_TG_TEX6:                                /* switch 1 */
        row = 0xB;
        break;
    case GX_TG_TEX7:                                /* switch 1 */
        row = 0xC;
        break;
    case GX_TG_TEXCOORD0: bumprow; break;
    case GX_TG_TEXCOORD1: bumprow; break;
    case GX_TG_TEXCOORD2: bumprow; break;
    case GX_TG_TEXCOORD3: bumprow; break;
    case GX_TG_TEXCOORD4: bumprow; break;
    case GX_TG_TEXCOORD5: bumprow; break;
    case GX_TG_TEXCOORD6: bumprow; break;
    default:                                        /* switch 1 */
        OSPanic(__FILE__, 0x3AF, "GXSetTexCoordGen: Invalid source parameter");
        break;
    }
    switch (func)
    {
    case GX_TG_MTX2x4:
        SET_REG(0x3B8, reg, 1, 1, 0);
        SET_REG(0x3B9, reg, 1, 2, form);
        SET_REG(0x3BA, reg, 3, 4, 0);
        SET_REG(0x3BB, reg, 5, 7, row);
        break;
    case GX_TG_MTX3x4:
        SET_REG(0x3BF, reg, 1, 1, 1);
        SET_REG(0x3C0, reg, 1, 2, form);
        SET_REG(0x3C1, reg, 3, 4, 0);
        SET_REG(0x3C2, reg, 5, 7, row);
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
        ASSERTMSGLINE(__FILE__, 0x3CF, src_param >= 12 && src_param <= 18, "GXSetTexCoordGen:  Bump source texture value is invalid");
        SET_REG(0x3D0, reg, 1, 1, 0);
        SET_REG(0x3D1, reg, 1, 2, form);
        SET_REG(0x3D2, reg, 3, 4, 1);
        SET_REG(0x3D3, reg, 5, 7, row);
        SET_REG(0x3D4, reg, 3, 12, src_param - 12);
        SET_REG(0x3D5, reg, 3, 15, func - 2);
		break;
	case GX_TG_SRTG:
        SET_REG(0x3D9, reg, 1, 1, 0);
        SET_REG(0x3DA, reg, 1, 2, form);
		if (src_param == GX_TG_COLOR0) {
            SET_REG(0, reg, 3, 4, 2);
		} else {
            SET_REG(0, reg, 3, 4, 3);
		}
        SET_REG(0, reg, 5, 7, 2);
		break;
	default:
        ASSERTMSGLINE(__FILE__, 0x3E5, 0, "GXSetTexCoordGen:  Invalid function");
		break;
    }
    GX_WRITE_U8(16);
    GX_WRITE_U32(dst_coord + 0x1040);
    GX_WRITE_U32(reg);
    {
		long regAddr = dst_coord + 0x40;
		if ((regAddr >= 0) && (regAddr < 0x50)) {
			__gxVerif->xfRegs[regAddr] = reg;
			__gxVerif->xfRegsDirty[regAddr] = 1;
		}
	}
    reg = 0;
    SET_REG(0x3F8, reg, 6, 0, pt_texmtx - 64);
    SET_REG(0x3F9, reg, 1, 8, normalize);
    GX_WRITE_U8(16);
    GX_WRITE_U32(dst_coord + 0x1050);
    GX_WRITE_U32(reg);
    {
		long regAddr = dst_coord + 0x50;
		if ((regAddr >= 0) && (regAddr < 0x50)) {
			__gxVerif->xfRegs[regAddr] = reg;
			__gxVerif->xfRegsDirty[regAddr] = 1;
		}
	}
    switch (dst_coord) {                            /* switch 2 */
    case GX_TEXCOORD0:                              /* switch 2 */
        SET_REG(0x402, gx->matIdxA, 6, 6, mtx);
        break;
    case GX_TEXCOORD1:                              /* switch 2 */
        SET_REG(0x403, gx->matIdxA, 6, 12, mtx);
        break;
    case GX_TEXCOORD2:                              /* switch 2 */
        SET_REG(0x404, gx->matIdxA, 6, 18, mtx);
        break;
    case GX_TEXCOORD3:                              /* switch 2 */
        SET_REG(0x405, gx->matIdxA, 6, 24, mtx);
        break;
    case GX_TEXCOORD4:                              /* switch 2 */
        SET_REG(0x406, gx->matIdxB, 6, 0, mtx);
        break;
    case GX_TEXCOORD5:                              /* switch 2 */
        SET_REG(0x407, gx->matIdxB, 6, 6, mtx);
        break;
    case GX_TEXCOORD6:                              /* switch 2 */
        SET_REG(0x408, gx->matIdxB, 6, 12, mtx);
        break;
    default:                                        /* switch 2 */
        SET_REG(0x409, gx->matIdxB, 6, 18, mtx);
        break;
    }
    mtxIdAttr = dst_coord + 1;
    __GXSetMatrixIndex(mtxIdAttr);
}

void GXSetNumTexGens(u8 nTexGens)
{
	CHECK_GXBEGIN(0x41B, "GXSetNumTexGens");

	SET_REG(0x41D, gx->genMode, 4, 0, nTexGens);
    GX_WRITE_U8(16);
    GX_WRITE_U32(0x103F);
    GX_WRITE_U32(nTexGens);
    {
		long regAddr = 0x3F;
		if ((regAddr >= 0) && (regAddr < 0x50)) {
			__gxVerif->xfRegs[regAddr] = nTexGens;
			__gxVerif->xfRegsDirty[regAddr] = 1;
		}
	}
    gx->dirtyState |= 4;
}
