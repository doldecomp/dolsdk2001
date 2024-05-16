#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/si.h>
#include "__os.h"

static struct SIControl Si = {
    /* chan */      -1,
    /* poll */       0,
    /* inputBytes*/  0,
    /* input */      NULL,
    /* callback */   NULL
};

#define SI_MAX_TYPE 4

static struct SIPacket Packet[4];
static struct OSAlarm Alarm[4];
#if DOLPHIN_REVISION >= 45
static u32 Type[4] = {8, 8, 8, 8}; // size: 0x10, address: 0x14
static OSTime TypeTime[4]; // size: 0x20, address: 0x120
static OSTime XferTime[4]; // size: 0x20, address: 0x140
static void (*TypeCallback[4][4])(s32, u32); // size: 0x40, address: 0x160
static void (*RDSTHandler[4])(s16, OSContext *); // size: 0x10, address: 0x1A0
static int InputBufferValid[4]; // size: 0x10, address: 0x1B0
static u32 InputBuffer[4][2]; // size: 0x20, address: 0x1C0
u32 __PADFixBits;
long long SIDelay;
#endif

static unsigned long CompleteTransfer();
static void SITransferNext(long chan);
static void SIIntrruptHandler(short unused, struct OSContext * context);
static int __SITransfer(long chan, void * output, unsigned long outputBytes, void * input, unsigned long inputBytes, void (* callback)(long, unsigned long, struct OSContext *));
static void AlarmHandler(struct OSAlarm * alarm, struct OSContext * context);
#if DOLPHIN_REVISION >= 45
static void SIGetResponseRaw(s32 chan);
static void GetTypeCallback(s32 chan, u32 error);
unsigned long SIGetType(s32 chan);
#endif

int SIBusy() {
    return (Si.chan != -1) ? 1 : 0;
}

#if DOLPHIN_REVISION >= 45
int SIIsChanBusy(s32 chan)
{
    return (Packet[chan].chan != -1 || Si.chan == chan);
}

static void SIClearTCInterrupt(void)
{
    unsigned long reg;

    reg = __SIRegs[SI_COMCSR_IDX];
    reg |= SI_COMCSR_TCINT_MASK;
    reg &= ~1;
    __SIRegs[SI_COMCSR_IDX] = reg;
}
#endif

static unsigned long CompleteTransfer() {
    unsigned long sr;
    unsigned long i;
    unsigned long rLen;
    unsigned char * input;
    unsigned long temp;

    sr = __SIRegs[SI_STATUS_IDX];
#if DOLPHIN_REVISION >= 45
    SIClearTCInterrupt();
#else
    __SIRegs[SI_COMCSR_IDX] = SI_COMCSR_TCINT_MASK;
#endif

    if (Si.chan != -1) {
#if DOLPHIN_REVISION >= 45
        XferTime[Si.chan] = __OSGetSystemTime();
#endif
        input = Si.input;
        rLen = (Si.inputBytes / 4);
        for(i = 0; i < rLen; i++) {
            *((u32*)input)++ = __SIRegs[i+0x20];
        }

        rLen = Si.inputBytes & 3;
        if (rLen != 0) {
            temp = __SIRegs[i + 32];
            for(i = 0; i < rLen; i++) {
                *(input++) = temp >> ((3 - i) * 8);
            }
        }
#if DOLPHIN_REVISION >= 45
        if ((__SIRegs[13] & 0x20000000) != 0) {
#endif
            sr >>= ((3 - Si.chan) * 8);
            sr &= 0xF;
#if DOLPHIN_REVISION >= 45
            if ((sr & 8) != 0 && (Type[Si.chan] & 0x80) == 0) {
                Type[Si.chan] = 8;
            }
            if (sr == 0) {
                sr = 4;
            }
        } else {
            TypeTime[Si.chan] = __OSGetSystemTime();
            sr = 0;
        }
#endif
        Si.chan = -1;
    }

    return sr;
}

