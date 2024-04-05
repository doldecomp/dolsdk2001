#ifndef _DOLPHIN_AX_H_
#define _DOLPHIN_AX_H_

typedef struct _AXPBMIX {
    /* 0x00 */ u16 vL;
    /* 0x02 */ u16 vDeltaL;
    /* 0x04 */ u16 vR;
    /* 0x06 */ u16 vDeltaR;
    /* 0x08 */ u16 vAuxAL;
    /* 0x0A */ u16 vDeltaAuxAL;
    /* 0x0C */ u16 vAuxAR;
    /* 0x0E */ u16 vDeltaAuxAR;
    /* 0x10 */ u16 vAuxBL;
    /* 0x12 */ u16 vDeltaAuxBL;
    /* 0x14 */ u16 vAuxBR;
    /* 0x16 */ u16 vDeltaAuxBR;
    /* 0x18 */ u16 vAuxBS;
    /* 0x1A */ u16 vDeltaAuxBS;
    /* 0x1C */ u16 vS;
    /* 0x1E */ u16 vDeltaS;
    /* 0x20 */ u16 vAuxAS;
    /* 0x22 */ u16 vDeltaAuxAS;
} AXPBMIX;

typedef struct _AXPBITD {
    /* 0x00 */ u16 flag;
    /* 0x02 */ u16 bufferHi;
    /* 0x04 */ u16 bufferLo;
    /* 0x06 */ u16 shiftL;
    /* 0x08 */ u16 shiftR;
    /* 0x0A */ u16 targetShiftL;
    /* 0x0C */ u16 targetShiftR;
} AXPBITD;

typedef struct _AXPBUPDATE {
    /* 0x00 */ u16 updNum[5];
    /* 0x0A */ u16 dataHi;
    /* 0x0C */ u16 dataLo;
} AXPBUPDATE;

typedef struct _AXPBDPOP {
    /* 0x00 */ s16 aL;
    /* 0x02 */ s16 aAuxAL;
    /* 0x04 */ s16 aAuxBL;
    /* 0x06 */ s16 aR;
    /* 0x08 */ s16 aAuxAR;
    /* 0x0A */ s16 aAuxBR;
    /* 0x0C */ s16 aS;
    /* 0x0E */ s16 aAuxAS;
    /* 0x10 */ s16 aAuxBS;
} AXPBDPOP;

typedef struct _AXPBVE {
    /* 0x00 */ u16 currentVolume;
    /* 0x02 */ s16 currentDelta;
} AXPBVE;

typedef struct _AXPBFIR {
    /* 0x00 */ u16 numCoefs;
    /* 0x02 */ u16 coefsHi;
    /* 0x04 */ u16 coefsLo;
} AXPBFIR;

typedef struct _AXPBADDR {
    /* 0x00 */ u16 loopFlag;
    /* 0x02 */ u16 format;
    /* 0x04 */ u16 loopAddressHi;
    /* 0x06 */ u16 loopAddressLo;
    /* 0x08 */ u16 endAddressHi;
    /* 0x0A */ u16 endAddressLo;
    /* 0x0C */ u16 currentAddressHi;
    /* 0x0E */ u16 currentAddressLo;
} AXPBADDR;

typedef struct _AXPBADPCM {
    /* 0x00 */ u16 a[8][2];
    /* 0x20 */ u16 gain;
    /* 0x22 */ u16 pred_scale;
    /* 0x24 */ u16 yn1;
    /* 0x26 */ u16 yn2;
} AXPBADPCM;

typedef struct _AXPBSRC {
    /* 0x00 */ u16 ratioHi;
    /* 0x02 */ u16 ratioLo;
    /* 0x04 */ u16 currentAddressFrac;
    /* 0x06 */ u16 last_samples[4];
} AXPBSRC;

typedef struct _AXPBADPCMLOOP {
    /* 0x00 */ u16 loop_pred_scale;
    /* 0x02 */ u16 loop_yn1;
    /* 0x04 */ u16 loop_yn2;
} AXPBADPCMLOOP;

typedef struct _AXPB {
    /* 0x00 */ u16 nextHi;
    /* 0x02 */ u16 nextLo;
    /* 0x04 */ u16 currHi;
    /* 0x06 */ u16 currLo;
    /* 0x08 */ u16 srcSelect;
    /* 0x0A */ u16 coefSelect;
    /* 0x0C */ u16 mixerCtrl;
    /* 0x0E */ u16 state;
    /* 0x10 */ u16 type;
    /* 0x12 */ AXPBMIX mix;
    /* 0x36 */ AXPBITD itd;
    /* 0x44 */ AXPBUPDATE update;
    /* 0x52 */ AXPBDPOP dpop;
    /* 0x64 */ AXPBVE ve;
    /* 0x68 */ AXPBFIR fir;
    /* 0x6E */ AXPBADDR addr;
    /* 0x7E */ AXPBADPCM adpcm;
    /* 0xA6 */ AXPBSRC src;
    /* 0xB4 */ AXPBADPCMLOOP adpcmLoop;
    /* 0xBA */ u16 pad[3];
} AXPB;

typedef struct _AXVPB {
    /* 0x000 */ void * next;
    /* 0x004 */ void * prev;
    /* 0x008 */ void * next1;
    /* 0x00C */ u32 priority;
    /* 0x010 */ void (* callback)(void *);
    /* 0x014 */ u32 userContext;
    /* 0x018 */ u32 index;
    /* 0x01C */ u32 sync;
    /* 0x020 */ u32 depop;
    /* 0x024 */ u32 updateMS;
    /* 0x028 */ u32 updateCounter;
    /* 0x02C */ u32 updateTotal;
    /* 0x030 */ u16 * updateWrite;
    /* 0x034 */ u16 updateData[128];
    /* 0x134 */ void * itdBuffer;
    /* 0x138 */ AXPB pb;
} AXVPB;

// unsorted externs
extern void AXSetVoicePriority(struct _AXVPB * p /* r29 */, unsigned long priority /* r30 */);
extern void AXFreeVoice(struct _AXVPB * p /* r30 */);
extern AXVPB *AXAcquireVoice(u32, void *, struct SYNSYNTH *);

#endif // _DOLPHIN_AX_H_
