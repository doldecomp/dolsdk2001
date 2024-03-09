#include <dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

static asm void SystemCallVector(void) {
__OSSystemCallVectorStart:
    nofralloc
    mfspr r9, HID0
    ori r10, r9, 0x8
    mtspr HID0, r10
    isync
    sync
    mtspr HID0, r9
    rfi
__OSSystemCallVectorEnd:
    nop
}

void __OSInitSystemCall(void) {
    void *addr = (void*)OSPhysicalToCached(0xC00);

    memcpy(addr, (void*)&__OSSystemCallVectorStart, ((char*)&__OSSystemCallVectorEnd - (char*)&__OSSystemCallVectorStart));
    DCFlushRangeNoSync(addr, 0x100);
    __sync();
    ICInvalidateRange(addr, 0x100);
}
