#include <dolphin/gx.h>
#include <dolphin/os.h>

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
    if (__GXinBegin) {
        OSPanic("GXAttr.c", 0xCC, "'GXSetVtxDesc' is not allowed between GXBegin/GXEnd");
    }
    if ((attr < 0) || (attr >= 0x1A)) {
        OSPanic("GXAttr.c", 0xCF, "GXSetVtxDesc: Invalid vertex attribute name");
    }
    if ((type < 0) || (type > 3)) {
        OSPanic("GXAttr.c", 0xD1, "GXSetVtxDesc: Invalid vertex attribute type");
    }
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
    if (__GXinBegin) {
        OSPanic("GXAttr.c", 0xF5, "'GXSetVtxDescv' is not allowed between GXBegin/GXEnd");
    }
    if (attrPtr == NULL) {
        OSPanic("GXAttr.c", 0xF6, "GXSetVtxDescv: attrPtr is NULL");
    }
    while (attrPtr->attr != 0xFF) {
        if (attrPtr->attr < 0 || attrPtr->attr >= 0x1A) {
            OSPanic("GXAttr.c", 0xFB, "GXSetVtxDesc: Invalid vertex attribute name");
        }
        if (attrPtr->type < 0 || attrPtr->type > 3) {
            OSPanic("GXAttr.c", 0xFE, "GXSetVtxDesc: Invalid vertex attribute type");
        }
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

    if (__GXinBegin) {
        OSPanic("GXAttr.c", 0x185, "'GXGetVtxDesc' is not allowed between GXBegin/GXEnd");
    }
    if ((attr < 0) || (attr >= 0x1A)) {
        OSPanic("GXAttr.c", 0x187, "GXSetVtxDesc: Invalid vertex attribute name");
    }
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
