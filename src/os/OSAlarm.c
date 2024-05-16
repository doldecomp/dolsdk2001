#include <dolphin.h>
#include <dolphin/os.h>

// internal header
#include "__os.h"

struct OSAlarmQueue {
    struct OSAlarm * head;
    struct OSAlarm * tail;
};

static struct OSAlarmQueue AlarmQueue;

// functions
static void SetTimer(struct OSAlarm * alarm);
static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler);
static void DecrementerExceptionCallback(register __OSException exception, register OSContext* context);
static void DecrementerExceptionHandler(__OSException exception, OSContext* context);

#define ASSERTREPORT(line, cond) \
    if (!(cond)) { OSReport("OSCheckAlarmQueue: Failed " #cond " in %d", line); return 0; }

#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 7 : 0)

BOOL OSCheckAlarmQueue(void) {
    struct OSAlarm * alarm;

    ASSERTREPORT(0x70+LINE_OFFSET, AlarmQueue.head == NULL && AlarmQueue.tail == NULL || AlarmQueue.head != NULL && AlarmQueue.tail != NULL);
    ASSERTREPORT(0x71+LINE_OFFSET, AlarmQueue.head == NULL || AlarmQueue.head->prev == NULL);
    ASSERTREPORT(0x72+LINE_OFFSET, AlarmQueue.tail == NULL || AlarmQueue.tail->next == NULL);

    for(alarm = AlarmQueue.head; alarm; alarm = alarm->next) {
        ASSERTREPORT(0x75+LINE_OFFSET, alarm->next == NULL || alarm->next->prev == alarm);
        ASSERTREPORT(0x76+LINE_OFFSET, alarm->next != NULL || AlarmQueue.tail == alarm);
    }
    return TRUE;
}

static void SetTimer(struct OSAlarm * alarm) {
#if DOLPHIN_REVISION >= 45
    OSTime delta = alarm->fire - __OSGetSystemTime();
#else
    OSTime delta = alarm->fire - OSGetTime();
#endif

    if (delta < 0) {
        PPCMtdec(0);
    } else if (delta < 0x80000000) {
        PPCMtdec((u32)delta);
    } else {
        PPCMtdec(0x7fffffff);
    }
}

void OSInitAlarm(void) {
    if (__OSGetExceptionHandler(8) != DecrementerExceptionHandler) {
        AlarmQueue.head = AlarmQueue.tail = NULL;
        __OSSetExceptionHandler(8, DecrementerExceptionHandler);
    }
}

void OSCreateAlarm(OSAlarm* alarm) {
    alarm->handler = 0;
}

static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler) {
    OSAlarm* next;
    OSAlarm* prev;
    
    if (0 < alarm->period) {
#if DOLPHIN_REVISION >= 45
        OSTime time = __OSGetSystemTime();
#else
        OSTime time = OSGetTime();
#endif

        fire = alarm->start;
        if (alarm->start < time) {
            fire += alarm->period * ((time - alarm->start) / alarm->period + 1);
        }
    }
    
    ASSERTLINE(0xD6+LINE_OFFSET, alarm->handler == 0);
    
    alarm->handler = handler;
    alarm->fire = fire;
    
    for (next = AlarmQueue.head; next; next = next->next) {
        if (next->fire <= fire) {
            continue;
        }
        
        alarm->prev = next->prev;
        next->prev = alarm;
        alarm->next = next;
        prev = alarm->prev;

        if (prev) {
            prev->next = alarm;
        } else {
            AlarmQueue.head = alarm;
            SetTimer(alarm);
        }

        return;
    }

    ASSERTLINE(0xF3+LINE_OFFSET, next == 0);

    alarm->next = 0;
    prev = AlarmQueue.tail;
    AlarmQueue.tail = alarm;
    alarm->prev = prev;

    if (prev) {
        prev->next = alarm;
    } else {
        AlarmQueue.head = AlarmQueue.tail = alarm;
        SetTimer(alarm);
    }
}

void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler) {
    BOOL enabled;
    ASSERTMSGLINE(0x114+LINE_OFFSET, tick > 0, "OSSetAlarm(): tick was less than zero.");
    ASSERTMSGLINE(0x115+LINE_OFFSET, handler, "OSSetAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = 0;
#if DOLPHIN_REVISION >= 45
    InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
#else
    InsertAlarm(alarm, OSGetTime() + tick, handler);
#endif
    ASSERTLINE(0x11C+LINE_OFFSET, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 9 : 0)

void OSSetAbsAlarm(struct OSAlarm * alarm, long long time, void (* handler)(struct OSAlarm *, struct OSContext *)) {
    int enabled;

    ASSERTMSGLINE(0x130+LINE_OFFSET, handler, "OSSetAbsAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = 0;
#if DOLPHIN_REVISION >= 45
    InsertAlarm(alarm, __OSTimeToSystemTime(time), handler);
#else
    InsertAlarm(alarm, time, handler);
#endif
    ASSERTLINE(0x137+LINE_OFFSET, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 11 : 0)

void OSSetPeriodicAlarm(OSAlarm* alarm, OSTime start, OSTime period, OSAlarmHandler handler) {
    BOOL enabled;
    ASSERTMSGLINE(0x14D+LINE_OFFSET, period > 0, "OSSetPeriodicAlarm(): period was less than zero.");
    ASSERTMSGLINE(0x14E+LINE_OFFSET, handler, "OSSetPeriodicAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = period;
#if DOLPHIN_REVISION >= 45
    alarm->start = __OSTimeToSystemTime(start);
#else
    alarm->start = start;
#endif
    InsertAlarm(alarm, 0, handler);
    ASSERTLINE(0x156+LINE_OFFSET, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

void OSCancelAlarm(OSAlarm* alarm) {
    OSAlarm* next;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (alarm->handler == 0) {
        OSRestoreInterrupts(enabled);
        return;
    }

    next = alarm->next;
    if (next == 0) {
        AlarmQueue.tail = alarm->prev;
    } else {
        next->prev = alarm->prev;
    }
    if (alarm->prev) {
        alarm->prev->next = next;
    } else {
        AlarmQueue.head = next;
        if (next) {
            SetTimer(next);
        }
    }
    alarm->handler = 0;
    ASSERTLINE(0x189+LINE_OFFSET, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 12 : 0)

static void DecrementerExceptionCallback(register __OSException exception,
                                         register OSContext* context) {
    OSAlarm* alarm;
    OSAlarm* next;
    OSAlarmHandler handler;
    OSTime time;
#if DOLPHIN_REVISION >= 45
    OSContext exceptionContext;
#endif

#if DOLPHIN_REVISION >= 45
    time = __OSGetSystemTime();
#else
    time = OSGetTime();
#endif
    alarm = AlarmQueue.head;
    if (alarm == 0) {
        OSLoadContext(context);
    }

    if (time < alarm->fire) {
        SetTimer(alarm);
        OSLoadContext(context);
    }

    next = alarm->next;
    AlarmQueue.head = next;
    if (next == 0) {
        AlarmQueue.tail = 0;
    } else {
        next->prev = 0;
    }
    ASSERTLINE(0x1C2+LINE_OFFSET, OSCheckAlarmQueue());
    handler = alarm->handler;
    alarm->handler = 0;
    if (0 < alarm->period) {
        InsertAlarm(alarm, 0, handler);
        ASSERTLINE(0x1CC+LINE_OFFSET, OSCheckAlarmQueue());
    }

    if (AlarmQueue.head) {
        SetTimer(AlarmQueue.head);
    }

    OSDisableScheduler();
#if DOLPHIN_REVISION >= 45
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
#endif
    handler(alarm, context);
#if DOLPHIN_REVISION >= 45
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
#endif
    OSEnableScheduler();
    __OSReschedule();
    OSLoadContext(context);
}

static asm void DecrementerExceptionHandler(register __OSException exception,
                                            register OSContext* context) {
    nofralloc 
    OS_EXCEPTION_SAVE_GPRS(context)
    b DecrementerExceptionCallback
}
