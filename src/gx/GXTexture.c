#include <dolphin/gx.h>

#include "__gx.h"

// GXTexObj internal data
typedef struct __GXTexObjInt_struct {
    u32 mode0; // offset 0x0, size 0x4
    u32 mode1; // offset 0x4, size 0x4
    u32 image0; // offset 0x8, size 0x4
    u32 image3; // offset 0xC, size 0x4
    void *userData; // offset 0x10, size 0x4
    GXTexFmt fmt; // offset 0x14, size 0x4
    u32 tlutName; // offset 0x18, size 0x4
    u16 loadCnt; // offset 0x1C, size 0x2
    u8 loadFmt; // offset 0x1E, size 0x1
    u8 flags; // offset 0x1F, size 0x1
} __GXTexObjInt;

typedef struct __GXTexRegionInt_struct {
    // total size: 0x10
    unsigned long image1; // offset 0x0, size 0x4
    unsigned long image2; // offset 0x4, size 0x4
    unsigned short sizeEven; // offset 0x8, size 0x2
    unsigned short sizeOdd; // offset 0xA, size 0x2
    unsigned char is32bMipmap; // offset 0xC, size 0x1
    unsigned char isCached; // offset 0xD, size 0x1
} __GXTexRegionInt;

u8 GXTexMode0Ids[8] = { 0x80, 0x81, 0x82, 0x83, 0xA0, 0xA1, 0xA2, 0xA3 }; // size: 0x8, address: 0x0
u8 GXTexMode1Ids[8] = { 0x84, 0x85, 0x86, 0x87, 0xA4, 0xA5, 0xA6, 0xA7 }; // size: 0x8, address: 0x8
u8 GXTexImage0Ids[8] = { 0x88, 0x89, 0x8A, 0x8B, 0xA8, 0xA9, 0xAA, 0xAB }; // size: 0x8, address: 0x10
u8 GXTexImage1Ids[8] = { 0x8C, 0x8D, 0x8E, 0x8F, 0xAC, 0xAD, 0xAE, 0xAF }; // size: 0x8, address: 0x18
u8 GXTexImage2Ids[8] = { 0x90, 0x91, 0x92, 0x93, 0xB0, 0xB1, 0xB2, 0xB3 }; // size: 0x8, address: 0x20
u8 GXTexImage3Ids[8] = { 0x94, 0x95, 0x96, 0x97, 0xB4, 0xB5, 0xB6, 0xB7 }; // size: 0x8, address: 0x28
u8 GXTexTlutIds[8] = { 0x98, 0x99, 0x9A, 0x9B, 0xB8, 0xB9, 0xBA, 0xBB }; // size: 0x8, address: 0x30
static u8 GX2HWFiltConv[6] = { 0x00, 0x04, 0x01, 0x05, 0x02, 0x06 }; // size: 0x6, address: 0x38
static u8 HW2GXFiltConv[8] = { 0x00, 0x02, 0x04, 0x00, 0x01, 0x03, 0x05, 0x00 }; // size: 0x8, address: 0x40

static void __GXGetTexTileShift(GXTexFmt fmt, u32 *rowTileS, u32 *colTileS)
{
    switch (fmt) {
    case GX_TF_I4:
    case 0x8:
    case GX_TF_CMPR:
    case GX_CTF_R4:
    case GX_CTF_Z4:
        *rowTileS = 3;
        *colTileS = 3;
        break;
    case GX_TF_I8:
    case GX_TF_IA4:
    case 0x9:
    case GX_TF_Z8:
    case GX_CTF_RA4:
    case GX_TF_A8:
    case GX_CTF_R8:
    case GX_CTF_G8:
    case GX_CTF_B8:
    case GX_CTF_Z8M:
    case GX_CTF_Z8L:
        *rowTileS = 3;
        *colTileS = 2;
        break;
    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case GX_TF_RGBA8:
    case 0xA:
    case GX_TF_Z16:
    case GX_TF_Z24X8:
    case GX_CTF_RA8:
    case GX_CTF_RG8:
    case GX_CTF_GB8:
    case GX_CTF_Z16L:
        *rowTileS = 2;
        *colTileS = 2;
        break;
    default:
        *rowTileS = *colTileS = 0;
        ASSERTMSGLINEV(0x184, 0, "%s: invalid texture format", "GX");
        break;
    }
}

