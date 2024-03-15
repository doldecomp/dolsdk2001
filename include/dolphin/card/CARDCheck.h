#ifndef _DOLPHIN_CARDCHECK_H_
#define _DOLPHIN_CARDCHECK_H_

#if DOLPHIN_REVISION >= 37
s32 CARDCheckExAsync(s32 chan, s32 *xferBytes, CARDCallback callback);
#endif
s32 CARDCheckAsync(s32 chan, CARDCallback callback);
long CARDCheck(long chan);

#endif // _DOLPHIN_CARDCHECK_H_
