#include <dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

long long __get_clock(void) {
#if DOLPHIN_REVISION >= 45
    return __OSGetSystemTime();
#else
    return -1;
#endif
}

unsigned long __get_time(void) {
    return (u32)OSTicksToSeconds(OSGetTime()) - 0x43E83E00;
}

int __to_gm_time() {
    return 0;
}
