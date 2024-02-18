#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/dvd.h>

unsigned long OSGetPhysicalMemSize() {
#if DEBUG
    OSBootInfo * BootInfo = (OSBootInfo *)OSPhysicalToCached(0);

    return BootInfo->memorySize;
#else
    return __OSPhysicalMemSize;
#endif
}

unsigned long OSGetConsoleSimulatedMemSize() {
#if DEBUG
    unsigned long * memSize = (unsigned long *)OSPhysicalToCached(0xF0);

    return *memSize;
#else
    return __OSSimulatedMemSize;
#endif
}
