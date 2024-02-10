#include <dolphin.h>
#include <dolphin/os.h>

long long __get_clock() {
    return -1;
}

unsigned long __get_time(void) {
    return (u32)(OSGetTime() / (*(u32* )0x800000F8 >> 2U)) - 0x43E83E00;
}

int __to_gm_time() {
    return 0;
}
