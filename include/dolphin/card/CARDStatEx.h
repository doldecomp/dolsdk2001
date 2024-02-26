#ifndef _DOLPHIN_CARDSTATEX_H_
#define _DOLPHIN_CARDSTATEX_H_

// functions
long __CARDGetStatusEx(long chan, long fileNo, struct CARDDir * dirent);
long __CARDSetStatusExAsync(long chan, long fileNo, struct CARDDir * dirent, void (* callback)(long, long));
long __CARDSetStatusEx(long chan, long fileNo, struct CARDDir * dirent);

#endif // _DOLPHIN_CARDSTATEX_H_
