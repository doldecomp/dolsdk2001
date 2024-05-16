#if DEBUG

#include <stdio.h>

#include <dolphin/gx.h>

#include "__gx.h"

static u8 internalDebug;
static u32 DumpCount;
static s8 XFBuf[128];
static u32 numRegularTextures;
static u32 numBumpmapTextures;
static u32 numColor0Textures;
static u32 numColor1Textures;
static u32 numColorTextures;
static s32 XFChannel = -1;

static GXAttr TextureEnums[8] = {
    GX_VA_TEX0,
    GX_VA_TEX1,
    GX_VA_TEX2,
    GX_VA_TEX3,
    GX_VA_TEX4,
    GX_VA_TEX5,
    GX_VA_TEX6,
    GX_VA_TEX7,
};

static u8 lightRegisterNames[13][256] = {
    "Light Color RGBA",
    "Cosine Attenuation A0",
    "Cosine Attenuation A1",
    "Cosine Attenuation A2",
    "Distance Attenuation K0",
    "Distance Attenuation K1",
    "Distance Attenuation K2",
    "X Light Position / Infinite Light X Direction",
    "Y Light Position / Infinite Light Y Direction",
    "Z Light Position / Infinite Light Z Direction",
    "X Light Direction / Half Angle X Component",
    "Y Light Direction / Half Angle Y Component",
    "Z Light Direction / Half Angle Z Component",
};

#define LOWORD(var) (((u16 *)&(var))[0])
#define HIWORD(var) (((u16 *)&(var))[1])

#define BYTE0(var) (((u8 *)&(var))[0])
#define BYTE1(var) (((u8 *)&(var))[1])
#define BYTE2(var) (((u8 *)&(var))[2])
#define BYTE3(var) (((u8 *)&(var))[3])

static void CountTextureTypes(void)
{
    u32 i;
    u32 texgen_type;

    numRegularTextures = 0;
    numBumpmapTextures = 0;
    numColor0Textures = 0;
    numColor1Textures = 0;
    for (i = 0; i < __gxVerif->xfRegs[0x3F]; i++) {
        texgen_type = BYTE3(__gxVerif->xfRegs[i + 64]);
        texgen_type = (texgen_type >> 4) & 7;
        if (texgen_type == 0) {
            numRegularTextures++;
        } else if (texgen_type == 1) {
            numBumpmapTextures++;
        } else if (texgen_type == 2) {
            numColor0Textures++;
        } else if (texgen_type == 3) {
            numColor1Textures++;
        } else {
#if DOLPHIN_REVISION >= 45
            if (__gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_INVALID_TG_TYPE]) {
#else
            if (__gxVerif->verifyLevel >= 1) {
#endif
                __GX_WARNF(GXWARN_INVALID_TG_TYPE, texgen_type, i);
            }
        }
    }
    numColorTextures = numColor0Textures + numColor1Textures;
}

static void InitializeXFVerifyData(void)
{
    CountTextureTypes();
}

static void CheckDirty(u32 index, const char *name)
{
#if DOLPHIN_REVISION >= 45
    if (!__gxVerif->xfRegsDirty[index - 0x1000] && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_XF_CTRL_UNINIT]) {
#else
    if (__gxVerif->verifyLevel >= 1 && !__gxVerif->xfRegsDirty[index - 0x1000]) {
#endif
        __GX_WARNF(GXWARN_XF_CTRL_UNINIT, index, name);
    }
}

static void CheckClean(u32 index, const char *name)
{
#if DOLPHIN_REVISION >= 45
    if (__gxVerif->xfRegsDirty[index - 0x1000] && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_XF_CTRL_INIT]) {
#else
    if (__gxVerif->verifyLevel >= 1 && __gxVerif->xfRegsDirty[index - 0x1000]) {
#endif
        __GX_WARNF(GXWARN_XF_CTRL_INIT, index, name);
    }
}

static void CheckCTGColors(void)
{
    if (BYTE3(__gxVerif->xfRegs[9]) & 3) {
        if ((u32)(BYTE3(__gxVerif->xfRegs[9]) & 3) == 1) {
            if (numColorTextures != 0 && numColorTextures != 1) {
                __GX_WARNF_CHECKED(GXWARN_INV_COLOR_TG_COMB, (u8)(BYTE3(__gxVerif->xfRegs[9]) & 3), numColorTextures);
            }
        } else if ((u32)(BYTE3(__gxVerif->xfRegs[9]) & 3) == 2) {
            if (numColorTextures != 0 && numColorTextures != 2) {
                __GX_WARNF_CHECKED(GXWARN_INV_COLOR_TG_COMB, (u8)(BYTE3(__gxVerif->xfRegs[9]) & 3), numColorTextures);
            }
        } else {
            __GX_WARNF_CHECKED(GXWARN_INV_NUM_COLORS, (u8)(BYTE3(__gxVerif->xfRegs[9]) & 3));
        }
    }
}