static void SITransferNext(long chan) {
    int i;
    struct SIPacket * packet;

    for(i = 0; i < 4; i++) {
        chan++;
        chan %= 4;
        packet = &Packet[chan];

        if (packet->chan != -1) {
#if DOLPHIN_REVISION >= 45
            if (packet->fire <= __OSGetSystemTime()) {
#else
            if (packet->time <= OSGetTime()) {
#endif
                if (__SITransfer(packet->chan, packet->output, packet->outputBytes, packet->input, packet->inputBytes, packet->callback) != 0) {
                    OSCancelAlarm(&Alarm[chan]);
                    packet->chan = -1;
                }
                return;
            }
        }
    }
}

#if DOLPHIN_REVISION >= 45
static void SIInterruptHandler(s16 interrupt, struct OSContext * context)
{
    OSTime t0;
    u32 reg;
    s32 chan;
    u32 sr;
    void (*callback)(s32, u32, OSContext *);
    int i;
    u32 mask;
    static u32 cmdTypeAndStatus;

    t0 = __OSGetSystemTime();
    reg = __SIRegs[13];
    if ((reg & 0xC0000000) == 0xC0000000) {
        ASSERTLINE(0X15A, Si.chan != CHAN_NONE);
        chan = Si.chan;
        sr = CompleteTransfer();
        callback = Si.callback;
        Si.callback = NULL;
        SITransferNext(chan);
        if (callback) {
            callback(chan, sr, context);
        }
        sr = __SIRegs[14];
        sr &= 0xF000000 >> (chan * 8);
        __SIRegs[14] = sr;
        if (Type[chan] == 0x80 && !SIIsChanBusy(chan)) {
            SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3, (void *)GetTypeCallback, (u32)OSMicrosecondsToTicks(65));
        }
    }
    if ((reg & 0x18000000) != 0x18000000) {
        return;
    }
    mask = 0;
    if ((Si.poll & 0x80) != 0) {
        mask |= 0x20000000;
    }
    if ((Si.poll & 0x40) != 0) {
        mask |= 0x200000;
    }
    if ((Si.poll & 0x20) != 0) {
        mask |= 0x2000;
    }
    if ((Si.poll & 0x10) != 0) {
        mask |= 0x20;
    }
    while ((__SIRegs[14] & mask) != mask && __OSGetSystemTime() - t0 < OSMicrosecondsToTicks(65)) {
    }
    SIDelay = __OSGetSystemTime() - t0;
    for (i = 0; i < 4; i++) {
        SIGetResponseRaw(i);
    }
    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i]) {
            RDSTHandler[i](interrupt, context);
        }
    }
}

static int SIEnablePollingInterrupt(BOOL enable)
{
    BOOL enabled;
    int rc;
    u32 reg;

    enabled = OSDisableInterrupts();
    reg = __SIRegs[13];
    rc = (reg & 0x8000000) ? 1 : 0;
    if (enable) {
        reg |= 0x8000000;
    } else {
        reg &= ~0x8000000;
    }
    reg &= ~0x80000001;
    __SIRegs[13] = reg;
    OSRestoreInterrupts(enabled);
    return rc;
}

int SIRegisterPollingHandler(void (*handler)(s16, OSContext *))
{
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();
    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == handler) {
            OSRestoreInterrupts(enabled);
            return 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (!RDSTHandler[i]) {
            RDSTHandler[i] = handler;
            SIEnablePollingInterrupt(TRUE);
            OSRestoreInterrupts(enabled);
            return 1;
        }
    }
    OSRestoreInterrupts(enabled);
    return 0;
}

