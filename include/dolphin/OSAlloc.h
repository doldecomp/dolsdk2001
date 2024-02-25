#ifndef _DOLPHIN_OSALLOC_H_
#define _DOLPHIN_OSALLOC_H_

#include <dolphin/types.h>

#ifdef DEBUG
#define ENABLE_HEAPDESC
#endif

struct Cell {
    struct Cell * prev;
    struct Cell * next;
    long size;
#ifdef ENABLE_HEAPDESC
    struct HeapDesc * hd;
    long requested;
#endif
};

struct HeapDesc {
    long size;
    struct Cell * free;
    struct Cell * allocated;
#ifdef ENABLE_HEAPDESC
    unsigned long paddingBytes;
    unsigned long headerBytes;
    unsigned long payloadBytes;
#endif
};

typedef int OSHeapHandle;

extern volatile OSHeapHandle __OSCurrHeap;

void * OSAllocFromHeap(int heap, unsigned long size);
void * OSAllocFixed(void * rstart, void * rend);
void OSFreeToHeap(int heap, void * ptr);
int OSSetCurrentHeap(int heap);
void * OSInitAlloc(void * arenaStart, void * arenaEnd, int maxHeaps);
int OSCreateHeap(void * start, void * end);
void OSDestroyHeap(int heap);
void OSAddToHeap(int heap, void * start, void * end);
long OSCheckHeap(int heap);
unsigned long OSReferentSize(void * ptr);
void OSDumpHeap(int heap);
void OSVisitAllocated(void (* visitor)(void *, unsigned long));

#define OSAlloc(size) OSAllocFromHeap(__OSCurrHeap, (size))
#define OSFree(ptr)   OSFreeToHeap(__OSCurrHeap, (ptr))

#define ALIGNMENT 32

#define InRange(cell, arenaStart, arenaEnd) \
    ((u32) arenaStart <= (u32) cell) && ((u32) cell < (u32) arenaEnd)

#define HEADERSIZE 32u
#define MINOBJSIZE 64u

#endif