static GXBool __GXVertexPacketHas(GXAttr attr)
{
    switch (attr) {
    case GX_VA_POS:        return GET_REG_FIELD(gx->vcdLo, 2, 9) != 0;
    case GX_VA_NRM:        return gx->hasNrms ? GET_REG_FIELD(gx->vcdLo, 2, 11) != 0 : GX_FALSE;
    case GX_VA_NBT:        return gx->hasBiNrms ? GET_REG_FIELD(gx->vcdLo, 2, 11) != 0 : GX_FALSE;
    case GX_VA_CLR0:       return GET_REG_FIELD(gx->vcdLo, 2, 13) != 0;
    case GX_VA_CLR1:       return GET_REG_FIELD(gx->vcdLo, 2, 15) != 0;
    case GX_VA_TEX0:       return GET_REG_FIELD(gx->vcdHi, 2, 0) != 0;
    case GX_VA_TEX1:       return GET_REG_FIELD(gx->vcdHi, 2, 2) != 0;
    case GX_VA_TEX2:       return GET_REG_FIELD(gx->vcdHi, 2, 4) != 0;
    case GX_VA_TEX3:       return GET_REG_FIELD(gx->vcdHi, 2, 6) != 0;
    case GX_VA_TEX4:       return GET_REG_FIELD(gx->vcdHi, 2, 8) != 0;
    case GX_VA_TEX5:       return GET_REG_FIELD(gx->vcdHi, 2, 10) != 0;
    case GX_VA_TEX6:       return GET_REG_FIELD(gx->vcdHi, 2, 12) != 0;
    case GX_VA_TEX7:       return GET_REG_FIELD(gx->vcdHi, 2, 14) != 0;
    case GX_VA_PNMTXIDX:   return GET_REG_FIELD(gx->vcdLo, 1, 0) != 0;
    case GX_VA_TEX0MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 1) != 0;
    case GX_VA_TEX1MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 2) != 0;
    case GX_VA_TEX2MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 3) != 0;
    case GX_VA_TEX3MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 4) != 0;
    case GX_VA_TEX4MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 5) != 0;
    case GX_VA_TEX5MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 6) != 0;
    case GX_VA_TEX6MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 7) != 0;
    case GX_VA_TEX7MTXIDX: return GET_REG_FIELD(gx->vcdLo, 1, 8) != 0;
    default:
        return GX_FALSE;
    }
}

static void CheckVertexPacket(void)
{
    u32 numHostTextures;
    u32 i;

    if (!__GXVertexPacketHas(GX_VA_POS)) {
        __GX_WARN_CHECKED(GXWARN_VTX_NO_GEOM);
    }
    if ((BYTE3(__gxVerif->xfRegs[8]) & 3) == 0) {
        if (__GXVertexPacketHas(GX_VA_CLR0) || __GXVertexPacketHas(GX_VA_CLR1)) {
            __GX_WARN_CHECKED(GXWARN_CLR_XF0_CP1);
        }
    } else if ((u32)(BYTE3(__gxVerif->xfRegs[8]) & 3) == 1) {
        if (!__GXVertexPacketHas(GX_VA_CLR0)) {
            __GX_WARN_CHECKED(GXWARN_CLR_XF1_CP0);
        }
        if (__GXVertexPacketHas(GX_VA_CLR1)) {
            __GX_WARN_CHECKED(GXWARN_CLR_XF1_CP2);
        }
    } else if ((u32)(BYTE3(__gxVerif->xfRegs[8]) & 3) == 2) {
        if (!__GXVertexPacketHas(GX_VA_CLR0)) {
            __GX_WARN_CHECKED(GXWARN_CLR_XF2_CPN1);
        }
        if (!__GXVertexPacketHas(GX_VA_CLR1)) {
            __GX_WARN_CHECKED(GXWARN_CLR_XF2_CPN2);
        }
    } else {
        __GX_WARNF_CHECKED(GXWARN_INV_IVS_CLR, (u8)(BYTE3(__gxVerif->xfRegs[8]) & 3));
    }
    if (((BYTE3(__gxVerif->xfRegs[8]) >> 2) & 3) == 0) {
        if (__GXVertexPacketHas(GX_VA_NRM)) {
            __GX_WARN_CHECKED(GXWARN_NRM_XF0_CP1);
        }
        if (__GXVertexPacketHas(GX_VA_NBT)) {
            __GX_WARN_CHECKED(GXWARN_NRM_XF0_CP3);
        }
    } else if ((u32)((BYTE3(__gxVerif->xfRegs[8]) >> 2) & 3) == 1) {
        if (!__GXVertexPacketHas(GX_VA_NRM)) {
            __GX_WARN_CHECKED(GXWARN_NRM_XF1_CP0);
        }
        if (__GXVertexPacketHas(GX_VA_NBT)) {
            __GX_WARN_CHECKED(GXWARN_NRM_XF1_CP3);
        }
    } else if ((u32)((BYTE3(__gxVerif->xfRegs[8]) >> 2) & 3) == 2) {
        if (__GXVertexPacketHas(GX_VA_NRM)) {
            __GX_WARN_CHECKED(GXWARN_NRM_XF3_CP1);
        }
        if (!__GXVertexPacketHas(GX_VA_NBT)) {
            __GX_WARN_CHECKED(GXWARN_NRM_XF3_CP0);
        }
    } else {
        __GX_WARNF_CHECKED(GXWARN_INV_IVS_NRM, (u8)((BYTE3(__gxVerif->xfRegs[8]) >> 2) & 3));
    }
    numHostTextures = 0;
    for (i = 0; i <= 7; i++) {
        if (__GXVertexPacketHas(TextureEnums[i])) {
            numHostTextures += 1;
        }
    }
    if (numHostTextures != (u32)((BYTE3(__gxVerif->xfRegs[8]) >> 4) & 0xF)) {
        __GX_WARNF_CHECKED(GXWARN_TEX_XFN_CPM, (u8)((BYTE3(__gxVerif->xfRegs[8]) >> 4) & 0xF), numHostTextures);
    }
}