int SIUnregisterPollingHandler(void (*handler)(signed short, struct OSContext *))
{
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();
    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == handler) {
            RDSTHandler[i] = NULL;
            for (i = 0; i < 4; i++) {
                if (RDSTHandler[i]) {
                    break;
                }
            }
            if (i == 4) {
                SIEnablePollingInterrupt(FALSE);
            }
            OSRestoreInterrupts(enabled);
            return 1;
        }
    }
    OSRestoreInterrupts(enabled);
    return 0;
}
#else
static void SIIntrruptHandler(short unused, struct OSContext * context) {
    long chan;
    unsigned long sr;
    void (* callback)(long, unsigned long, struct OSContext *);

    ASSERTLINE(0xE2, Si.chan != CHAN_NONE);

    chan = Si.chan;
    sr = CompleteTransfer();
    callback = Si.callback;
    Si.callback = NULL;
    SITransferNext(chan);
    if(callback) {
       callback(chan, sr, context);
    }
}
#endif

void SIInit() {
    Packet[0].chan = Packet[1].chan = Packet[2].chan = Packet[3].chan = -1;
    __SIRegs[0x30/4] = 0;
    do {} while(__SIRegs[SI_COMCSR_IDX] & SI_COMCSR_TSTART_MASK);
    __SIRegs[SI_COMCSR_IDX] = SI_COMCSR_TCINT_MASK;
#if DOLPHIN_REVISION >= 45
    __OSSetInterruptHandler(0x14, SIInterruptHandler);
#else
    __OSSetInterruptHandler(0x14, SIIntrruptHandler);
#endif
    __OSUnmaskInterrupts(0x800);
#if DOLPHIN_REVISION >= 45
    SIGetType(0);
    SIGetType(1);
    SIGetType(2);
    SIGetType(3);
#endif
}

#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 277 : 0)

static int __SITransfer(long chan, void * output, unsigned long outputBytes, void * input, unsigned long inputBytes, void (* callback)(long, unsigned long, struct OSContext *)) {
    int enabled;
    unsigned long rLen;
    unsigned long i;
    unsigned long sr;
    union {
        unsigned long val;
        struct {
            unsigned long tcint : 1;
            unsigned long tcintmsk : 1;
            unsigned long comerr : 1;
            unsigned long rdstint : 1;
            unsigned long rdstintmsk : 1;
            unsigned long pad2 : 4;
            unsigned long outlngth : 7;
            unsigned long pad1 : 1;
            unsigned long inlngth : 7;
            unsigned long pad0 : 5;
            unsigned long channel : 2;
            unsigned long tstart : 1;
        } f;
    } comcsr;

    ASSERTMSGLINE(0x12A+LINE_OFFSET, (chan >= 0) && (chan < 4), "SITransfer(): invalid channel.");
    ASSERTMSGLINE(0x12C+LINE_OFFSET, (outputBytes != 0) && (outputBytes <= 128), "SITransfer(): output size is out of range (must be 1 to 128).");
    ASSERTMSGLINE(0x12E+LINE_OFFSET, (inputBytes != 0) && (inputBytes <= 128), "SITransfer(): input size is out of range (must be 1 to 128).");

    enabled = OSDisableInterrupts();
    if (Si.chan != -1) {
        OSRestoreInterrupts(enabled);
        return 0;
    }
    ASSERTLINE(0x138+LINE_OFFSET, (__SIRegs[SI_COMCSR_IDX] & (SI_COMCSR_TSTART_MASK | SI_COMCSR_TCINT_MASK)) == 0);
    sr = __SIRegs[SI_STATUS_IDX];
    sr &= (0x0F000000 >> (chan * 8));
    __SIRegs[SI_STATUS_IDX] = sr;

    Si.chan = chan;
    Si.callback = callback;
    Si.inputBytes = inputBytes;
    Si.input = input;

    rLen = ROUND(outputBytes, 4) / 4;
    for (i = 0; i < rLen; i++) {
        __SIRegs[i+0x20] = ((u32*)output)[i];
    }

#if DOLPHIN_REVISION >= 45
    comcsr.val = __SIRegs[13];
#else
    comcsr.val = 0;
#endif
    comcsr.f.tcint = 1;
    comcsr.f.tcintmsk = callback ? 1 : 0;
    comcsr.f.outlngth = outputBytes == 0x80 ? 0 : outputBytes;
    comcsr.f.inlngth = inputBytes == 0x80 ? 0 : inputBytes;
    comcsr.f.channel = chan;
    comcsr.f.tstart = 1;

    __SIRegs[SI_COMCSR_IDX] = comcsr.val;
    OSRestoreInterrupts(enabled);
    return 1;
}

