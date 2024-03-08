#if DEBUG

#include <stdio.h>

#include <dolphin/gx.h>

#include "__gx.h"

static u8 internalDebug; // size: 0x1, address: 0x0
static u32 DumpCount; // size: 0x4, address: 0x4
static s8 XFBuf[128]; // size: 0x80, address: 0x0
static u32 numRegularTextures; // size: 0x4, address: 0x8
static u32 numBumpmapTextures; // size: 0x4, address: 0xC
static u32 numColor0Textures; // size: 0x4, address: 0x10
static u32 numColor1Textures; // size: 0x4, address: 0x14
static u32 numColorTextures; // size: 0x4, address: 0x18
static s32 XFChannel; // size: 0x4, address: 0x0
static GXAttr TextureEnums[8]; // size: 0x20, address: 0x0
static u8 lightRegisterNames[13][256]; // size: 0xD00, address: 0x20

static void CountTextureTypes(void)
{
    u32 i;
    u32 texgen_type;

    numRegularTextures = 0;
    numBumpmapTextures = 0;
    numColor0Textures = 0;
    numColor1Textures = 0;
    for (i = 0; i < __gxVerif->xfRegs[0x3F]; i++) {
        // hmm...
        texgen_type = ((u8 *)__gxVerif + 0xB)[(i + 64) << 2];
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
            if (__gxVerif->verifyLevel >= 1) {
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
    if (__gxVerif->verifyLevel >= 1 && !__gxVerif->xfRegsDirty[index - 0x1000]) {
        __GX_WARNF(GXWARN_XF_CTRL_UNINIT, index, name);
    }
}

static void CheckClean(u32 index, const char *name)
{
    if (__gxVerif->verifyLevel >= 1 && __gxVerif->xfRegsDirty[index - 0x1000]) {
        __GX_WARNF(GXWARN_XF_CTRL_INIT, index, name);
    }
}

// No idea why it's accessing this as a u8, when it should be u32
#define WTF *((u8 *)&__gxVerif->xfRegs[9] + 3)
#define WTF2 *((u8 *)&__gxVerif->xfRegs[8] + 3)

static void CheckCTGColors(void)
{
    if (WTF & 3) {
        if ((u32)(WTF & 3) == 1) {
            if (numColorTextures != 0 && numColorTextures != 1) {
                __GX_WARNF(GXWARN_INV_COLOR_TG_COMB, (u8)(WTF & 3), numColorTextures);
            }
        } else if ((u32)(WTF & 3) == 2) {
            if (numColorTextures != 0 && numColorTextures != 2) {
                __GX_WARNF(GXWARN_INV_COLOR_TG_COMB, (u8)(WTF & 3), numColorTextures);
            }
        } else {
            __GX_WARNF(GXWARN_INV_NUM_COLORS, (u8)(WTF & 3));
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
        __GX_WARN(GXWARN_VTX_NO_GEOM);
    }
    if ((WTF2 & 3) == 0) {
        if (__GXVertexPacketHas(GX_VA_CLR0) || __GXVertexPacketHas(GX_VA_CLR1)) {
            __GX_WARN(GXWARN_CLR_XF0_CP1);
        }
    } else if ((u32)(WTF2 & 3) == 1) {
        if (!__GXVertexPacketHas(GX_VA_CLR0)) {
            __GX_WARN(GXWARN_CLR_XF1_CP0);
        }
        if (__GXVertexPacketHas(GX_VA_CLR1)) {
            __GX_WARN(GXWARN_CLR_XF1_CP2);
        }
    } else if ((u32)(WTF2 & 3) == 2) {
        if (!__GXVertexPacketHas(GX_VA_CLR0)) {
            __GX_WARN(GXWARN_CLR_XF2_CPN1);
        }
        if (!__GXVertexPacketHas(GX_VA_CLR1)) {
            __GX_WARN(GXWARN_CLR_XF2_CPN2);
        }
    } else {
        __GX_WARNF(GXWARN_INV_IVS_CLR, (u8)(WTF2 & 3));
    }
    if (((WTF2 >> 2) & 3) == 0) {
        if (__GXVertexPacketHas(GX_VA_NRM)) {
            __GX_WARN(GXWARN_NRM_XF0_CP1);
        }
        if (__GXVertexPacketHas(GX_VA_NBT)) {
            __GX_WARN(GXWARN_NRM_XF0_CP3);
        }
    } else if ((u32)((WTF2 >> 2) & 3) == 1) {
        if (!__GXVertexPacketHas(GX_VA_NRM)) {
            __GX_WARN(GXWARN_NRM_XF1_CP0);
        }
        if (__GXVertexPacketHas(GX_VA_NBT)) {
            __GX_WARN(GXWARN_NRM_XF1_CP3);
        }
    } else if ((u32)((WTF2 >> 2) & 3) == 2) {
        if (__GXVertexPacketHas(GX_VA_NRM)) {
            __GX_WARN(GXWARN_NRM_XF3_CP1);
        }
        if (!__GXVertexPacketHas(GX_VA_NBT)) {
            __GX_WARN(GXWARN_NRM_XF3_CP0);
        }
    } else {
        __GX_WARNF(GXWARN_INV_IVS_NRM, (u8)((WTF2 >> 2) & 3));
    }
    numHostTextures = 0;
    for (i = 0; i <= 7; i++) {
        if (__GXVertexPacketHas(TextureEnums[i])) {
            numHostTextures += 1;
        }
    }
    if (numHostTextures != (u32)((WTF2 >> 4) & 0xF)) {
        __GX_WARNF(GXWARN_TEX_XFN_CPM, (u8)((WTF2 >> 4) & 0xF), numHostTextures);
    }
}

// Yeah... why?
#define THISISWEIRD(i) (u32)(*(u16 *)&(((u8 *)__gxVerif + 0xA)[(i + 64) << 2]))

static void CheckSourceRows(void)
{
    u32 i;

    for (i = 0; i < numRegularTextures; i++) {
        switch ((THISISWEIRD(i) >> 7) & 0x1F) {
        case 0:
            if (!__GXVertexPacketHas(GX_VA_POS)) {
                __GX_WARNF(GXWARN_TEX_SRC_NPOS, i);
            }
            break;
        case 1:
            if (!__GXVertexPacketHas(GX_VA_NRM) && !__GXVertexPacketHas(GX_VA_NBT)) {
                __GX_WARNF(GXWARN_TEX_SRC_NNRM, i);
            }
            break;
        case 2:
            if (!__GXVertexPacketHas(GX_VA_CLR0)) {
                __GX_WARNF(GXWARN_TEX_SRC_NCLR0, i);
            }
            if (!__GXVertexPacketHas(GX_VA_CLR1)) {
                __GX_WARNF(GXWARN_TEX_SRC_NCLR1, i);
            }
            break;
        case 3:
        case 4:
            if (!__GXVertexPacketHas(GX_VA_NBT)) {
                __GX_WARNF(GXWARN_TEX_SRC_NNBT, i);
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
            if (!__GXVertexPacketHas(TextureEnums[((THISISWEIRD(i) >> 7) & 0x1F) - 5])) {
                __GX_WARNF(GXWARN_TEX_SRC_NTEX, i, ((THISISWEIRD(i) >> 7) & 0x1F) - 5);
            }
            break;
        default:
            __GX_WARNF(GXWARN_INV_TEX_SRC, i, (u8)((THISISWEIRD(i) >> 7) & 0x1F));
            break;
        }
    }
}

#define HMMM(count) ((u8 *)__gxVerif + 0xB)[((count) + 64) << 2]

static void CheckTextureOrder(void)
{
    u8 done = 0;
    u32 count = 0;

    while (!done) {
        if (count == __gxVerif->xfRegs[0x3F] || ((HMMM(count) >> 4) & 7)) {
            done = 1;
        } else {
            count += 1;
        }
    }

    done = 0;
    while (done == 0) {
        if (count == __gxVerif->xfRegs[0x3F]) {
            done = 1;
        } else if ((u32)((HMMM(count) >> 4) & 7) != 1) {
            if (!((HMMM(count) >> 4) & 7)) {
                __GX_WARN(GXWARN_INV_TG_ORDER);
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
        } else if (!((HMMM(count) >> 4) & 7) || (u32)((HMMM(count) >> 4) & 7) == 1) {
            __GX_WARN(GXWARN_INV_TG_ORDER);
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
            dirtyBit = __gxVerif->xfMtxDirty[i - 0x300];  // hmm...
        } else {
            dirtyBit = __gxVerif->xfMtxDirty[i];
        }
        if (dirtyBit == 0) {
            if (printedPreamble == 0) {
                __gxVerif->cb(1, WarnID, Str);
                printedPreamble = 1;
            }
            __GX_WARNF(GXWARN_ADDR_UNINIT, i);
        }
    }
}

#endif