static void CheckSourceRows(void)
{
    u32 i;

    for (i = 0; i < numRegularTextures; i++) {
        switch ((HIWORD(__gxVerif->xfRegs[i + 64]) >> 7) & 0x1F) {
        case 0:
            if (!__GXVertexPacketHas(GX_VA_POS)) {
                __GX_WARNF_CHECKED(GXWARN_TEX_SRC_NPOS, i);
            }
            break;
        case 1:
            if (!__GXVertexPacketHas(GX_VA_NRM) && !__GXVertexPacketHas(GX_VA_NBT)) {
                __GX_WARNF_CHECKED(GXWARN_TEX_SRC_NNRM, i);
            }
            break;
        case 2:
            if (!__GXVertexPacketHas(GX_VA_CLR0)) {
                __GX_WARNF_CHECKED(GXWARN_TEX_SRC_NCLR0, i);
            }
            if (!__GXVertexPacketHas(GX_VA_CLR1)) {
                __GX_WARNF_CHECKED(GXWARN_TEX_SRC_NCLR1, i);
            }
            break;
        case 3:
        case 4:
            if (!__GXVertexPacketHas(GX_VA_NBT)) {
                __GX_WARNF_CHECKED(GXWARN_TEX_SRC_NNBT, i);
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            if (!__GXVertexPacketHas(TextureEnums[((HIWORD(__gxVerif->xfRegs[i + 64]) >> 7) & 0x1F) - 5])) {
                __GX_WARNF_CHECKED(GXWARN_TEX_SRC_NTEX, i, ((HIWORD(__gxVerif->xfRegs[i + 64]) >> 7) & 0x1F) - 5);
            }
            break;
        default:
            __GX_WARNF_CHECKED(GXWARN_INV_TEX_SRC, i, (u8)((HIWORD(__gxVerif->xfRegs[i + 64]) >> 7) & 0x1F));
            break;
        }
    }
}

static void CheckTextureOrder(void)
{
    u8 done = 0;
    u32 count = 0;

    while (!done) {
        if (count == __gxVerif->xfRegs[0x3F] || ((BYTE3(__gxVerif->xfRegs[count + 64]) >> 4) & 7)) {
            done = 1;
        } else {
            count += 1;
        }
    }

    done = 0;
    while (done == 0) {
        if (count == __gxVerif->xfRegs[0x3F]) {
            done = 1;
        } else if ((u32)((BYTE3(__gxVerif->xfRegs[count + 64]) >> 4) & 7) != 1) {
            if (!((BYTE3(__gxVerif->xfRegs[count + 64]) >> 4) & 7)) {
                __GX_WARN_CHECKED(GXWARN_INV_TG_ORDER);
            }
            done = 1;
        } else {
            count += 1;
        }
    }

    done = 0;
    while (done == 0) {
        if (count == __gxVerif->xfRegs[0x3F]) {
            done = 1;
        } else if (!((BYTE3(__gxVerif->xfRegs[count + 64]) >> 4) & 7) || (u32)((BYTE3(__gxVerif->xfRegs[count + 64]) >> 4) & 7) == 1) {
            __GX_WARN_CHECKED(GXWARN_INV_TG_ORDER);
            done = 1;
        } else {
            count += 1;
        }
    }
}

static void CheckRAM(u8 Normal, u32 StartingAddress, u32 Count, s32 WarnID, char *Str)
{
    u32 i;
    u8 printedPreamble;
    u8 dirtyBit;

    printedPreamble = 0;

    for (i = StartingAddress; i < StartingAddress + Count; i++) {
        if (Normal != 0) {
            dirtyBit = __gxVerif->xfMtxDirty[i - 0x300];
        } else {
            dirtyBit = __gxVerif->xfMtxDirty[i];
        }
#if DOLPHIN_REVISION >= 45
        if (dirtyBit == 0 && printedPreamble == 0) {
            if (__gxVerif->verifyLevel >= __gxvWarnLev[WarnID]) {
                __gxVerif->cb(__gxvWarnLev[WarnID], WarnID, Str);
            }
            printedPreamble = 1;
            WarnID; printedPreamble;  // needed to match
        }
#else
        if (dirtyBit == 0) {
            if (printedPreamble == 0) {
                __gxVerif->cb(1, WarnID, Str);
                printedPreamble = 1;
            }
            __GX_WARNF(GXWARN_ADDR_UNINIT, i);
        }
#endif
    }
}

static void CheckBumpmapTextures(void)
{
    u32 i;
    u32 BumpMapSource;
    u32 BumpMapLight;
    u32 lightRAMOffset;
    char Preamble[256];

    if (!__GXVertexPacketHas(GX_VA_PNMTXIDX)) {
        if ((u32)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F) > 30) {
            __GX_WARNF_CHECKED(0x50, (u8)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F));
        }
        sprintf(Preamble, __gxvWarnings[0x6A], (u8)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F));
        CheckRAM(1, ((BYTE3(__gxVerif->xfRegs[24]) & 0x3F) * 3) + 0x400, 9U, 0x6A, Preamble);
    }

    for (i = 0; i < numBumpmapTextures; i++) {
        BumpMapSource = BYTE2(__gxVerif->xfRegs[numRegularTextures + i + 64]);
        BumpMapSource = (BumpMapSource >> 4) & 7;
        if ((BYTE3(__gxVerif->xfRegs[BumpMapSource + 64]) >> 4) & 7) {
#if DOLPHIN_REVISION >= 45
            __GX_WARNF_CHECKED(0x51, i + numRegularTextures, BumpMapSource);
#else
            __GX_WARNF_CHECKED(0x51, i, i + numRegularTextures, BumpMapSource);
#endif
        }
        BumpMapLight = __gxVerif->xfRegs[numRegularTextures + i + 0x40];
        BumpMapLight = (BumpMapLight >> 15) & 7;
        lightRAMOffset = (BumpMapLight * 0x10) + 0x60A;
        if (!__gxVerif->xfLightDirty[lightRAMOffset - 0x600 + 0]) {
#if DOLPHIN_REVISION >= 45
            __GX_WARNF_CHECKED(0x52, i + numRegularTextures, BumpMapLight, "X");
#else
            __GX_WARNF_CHECKED(0x52, i, i + numRegularTextures, BumpMapLight, "X");
#endif
        }
        if (!__gxVerif->xfLightDirty[lightRAMOffset - 0x600 + 1]) {
#if DOLPHIN_REVISION >= 45
            __GX_WARNF_CHECKED(0x52, i + numRegularTextures, BumpMapLight, "Y");
#else
            __GX_WARNF_CHECKED(0x52, i, i + numRegularTextures, BumpMapLight, "Y");
#endif
        }
        if (!__gxVerif->xfLightDirty[lightRAMOffset - 0x600 + 2]) {
#if DOLPHIN_REVISION >= 45
            __GX_WARNF_CHECKED(0x52, i + numRegularTextures, BumpMapLight, "Z");
#else
            __GX_WARNF_CHECKED(0x52, i, i + numRegularTextures, BumpMapLight, "Z");
#endif
        }
        if (!__GXVertexPacketHas(GX_VA_NBT)) {
            __GX_WARNF_CHECKED(0x53, i);
        }
    }

    lightRAMOffset;lightRAMOffset;  // needed to match
}

