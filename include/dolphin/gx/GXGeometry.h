#ifndef _DOLPHIN_GX_GXGEOMETRY_H_
#define _DOLPHIN_GX_GXGEOMETRY_H_

#include <dolphin/gx/GXEnum.h>
#include <dolphin/os.h>
#include <macros.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXSetVtxDesc(GXAttr attr, GXAttrType type);
void GXSetVtxDescv(GXVtxDescList * attrPtr);
void GXGetVtxDesc(GXAttr attr, GXAttrType *type);
void GXGetVtxDescv(GXVtxDescList *vcd);;
void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt,
    GXCompType type, u8 frac);
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList *list);
void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt *cnt, GXCompType *type, u8 *frac);
void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList *vat);
void GXInvalidateVtxCache(void);
void GXSetNumTexGens(u8 nTexGens);
void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts);
void GXSetTexCoordGen2(GXTexCoordID dst_coord, GXTexGenType func,
    GXTexGenSrc src_param, u32 mtx, GXBool normalize, u32 postmtx);
void GXSetLineWidth(u8 width, GXTexOffset texOffsets);
void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets);
void GXEnableTexOffsets(GXTexCoordID coord, GXBool line_enable, GXBool point_enable);

static inline void GXSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func,
    GXTexGenSrc src_param, u32 mtx)
{
    GXSetTexCoordGen2(dst_coord, func, src_param, mtx, GX_FALSE, GX_PTIDENTITY);
}

static inline void GXEnd(void)
{
#if DEBUG
    extern u8 __GXinBegin;
    ASSERTMSGLINE(0x6D, __GXinBegin, "GXEnd: called without a GXBegin");
    __GXinBegin = 0;
#endif
}

#ifdef __cplusplus
}
#endif

#endif
