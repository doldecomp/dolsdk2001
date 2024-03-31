#ifndef _DOLPHIN_SYN_INTERNAL_H_
#define _DOLPHIN_SYN_INTERNAL_H_

#include <dolphin/syn.h>

// unsorted externs
extern void __SYNRunVolumeEnvelope(struct SYNVOICE * voice);
extern void __SYNRunLfo(struct SYNVOICE * voice);
extern void __SYNRunPitchEnvelope(struct SYNVOICE * voice);
extern void __SYNUpdateMix(struct SYNVOICE * voice);
extern unsigned long __SYNGetNibbleAddress(unsigned long count);

// synpitch.c
float __SYNGetRelativePitch(struct SYNVOICE * voice);
void __SYNSetupPitch(struct SYNVOICE * voice);
void __SYNSetupSrc(struct SYNVOICE * voice);
void __SYNUpdateSrc(struct SYNVOICE * voice);

// synsample.c
void __SYNSetupSample(struct SYNVOICE * voice);

// synvoice.c
void __SYNClearVoiceReferences(void * p);
void __SYNSetVoiceToRelease(struct SYNVOICE * voice, unsigned long priority);
void __SYNServiceVoice(int i);

// synwt.c
int __SYNGetWavetableData(struct SYNVOICE * voice);

#endif // _DOLPHIN_SYN_INTERNAL_H_
