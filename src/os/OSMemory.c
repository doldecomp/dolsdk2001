#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/dvd.h>
#include "__os.h"

#if DOLPHIN_REVISION >= 45
static BOOL OnReset(BOOL final);

static OSResetFunctionInfo ResetFunctionInfo = {
    OnReset,
    127,
    NULL,
    NULL
};
#endif

u32 OSGetPhysicalMemSize(void) {
#if DEBUG
    OSBootInfo * BootInfo = (OSBootInfo *)OSPhysicalToCached(0);

    return BootInfo->memorySize;
#else
    return __OSPhysicalMemSize;
#endif
}

#if DOLPHIN_REVISION >= 45  // this got moved
u32 OSGetConsoleSimulatedMemSize(void) {
#if DEBUG
    unsigned long * memSize = (unsigned long *)OSPhysicalToCached(0xF0);

    return *memSize;
#else
    return __OSSimulatedMemSize;
#endif
}
#endif

#if DOLPHIN_REVISION >= 45
static BOOL OnReset(BOOL final)
{
    if (final) {
        __MEMRegs[8] = 0xFF;
        __OSMaskInterrupts(0xF0000000);
    }
    return TRUE;
}

static void MEMIntrruptHandler(__OSInterrupt interrupt, OSContext *context)
{
    u32 addr;
    u32 cause;

    cause = __MEMRegs[15];
    addr = ((__MEMRegs[18] << 16) & 0x3FF0000) | __MEMRegs[17];
    __MEMRegs[16] = 0;
    __OSUnhandledException(15, context, cause, addr);
}

#define OS_PROTECT_CONTROL_RDWR 3

void OSProtectRange(u32 chan, void *addr, u32 nBytes, u32 control)
{
    BOOL enabled;
    u32 start;
    u32 end;
    u16 reg;

    ASSERTLINE(0xA0, chan < 4);
    ASSERTLINE(0xA1, (control & ~(OS_PROTECT_CONTROL_RDWR)) == 0);
    if (chan >= 4) {
        return;
    }
    control &= 3;
    end = (u32)addr + nBytes;
    start = (u32)addr & ~0x3FF;
    end = (end + 0x3FF) & ~0x3FF;
    DCFlushRange((void *)start, end - start);
    enabled = OSDisableInterrupts();
    __OSMaskInterrupts(OS_INTERRUPTMASK(chan));
    __MEMRegs[chan * 2 + 0] = start >> 10;
    __MEMRegs[chan * 2 + 1] = end >> 10;
    reg = __MEMRegs[8];
    reg &= ~(3 << (chan * 2));
    reg |= control << (chan * 2);
    __MEMRegs[8] = reg;
    if (control != 3) {
        __OSUnmaskInterrupts(OS_INTERRUPTMASK(chan));
    }
    OSRestoreInterrupts(enabled);

}
#endif

#if DOLPHIN_REVISION < 45  // this got moved
u32 OSGetConsoleSimulatedMemSize(void) {
#if DEBUG
    unsigned long * memSize = (unsigned long *)OSPhysicalToCached(0xF0);

    return *memSize;
#else
    return __OSSimulatedMemSize;
#endif
}
#endif

#if DOLPHIN_REVISION >= 45
void __OSInitMemoryProtection(void)
{
    __MEMRegs[16] = 0;
    __MEMRegs[8] = 0xFF;
    __OSMaskInterrupts(0xF0000000);
    __OSSetInterruptHandler(0, MEMIntrruptHandler);
    __OSSetInterruptHandler(1, MEMIntrruptHandler);
    __OSSetInterruptHandler(2, MEMIntrruptHandler);
    __OSSetInterruptHandler(3, MEMIntrruptHandler);
    __OSSetInterruptHandler(4, MEMIntrruptHandler);
    OSRegisterResetFunction(&ResetFunctionInfo);

#if DEBUG
    if (OSGetConsoleSimulatedMemSize() < OSGetPhysicalMemSize()
     && OSGetConsoleSimulatedMemSize() - 0x1800000 == 0) {
         __MEMRegs[20] = 2;
    }
#else
{
    u32 sim = OSGetConsoleSimulatedMemSize();
    u32 phys = OSGetPhysicalMemSize();
    u8 unused[20];
    if (sim < phys
     && OSGetConsoleSimulatedMemSize() - 0x1800000 == 0) {
         __MEMRegs[20] = 2;
    }
}
#endif
    __OSUnmaskInterrupts(0x8000000);
}
#endif