u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u8 mipmap, u8 max_lod)
{
    u32 tileShiftX;
    u32 tileShiftY;
    u32 tileBytes;
    u32 bufferSize;
    u32 nx;
    u32 ny;
    u32 level;

    ASSERTMSGLINEV(0x194, width <= 1024, "%s: width too large", "GXGetTexBufferSize");
    ASSERTMSGLINEV(0x195, height <= 1024, "%s: height too large", "GXGetTexBufferSize");

    __GXGetTexTileShift(format, &tileShiftX, &tileShiftY);
    if (format == GX_TF_RGBA8 || format == GX_TF_Z24X8) {
        tileBytes = 64;
    } else {
        tileBytes = 32;
    }
    if (mipmap == 1) {
        nx = 1 << (31 - __cntlzw(width));
        ASSERTMSGLINEV(0x1A7, width == nx, "%s: width must be a power of 2", "GXGetTexBufferSize");
        ny = 1 << (31 - __cntlzw(height));
        ASSERTMSGLINEV(0x1AA, height == ny, "%s: height must be a power of 2", "GXGetTexBufferSize");

        bufferSize = 0;
        for (level = 0; level < max_lod; level++) {
            nx = (width + (1 << tileShiftX) - 1) >> tileShiftX;
            ny = (height + (1 << tileShiftY) - 1) >> tileShiftY;
            bufferSize += tileBytes * (nx * ny);
            if (width == 1 && height == 1) {
                break;
            }
            width = (width > 1) ? width >> 1 : 1;
            height = (height > 1) ? height >> 1 : 1;
        }
    } else {
        nx = (width + (1 << tileShiftX) - 1) >> tileShiftX;
        ny = (height + (1 << tileShiftY) - 1) >> tileShiftY;
        bufferSize = nx * ny * tileBytes;
    }
    return bufferSize;
}

void __GetImageTileCount(enum _GXTexFmt fmt, u16 wd, u16 ht, u32 *rowTiles, u32 *colTiles, u32 *cmpTiles)
{
    u32 texRowShift;
    u32 texColShift;

    __GXGetTexTileShift(fmt, &texRowShift, &texColShift);
    if (wd == 0) {
        wd = 1;
    }
    if (ht == 0) {
        ht = 1;
    }
    *rowTiles = (wd + (1 << texRowShift) - 1) >> texRowShift;
    *colTiles = (ht + (1 << texColShift) - 1) >> texColShift;
    *cmpTiles = (fmt == GX_TF_RGBA8 || fmt == GX_TF_Z24X8) ? 2 : 1;
}

void GXInitTexObj(GXTexObj *obj, void *image_ptr, u16 width, u16 height, GXTexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, u8 mipmap)
{
    u32 imageBase;
    u32 maxLOD;
    u16 rowT;
    u16 colT;
    u32 rowC;
    u32 colC;
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x1FD, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x1FF, "GXInitTexObj");
    ASSERTMSGLINEV(0x200, width <= 1024, "%s: width too large", "GXInitTexObj");
    ASSERTMSGLINEV(0x201, height <= 1024, "%s: height too large", "GXInitTexObj");
    ASSERTMSGLINEV(0x203, !(format & 0x20), "%s: invalid texture format", "GXInitTexObj");
#if DEBUG
    if (wrap_s != GX_CLAMP || mipmap != 0) {
        u32 mask = 1 << (31 - __cntlzw(width));
        ASSERTMSGLINEV(0x20D, width == mask, "%s: width must be a power of 2", "GXInitTexObj");
    }
    if (wrap_t != GX_CLAMP || mipmap != 0) {
        u32 mask = 1 << (31 - __cntlzw(height));
        ASSERTMSGLINEV(0x212, height == mask, "%s: height must be a power of 2", "GXInitTexObj");
    }