static void CheckTextureTransformMatrices(void)
{
    u32 i;
    u32 StartingAddress;
    u32 Size;
    u8 MtxIndexInVertexPacket;
    char Preamble[256];
    u32 Val;

    for (i = 0; i < numRegularTextures; i++) {
        MtxIndexInVertexPacket = 0;
        switch (i) {
        case 0:
            StartingAddress = (u8)((HIWORD(__gxVerif->xfRegs[0x18]) >> 4U) & 0xFC);
            Val = HIWORD(__gxVerif->xfRegs[0x18]);
            Val = (Val >> 6) & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX0MTXIDX);
            break;
        case 1:
            StartingAddress = (u8)((__gxVerif->xfRegs[0x18] >> 10) & 0xFC);
            Val = __gxVerif->xfRegs[0x18];
            Val = (Val >> 12) & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX1MTXIDX);
            break;
        case 2:
            StartingAddress = (u8)(BYTE1(__gxVerif->xfRegs[0x18]) & 0xFC);
            Val = BYTE1(__gxVerif->xfRegs[0x18]);
            Val = (Val >> 2) & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX2MTXIDX);
            break;
        case 3:
            StartingAddress = (BYTE0(__gxVerif->xfRegs[0x18]) * 4) & 0xFC;
            Val = BYTE0(__gxVerif->xfRegs[0x18]);
            Val = Val & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX3MTXIDX);
            break;
        case 4:
            StartingAddress = (BYTE3(__gxVerif->xfRegs[0x19]) * 4) & 0xFC;
            Val = BYTE3(__gxVerif->xfRegs[0x19]);
            Val = Val & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX4MTXIDX);
            break;
        case 5:
            StartingAddress = (u8)((HIWORD(__gxVerif->xfRegs[0x19]) >> 4) & 0xFC);
            Val = HIWORD(__gxVerif->xfRegs[0x19]);
            Val = (Val >> 6) & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX5MTXIDX);
            break;
        case 6:
            StartingAddress = (u8)((__gxVerif->xfRegs[0x19] >> 10) & 0xFC);
            Val = __gxVerif->xfRegs[0x19];
            Val = (Val >> 12) & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX6MTXIDX);
            break;
        case 7:
            StartingAddress = (u8)(BYTE1(__gxVerif->xfRegs[0x19]) & 0xFC);
            Val = BYTE1(__gxVerif->xfRegs[0x19]);
            Val = (Val >> 2) & 0x3F;
            MtxIndexInVertexPacket = __GXVertexPacketHas(GX_VA_TEX7MTXIDX);
            break;
        default:
            __GX_WARNF_CHECKED(0x54, i);
            break;
        }