unsigned long SISync() {
    int enabled; // r31
    unsigned long sr; // r30

    do {} while(__SIRegs[SI_COMCSR_IDX] & SI_COMCSR_TSTART_MASK);

    enabled = OSDisableInterrupts();
    sr = CompleteTransfer();
    SITransferNext(4);
    OSRestoreInterrupts(enabled);
    return sr;
}

unsigned long SIGetStatus() {
#if DOLPHIN_REVISION >= 45
    BOOL enabled;
    u32 sr;
    int chan;
    int chanShift;

    enabled = OSDisableInterrupts();
    sr = __SIRegs[SI_STATUS_IDX];
    for (chan = 0; chan < 4; chan++) {
        chanShift = (3 - chan) * 8;
        if ((sr & (8 << chanShift)) && !(Type[chan] & 0x80)) {
            Type[chan] = 8;
        }
    }
    OSRestoreInterrupts(enabled);
    return sr;
#else
    return __SIRegs[SI_STATUS_IDX];
#endif
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 295 : 0)

void SISetCommand(long chan, unsigned long command) {
    ASSERTMSGLINE(0x197+LINE_OFFSET, (chan >= 0) && (chan < 4), "SISetCommand(): invalid channel.");
    __SIRegs[chan * 3] = command;
}

unsigned long SIGetCommand(long chan) {
    ASSERTMSGLINE(0x1A9+LINE_OFFSET, (chan >= 0) && (chan < 4), "SIGetCommand(): invalid channel.");
    return __SIRegs[chan * 3];
}

void SITransferCommands() {
    __SIRegs[SI_STATUS_IDX] = SI_COMCSR_TCINT_MASK;
}