#endif
    memset(t, 0, 0x20);
    SET_REG_FIELD(0x220, t->mode0, 2, 0, wrap_s);
    SET_REG_FIELD(0x221, t->mode0, 2, 2, wrap_t);
    SET_REG_FIELD(0x222, t->mode0, 1, 4, 1);
    if (mipmap != 0) {
        u8 lmax;
        t->flags |= 1;
        t->mode0 = (t->mode0 & 0xFFFFFF1F) | 0xC0;
        if (width > height) {
            maxLOD = 31 - __cntlzw(width);
        } else {
            maxLOD = 31 - __cntlzw(height);
        }
        lmax = 16.0f * maxLOD;
        SET_REG_FIELD(0x234, t->mode1, 8, 8, lmax);
    } else {
        t->mode0 = (t->mode0 & 0xFFFFFF1F) | 0x80;
    }
    t->fmt = format;
    SET_REG_FIELD(0x240, t->image0, 10, 0, width - 1);
    SET_REG_FIELD(0x241, t->image0, 10, 10, height - 1);
    SET_REG_FIELD(0x242, t->image0, 4, 20, format & 0xF);
    ASSERTMSGLINEV(0x248, ((u32)image_ptr & 0x1F) == 0, "%s: %s pointer not aligned to 32B", "GXInitTexObj", "image");
    imageBase = (u32)((u32)image_ptr >> 5) & 0x01FFFFFF;
    SET_REG_FIELD(0x24A, t->image3, 21, 0, imageBase);
    switch (format & 0xF) {
    case 0:
    case 8:
        t->loadFmt = 1;
        rowT = 3;
        colT = 3;
        break;
    case 1:
    case 2:
    case 9:
        t->loadFmt = 2;
        rowT = 3;
        colT = 2;
        break;
    case 3:
    case 4:
    case 5:
    case 10:
        t->loadFmt = 2;
        rowT = 2;
        colT = 2;
        break;
    case 6:
        t->loadFmt = 3;
        rowT = 2;
        colT = 2;
        break;
    case 14:
        t->loadFmt = 0;
        rowT = 3;
        colT = 3;
        break;
    default:
        ASSERTMSGLINEV(0x275, 0, "%s: invalid texture format", "GXPreLoadEntireTexture");
        t->loadFmt = 2;
        rowT = 2;
        colT = 2;
        break;
    }
    rowC = (width + (1 << rowT) - 1) >> rowT;
    colC = (height + (1 << colT) - 1) >> colT;
    t->loadCnt = (rowC * colC) & 0x7FFF;
    t->flags |= 2;
}

void GXInitTexObjCI(GXTexObj *obj, void *image_ptr, u16 width, u16 height, GXCITexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, u8 mipmap, u32 tlut_name)
{
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x29B, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x29D, "GXInitTexObjCI");
    GXInitTexObj(obj, image_ptr, width, height, format, wrap_s, wrap_t, mipmap);
    t->flags &= 0xFFFFFFFD;
    t->tlutName = tlut_name;
}