#if DOLPHIN_REVISION >= 45
        if (MtxIndexInVertexPacket == 0) {
#else
        if (MtxIndexInVertexPacket != 0) {
#endif
            sprintf(Preamble, __gxvWarnings[0x6B], i, i, Val);
            if (!((BYTE3(__gxVerif->xfRegs[i + 64]) >> 1) & 1)) {
                Size = 8;
            } else {
                Size = 0xC;
            }
            CheckRAM(0U, StartingAddress, Size, 0x6B, Preamble);
        }
    }

    // needed to match
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    StartingAddress;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
    MtxIndexInVertexPacket;
}

static void CheckInputForms(void)
{
    u32 i;

    for (i = 0; i < numRegularTextures; i++) {
        switch ((HIWORD(__gxVerif->xfRegs[i + 64]) >> 7) & 0x1F) {
#if DOLPHIN_REVISION < 45
        case 2:
#endif
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            if ((BYTE3(__gxVerif->xfRegs[i + 64]) >> 2) & 1) {
                __GX_WARNF_CHECKED(0x55, i, (u8)((HIWORD(__gxVerif->xfRegs[i + 64]) >> 7) & 0x1F));
            }
        }
    }
}

static void CheckLight(u32 lightSource)
{
    u32 lightRAMOffset;
    u8 printedPreamble;
    u32 i;

    printedPreamble = 0;
    lightRAMOffset = (lightSource * 0x10) + 0x603;
    for (i = 0; i < 13; i++) {
        if (!__gxVerif->xfLightDirty[lightRAMOffset + i - 0x600]) {
            if (!printedPreamble) {
                __GX_WARNF_CHECKED(0x6C, lightSource);
                printedPreamble = 1;
            }
#if DOLPHIN_REVISION < 45
            __GX_WARNF(0x70, lightRAMOffset + i, lightRegisterNames[i]);
#endif
        }
    }
}

static void CheckColor0(void)
{
    char Preamble[256];
    u8 haveLight;
    u32 i;
    u8 lightUsed;

    if ((u8)(BYTE3(__gxVerif->xfRegs[9]) & 3) || numColorTextures != 0) {
        if (!__gxVerif->xfRegsDirty[14]) {
            __GX_WARNF_CHECKED(0x56, 0x100E, "Color 0 control register");
        }
        if (!__gxVerif->xfRegsDirty[16]) {
            __GX_WARNF_CHECKED(0x56, 0x1010, "Alpha 0 control register");
        }
        if (!(BYTE3(__gxVerif->xfRegs[14]) & 1) && !__gxVerif->xfRegsDirty[12]) {
            __GX_WARNF_CHECKED(0x57, 0, 0, 0x100C);
        }
        if (!((BYTE3(__gxVerif->xfRegs[14]) >> 6) & 1) && !__gxVerif->xfRegsDirty[10]) {
            __GX_WARNF_CHECKED(0x58, 0, 0, 0x100A);
        }
        if ((u32)((BYTE3(__gxVerif->xfRegs[14]) >> 1) & 1) == 1 || (u32)((BYTE3(__gxVerif->xfRegs[16]) >> 1) & 1) == 1) {
            haveLight = 0;
            for (i = 0; i < 8; i++) {
                lightUsed = 0;
                switch (i) {
                case 0:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[14]) >> 2) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[16]) >> 2) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 1:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[14]) >> 3) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[16]) >> 3) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 2:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[14]) >> 4) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[16]) >> 4) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 3:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[14]) >> 5) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[16]) >> 5) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 4:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[14]) >> 3) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[16]) >> 3) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 5:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[14]) >> 4) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[16]) >> 4) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 6:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[14]) >> 5) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[16]) >> 5) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 7:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[14]) >> 6) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[16]) >> 6) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                }
                if (lightUsed != 0) {
                    CheckLight(i);
                    haveLight = 1;
                }
            }
            if (haveLight != 0) {
                if (!((BYTE2(__gxVerif->xfRegs[14]) >> 2) & 1) && ((HIWORD(__gxVerif->xfRegs[14]) >> 7) & 3)) {
                    __GX_WARNF_CHECKED(0x59, "COLOR0", "COLOR0");
                }
                if (!((BYTE2(__gxVerif->xfRegs[16]) >> 2) & 1) && ((HIWORD(__gxVerif->xfRegs[16]) >> 7) & 3)) {
                    __GX_WARNF_CHECKED(0x59, "ALPHA0", "ALPHA0");
                }
                if (((HIWORD(__gxVerif->xfRegs[14]) >> 7) & 3)
                 || ((u8)((BYTE2(__gxVerif->xfRegs[14]) >> 1) & 1) && ((u32)((BYTE2(__gxVerif->xfRegs[14]) >> 2) & 1) == 1))
                 || ((HIWORD(__gxVerif->xfRegs[16]) >> 7) & 3)
                 || ((u8)((BYTE2(__gxVerif->xfRegs[16]) >> 1) & 1) && ((u32)((BYTE2(__gxVerif->xfRegs[16]) >> 2) & 1) == 1))) {
                    if ((__GXVertexPacketHas(GX_VA_NRM) == 0) && (__GXVertexPacketHas(GX_VA_NBT) == 0)) {
                        __GX_WARNF_CHECKED(0x5A, 0);
                    }
                    if (__GXVertexPacketHas(GX_VA_PNMTXIDX) == 0) {
                        if ((u32)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F) > 30) {
                            __GX_WARNF_CHECKED(0x5B, 0, (u8)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F));
                        }
                        sprintf(Preamble, __gxvWarnings[0x6D], 0, (u8)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F));
                        CheckRAM(1, ((BYTE3(__gxVerif->xfRegs[24]) & 0x3F) * 3) + 0x400, 9, 0x6D, Preamble);
                    }
                }
            }
        }
    }
}

