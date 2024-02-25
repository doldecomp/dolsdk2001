#include <dolphin.h>
#include <dolphin/os.h>

// internal function
extern __OSExceptionHandler __OSGetExceptionHandler(__OSException exception);

struct OSAlarmQueue {
    struct OSAlarm * head;
    struct OSAlarm * tail;
};

static struct OSAlarmQueue AlarmQueue;

// functions
int OSCheckAlarmQueue();
static void SetTimer(struct OSAlarm * alarm);
void OSInitAlarm(void);
void OSCreateAlarm(OSAlarm* alarm);
static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler);
void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler);
void OSSetAbsAlarm(struct OSAlarm * alarm, long long time, void (* handler)(struct OSAlarm *, struct OSContext *));
void OSSetPeriodicAlarm(OSAlarm* alarm, OSTime start, OSTime period, OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm* alarm);
static void DecrementerExceptionCallback(register __OSException exception, register OSContext* context);
static void DecrementerExceptionHandler(__OSException exception, OSContext* context);

#define ASSERTREPORT(file, line, cond) \
    if (!(cond)) { OSReport("OSCheckAlarmQueue: Failed " #cond " in %d", line); return 0; }

int OSCheckAlarmQueue() {
    struct OSAlarm * alarm;

    ASSERTREPORT("OSAlarm.c", 0x70, AlarmQueue.head == NULL && AlarmQueue.tail == NULL || AlarmQueue.head != NULL && AlarmQueue.tail != NULL);
    ASSERTREPORT("OSAlarm.c", 0x71, AlarmQueue.head == NULL || AlarmQueue.head->prev == NULL);
    ASSERTREPORT("OSAlarm.c", 0x72, AlarmQueue.tail == NULL || AlarmQueue.tail->next == NULL);

    for(alarm = AlarmQueue.head; alarm; alarm = alarm->next) {
        ASSERTREPORT("OSAlarm.c", 0x75, alarm->next == NULL || alarm->next->prev == alarm);
        ASSERTREPORT("OSAlarm.c", 0x76, alarm->next != NULL || AlarmQueue.tail == alarm);
    }
    return 1;
}

static void SetTimer(struct OSAlarm * alarm) {
    OSTime delta = alarm->fire - OSGetTime();

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
    OSTime time = OSGetTime();

    fire = alarm->start;
    if (alarm->start < time) {
      fire += alarm->period * ((time - alarm->start) / alarm->period + 1);
    }
  }

  ASSERTLINE("OSAlarm.c", 0xD6, alarm->handler == 0);

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
  ASSERTLINE("OSAlarm.c", 0xF3, next == 0);
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
    ASSERTMSGLINE("OSAlarm.c", 0x114, tick > 0, "OSSetAlarm(): tick was less than zero.");
    ASSERTMSGLINE("OSAlarm.c", 0x115, handler, "OSSetAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, OSGetTime() + tick, handler);
    ASSERTLINE("OSAlarm.c", 0x11C, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

void OSSetAbsAlarm(struct OSAlarm * alarm, long long time, void (* handler)(struct OSAlarm *, struct OSContext *)) {
    int enabled;

    ASSERTMSGLINE("OSAlarm.c", 0x130, handler, "OSSetAbsAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, time, handler);
    ASSERTLINE("OSAlarm.c", 0x137, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

void OSSetPeriodicAlarm(OSAlarm* alarm, OSTime start, OSTime period, OSAlarmHandler handler) {
    BOOL enabled;
    ASSERTMSGLINE("OSAlarm.c", 0x14D, period > 0, "OSSetPeriodicAlarm(): period was less than zero.");
    ASSERTMSGLINE("OSAlarm.c", 0x14E, handler, "OSSetPeriodicAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = period;
    alarm->start = start;
    InsertAlarm(alarm, 0, handler);
    ASSERTLINE("OSAlarm.c", 0x156, OSCheckAlarmQueue());
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
    ASSERTLINE("OSAlarm.c", 0x189, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

static void DecrementerExceptionCallback(register __OSException exception,
                                         register OSContext* context) {
    OSAlarm* alarm;
    OSAlarm* next;
    OSAlarmHandler handler;
    OSTime time;

    time = OSGetTime();
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
    ASSERTLINE("OSAlarm.c", 0x1C2, OSCheckAlarmQueue());
    handler = alarm->handler;
    alarm->handler = 0;
    if (0 < alarm->period) {
        InsertAlarm(alarm, 0, handler);
        ASSERTLINE("OSAlarm.c", 0x1CC, OSCheckAlarmQueue());
    }

    if (AlarmQueue.head) {
        SetTimer(AlarmQueue.head);
    }

    OSDisableScheduler();
    handler(alarm, context);
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
