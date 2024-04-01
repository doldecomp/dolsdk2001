#ifndef _DOLPHIN_MIX_H_
#define _DOLPHIN_MIX_H_

// unsorted externs
extern void MIXSetInput(struct _AXVPB * p /* r3 */, int dB /* r4 */);
extern void MIXSetAuxA(struct _AXVPB * p /* r3 */, int dB /* r4 */);
extern void MIXSetAuxB(struct _AXVPB * p /* r3 */, int dB /* r4 */);
extern void MIXSetFader(struct _AXVPB * p /* r3 */, int dB /* r4 */);
extern void MIXSetPan(struct _AXVPB * p /* r1+0x8 */, int pan /* r1+0xC */);
extern void MIXReleaseChannel(struct _AXVPB * axvpb /* r31 */);

#endif // _DOLPHIN_MIX_H_