void GXInitTexObjLOD(GXTexObj *obj, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod, f32 max_lod, f32 lod_bias, u8 bias_clamp, u8 do_edge_lod, GXAnisotropy max_aniso)
{
    u8 lbias;
    u8 lmin;
    u8 lmax;
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x2C2, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x2C4, "GXInitTexObjLOD");

    if (lod_bias < -4.0f) {
        lod_bias = -4.0f;
    } else if (lod_bias >= 4.0f) {
        lod_bias = 3.99f;
    }
    lbias = 32.0f * lod_bias;
    SET_REG_FIELD(0x2CE, t->mode0, 8, 9, lbias);
    SET_REG_FIELD(0x2CF, t->mode0, 1, 4, (mag_filt == GX_LINEAR) ? 1 : 0);
    ASSERTMSGLINE(0x2D1, (u32)min_filt <= 5, "GXInitTexObjLOD: invalid min_filt value");
    SET_REG_FIELD(0x2D2, t->mode0, 3, 5, GX2HWFiltConv[min_filt]);
    SET_REG_FIELD(0x2D3, t->mode0, 1, 8, do_edge_lod ? 0 : 1);
    t->mode0 &= 0xFFFDFFFF;
    t->mode0 &= 0xFFFBFFFF;
    SET_REG_FIELD(0x2D6, t->mode0, 2, 19, max_aniso);
    SET_REG_FIELD(0x2D7, t->mode0, 1, 21, bias_clamp);
    if (min_lod < 0.0f) {
        min_lod = 0.0f;
    } else if (min_lod > 10.0f) {
        min_lod = 10.0f;
    }
    lmin = 16.0f * min_lod;
    if (max_lod < 0.0f) {
        max_lod = 0.0f;
    } else if (max_lod > 10.0f) {
        max_lod = 10.0f;
    }
    lmax = 16.0f * max_lod;
    SET_REG_FIELD(0x2E5, t->mode1, 8, 0, lmin);
    SET_REG_FIELD(0x2E6, t->mode1, 8, 8, lmax);
}

void GXInitTexObjData(GXTexObj *obj, void *image_ptr)
{
    u32 imageBase;
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x2F9, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x2FB, "GXInitTexObjData");
    ASSERTMSGLINEV(0x2FE, ((u32)image_ptr & 0x1F) == 0, "%s: %s pointer not aligned to 32B", "GXInitTexObjData", "image");
    imageBase = ((u32)image_ptr >> 5) & 0x01FFFFFF;
    SET_REG_FIELD(0x301, t->image3, 21, 0, imageBase);
}

void GXInitTexObjWrapMode(GXTexObj *obj, GXTexWrapMode sm, GXTexWrapMode tm)
{
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x313, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x315, "GXInitTexObjWrapMode");
    SET_REG_FIELD(0x317, t->mode0, 2, 0, sm);
    SET_REG_FIELD(0x318, t->mode0, 2, 2, tm);
}

void GXInitTexObjTlut(GXTexObj *obj, u32 tlut_name)
{
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x329, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x32B, "GXInitTexObjTlut");
    t->tlutName = tlut_name;
}

void GXInitTexObjUserData(GXTexObj *obj, void *user_data)
{
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x33E, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x33F, "GXInitTexObjUserData");
    t->userData = user_data;
}

void *GXGetTexObjUserData(GXTexObj *obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x345, obj, "Texture Object Pointer is null");
    return t->userData;
}

void GXGetTexObjAll(GXTexObj *obj, void **image_ptr, u16 *width, u16 *height, GXTexFmt *format, GXTexWrapMode *wrap_s, GXTexWrapMode *wrap_t, u8 *mipmap)
{
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x359, obj, "Texture Object Pointer is null");
    *image_ptr = (void *)(GET_REG_FIELD(t->image3, 21, 0) << 5);
    *width = (u32)GET_REG_FIELD(t->image0, 10, 0) + 1;
    *height = (u32)GET_REG_FIELD(t->image0, 10, 10) + 1;
    *format = t->fmt;
    *wrap_s = GET_REG_FIELD(t->mode0, 2, 0);
    *wrap_t = GET_REG_FIELD(t->mode0, 2, 2);
    *mipmap = (t->flags & 1) == 1;
}

void *GXGetTexObjData(GXTexObj *to)
{
    __GXTexObjInt *t = (__GXTexObjInt *)to;

    ASSERTMSGLINE(0x366, to, "Texture Object Pointer is null");
    return (void *)(GET_REG_FIELD(t->image3, 21, 0) << 5);
}

u16 GXGetTexObjWidth(GXTexObj *to)
{
    __GXTexObjInt *t = (__GXTexObjInt *)to;

    ASSERTMSGLINE(0x36C, to, "Texture Object Pointer is null");
    return (u32)GET_REG_FIELD(t->image0, 10, 0) + 1;
}