static void CheckColor1(void)
{
    u8 usingColor1;
    char Preamble[256];
    u8 haveLight;
    u32 i;
    u8 lightUsed;

    if (numColorTextures > 1 && ((u32)((BYTE3(__gxVerif->xfRegs[numRegularTextures + numBumpmapTextures + 1 + 64]) >> 4) & 7) == 3)) {
        usingColor1 = 1;
    } else {
        usingColor1 = 0;
    }

    if ((u32)(BYTE3(__gxVerif->xfRegs[9]) & 3) == 2 || usingColor1) {
        if (!__gxVerif->xfRegsDirty[15]) {
            __GX_WARNF_CHECKED(0x56, 0x100F, "Color 1 control register");
        }
        if (!__gxVerif->xfRegsDirty[17]) {
            __GX_WARNF_CHECKED(0x56, 0x1011, "Alpha 1 control register");
        }
        if (!(BYTE3(__gxVerif->xfRegs[15]) & 1) && !__gxVerif->xfRegsDirty[13]) {
            __GX_WARNF_CHECKED(0x57, 1, 1, 0x100D);
        }
        if (!((BYTE3(__gxVerif->xfRegs[15]) >> 6) & 1) && !__gxVerif->xfRegsDirty[11]) {
            __GX_WARNF_CHECKED(0x58, 1, 1, 0x100B);
        }
        if ((u32)((BYTE3(__gxVerif->xfRegs[15]) >> 1) & 1) == 1 || (u32)((BYTE3(__gxVerif->xfRegs[17]) >> 1) & 1) == 1) {
            haveLight = 0;
            for (i = 0; i < 8; i++) {
                lightUsed = 0;
                switch (i) {
                case 0:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[15]) >> 2) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[17]) >> 2) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 1:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[15]) >> 3) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[17]) >> 3) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 2:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[15]) >> 4) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[17]) >> 4) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 3:
                    if ((u8)((BYTE3(__gxVerif->xfRegs[15]) >> 5) & 1) || (u8)((BYTE3(__gxVerif->xfRegs[17]) >> 5) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 4:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[15]) >> 3) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[17]) >> 3) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 5:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[15]) >> 4) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[17]) >> 4) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 6:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[15]) >> 5) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[17]) >> 5) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                case 7:
                    if ((u8)((BYTE2(__gxVerif->xfRegs[15]) >> 6) & 1) || (u8)((BYTE2(__gxVerif->xfRegs[17]) >> 6) & 1)) {
                        lightUsed = 1;
                    }
                    break;
                }
                if (lightUsed != 0) {
                    CheckLight(i);
                    haveLight = 1;
                }
            }
            if (haveLight != 0) {
                if (!((BYTE2(__gxVerif->xfRegs[15]) >> 2) & 1) && ((HIWORD(__gxVerif->xfRegs[15]) >> 7) & 3)) {
                    __GX_WARNF_CHECKED(0x59, "COLOR1", "COLOR1");
                }
                if (!((BYTE2(__gxVerif->xfRegs[17]) >> 2) & 1) && ((HIWORD(__gxVerif->xfRegs[17]) >> 7) & 3)) {
                    __GX_WARNF_CHECKED(0x59, "ALPHA1", "ALPHA1");
                }
                if (((HIWORD(__gxVerif->xfRegs[15]) >> 7) & 3)
                 || ((u8)((BYTE2(__gxVerif->xfRegs[15]) >> 1) & 1) && ((u32)((BYTE2(__gxVerif->xfRegs[15]) >> 2) & 1) == 1))
                 || ((HIWORD(__gxVerif->xfRegs[17]) >> 7) & 3)
                 || ((u8)((BYTE2(__gxVerif->xfRegs[17]) >> 1) & 1) && ((u32)((BYTE2(__gxVerif->xfRegs[17]) >> 2) & 1) == 1))) {
                    if ((__GXVertexPacketHas(GX_VA_NRM) == 0) && (__GXVertexPacketHas(GX_VA_NBT) == 0)) {
                        __GX_WARNF_CHECKED(0x5A, 1);
                    }
                    if (__GXVertexPacketHas(GX_VA_PNMTXIDX) == 0) {
                        if ((u32)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F) > 30) {
                            __GX_WARNF_CHECKED(0x5B, 1, (u8)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F));
                        }
                        sprintf(Preamble, __gxvWarnings[0x6D], 1, (u8)(BYTE3(__gxVerif->xfRegs[24]) & 0x3F));
                        CheckRAM(1, ((BYTE3(__gxVerif->xfRegs[24]) & 0x3F) * 3) + 0x400, 9, 0x6D, Preamble);
                    }
                }
            }
        }
    }
}

static void ComputeSignExponentMantissa(f32 floatVal, u32 *sign, u32 *exponent, u32 *mantissa)
{
    u32 intVal = *(u32 *)&floatVal;

    *sign = (intVal >> 31) & 1;
    *exponent = (intVal >> 23) & 0xFF;
    *mantissa = intVal & 0x7FFFFF;
}

