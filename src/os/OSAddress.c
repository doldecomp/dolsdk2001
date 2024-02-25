#include <dolphin.h>
#include <dolphin/os.h>

// undefine the macros so they do not error the file.
#undef OSPhysicalToCached
#undef OSPhysicalToUncached
#undef OSCachedToPhysical
#undef OSUncachedToPhysical
#undef OSCachedToUncached
#undef OSUncachedToCached

void * OSPhysicalToCached(unsigned long paddr) {
    ASSERTMSGLINE("OSAddress.c", 0x2C, paddr < 0x10000000U, "OSPhysicalToCached(): illegal address.");
    return (void*)(paddr + 0x80000000);
}

void * OSPhysicalToUncached(unsigned long paddr) {
    ASSERTMSGLINE("OSAddress.c", 0x3B, paddr < 0x10000000U, "OSPhysicalToUncached(): illegal address.");
    return (void*)(paddr - 0x40000000);
}

unsigned long OSCachedToPhysical(void * caddr) {
    ASSERTMSGLINE("OSAddress.c", 0x4A, 0x80000000U <= (u32)caddr && (u32)caddr < 0x90000000U, "OSCachedToPhysical(): illegal address.");
    return (u32)caddr + 0x80000000;
}

unsigned long OSUncachedToPhysical(void * ucaddr) {
    ASSERTMSGLINE("OSAddress.c", 0x59, 0xC0000000U <= (u32)ucaddr && (u32)ucaddr < 0xD0000000U, "OSUncachedToPhysical(): illegal address.");
    return (u32)ucaddr + 0x40000000;
}

void * OSCachedToUncached(void * caddr) {
    ASSERTMSGLINE("OSAddress.c", 0x68, 0x80000000U <= (u32)caddr && (u32)caddr < 0x90000000U, "OSCachedToUncached(): illegal address.");
    return (void*)((u32)caddr + 0x40000000);
}

void * OSUncachedToCached(void * ucaddr) {
    ASSERTMSGLINE("OSAddress.c", 0x77, 0xC0000000U <= (u32)ucaddr && (u32)ucaddr < 0xD0000000U, "OSUncachedToCached(): illegal address.");
    return (void*)((u32)ucaddr - 0x40000000);
}