unsigned long SISetXY(unsigned long x, unsigned long y) {
    unsigned long poll;
    int enabled;

    ASSERTMSGLINE(0x1CA+LINE_OFFSET, x >= 8, "SISetXY(): x is out of range (8 <= x <= 255).");
    ASSERTMSGLINE(0x1CB+LINE_OFFSET, x <= 255, "SISetXY(): x is out of range (8 <= x <= 255).");
    ASSERTMSGLINE(0x1CC+LINE_OFFSET, y <= 255, "SISetXY(): y is out of range (0 <= y <= 255).");

    poll = x << 0x10;
    poll |= y << 8;
    enabled = OSDisableInterrupts();
    Si.poll &= 0xFC0000FF;
    Si.poll |= poll;
    poll = Si.poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

unsigned long SIEnablePolling(unsigned long poll) {
    int enabled;
    unsigned long en;

    ASSERTMSGLINE(0x1E8+LINE_OFFSET, !(poll & 0x0FFFFFFF), "SIEnablePolling(): invalid chan bit(s).");
    if (poll == 0) {
        return Si.poll;
    }

    enabled = OSDisableInterrupts();
#if DOLPHIN_REVISION >= 37
    poll >>= 24;
#if DOLPHIN_REVISION >= 45
    en = poll & 0xF0;
    ASSERTLINE(0x32E, en);
    poll &= 0x03FFFFF0 | (en >> 4);
#else
    Si.poll &= ~((poll >> 4) & 0xF);
    poll &= 0x03FFFFF0 | ((poll >> 4) & 0xF);
#endif
    poll &= 0xFC0000FF;
#else
    __SIRegs[0x30/4] = 0;
    poll = poll >> 24;
    en = poll & 0xF0;
    ASSERTLINE(0x202, en);
    poll &= ((en >> 4) | 0x03FFFFF0);
    poll &= 0xFC0000FF;
    Si.poll &= ~(en >> 4);
#endif
#if DOLPHIN_REVISION >= 45
    Si.poll &= ~(en >> 4);
#endif
    Si.poll |= poll;
    poll = Si.poll;
#if DOLPHIN_REVISION >= 45
    SITransferCommands();
#else
    __SIRegs[0x38/4] = 0x80000000;
#endif
    __SIRegs[0x30/4] = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 300 : 0)

unsigned long SIDisablePolling(unsigned long poll) {
    int enabled;

    ASSERTMSGLINE(0x22D+LINE_OFFSET, !(poll & 0x0FFFFFFF), "SIDisablePolling(): invalid chan bit(s).");
    if (poll == 0) {
        return Si.poll;
    }
    enabled = OSDisableInterrupts();
    poll = poll >> 24;
    poll &= 0xF0;
    ASSERTLINE(0x23A+LINE_OFFSET, poll);
    poll = Si.poll & ~poll;
    __SIRegs[0x30/4] = poll;
    Si.poll = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

#if DOLPHIN_REVISION >= 45
static void SIGetResponseRaw(s32 chan)
{
    u32 sr;
    int chanShift;

    sr = SIGetStatus();
    chanShift = (3 - chan) * 8;
    if (sr & (0x20 << chanShift)) {
        InputBuffer[chan][0] = __SIRegs[1 + chan * 3];
        InputBuffer[chan][1] = __SIRegs[2 + chan * 3];
        InputBufferValid[chan] = TRUE;
    }
}
#endif

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 328 : 0)

#if DOLPHIN_REVISION >= 45
int
#else
void
#endif
SIGetResponse(long chan, void * data) {
#if DOLPHIN_REVISION >= 45
    int rc;
    BOOL enabled;
#endif

    ASSERTMSGLINE(0x250+LINE_OFFSET, ((chan >= 0) && (chan < 4)), "SIGetResponse(): invalid channel.");
#if DOLPHIN_REVISION >= 45
    enabled = OSDisableInterrupts();
    SIGetResponseRaw(chan);
    rc = InputBufferValid[chan];
    InputBufferValid[chan] = FALSE;
    if (rc) {
        ((u32*)data)[0] = InputBuffer[chan][0];
        ((u32*)data)[1] = InputBuffer[chan][1];
    }
    OSRestoreInterrupts(enabled);
    return rc;
#else
    ((u32*)data)[0] = __SIRegs[chan * 3 + 1];
    ((u32*)data)[1] = __SIRegs[chan * 3 + 2];
#endif
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 337 : 0)

static void AlarmHandler(struct OSAlarm * alarm, struct OSContext * context) {
    long chan;
    struct SIPacket * packet;

    chan = alarm-Alarm;

    ASSERTLINE(0x266+LINE_OFFSET, 0 <= chan && chan < SI_MAX_CHAN);
#if DOLPHIN_REVISION >= 45
    packet = &Packet[chan];
    if (packet->chan != -1) {
        ASSERTLINE(0x3BB, packet->fire <= __OSGetSystemTime()); // WTF? Dereferencing a NULL POINTER?
        if (__SITransfer(packet->chan, packet->output, packet->outputBytes, packet->input, packet->inputBytes, packet->callback)) {
            packet->chan = -1;
        }
    }
#else
    ASSERTLINE(0x267, packet->time <= OSGetTime()); // WTF? Dereferencing a NULL POINTER?
    packet = &Packet[chan];

    if (packet->chan != -1 && __SITransfer(packet->chan, packet->output, packet->outputBytes, packet->input, packet->inputBytes, packet->callback)) {
        packet->chan = -1;
    }
#endif
}

int SITransfer(long chan, void * output, unsigned long outputBytes, void * input, unsigned long inputBytes,
                void (* callback)(long, unsigned long, struct OSContext *), long long time)
{
    int enabled;
    struct SIPacket * packet;
    long long now;
#if DOLPHIN_REVISION >= 45
    long long fire;
#endif

    packet = &Packet[chan];
    enabled = OSDisableInterrupts();

#if DOLPHIN_REVISION >= 45
    if (packet->chan != -1 || Si.chan == chan) {
#else
    if (packet->chan != -1) {
#endif
        OSRestoreInterrupts(enabled);
        return 0;
    }
#if DOLPHIN_REVISION >= 45
    now = __OSGetSystemTime();
    if (time == 0) {
        fire = now;
    } else {
        fire = time + XferTime[chan];
    }
#else
    now = OSGetTime();
    if (time == 0) {
        time = now;
    }
#endif
#if DOLPHIN_REVISION >= 45
    if (now < fire) {
        time = fire - now;
        OSSetAlarm(&Alarm[chan], time, AlarmHandler);
#else
    if (now < time) {
        OSSetAbsAlarm(&Alarm[chan], time, AlarmHandler);
#endif
    } else if (__SITransfer(chan, output, outputBytes, input, inputBytes, callback)) {
        OSRestoreInterrupts(enabled);
        return 1;
    }
    packet->chan = chan;
    packet->output = output;
    packet->outputBytes = outputBytes;
    packet->input = input;
    packet->inputBytes = inputBytes;
    packet->callback = callback;
#if DOLPHIN_REVISION >= 45
    packet->fire = fire;
#else
    packet->time = time;
#endif
    OSRestoreInterrupts(enabled);
    return 1;
}

#if DOLPHIN_REVISION >= 45
static void CallTypeAndStatusCallback(s32 chan, u32 type)
{
    void (*callback)(s32, u32);
    int i;

    for (i = 0; i < 4; i++) {
        callback = TypeCallback[chan][i];
        if (callback) {
            TypeCallback[chan][i] = NULL;
            callback(chan, type);
        }
    }
}

static void GetTypeCallback(s32 chan, u32 error)
{
    u32 type;
    u32 chanBit;
    int fix;
    u32 id;
    static u32 cmdFixDevice[4];
    u8 unused[8];

    ASSERTLINE(0x43E, 0 <= chan && chan < SI_MAX_CHAN);
    ASSERTLINE(0x440, (Type[chan] & 0xff) == SI_ERROR_BUSY);
    Type[chan] &= ~0x80;
    Type[chan] |= error;
    TypeTime[chan] = __OSGetSystemTime();
    type = Type[chan];
    chanBit = 0x80000000 >> chan;
    fix = __PADFixBits & chanBit;
    __PADFixBits &= ~chanBit;
    if ((error & 0xF) || (type & 0x18000000) != 0x08000000 || !(type & 0x80000000) || (type & 0x4000000)) {
        OSSetWirelessID(chan, 0);
        CallTypeAndStatusCallback(chan, Type[chan]);
        return;
    }
    id = (OSGetWirelessID(chan) << 8) & 0xFFFF00;
    if (fix && (id & 0x100000)) {
        cmdFixDevice[chan] = (id & 0xCFFF00) | 0x4E000000 | 0x100000;
        Type[chan] = 0x80;
        SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, (void *)GetTypeCallback, 0);
        return;
    }
    if (type & 0x100000) {
        if ((id & 0xCFFF00) != (type & 0xCFFF00)) {
            if (!(id & 0x100000)) {
                id = (type & 0xCFFF00);
                id |= 0x100000;
                OSSetWirelessID(chan, (id >> 8) & 0xFFFF);
            }
            cmdFixDevice[chan] = id | 0x4E000000;
            Type[chan] = 0x80;
            SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, (void *)GetTypeCallback, 0);
            return;
        }
    } else if (type & 0x40000000) {
        id = (type & 0xCFFF00);
        id |= 0x100000;
        OSSetWirelessID(chan, (id >> 8) & 0xFFFF);
        cmdFixDevice[chan] = id | 0x4E000000;
        Type[chan] = 0x80;
        SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, (void *)GetTypeCallback, 0);
        return;
    } else {
        OSSetWirelessID(chan, 0);
    }
    CallTypeAndStatusCallback(chan, Type[chan]);
}

