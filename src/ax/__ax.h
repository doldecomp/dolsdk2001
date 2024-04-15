#ifndef _DOLPHIN_AX_INTERNAL_H_
#define _DOLPHIN_AX_INTERNAL_H_

#include <dolphin/ax.h>

// unsorted externs
extern unsigned long __AXClMode;

extern AXVPB *__AXGetStackHead(u32);
extern void __AXPushCallbackStack(AXVPB *);
extern u32 __AXGetCommandListCycles(void);
extern void __AXPushFreeStack(AXVPB *);

// AXSPB.c
u32 __AXGetStudio(void);
void __AXDepopFade(long * hostSum, long * dspVolume, s16 * dspDelta);
void __AXPrintStudio(void);
void __AXSPBInit(void);
void __AXSPBQuit(void);
void __AXDepopVoice(AXPB * p);

// AXVPB.c
u32 __AXGetNumVoices(void);
void __AXServiceVPB(AXVPB *pvpb);
void __AXDumpVPB(AXVPB * pvpb);
void __AXSyncPBs(u32 lessDspCycles);
AXPB * __AXGetPBs(void);
void __AXSetPBDefault(AXVPB * p);
void __AXVPBInit(void);
void __AXVPBQuit(void);

#endif // _DOLPHIN_AX_INTERNAL_H_