static void CheckFloatingPointValue(u8 dirtyBit, u32 value, char *label)
{
    u32 sign;
    u32 exponent;
    u32 mantissa;
    f32 valuef;

    &valuef;

    if ((dirtyBit == 0)) {
        return;
    }
    valuef = *(f32 *)&value;
    ComputeSignExponentMantissa(valuef, &sign, &exponent, &mantissa);

    if (exponent == 0 && mantissa == 0) {
        return;
    }

    if (exponent == 0xFF) {
        if (__gxVerif->verifyLevel >= 2) {
            if (mantissa == 0) {
                if (sign != 0) {
                    __GX_WARN2F_CHECKED(GX_WARN_MEDIUM, 0x5C, label, "-", *(u32 *)&valuef);
                } else {
                    __GX_WARN2F_CHECKED(GX_WARN_MEDIUM, 0x5C, label, "+", *(u32 *)&valuef);
                }
            } else {
                __GX_WARN2F_CHECKED(GX_WARN_MEDIUM, 0x5D, label, *(u32 *)&valuef);
            }
        }
    } else if (__gxVerif->verifyLevel >= 3) {
        if (exponent < 0x6BU) {
            __GX_WARN2F_CHECKED(GX_WARN_ALL, 0x5E, label, valuef, *(u32 *)&valuef);
        } else if (exponent > 0x96U) {
            __GX_WARN2F_CHECKED(GX_WARN_ALL, 0x5F, label, valuef, *(u32 *)&valuef);
        }
    }
}

static void CheckMatrixRAMRanges(void)
{
    u32 i;
    char label[256];

    for (i = 0; i <= 255; i++) {
        sprintf(label, "Geometry/Texture Matrix ram address 0x%04x", i);
        CheckFloatingPointValue(__gxVerif->xfMtxDirty[i], __gxVerif->xfMtx[i], label);
    }
}

static void CheckNormalRAMRanges(void)
{
    u32 i;
    char label[256];

    for (i = 1024; i <= 1119; i++) {
        sprintf(label, "Normal Matrix ram address 0x%04x", i);
        CheckFloatingPointValue(__gxVerif->xfNrmDirty[i - 1024], __gxVerif->xfNrm[i - 1024], label);
    }
}

static void CheckDMatrixRAMRanges(void)
{
    u32 i;
    char label[256];

    for (i = 1280; i <= 1535; i++) {
        sprintf(label, "Dual Texture Matrix ram address 0x%04x", i);
        CheckFloatingPointValue(__gxVerif->xfDMtxDirty[i - 1280], __gxVerif->xfDMtx[i - 1280], label);
    }
}

static void CheckLightRAMRanges(void)
{
    u32 lightSource;
    u32 lightRAMOffset;
    char label[256];
    u32 i;

    for (lightSource = 0; lightSource < 8; lightSource++) {
        for (i = 1; i < 13; i++) {
            lightRAMOffset = (lightSource << 4) + i;
            lightRAMOffset += 0x603;
            sprintf(label, "Light %d %s (address 0x%04x)", lightSource, lightRegisterNames[i], lightRAMOffset);
            CheckFloatingPointValue(__gxVerif->xfLightDirty[lightRAMOffset - 0x600], __gxVerif->xfLight[(s32) (lightRAMOffset - 0x600)], label);
        }

    }

    i; lightSource;  // needed to match
}

static void CheckControlRAMRanges(void)
{
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x1A], __gxVerif->xfRegs[0x1A], "Viewport Scale X");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x1B], __gxVerif->xfRegs[0x1B], "Viewport Scale Y");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x1C], __gxVerif->xfRegs[0x1C], "Viewport Scale Z");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x1D], __gxVerif->xfRegs[0x1D], "Viewport Offset X");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x1E], __gxVerif->xfRegs[0x1E], "Viewport Offset Y");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x1F], __gxVerif->xfRegs[0x1F], "Viewport Offset Z");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x20], __gxVerif->xfRegs[0x20], "Projection Matrix A Value");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x21], __gxVerif->xfRegs[0x21], "Projection Matrix B Value");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x22], __gxVerif->xfRegs[0x22], "Projection Matrix C Value");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x23], __gxVerif->xfRegs[0x23], "Projection Matrix D Value");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x24], __gxVerif->xfRegs[0x24], "Projection Matrix E Value");
    CheckFloatingPointValue(__gxVerif->xfRegsDirty[0x25], __gxVerif->xfRegs[0x25], "Projection Matrix F Value");
}

static void CheckFloatingPointRanges(void)
{
    CheckMatrixRAMRanges();
    CheckNormalRAMRanges();
    CheckDMatrixRAMRanges();
    CheckLightRAMRanges();
    CheckControlRAMRanges();
}

