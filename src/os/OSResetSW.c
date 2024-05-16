#if DOLPHIN_REVISION >= 45
#include <stddef.h>
#endif
#include <dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

static OSResetCallback ResetCallback;
static int Down;
#if DOLPHIN_REVISION >= 37
static int LastState;
static long long HoldUp;
static long long HoldDown;
#else
static long long Hold;
#endif

void __OSResetSWInterruptHandler(short exception, struct OSContext *context) {
    OSResetCallback callback;

#if DOLPHIN_REVISION >= 37
    HoldDown = __OSGetSystemTime();
    do {
        if (__OSGetSystemTime() - HoldDown >= OSMicrosecondsToTicks(100))
            break;
    } while (!(__PIRegs[0] & 0x10000));

    if (!(__PIRegs[0] & 0x10000)) {
#if DOLPHIN_REVISION >= 45
        LastState = Down = 1;
#else
        Down = 1;
        LastState = 1;
#endif
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

OSResetCallback OSSetResetCallback(OSResetCallback callback) {
    int enabled;
    OSResetCallback prevCallback;

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

#if DOLPHIN_REVISION >= 45
int OSGetResetButtonState(void)
{
    BOOL enabled;
    int state;
    u32 reg;
    OSTime now;
    OSTime fire;

    enabled = OSDisableInterrupts();
    now = __OSGetSystemTime();
    ASSERTLINE(0x97, 0 <= now);
    ASSERTLINE(0x98, HoldUp == 0 || HoldUp < now);
    ASSERTLINE(0x99, HoldDown == 0 || HoldDown < now);
    reg = __PIRegs[0];
    if (!(reg & 0x10000)) {
        if (Down == 0) {
            Down = 1;
            state = HoldUp ? 1 : 0;
            HoldDown = now;
        }
        else {
            state = (HoldUp || OSMicrosecondsToTicks(100) < (now - HoldDown)) ? 1 : 0;
        }
    } else if (Down) {
        Down = 0;
        state = LastState;
        if (state != 0) {
            HoldUp = now;
        } else {
            HoldUp = 0;
        }
    } else if (HoldUp && (now - HoldUp) < OSMillisecondsToTicks(40)) {
        state = 1;
    } else {
        state = 0;
        HoldUp = 0;
    }
    LastState = state;
    if ((state == 0) && (UNK_800030E3 & 0x3F)) {
        fire = (UNK_800030E3 & 0x3F) * 60;
        fire = __OSStartTime + OSSecondsToTicks(fire);
        if (fire < now) {
            now -= fire;
            now = OSTicksToSeconds(now) / 2;
            if ((now & 1) == 0 || Down) {
                state = 1;
            }
        }
    }
    OSRestoreInterrupts(enabled);
    return state;
}
#endif

int OSGetResetSwitchState() {
#if DOLPHIN_REVISION >= 45
    return OSGetResetButtonState();
#else
    int enabled;
    int state;
    unsigned long reg;

    enabled = OSDisableInterrupts();
    reg = __PIRegs[0];

#if DOLPHIN_REVISION >= 37
    if (!(reg & 0x10000)) {
        if (Down == 0) {
            Down = 1;
            state = (HoldUp != 0) ? 1 : 0;
            HoldDown = __OSGetSystemTime();
        }
        else {
            state = (HoldUp != 0 || (__OSGetSystemTime() - HoldDown) > OSMicrosecondsToTicks(100)) ? 1 : 0;
        }
    } else if (Down != 0) {
        Down = 0;
        state = LastState;
        if (LastState != 0)
            HoldUp = __OSGetSystemTime();
    } else if (HoldUp && (__OSGetSystemTime() - HoldUp) < OSMillisecondsToTicks(40)) {
        state = 1;
    } else {
        state = 0;
        HoldUp = 0;
    }
    LastState = state;
#else
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
#endif
    OSRestoreInterrupts(enabled);
    return state;
#endif
}

#if DOLPHIN_REVISION >= 45
void __OSSetResetButtonTimer(u8 min)
{
    BOOL enabled;

    enabled = OSDisableInterrupts();
    if (min > 63) {
        min = 63;
    }
    UNK_800030E3 &= 0xC0;
    UNK_800030E3 |= min;
    OSRestoreInterrupts(enabled);
}
#endif