u16 GXGetTexObjHeight(GXTexObj *to)
{
    __GXTexObjInt *t = (__GXTexObjInt *)to;

    ASSERTMSGLINE(0x372, to, "Texture Object Pointer is null");
    return (u32)GET_REG_FIELD(t->image0, 10, 10) + 1;
}

GXTexFmt GXGetTexObjFmt(GXTexObj *to)
{
    __GXTexObjInt *t = (__GXTexObjInt *)to;

    ASSERTMSGLINE(0x378, to, "Texture Object Pointer is null");
    return t->fmt;
}

GXTexWrapMode GXGetTexObjWrapS(GXTexObj *to)
{
    __GXTexObjInt *t = (__GXTexObjInt *)to;

    ASSERTMSGLINE(0x37E, to, "Texture Object Pointer is null");
    return GET_REG_FIELD(t->mode0, 2, 0);
}

GXTexWrapMode GXGetTexObjWrapT(GXTexObj *to)
{
    __GXTexObjInt *t = (__GXTexObjInt *)to;

    ASSERTMSGLINE(0x384, to, "Texture Object Pointer is null");
    return GET_REG_FIELD(t->mode0, 2, 2);
}

u8 GXGetTexObjMipMap(GXTexObj *to)
{
    __GXTexObjInt *t = (__GXTexObjInt *)to;

    ASSERTMSGLINE(0x38A, to, "Texture Object Pointer is null");
    return (t->flags & 1) == 1;
}

void GXGetTexObjLODAll(GXTexObj *tex_obj, GXTexFilter *min_filt, GXTexFilter *mag_filt, f32 *min_lod, f32 *max_lod, f32 *lod_bias, u8 *bias_clamp, u8 *do_edge_lod, GXAnisotropy *max_aniso)
{
    s16 tmp;
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3A0, tex_obj, "Texture Object Pointer is null");
    *min_filt = HW2GXFiltConv[GET_REG_FIELD(t->mode0, 3, 5)];
    *mag_filt = GET_REG_FIELD(t->mode0, 1, 4);
    *min_lod = (u8)t->mode1 / 16.0f;
    *max_lod = (u32)GET_REG_FIELD(t->mode1, 8, 8) / 16.0f;
    tmp = (s32)GET_REG_FIELD(t->mode0, 8, 9);
    if (tmp & 0x80) {
        tmp = -(tmp & 0x7F);
    }
    *lod_bias = 32.0f * tmp;
    *bias_clamp = (u32)GET_REG_FIELD(t->mode0, 1, 21);
    *do_edge_lod = !GET_REG_FIELD(t->mode0, 1, 8);
    *max_aniso = GET_REG_FIELD(t->mode0, 2, 19);
}

GXTexFilter GXGetTexObjMinFilt(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3B2, tex_obj, "Texture Object Pointer is null");
    return HW2GXFiltConv[GET_REG_FIELD(t->mode0, 3, 5)];
}

GXTexFilter GXGetTexObjMagFilt(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3B9, tex_obj, "Texture Object Pointer is null");
    return GET_REG_FIELD(t->mode0, 1, 4);
}

f32 GXGetTexObjMinLOD(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3BF, tex_obj, "Texture Object Pointer is null");
    return (u32)GET_REG_FIELD(t->mode1, 8, 0) / 16.0f;
}

f32 GXGetTexObjMaxLOD(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3C5, tex_obj, "Texture Object Pointer is null");
    return (u32)GET_REG_FIELD(t->mode1, 8, 8) / 16.0f;
}

f32 GXGetTexObjLODBias(GXTexObj *tex_obj)
{
    s16 tmp;
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3CC, tex_obj, "Texture Object Pointer is null");
    tmp = (s32)GET_REG_FIELD(t->mode0, 8, 9);
    if (tmp & 0x80) {
        tmp = -(tmp & 0x7F);
    }
    return 32.0f * tmp;
}

