#ifndef _DOLPHIN_DVD_INTERNAL_H_
#define _DOLPHIN_DVD_INTERNAL_H_

#include <dolphin/dvd.h>

// dvdfs.c
extern struct OSThreadQueue __DVDThreadQueue;
extern unsigned long __DVDLongFileNameFlag;

void __DVDFSInit();

// dvdlow.c
void __DVDInterruptHandler(int unused, struct OSContext * context);

// dvdqueue.c
void __DVDClearWaitingQueue();
int __DVDPushWaitingQueue(long prio, struct DVDCommandBlock * block);
struct DVDCommandBlock * __DVDPopWaitingQueue();
int __DVDCheckWaitingQueue();
int __DVDDequeueWaitingQueue(struct DVDCommandBlock * block);
int __DVDIsBlockInWaitingQueue(struct DVDCommandBlock * block);

#endif // _DOLPHIN_DVD_INTERNAL_H_