u32 SIGetType(s32 chan)
{
    BOOL enabled;
    u32 type;
    OSTime diff;
    static u32 cmdTypeAndStatus;

    enabled = OSDisableInterrupts();
    ASSERTLINE(0x4A8, 0 <= chan && chan < SI_MAX_CHAN);
    type = Type[chan];
    diff = __OSGetSystemTime() - TypeTime[chan];
    if (Si.poll & (0x80 >> chan)) {
        if (type != 8) {
            TypeTime[chan] = __OSGetSystemTime();
            OSRestoreInterrupts(enabled);
            return type;
        }
        type = 0x80;
        Type[chan] = type;
    } else {
        if (OSMillisecondsToTicks(50) >= diff && type != 8) {
            OSRestoreInterrupts(enabled);
            return type;
        }
        if (OSMillisecondsToTicks(75) >= diff) {
            Type[chan] = 0x80;
        } else {
            type = 0x80;
            Type[chan] = type;
        }
    }
    TypeTime[chan] = __OSGetSystemTime();
    SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3, (void *)GetTypeCallback, OSMicrosecondsToTicks(65));
    OSRestoreInterrupts(enabled);
    Type;Type;  // needed to match
    return type;
}

u32 SIGetTypeAsync(s32 chan, void (*callback)(s32, u32))
{
    BOOL enabled;
    u32 type;
    int i;

    enabled = OSDisableInterrupts();
    type = SIGetType(chan);
    if (Type[chan] & 0x80) {
        for (i = 0; i < 4; i++) {
            if (TypeCallback[chan][i] == callback) {
                break;
            }
            if (!TypeCallback[chan][i]) {
                TypeCallback[chan][i] = callback;
                break;
            }
        }
        ASSERTLINE(0x4F9, i < SI_MAX_TYPE);
    } else {
        callback(chan, type);
    }
    OSRestoreInterrupts(enabled);
    return type;
}