u8 GXGetTexObjBiasClamp(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3D5, tex_obj, "Texture Object Pointer is null");
    return (u32)GET_REG_FIELD(t->mode0, 1, 21);
}

u8 GXGetTexObjEdgeLOD(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3DB, tex_obj, "Texture Object Pointer is null");
    return !GET_REG_FIELD(t->mode0, 1, 8);
}

enum _GXAnisotropy GXGetTexObjMaxAniso(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3E1, tex_obj, "Texture Object Pointer is null");
    return GET_REG_FIELD(t->mode0, 2, 19);
}

u32 GXGetTexObjTlut(GXTexObj *tex_obj)
{
    __GXTexObjInt *t = (__GXTexObjInt *)tex_obj;

    ASSERTMSGLINE(0x3E7, tex_obj, "Texture Object Pointer is null");
    return t->tlutName;
}

typedef struct __GXTlutObjInt_struct {
    // total size: 0xC
    u32 tlut; // offset 0x0, size 0x4
    u32 loadTlut0; // offset 0x4, size 0x4
    u16 numEntries; // offset 0x8, size 0x2
} __GXTlutObjInt;
typedef struct __GXTlutRegionInt_struct {
    // total size: 0x10
    u32 loadTlut1; // offset 0x0, size 0x4
    __GXTlutObjInt tlutObj; // offset 0x4, size 0xC
} __GXTlutRegionInt;

void GXLoadTexObjPreLoaded(GXTexObj *obj, GXTexRegion *region, GXTexMapID id)
{
    __GXTlutRegionInt *tlr;
    __GXTexObjInt *t = (__GXTexObjInt *)obj;
    __GXTexRegionInt *r = (__GXTexRegionInt *)region;

    ASSERTMSGLINE(0x3FE, obj, "Texture Object Pointer is null");
    ASSERTMSGLINE(0x3FE, region, "TexRegion Object Pointer is null");
    CHECK_GXBEGIN(0x400, "GXLoadTexObjPreLoaded");
    ASSERTMSGLINEV(0x401, id < 8, "%s: invalid texture map ID", "GXLoadTexObj");

    SET_REG_FIELD(0x403, t->mode0, 8, 24, GXTexMode0Ids[id]);
    SET_REG_FIELD(0x404, t->mode1, 8, 24, GXTexMode1Ids[id]);
    SET_REG_FIELD(0x405, t->image0, 8, 24, GXTexImage0Ids[id]);
    SET_REG_FIELD(0x406, r->image1, 8, 24, GXTexImage1Ids[id]);
    SET_REG_FIELD(0x407, r->image2, 8, 24, GXTexImage2Ids[id]);
    SET_REG_FIELD(0x408, t->image3, 8, 24, GXTexImage3Ids[id]);

    GX_WRITE_RAS_REG(t->mode0);
    GX_WRITE_RAS_REG(t->mode1);
    GX_WRITE_RAS_REG(t->image0);
    GX_WRITE_RAS_REG(r->image1);
    GX_WRITE_RAS_REG(r->image2);
    GX_WRITE_RAS_REG(t->image3);

    if (!(t->flags & 2)) {
        ASSERTMSGLINEV(0x413, gx->tlutRegionCallback, "%s: Tex/Tlut Region Callback not set", "GXLoadTexObj/Preloaded");
        tlr = (__GXTlutRegionInt *)gx->tlutRegionCallback(t->tlutName);
        ASSERTMSGLINEV(0x415, tlr, "%s: Tex/Tlut Region Callback returns NULL", "GXLoadTexObj/Preloaded");

        SET_REG_FIELD(0x417, tlr->tlutObj.tlut, 8, 24, GXTexTlutIds[id]);
        GX_WRITE_RAS_REG(tlr->tlutObj.tlut);
    }
    gx->tImage0[id] = t->image0;
    gx->tMode0[id] = t->mode0;
    gx->dirtyState |= 1;
    gx->bpSent = 1;
}

