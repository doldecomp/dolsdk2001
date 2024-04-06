#ifndef _DOLPHIN_MTX_H_
#define _DOLPHIN_MTX_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	f32 x, y, z;
} Vec, *VecPtr, Point3d, *Point3dPtr;

typedef struct
{
    s16 x, y, z;
} S16Vec, *S16VecPtr;

typedef struct
{
	f32 x, y, z, w;
} Quaternion, *QuaternionPtr, Qtrn, *QtrnPtr;

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef f32 ROMtx[4][3];

void MTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);

void C_MTXFrustum(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f);
void C_MTXLookAt(Mtx m, Point3dPtr camPos, VecPtr camUp, Point3dPtr target);
void C_MTXIdentity(Mtx m);

void PSMTXIdentity(Mtx m);

#define MTXFrustum     C_MTXFrustum
#define MTXPerspective C_MTXPerspective 
#define MTXLookAt      C_MTXLookAt

#ifdef DEBUG
#define PSMTXIdentity  C_MTXIdentity
#endif

#ifdef DEBUG
#define VECSquareMag C_VECSquareMag
#define VECNormalize C_VECNormalize
#define VECAdd C_VECAdd
#define VECDotProduct C_VECDotProduct
#define VECSquareDistance C_VECSquareDistance
#define MTXMultVec C_MTXMultVec
#define MTXMultVecArray C_MTXMultVecArray
#else
#define VECSquareMag PSVECSquareMag
#define VECNormalize PSVECNormalize
#define VECAdd PSVECAdd
#define VECDotProduct PSVECDotProduct
#define VECSquareDistance PSVECSquareDistance
#define MTXMultVec PSMTXMultVec
#define MTXMultVecArray PSMTXMultVecArray
#endif

// asm only
#define MTXReorder PSMTXReorder
#define MTXROMultVecArray PSMTXROMultVecArray
#define MTXROSkin2VecArray PSMTXROSkin2VecArray
#define MTXROMultS16VecArray PSMTXROMultS16VecArray
#define MTXMultS16VecArray PSMTXMultS16VecArray

#ifdef __cplusplus
}
#endif

#endif