u32 SIDecodeType(u32 type)
{
    u32 error;

    error = type & 0xFF;
    type &= 0xFFFFFF00;
    if (error & 8) {
        return 8;
    }
    if (error & 0x47) {
        return 0x40;
    }
    if (error != 0) {
        ASSERTLINE(0x528, error == SI_ERROR_BUSY);
        return 0x80;
    }
    if (!(type & 0x18000000)) {
        switch (type & 0xFFFF0000) {
        case 0x40000:
        case 0x20000:
        case 0x10000:
        case 0x5000000:
        case 0x2000000:
            return type & 0xFFFF0000;
        default:
            return 0x40;
        }
    }
    if ((type & 0x18000000) != 0x08000000) {
        return 0x40;
    }
    switch (type & 0xFFFF0000) {
    case 0x9000000:
        return type & 0xFFFF0000;
    default:
        if ((type & 0x80000000) && !(type & 0x04000000)) {
            if ((type & 0x8B100000) == 0x8B100000) {
                return 0x8B100000;
            }
            if (!(type & 0x02000000)) {
                return 0x88000000;
            }
        }
        if ((type & 0x09000000) == 0x09000000) {
            return 0x09000000;
        }
        return 0x40;
    }
}

u32 SIProbe(s32 chan)
{
    return SIDecodeType(SIGetType(chan));
}

char *SIGetTypeString(u32 type)
{
    switch (SIDecodeType(type)) {
    case 0x8:
        return "No response";
    case 0x80:
        return "Busy";
    case 0x5000000:
        return "N64 controller";
    case 0x10000:
        return "N64 microphone";
    case 0x20000:
        return "N64 keyboard";
    case 0x2000000:
        return "N64 mouse";
    case 0x40000:
        return "GameBoy Advance";
    case 0x9000000:
        return "Standard controller";
    case 0x88000000:
        return "Wireless receiver";
    case 0x8B100000:
        return "WaveBird controller";
    case 0x40:
    default:
        return "Unknown";
    }
}

#endif
