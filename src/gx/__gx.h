#define GX_WRITE_U8(ub)	    \
    GXWGFifo.u8 = (u8)(ub)

#define GX_WRITE_U16(us)   \
   GXWGFifo.u16 = (u16)(us)

#define GX_WRITE_U32(ui)   \
   GXWGFifo.u32 = (u32)(ui)

#define GX_WRITE_F32(f)	 	\
   GXWGFifo.f32 = (f32)(f);

#if DEBUG
#define GX_WRITE_SOME_REG1(a, b, c, addr) \
do { \
	long regAddr; \
	GX_WRITE_U8(a); \
    GX_WRITE_U32(b); \
    GX_WRITE_U32(c); \
    regAddr = addr; \
    if (regAddr >= 0 && regAddr < 0x50) { \
		__gxVerif->xfRegs[regAddr] = c; \
		__gxVerif->xfRegsDirty[regAddr] = TRUE; \
    } \
} while (0)
#else
#define GX_WRITE_SOME_REG1(a, b, c, addr) \
do { \
	long regAddr; \
	GX_WRITE_U8(a); \
    GX_WRITE_U32(b); \
    GX_WRITE_U32(c); \
    regAddr = addr; \
} while (0)
#endif

#define GX_WRITE_SOME_REG2(a, b, c, addr) \
do { \
	long regAddr; \
	GX_WRITE_U8(a); \
    GX_WRITE_U8(b); \
    GX_WRITE_U32(c); \
	regAddr = addr; \
	if (regAddr >= 0 && regAddr < 4) { \
		gx->indexBase[regAddr] = c; \
	} \
} while (0)
#define GX_WRITE_SOME_REG3(a, b, c, addr) \
do { \
	long regAddr; \
	GX_WRITE_U8(a); \
    GX_WRITE_U8(b); \
    GX_WRITE_U32(c); \
	regAddr = addr; \
	if (regAddr >= 0 && regAddr < 4) { \
		gx->indexStride[regAddr] = c; \
	} \
} while (0)
#define GX_WRITE_SOME_REG4(a, b, c, addr) \
do { \
	long regAddr; \
	GX_WRITE_U8(a); \
    GX_WRITE_U8(b); \
    GX_WRITE_U32(c); \
	regAddr = addr; \
} while (0)

#define GET_REG_FIELD(reg, size, shift) ((int)((reg) >> (shift)) & ((1 << (size)) - 1))

#define SET_REG_FIELD(line, reg, size, shift, val) \
do { \
    ASSERTMSGLINE(__FILE__, line, ((val) & ~((1 << (size)) - 1)) == 0, "GX Internal: Register field out of range"); \
    (reg) = ((u32)(reg) & ~(((1 << (size)) - 1) << (shift))) | ((u32)(val) << (shift)); \
} while (0)

#define CHECK_GXBEGIN(line, name) ASSERTMSGLINE(__FILE__, line, !__GXinBegin, "'" name "' is not allowed between GXBegin/GXEnd")

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

extern volatile u32 *__piReg;
