#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <macros.h>

#include "__gx.h"

GXRenderModeObj GXNtsc240Ds = {
    1,
    640, 240, 240,
    40, 0,
    640, 480,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc240DsAa = {
    1, 640, 240, 240, 40, 0, 640, 480, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc240Int = {
    0, 640, 240, 240, 40, 0, 640, 480, 0, 1, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc240IntAa = {
    0, 640, 240, 240, 40, 0, 640, 480, 0, 1, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc480IntDf = {
    0, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 }
};

GXRenderModeObj GXNtsc480Int = {
    0, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc480IntAa = {
    0, 640, 242, 480, 40, 0, 640, 480, 1, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 }
};

GXRenderModeObj GXNtsc480Prog = {
    2, 640, 480, 480, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc480ProgAa = {
    2, 640, 242, 480, 40, 0, 640, 480, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 }
};

GXRenderModeObj GXMpal240Ds = {9, 640, 240, 240, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal240DsAa = {9, 640, 240, 240, 40, 0, 640, 480, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal240Int = {8, 640, 240, 240, 40, 0, 640, 480, 0, 1, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal240IntAa = {8, 640, 240, 240, 40, 0, 640, 480, 0, 1, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal480IntDf = {8, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 } };
GXRenderModeObj GXMpal480Int = {8, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal480IntAa = {8, 640, 242, 480, 40, 0, 640, 480, 1, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 } };
GXRenderModeObj GXPal264Ds = {5, 640, 264, 264, 40, 23, 640, 528, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal264DsAa = {5, 640, 264, 264, 40, 23, 640, 528, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal264Int = {4, 640, 264, 264, 40, 23, 640, 528, 0, 1, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal264IntAa = {4, 640, 264, 264, 40, 23, 640, 528, 0, 1, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal528IntDf = {4, 640, 528, 528, 40, 23, 640, 528, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 } };
GXRenderModeObj GXPal528Int = {4, 640, 528, 528, 40, 23, 640, 528, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal524IntAa = {4, 640, 264, 524, 40, 23, 640, 524, 1, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 } };
GXRenderModeObj GXRmHW = {1, 320, 240, 240, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };

void GXAdjustForOverscan(GXRenderModeObj *rmin, GXRenderModeObj *rmout, u16 hor, u16 ver)
{
    unsigned short hor2 = hor * 2;
    unsigned short ver2 = ver * 2;
    unsigned long verf;

    if (rmin != rmout) {
        *rmout = *rmin;
    }

    rmout->fbWidth = rmin->fbWidth - hor2;
    verf = (ver2 * rmin->efbHeight) / (u32)rmin->xfbHeight;
    rmout->efbHeight = rmin->efbHeight - verf;
    if (rmin->xFBmode == VI_XFBMODE_SF && (rmin->viTVmode & 2) != 2) {
        rmout->xfbHeight = rmin->xfbHeight - ver;
    } else {
        rmout->xfbHeight = rmin->xfbHeight - ver2;
    }
    rmout->viWidth = rmin->viWidth - hor2;
    rmout->viHeight = rmin->viHeight - ver2;
    rmout->viXOrigin = rmin->viXOrigin + hor;
    rmout->viYOrigin = rmin->viYOrigin + ver;
}

#define LINE_OFFSET ((DOLPHIN_REVISION >= 45) ? 3 : 0)

void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht)
{
    CHECK_GXBEGIN(0x3B9+LINE_OFFSET, "GXSetDispCopySrc");

    gx->cpDispSrc = 0;
    SET_REG_FIELD(0x3BC+LINE_OFFSET, gx->cpDispSrc, 10,  0, left);
    SET_REG_FIELD(0x3BD+LINE_OFFSET, gx->cpDispSrc, 10, 10, top);
    SET_REG_FIELD(0x3BE+LINE_OFFSET, gx->cpDispSrc,  8, 24, 0x49);

    gx->cpDispSize = 0;
    SET_REG_FIELD(0x3C1+LINE_OFFSET, gx->cpDispSize, 10,  0, wd - 1);
    SET_REG_FIELD(0x3C2+LINE_OFFSET, gx->cpDispSize, 10, 10, ht - 1);
    SET_REG_FIELD(0x3C3+LINE_OFFSET, gx->cpDispSize,  8, 24, 0x4A);
}


void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht)
{
    CHECK_GXBEGIN(0x3D5+LINE_OFFSET, "GXSetTexCopySrc");

    gx->cpTexSrc = 0;
    SET_REG_FIELD(0x3D8+LINE_OFFSET, gx->cpTexSrc, 10,  0, left);
    SET_REG_FIELD(0x3D9+LINE_OFFSET, gx->cpTexSrc, 10, 10, top);
    SET_REG_FIELD(0x3DA+LINE_OFFSET, gx->cpTexSrc,  8, 24, 0x49);

    gx->cpTexSize = 0;
    SET_REG_FIELD(0x3DD+LINE_OFFSET, gx->cpTexSize, 10,  0, wd - 1);
    SET_REG_FIELD(0x3DE+LINE_OFFSET, gx->cpTexSize, 10, 10, ht - 1);
    SET_REG_FIELD(0x3DF+LINE_OFFSET, gx->cpTexSize,  8, 24, 0x4A);
}

void GXSetDispCopyDst(u16 wd, u16 ht)
{
    u16 stride;

    ASSERTMSGLINE(0x3F3+LINE_OFFSET, (wd & 0xF) == 0, "GXSetDispCopyDst: Width must be a multiple of 16");
    CHECK_GXBEGIN(0x3F4+LINE_OFFSET, "GXSetDispCopyDst");

    stride = (int)wd * 2;
    gx->cpDispStride = 0;
    SET_REG_FIELD(0x3FA+LINE_OFFSET, gx->cpDispStride, 10,  0, (stride >> 5) );
    SET_REG_FIELD(0x3FB+LINE_OFFSET, gx->cpDispStride,  8, 24, 0x4D);
}

void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap)
{
    u32 rowTiles;
    u32 colTiles;
    u32 cmpTiles;
    u32 peTexFmt;
    u32 peTexFmtH;

    CHECK_GXBEGIN(0x415+LINE_OFFSET, "GXSetTexCopyDst");

    gx->cpTexZ = 0;
    peTexFmt = fmt & 0xF;
    ASSERTMSGLINEV(0x434+LINE_OFFSET, peTexFmt < 13, "%s: invalid texture format", "GXSetTexCopyDst");

    if (fmt == GX_TF_Z16) {
        peTexFmt = 0xB;
    }
    switch (fmt) {
    case GX_TF_I4:
    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_IA8:
    case GX_CTF_YUVA8:
        SET_REG_FIELD(0, gx->cpTex, 2, 15, 3);
        break;
    default:
        SET_REG_FIELD(0, gx->cpTex, 2, 15, 2);
        break;
    }

    gx->cpTexZ = (fmt & _GX_TF_ZTF) == _GX_TF_ZTF;
    peTexFmtH = (peTexFmt >> 3) & 1;
    !peTexFmt;
    SET_REG_FIELD(0x44B+LINE_OFFSET, gx->cpTex, 1, 3, peTexFmtH);
    peTexFmt = peTexFmt & 7;
    __GetImageTileCount(fmt, wd, ht, &rowTiles, &colTiles, &cmpTiles);

    gx->cpTexStride = 0;
    SET_REG_FIELD(0x454+LINE_OFFSET, gx->cpTexStride, 10,  0, rowTiles * cmpTiles);
    SET_REG_FIELD(0x455+LINE_OFFSET, gx->cpTexStride,  8, 24, 0x4D);
    SET_REG_FIELD(0x456+LINE_OFFSET, gx->cpTex, 1, 9, mipmap);
    SET_REG_FIELD(0x457+LINE_OFFSET, gx->cpTex, 3, 4, peTexFmt);
}

void GXSetDispCopyFrame2Field(GXCopyMode mode)
{
    CHECK_GXBEGIN(0x468+LINE_OFFSET, "GXSetDispCopyFrame2Field");
    SET_REG_FIELD(0x469+LINE_OFFSET, gx->cpDisp, 2, 12, mode);
    SET_REG_FIELD(0x46A+LINE_OFFSET, gx->cpTex, 2, 12, 0);
}

void GXSetCopyClamp(GXFBClamp clamp)
{
    u8 clmpB;
    u8 clmpT;

    CHECK_GXBEGIN(0x47D+LINE_OFFSET, "GXSetCopyClamp");

    clmpT = (clamp & 1) == 1;
    clmpB = (clamp & 2) == 2;

    SET_REG_FIELD(0x481+LINE_OFFSET, gx->cpDisp, 1, 0, clmpT);
    SET_REG_FIELD(0x482+LINE_OFFSET, gx->cpDisp, 1, 1, clmpB);

    SET_REG_FIELD(0x484+LINE_OFFSET, gx->cpTex, 1, 0, clmpT);
    SET_REG_FIELD(0x485+LINE_OFFSET, gx->cpTex, 1, 1, clmpB);
}

u32 GXSetDispCopyYScale(f32 vscale)
{
    u8 enable;
    u32 iScale;
    f32 fScale;
    u32 ht;
    u32 reg;

    CHECK_GXBEGIN(0x49B+LINE_OFFSET, "GXSetDispCopyYScale");

    ASSERTMSGLINE(0x49D+LINE_OFFSET, vscale >= 1.0f, "GXSetDispCopyYScale: Vertical scale must be >= 1.0");

    iScale = (u32) (256.0f / vscale) & 0x1FF;
    fScale = 256.0f / (f32) iScale;
    enable = (iScale != 256);

    reg = 0;
    SET_REG_FIELD(0x4A6+LINE_OFFSET, reg, 9, 0, iScale);
    SET_REG_FIELD(0x4A7+LINE_OFFSET, reg, 8, 24, 0x4E);
    GX_WRITE_RAS_REG(reg);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
    SET_REG_FIELD(0x4AB+LINE_OFFSET, gx->cpDisp, 1, 10, enable);
    ht = GET_REG_FIELD(gx->cpDispSize, 10, 10) + 1;
    return ht * fScale;
}

void GXSetCopyClear(GXColor clear_clr, u32 clear_z)
{
    u32 reg;

    CHECK_GXBEGIN(0x4C4+LINE_OFFSET, "GXSetCopyClear");
    ASSERTMSGLINE(0x4C6+LINE_OFFSET, clear_z <= 0xFFFFFF, "GXSetCopyClear: Z clear value is out of range");

    reg = 0;
    SET_REG_FIELD(0x4C9+LINE_OFFSET, reg, 8, 0, clear_clr.r);
    SET_REG_FIELD(0x4CA+LINE_OFFSET, reg, 8, 8, clear_clr.a);
    SET_REG_FIELD(0x4CB+LINE_OFFSET, reg, 8, 24, 0x4F);
    GX_WRITE_RAS_REG(reg);

    reg = 0;
    SET_REG_FIELD(0x4CF+LINE_OFFSET, reg, 8, 0, clear_clr.b);
    SET_REG_FIELD(0x4D0+LINE_OFFSET, reg, 8, 8, clear_clr.g);
    SET_REG_FIELD(0x4D1+LINE_OFFSET, reg, 8, 24, 0x50);
    GX_WRITE_RAS_REG(reg);

    reg = 0;
    SET_REG_FIELD(0x4D5+LINE_OFFSET, reg, 24, 0, clear_z);
    SET_REG_FIELD(0x4D6+LINE_OFFSET, reg, 8, 24, 0x51);
    GX_WRITE_RAS_REG(reg);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]) {
    u32 msLoc[4];
    u32 coeff0;
    u32 coeff1;

    CHECK_GXBEGIN(0x4F1+LINE_OFFSET, "GXSetCopyFilter");

    if (aa != 0) {
        msLoc[0] = 0;
        SET_REG_FIELD(0x4F5+LINE_OFFSET, msLoc[0], 4,  0, sample_pattern[0][0]);
        SET_REG_FIELD(0x4F6+LINE_OFFSET, msLoc[0], 4,  4, sample_pattern[0][1]);
        SET_REG_FIELD(0x4F7+LINE_OFFSET, msLoc[0], 4,  8, sample_pattern[1][0]);
        SET_REG_FIELD(0x4F8+LINE_OFFSET, msLoc[0], 4, 12, sample_pattern[1][1]);
        SET_REG_FIELD(0x4F9+LINE_OFFSET, msLoc[0], 4, 16, sample_pattern[2][0]);
        SET_REG_FIELD(0x4FA+LINE_OFFSET, msLoc[0], 4, 20, sample_pattern[2][1]);
        SET_REG_FIELD(0x4FB+LINE_OFFSET, msLoc[0], 8, 24, 1);

        msLoc[1] = 0;
        SET_REG_FIELD(0x4FE+LINE_OFFSET, msLoc[1], 4,  0, sample_pattern[3][0]);
        SET_REG_FIELD(0x4FF+LINE_OFFSET, msLoc[1], 4,  4, sample_pattern[3][1]);
        SET_REG_FIELD(0x500+LINE_OFFSET, msLoc[1], 4,  8, sample_pattern[4][0]);
        SET_REG_FIELD(0x501+LINE_OFFSET, msLoc[1], 4, 12, sample_pattern[4][1]);
        SET_REG_FIELD(0x502+LINE_OFFSET, msLoc[1], 4, 16, sample_pattern[5][0]);
        SET_REG_FIELD(0x503+LINE_OFFSET, msLoc[1], 4, 20, sample_pattern[5][1]);
        SET_REG_FIELD(0x504+LINE_OFFSET, msLoc[1], 8, 24, 2);

        msLoc[2] = 0;
        SET_REG_FIELD(0x507+LINE_OFFSET, msLoc[2], 4,  0, sample_pattern[6][0]);
        SET_REG_FIELD(0x508+LINE_OFFSET, msLoc[2], 4,  4, sample_pattern[6][1]);
        SET_REG_FIELD(0x509+LINE_OFFSET, msLoc[2], 4,  8, sample_pattern[7][0]);
        SET_REG_FIELD(0x50A+LINE_OFFSET, msLoc[2], 4, 12, sample_pattern[7][1]);
        SET_REG_FIELD(0x50B+LINE_OFFSET, msLoc[2], 4, 16, sample_pattern[8][0]);
        SET_REG_FIELD(0x50C+LINE_OFFSET, msLoc[2], 4, 20, sample_pattern[8][1]);
        SET_REG_FIELD(0x50D+LINE_OFFSET, msLoc[2], 8, 24, 3);

        msLoc[3] = 0;
        SET_REG_FIELD(0x510+LINE_OFFSET, msLoc[3], 4,  0, sample_pattern[9][0]);
        SET_REG_FIELD(0x511+LINE_OFFSET, msLoc[3], 4,  4, sample_pattern[9][1]);
        SET_REG_FIELD(0x512+LINE_OFFSET, msLoc[3], 4,  8, sample_pattern[10][0]);
        SET_REG_FIELD(0x513+LINE_OFFSET, msLoc[3], 4, 12, sample_pattern[10][1]);
        SET_REG_FIELD(0x514+LINE_OFFSET, msLoc[3], 4, 16, sample_pattern[11][0]);
        SET_REG_FIELD(0x515+LINE_OFFSET, msLoc[3], 4, 20, sample_pattern[11][1]);
        SET_REG_FIELD(0x516+LINE_OFFSET, msLoc[3], 8, 24, 4);
    } else {
        msLoc[0] = 0x01666666;
        msLoc[1] = 0x02666666;
        msLoc[2] = 0x03666666;
        msLoc[3] = 0x04666666;
    }
    GX_WRITE_RAS_REG(msLoc[0]);
    GX_WRITE_RAS_REG(msLoc[1]);
    GX_WRITE_RAS_REG(msLoc[2]);
    GX_WRITE_RAS_REG(msLoc[3]);

    coeff0 = 0;
    SET_REG_FIELD(0, coeff0, 8, 24, 0x53);
    coeff1 = 0;
    SET_REG_FIELD(0, coeff1, 8, 24, 0x54);
    if (vf != 0) {
        SET_REG_FIELD(0x52E+LINE_OFFSET, coeff0, 6,  0, vfilter[0]);
        SET_REG_FIELD(0x52F+LINE_OFFSET, coeff0, 6,  6, vfilter[1]);
        SET_REG_FIELD(0x530+LINE_OFFSET, coeff0, 6, 12, vfilter[2]);
        SET_REG_FIELD(0x531+LINE_OFFSET, coeff0, 6, 18, vfilter[3]);
        SET_REG_FIELD(0x532+LINE_OFFSET, coeff1, 6,  0, vfilter[4]);
        SET_REG_FIELD(0x533+LINE_OFFSET, coeff1, 6,  6, vfilter[5]);
        SET_REG_FIELD(0x534+LINE_OFFSET, coeff1, 6, 12, vfilter[6]);
    } else {
        SET_REG_FIELD(0, coeff0, 6,  0, 0);
        SET_REG_FIELD(0, coeff0, 6,  6, 0);
        SET_REG_FIELD(0, coeff0, 6, 12, 21);
        SET_REG_FIELD(0, coeff0, 6, 18, 22);
        SET_REG_FIELD(0, coeff1, 6,  0, 21);
        SET_REG_FIELD(0, coeff1, 6,  6,  0);
        SET_REG_FIELD(0, coeff1, 6, 12,  0);
    }
    GX_WRITE_RAS_REG(coeff0);
    GX_WRITE_RAS_REG(coeff1);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

void GXSetDispCopyGamma(GXGamma gamma)
{
    CHECK_GXBEGIN(0x555+LINE_OFFSET, "GXSetDispCopyGamma");
    SET_REG_FIELD(0x556+LINE_OFFSET, gx->cpDisp, 2, 7, gamma);
}

#if DEBUG
static void __GXVerifCopy(void *dest, u8 clear)
{
    u8 clmpT;
    u8 clmpB;
    u32 x0;
    u32 y0;
    u32 dx;
    u32 dy;

    CHECK_GXBEGIN(0x56A+LINE_OFFSET, "GXCopyDisp");

    clmpT = GET_REG_FIELD(gx->cpDisp, 1, 0);
    clmpB = (u32)GET_REG_FIELD(gx->cpDisp, 1, 1);
    x0 = GET_REG_FIELD(gx->cpDispSrc, 10, 0);
    dx = GET_REG_FIELD(gx->cpDispSize, 10, 0) + 1;
    y0 = GET_REG_FIELD(gx->cpDispSrc, 10, 10);
    dy = GET_REG_FIELD(gx->cpDispSize, 10, 10) + 1;

    ASSERTMSGLINE(0x574+LINE_OFFSET, clmpT || y0 != 0, "GXCopy: Have to set GX_CLAMP_TOP if source top == 0");
    ASSERTMSGLINE(0x576+LINE_OFFSET, clmpB || y0 + dy <= 528, "GXCopy: Have to set GX_CLAMP_BOTTOM if source bottom > 528");
    ASSERTMSGLINE(0x57B+LINE_OFFSET, (gx->peCtrl & 7) != 3 || clear == 0, "GXCopy: Can not do clear while pixel type is Z");
    if ((u32) (gx->peCtrl & 7) == 5) {
        ASSERTMSGLINE(0x581+LINE_OFFSET, clear == 0, "GXCopy: Can not clear YUV framebuffer");
        ASSERTMSGLINE(0x583+LINE_OFFSET, (x0 & 3) == 0, "GXCopy: Source x is not multiple of 4 for YUV copy");
        ASSERTMSGLINE(0x585+LINE_OFFSET, (y0 & 3) == 0, "GXCopy: Source y is not multiple of 4 for YUV copy");
        ASSERTMSGLINE(0x587+LINE_OFFSET, (dx & 3) == 0, "GXCopy: Source width is not multiple of 4 for YUV copy");
        ASSERTMSGLINE(0x589+LINE_OFFSET, (dy & 3) == 0, "GXCopy: Source height is not multiple of 4 for YUV copy");
    } else {
        ASSERTMSGLINE(0x58D+LINE_OFFSET, (x0 & 1) == 0, "GXCopy: Source x is not multiple of 2 for RGB copy");
        ASSERTMSGLINE(0x58F+LINE_OFFSET, (y0 & 1) == 0, "GXCopy: Source y is not multiple of 2 for RGB copy");
        ASSERTMSGLINE(0x591+LINE_OFFSET, (dx & 1) == 0, "GXCopy: Source width is not multiple of 2 for RGB copy");
        ASSERTMSGLINE(0x593+LINE_OFFSET, (dy & 1) == 0, "GXCopy: Source height is not multiple of 2 for RGB copy");
    }
    ASSERTMSGLINE(0x597+LINE_OFFSET, ((u32)dest & 0x1F) == 0, "GXCopy: Display destination address not 32B aligned");
}
#endif

void GXCopyDisp(void *dest, GXBool clear)
{
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    u8 changePeCtrl;

    CHECK_GXBEGIN(0x5B1+LINE_OFFSET, "GXCopyDisp");

#if DEBUG
    __GXVerifCopy(dest, clear);
#endif
    if (clear) {
        reg = gx->zmode;
        SET_REG_FIELD(0, reg, 1, 0, 1);
        SET_REG_FIELD(0, reg, 3, 1, 7);
        GX_WRITE_RAS_REG(reg);

        reg = gx->cmode0;
        SET_REG_FIELD(0, reg, 1, 0, 0);
        SET_REG_FIELD(0, reg, 1, 1, 0);
        GX_WRITE_RAS_REG(reg);
    }
    changePeCtrl = FALSE;
    if ((clear || (u32)GET_REG_FIELD(gx->peCtrl, 3, 0) == 3)
     && (u32)GET_REG_FIELD(gx->peCtrl, 1, 6) == 1) {
        changePeCtrl = TRUE;
        tempPeCtrl = gx->peCtrl;
        SET_REG_FIELD(0, tempPeCtrl, 1, 6, 0);
        GX_WRITE_RAS_REG(tempPeCtrl);
    }
    GX_WRITE_RAS_REG(gx->cpDispSrc);
    GX_WRITE_RAS_REG(gx->cpDispSize);
    GX_WRITE_RAS_REG(gx->cpDispStride);

    phyAddr = (u32)dest & 0x3FFFFFFF;
    reg = 0;
    SET_REG_FIELD(0x5D8+LINE_OFFSET, reg, 21, 0, phyAddr >> 5);
    SET_REG_FIELD(0x5D9+LINE_OFFSET, reg, 8, 24, 0x4B);
    GX_WRITE_RAS_REG(reg);

    SET_REG_FIELD(0x5DC+LINE_OFFSET, gx->cpDisp, 1, 11, clear);
    SET_REG_FIELD(0x5DD+LINE_OFFSET, gx->cpDisp, 1, 14, 1);
    SET_REG_FIELD(0x5DE+LINE_OFFSET, gx->cpDisp, 8, 24, 0x52);
    GX_WRITE_RAS_REG(gx->cpDisp);

    if (clear) {
        GX_WRITE_RAS_REG(gx->zmode);
        GX_WRITE_RAS_REG(gx->cmode0);
    }
    if (changePeCtrl) {
        GX_WRITE_RAS_REG(gx->peCtrl);
    }
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

void GXCopyTex(void *dest, GXBool clear)
{
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    u8 changePeCtrl;

    CHECK_GXBEGIN(0x604+LINE_OFFSET, "GXCopyTex");

#if DEBUG
    __GXVerifCopy(dest, clear);
#endif
    if (clear) {
        reg = gx->zmode;
        SET_REG_FIELD(0, reg, 1, 0, 1);
        SET_REG_FIELD(0, reg, 3, 1, 7);
        GX_WRITE_RAS_REG(reg);

        reg = gx->cmode0;
        SET_REG_FIELD(0, reg, 1, 0, 0);
        SET_REG_FIELD(0, reg, 1, 1, 0);
        GX_WRITE_RAS_REG(reg);
    }
    changePeCtrl = 0;
    tempPeCtrl = gx->peCtrl;
    if (((u8) gx->cpTexZ != 0) && ((u32) (tempPeCtrl & 7) != 3)) {
        changePeCtrl = 1;
        tempPeCtrl = (tempPeCtrl & 0xFFFFFFF8) | 3;
    }
    if (((clear != 0) || ((u32) (tempPeCtrl & 7) == 3)) && ((u32) ((tempPeCtrl >> 6U) & 1) == 1)) {
        changePeCtrl = 1;
        tempPeCtrl &= 0xFFFFFFBF;
    }
    if (changePeCtrl) {
        GX_WRITE_RAS_REG(tempPeCtrl);
    }
    GX_WRITE_RAS_REG(gx->cpTexSrc);
    GX_WRITE_RAS_REG(gx->cpTexSize);
    GX_WRITE_RAS_REG(gx->cpTexStride);

    phyAddr = (u32)dest & 0x3FFFFFFF;
    reg = 0;
    SET_REG_FIELD(0x635+LINE_OFFSET, reg, 21, 0, phyAddr >> 5);
    SET_REG_FIELD(0x636+LINE_OFFSET, reg, 8, 24, 0x4B);
    GX_WRITE_RAS_REG(reg);

    SET_REG_FIELD(0x639+LINE_OFFSET, gx->cpTex, 1, 11, clear);
    SET_REG_FIELD(0x63A+LINE_OFFSET, gx->cpTex, 1, 14, 0);
    SET_REG_FIELD(0x63B+LINE_OFFSET, gx->cpTex, 8, 24, 0x52);
    GX_WRITE_RAS_REG(gx->cpTex);

    if (clear != 0) {
        GX_WRITE_RAS_REG(gx->zmode);
        GX_WRITE_RAS_REG(gx->cmode0);
    }
    if (changePeCtrl) {
        GX_WRITE_RAS_REG(gx->peCtrl);
    }
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

void GXClearBoundingBox(void)
{
    u32 reg;

    CHECK_GXBEGIN(0x65B+LINE_OFFSET, "GXClearBoundingBox");
    reg = 0x550003FF;
    GX_WRITE_RAS_REG(reg);
    reg = 0x560003FF;
    GX_WRITE_RAS_REG(reg);
#if DOLPHIN_REVISION >= 45
    gx->bpSentNot = 0;
#else
    gx->bpSent = 1;
#endif
}

void GXReadBoundingBox(u16 *left, u16 *top, u16 *right, u16 *bottom)
{
#if DOLPHIN_REVISION < 45
    CHECK_GXBEGIN(0x671+LINE_OFFSET, "GXReadBoundingBox");
#endif
    *left = __peReg[8];
    *top = __peReg[10];
    *right = __peReg[9];
    *bottom = __peReg[11];
}