void GXLoadTexObj(GXTexObj *obj, GXTexMapID id)
{
    GXTexRegion *r;

    CHECK_GXBEGIN(0x432, "GXLoadTexObj");
    ASSERTMSGLINEV(0x433, id < 8, "%s: invalid texture map ID", "GXLoadTexObj");
    ASSERTMSGLINEV(0x438, gx->texRegionCallback, "%s: Tex/Tlut Region Callback not set", "GXLoadTexObj");
    r = gx->texRegionCallback(obj, id);
    ASSERTMSGLINEV(0x43A, r, "%s: Tex/Tlut Region Callback returns NULL", "GXLoadTexObj");
    GXLoadTexObjPreLoaded(obj, r, id);
}

void GXInitTlutObj(GXTlutObj *tlut_obj, void *lut, GXTlutFmt fmt, u16 n_entries)
{
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x452, tlut_obj, "Tlut Object Pointer is null");
    CHECK_GXBEGIN(0x453, "GXInitTlutObj");
    ASSERTMSGLINEV(0x456, n_entries <= 0x4000, "%s: number of entries exceeds maximum", "GXInitTlutObj");
    ASSERTMSGLINEV(0x458, ((u32)lut & 0x1F) == 0, "%s: %s pointer not aligned to 32B", "GXInitTlutObj", "Tlut");
    t->tlut = 0;
    SET_REG_FIELD(0x45B, t->tlut, 2, 10, fmt);
    SET_REG_FIELD(0x45C, t->loadTlut0, 21, 0, ((u32)lut & 0x3FFFFFFF) >> 5);
    SET_REG_FIELD(0x45D, t->loadTlut0, 8, 24, 0x64);
    t->numEntries = n_entries;
}

void GXGetTlutObjAll(GXTlutObj *tlut_obj, void **data, GXTlutFmt *format, u16 *numEntries)
{
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x472, tlut_obj, "Tlut Object Pointer is null");
    *data = (void *)(GET_REG_FIELD(t->loadTlut0, 21, 0) << 5);
    *format = GET_REG_FIELD(t->tlut, 2, 10);
    *numEntries = t->numEntries;
}

void *GXGetTlutObjData(GXTlutObj *tlut_obj)
{
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x47B, tlut_obj, "Tlut Object Pointer is null");
    return (void *)(GET_REG_FIELD(t->loadTlut0, 21, 0) << 5);
}

GXTlutFmt GXGetTlutObjFmt(GXTlutObj *tlut_obj)
{
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x482, tlut_obj, "Tlut Object Pointer is null");
    return GET_REG_FIELD(t->tlut, 2, 10);
}

u16 GXGetTlutObjNumEntries(GXTlutObj *tlut_obj)
{
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x489, tlut_obj, "Tlut Object Pointer is null");
    return t->numEntries;
}

void GXLoadTlut(GXTlutObj *tlut_obj, u32 tlut_name)
{
    __GXTlutRegionInt *r;
    u32 tlut_offset;
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x4A4, tlut_obj, "Tlut Object Pointer is null");
    CHECK_GXBEGIN(0x4A6, "GXLoadTlut");
    ASSERTMSGLINEV(0x4A7, gx->tlutRegionCallback, "%s: Tex/Tlut Region Callback not set", "GXLoadTlut");
    r = (__GXTlutRegionInt *)gx->tlutRegionCallback(tlut_name);
    ASSERTMSGLINEV(0x4A9, r, "%s: Tex/Tlut Region Callback returns NULL", "GXLoadTlut");
    __GXFlushTextureState();
    GX_WRITE_RAS_REG(t->loadTlut0);
    GX_WRITE_RAS_REG(r->loadTlut1);
    __GXFlushTextureState();
    tlut_offset = r->loadTlut1 & 0x3FF;
    SET_REG_FIELD(0x4B9, t->tlut, 10, 0, tlut_offset);
    r->tlutObj = *t;
}