static void CheckMatrixIndices(void)
{
    if (!__GXVertexPacketHas(GX_VA_PNMTXIDX)
     || !__GXVertexPacketHas(GX_VA_TEX0MTXIDX)
     || !__GXVertexPacketHas(GX_VA_TEX1MTXIDX)
     || !__GXVertexPacketHas(GX_VA_TEX2MTXIDX)
     || !__GXVertexPacketHas(GX_VA_TEX3MTXIDX)) {
        CheckDirty(0x1018U, "Geometry & Textures [0-3] transform matrix indices");
    }
    if (__gxVerif->verifyLevel >= 1 && !__GXVertexPacketHas(GX_VA_PNMTXIDX)) {
        CheckRAM(0U, (BYTE3(__gxVerif->xfRegs[24]) * 4) & 0xFC, 0xCU, 0x6E, __gxvWarnings[0x6E]);
    }
    if ((!__GXVertexPacketHas(GX_VA_TEX4MTXIDX)
      || !__GXVertexPacketHas(GX_VA_TEX5MTXIDX)
      || !__GXVertexPacketHas(GX_VA_TEX6MTXIDX)
      || !__GXVertexPacketHas(GX_VA_TEX7MTXIDX))
     && numRegularTextures > 4
     && __gxVerif->verifyLevel >= 1
     && !__gxVerif->xfRegsDirty[0x19]) {
        __GX_WARNF_CHECKED(0x60, numRegularTextures, 0x1019U);
    }
}

static void CheckErrors(void)
{
    u32 i;
    char registerName[80];

    CheckDirty(0x103FU, "Number of XF output textures");
    CheckDirty(0x1009U, "Number of XF output colors");
    CheckDirty(0x1008U, "InVertexSpec");
    CheckDirty(0x101AU, "Viewport ScaleX");
    CheckDirty(0x101BU, "Viewport ScaleY");
    CheckDirty(0x101CU, "Viewport ScaleZ");
    CheckDirty(0x101DU, "Viewport OffsetX");
    CheckDirty(0x101EU, "Viewport OffsetY");
    CheckDirty(0x101FU, "Viewport OffsetZ");
    CheckDirty(0x1020U, "Projection matrix 'A' value");
    CheckDirty(0x1021U, "Projection matrix 'B' value");
    CheckDirty(0x1022U, "Projection matrix 'C' value");
    CheckDirty(0x1023U, "Projection matrix 'D' value");
    CheckDirty(0x1024U, "Projection matrix 'E' value");
    CheckDirty(0x1025U, "Projection matrix 'F' value");
    CheckDirty(0x1026U, "Projection matrix orthographic/perspective select");
    CheckMatrixIndices();
    if (__gxVerif->verifyLevel >= 1) {
#if DOLPHIN_REVISION < 45
        if ((u32)((__gxVerif->rasRegs[0] & 0xFF000000) + 0x01000000) == 0U) {
            __GX_WARN(0x61);
        }
        if ((u32)__gxVerif->xfRegs[0x3F] != (u32)(__gxVerif->rasRegs[0] & 0xF)) {
            __GX_WARN(0x62);
        }
        if ((u32)(BYTE3(__gxVerif->xfRegs[9]) & 3) != (u8)((__gxVerif->rasRegs[0] >> 4U) & 7)) {
            __GX_WARN(0x63);
        }
#endif
        CheckCTGColors();
        if (__gxVerif->xfRegs[0x3F] > 8) {
            __GX_WARNF_CHECKED(0x64, __gxVerif->xfRegs[0x3F], 8);
        }
        if (numRegularTextures > 8) {
            __GX_WARNF_CHECKED(0x65, numRegularTextures, 8);
        }
        if (numBumpmapTextures > 3) {
            __GX_WARNF_CHECKED(0x66, numBumpmapTextures, 3);
        }
        if (numColorTextures > 2) {
            __GX_WARNF_CHECKED(0x67, numColorTextures, 2);
        }
        if (numColor0Textures > 1) {
            __GX_WARNF_CHECKED(0x69, 0);
        }
        if (numColor1Textures > 1) {
            __GX_WARNF_CHECKED(0x69, 1);
        }
        CheckVertexPacket();

        for (i = 0; i < __gxVerif->xfRegs[0x3F]; i++) {
            sprintf(registerName, "Texture %d settings", i);
            CheckDirty(i + 0x1040, registerName);
        }
        CheckSourceRows();
        CheckTextureOrder();
        if (numBumpmapTextures != 0) {
            CheckBumpmapTextures();
        }
        CheckTextureTransformMatrices();
        if (numColorTextures != 0 && (u32)((BYTE3(__gxVerif->xfRegs[numRegularTextures + numBumpmapTextures + 64]) >> 4) & 7) != 2) {//((u32) (((u8) *(__gxVerif + (((numRegularTextures + (numBumpmapTextures + 0x40)) * 4) + 0xB)) >> 4U) & 7) != 2)) {
            __GX_WARN_CHECKED(0x68U);
        }
        CheckColor0();
        CheckColor1();
    }
}

static void CheckWarnings(void)
{
    if (__gxVerif->verifyLevel >= 1) {
        CheckInputForms();
    }
    CheckClean(0x1000U, "Internal error register");
    CheckClean(0x1001U, "Internal diagnostic register");
    CheckClean(0x1002U, "Internal state register 0");
    CheckClean(0x1003U, "Internal state register 1");
    CheckClean(0x1004U, "Power savings register");
    if (__gxVerif->verifyLevel >= 2) {
        CheckFloatingPointRanges();
    }
}

static void DumpXFRegisters(void)
{
    static u8 firstTime = 1;
}

void __GXVerifyXF(void)
{
    if (internalDebug) {
        DumpXFRegisters();
    }
    InitializeXFVerifyData();
    CheckErrors();
    CheckWarnings();
    DumpCount++;
}

#endif
