#include <dolphin.h>
#include <dolphin/os.h>

static void (* ResetCallback)();
static int Down;
#if DOLPHIN_REVISION >= 37
static int LastState;
static long long HoldUp;
static long long HoldDown;
#define Hold HoldUp  // Hold was renamed to HoldUp
#else
static long long Hold;
#endif

void __OSResetSWInterruptHandler(short exception, struct OSContext *context) {
    void (* callback)();

#if DOLPHIN_REVISION >= 37
    HoldDown = __OSGetSystemTime();  // return value unused?
    do {
        if (__OSGetSystemTime() - HoldDown >= OSMicrosecondsToTicks(100))
            break;
    } while (!(__PIRegs[0] & 0x10000));

    if (!(__PIRegs[0] & 0x10000)) {
        Down = 1;
        LastState = 1;
        __OSMaskInterrupts(0x200);
        if (ResetCallback != NULL) {
            callback = ResetCallback;
            ResetCallback = NULL;
            callback();
        }
    }
    __PIRegs[0] = 2;
#else

    Down = 1;
    __PIRegs[0] = 2;
    __OSMaskInterrupts(0x200);

    if (ResetCallback) {
        callback = ResetCallback;
        ResetCallback = NULL;
        callback();
    }
#endif
}

void (* OSSetResetCallback(void (* callback)()))() {
    int enabled;
    void (* prevCallback)();

    enabled = OSDisableInterrupts();
    prevCallback = ResetCallback;
    ResetCallback = callback;

    if (callback) {
        __PIRegs[0] = 2;
        __OSUnmaskInterrupts(0x200);
    } else {
        __OSMaskInterrupts(0x200);
    }
    OSRestoreInterrupts(enabled);
    return prevCallback;
}

int OSGetResetSwitchState() {
    int enabled;
    int state;
    unsigned long reg;

    enabled = OSDisableInterrupts();
    reg = __PIRegs[0];

    if (!(reg & 0x10000)) {
        Down = 1;
        state = 1;
    } else if (Down != 0) {
        if (reg & 2) {
            __PIRegs[0] = 2;
            Down = 1;
        } else {
            Down = 0;
            Hold = __OSGetSystemTime();
        }
        state = 1;
    } else if (Hold && (__OSGetSystemTime() - Hold) < OSMillisecondsToTicks(50)) {
        state = 1;
    } else {
        state = 0;
        Hold = 0;
    }
    OSRestoreInterrupts(enabled);
    return state;
}
