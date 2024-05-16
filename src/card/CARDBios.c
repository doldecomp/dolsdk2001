#if DOLPHIN_REVISION >= 45
#include <stddef.h>  // need NULL to be defined as 0L
#endif
#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/card.h>

#include "os/__os.h"
#include "__card.h"

struct CARDControl __CARDBlock[2];

DVDDiskID * __CARDDiskID;
DVDDiskID __CARDDiskNone;

// functions
static void TimeoutHandler(OSAlarm *alarm, OSContext *context);
static void SetupTimeoutAlarm(CARDControl *card);
static s32 Retry(s32 chan);
static void UnlockedCallback(s32 chan, s32 result);
static BOOL OnReset(BOOL f);

static OSResetFunctionInfo ResetFunctionInfo = {OnReset, 127};

void __CARDDefaultApiCallback(s32 chan, s32 result) {}

void __CARDSyncCallback(s32 chan, s32 result) {
    struct CARDControl * card;

    card = &__CARDBlock[chan];
    OSWakeupThread(&card->threadQueue);
}

#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 16 : 0)

void __CARDExtHandler(s32 chan, OSContext *context) {
    CARDControl *card;
    CARDCallback callback;

    ASSERTLINE(0xB6+LINE_OFFSET, 0 <= chan && chan < 2);

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 19 : 0)

    card = &__CARDBlock[chan];
    if (card->attached)
    {
        ASSERTLINE(0xBA+LINE_OFFSET, card->txCallback == 0);
        card->attached = FALSE;
#if DOLPHIN_REVISION < 45
        card->result = CARD_RESULT_NOCARD;
#endif
        EXISetExiCallback(chan, 0);
#if DOLPHIN_REVISION >= 37
        OSCancelAlarm(&card->alarm);
#endif
        callback = card->exiCallback;

        if (callback)
        {
            card->exiCallback = 0;
            callback(chan, CARD_RESULT_NOCARD);
        }
#if DOLPHIN_REVISION >= 37
        if (card->result != -1) {
            card->result = -3;
        }
#endif

        callback = card->extCallback;
        if (callback && CARD_MAX_MOUNT_STEP <= card->mountStep)
        {
            card->extCallback = 0;
            callback(chan, CARD_RESULT_NOCARD);
        }
    }
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 29 : 0)

void __CARDExiHandler(s32 chan, OSContext *context) {
    CARDControl *card;
    CARDCallback callback;
    u8 status;
    s32 result;

    ASSERTLINE(0xDC+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];

    OSCancelAlarm(&card->alarm);

    if (!card->attached)
    {
        return;
    }

    if (!EXILock(chan, 0, 0))
    {
        result = CARD_RESULT_FATAL_ERROR;
        goto fatal;
    }

    if ((result = __CARDReadStatus(chan, &status)) < 0 || (result = __CARDClearStatus(chan)) < 0)
    {
        goto error;
    }

    if ((result = (status & 0x18) ? CARD_RESULT_IOERROR : CARD_RESULT_READY) ==
            CARD_RESULT_IOERROR &&
        --card->retry > 0)
    {
        result = Retry(chan);
        if (result >= 0)
        {
            return;
        }
        goto fatal;
    }

error:
    EXIUnlock(chan);

fatal:
    callback = card->exiCallback;
    if (callback)
    {
        card->exiCallback = 0;
        callback(chan, result);
    }
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 30 : 0)

void __CARDTxHandler(s32 chan, OSContext *context) {
    CARDControl *card;
    CARDCallback callback;
#if DOLPHIN_REVISION >= 37
    BOOL r31;
#endif

    ASSERTLINE(0x12D+LINE_OFFSET, 0 <= chan && chan < 2);
    
    card = &__CARDBlock[chan];
#if DOLPHIN_REVISION >= 37
    r31 = !EXIDeselect(chan);
#else
    !EXIDeselect(chan);
#endif
    !EXIUnlock(chan);
    callback = card->txCallback;
    if (callback)
    {
        card->txCallback = NULL;
#if DOLPHIN_REVISION >= 37
        callback(chan, (!r31 && EXIProbe(chan)) ? CARD_RESULT_READY : CARD_RESULT_NOCARD);
#else
        callback(chan, (EXIProbe(chan)) ? CARD_RESULT_READY : CARD_RESULT_NOCARD);
#endif
    }
}

void __CARDUnlockedHandler(s32 chan, OSContext *context) {
    CARDControl *card;
    CARDCallback callback;

    ASSERTLINE(0x15C+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    callback = card->unlockCallback;
    if (callback)
    {
        card->unlockCallback = 0;
        callback(chan, EXIProbe(chan) ? CARD_RESULT_UNLOCKED : CARD_RESULT_NOCARD);
    }
}

int __CARDReadNintendoID(s32 chan, u32 *id) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(0x16F+LINE_OFFSET, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, 0)) {
        return CARD_RESULT_NOCARD;
    }
    cmd = 0;
    err = 0;
    err |= !EXIImm(chan, &cmd, 2, 1, 0);
    err |= !EXISync(chan);
    err |= !EXIImm(chan, id, 4, 0, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    if (err)
    {
        return CARD_RESULT_NOCARD;
    }
    if ((*id & 0xFFFF0000) || (*id & 3))
    {
        return CARD_RESULT_WRONGDEVICE;
    }
    return CARD_RESULT_READY;
}

s32 __CARDEnableInterrupt(s32 chan, BOOL enable) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(0x190+LINE_OFFSET, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, 4)) {
        return CARD_RESULT_NOCARD;
    }

    cmd = enable ? 0x81010000 : 0x81000000;
    err = FALSE;
    err |= !EXIImm(chan, &cmd, 2, 1, NULL);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

s32 __CARDReadStatus(s32 chan, u8 *status) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(0x1A3+LINE_OFFSET, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, 4)) {
        return CARD_RESULT_NOCARD;
    }

    cmd = 0x83000000;
    err = FALSE;
    err |= !EXIImm(chan, &cmd, 2, 1, NULL);
    err |= !EXISync(chan);
    err |= !EXIImm(chan, status, 1, 0, NULL);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

s32 __CARDClearStatus(s32 chan) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(0x1B8+LINE_OFFSET, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, 4)) {
        return CARD_RESULT_NOCARD;
    }

    cmd = 0x89000000;
    err = FALSE;
    err |= !EXIImm(chan, &cmd, 1, 1, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);

    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

long __CARDSleep(long chan) {
    int err;
    unsigned long cmd;

    ASSERTLINE(0x1CB+LINE_OFFSET, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, 4)) {
        return CARD_RESULT_NOCARD;
    }
    cmd = 0x88000000;
    err = 0;
    err |= !EXIImm(chan, &cmd, 1, 1, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);

    if(err) {
        return CARD_RESULT_NOCARD;
    }
    return CARD_RESULT_READY;
}

long __CARDWakeup(long chan) {
    int err;
    unsigned long cmd;

    ASSERTLINE(0x1DE+LINE_OFFSET, 0 <= chan && chan < 2);
    if (!EXISelect(chan, 0, 4)) {
        return CARD_RESULT_NOCARD;
    }
    cmd = 0x87000000;
    err = 0;
    err |= !EXIImm(chan, &cmd, 1, 1, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);

    if(err) {
        return CARD_RESULT_NOCARD;
    }
    return CARD_RESULT_READY;
}

static void TimeoutHandler(OSAlarm *alarm, OSContext *context) {
    s32 chan;
    CARDControl *card;
    CARDCallback callback;
    for (chan = 0; chan < 2; ++chan)
    {
        card = &__CARDBlock[chan];
        if (alarm == &card->alarm)
        {
            break;
        }
    }

#if DOLPHIN_REVISION == 37
    if (!card->attached) {
        return;
    }
#endif

    ASSERTLINE(0x20E+LINE_OFFSET, 0 <= chan && chan < 2);
#if DOLPHIN_REVISION >= 45
    if (!card->attached) {
        return;
    }
#endif

    EXISetExiCallback(chan, NULL);
    callback = card->exiCallback;
    if (callback)
    {
        card->exiCallback = 0;
        callback(chan, CARD_RESULT_IOERROR);
    }
}

static void SetupTimeoutAlarm(CARDControl *card) {
    OSCancelAlarm(&card->alarm);
    switch (card->cmd[0])
    {
    case 0xF3: 
        break;
    case 0xF2:
#if DOLPHIN_REVISION >= 37
        OSSetAlarm(&card->alarm, OSMillisecondsToTicks(100), TimeoutHandler);
        break;
#endif
    case 0xF4:
    case 0xF1:
        OSSetAlarm(&card->alarm, OSSecondsToTicks((OSTime)2) * (card->sectorSize / 0x2000),
                   TimeoutHandler);
        break;
    }
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 41 : 0)

static s32 Retry(s32 chan) {
    CARDControl *card;

    ASSERTLINE(0x247+LINE_OFFSET, 0 <= chan && chan < 2);
    
    card = &__CARDBlock[chan];
    if (!EXISelect(chan, 0, 4))
    {
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    SetupTimeoutAlarm(card);

    if (!EXIImmEx(chan, card->cmd, card->cmdlen, 1))
    {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    if (card->cmd[0] == 0x52 &&
        !EXIImmEx(chan, (u8 *)card->workArea + sizeof(CARDID), card->latency, 1))
    {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    if (card->mode == 0xffffffff)
    {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_READY;
    }

    if (!EXIDma(chan, card->buffer, (s32)((card->cmd[0] == 0x52) ? 512 : 128), card->mode,
                __CARDTxHandler))
    {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    return CARD_RESULT_READY;
}

static void UnlockedCallback(s32 chan, s32 result) {
    CARDCallback callback;
    CARDControl *card;

    ASSERTLINE(0x287+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (result >= 0)
    {
        card->unlockCallback = UnlockedCallback;
        if (!EXILock(chan, 0, __CARDUnlockedHandler))
        {
            result = CARD_RESULT_READY;
        }
        else
        {
            card->unlockCallback = 0;
            result = Retry(chan);
        }
    }

    if (result < 0)
    {
        switch (card->cmd[0])
        {
        case 0x52:
            callback = card->txCallback;
            if (callback)
            {
                card->txCallback = NULL;
                callback(chan, result);
            }

            break;
        case 0xF2:
        case 0xF4:
        case 0xF1:
            callback = card->exiCallback;
            if (callback)
            {
                card->exiCallback = 0;
                callback(chan, result);
            }
            break;
        }
    }
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 45 : 0)

static s32 __CARDStart(s32 chan, CARDCallback txCallback, CARDCallback exiCallback) {
#if DOLPHIN_REVISION >= 45
    BOOL enabled;
#endif
    CARDControl *card;
    s32 result;

#if DOLPHIN_REVISION >= 45
    enabled = OSDisableInterrupts();
#endif

    ASSERTLINE(0x2C5+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (!card->attached)
    {
#if DOLPHIN_REVISION >= 45
        result = CARD_RESULT_NOCARD;
#else
        return CARD_RESULT_NOCARD;
#endif
    }
    else
    {

        if (txCallback)
        {
            card->txCallback = txCallback;
        }
        if (exiCallback)
        {
            card->exiCallback = exiCallback;
        }
        card->unlockCallback = UnlockedCallback;
        if (!EXILock(chan, 0, __CARDUnlockedHandler))
        {
#if DOLPHIN_REVISION >= 45
            result = CARD_RESULT_BUSY;
#else
            return CARD_RESULT_BUSY;
#endif
        }
        else
        {
            card->unlockCallback = 0;

            if (!EXISelect(chan, 0, 4))
            {
                EXIUnlock(chan);
#if DOLPHIN_REVISION >= 45
                result = CARD_RESULT_NOCARD;
#else
                return CARD_RESULT_NOCARD;
#endif
            }
            else
            {
                SetupTimeoutAlarm(card);
#if DOLPHIN_REVISION >= 45
                result = CARD_RESULT_READY;
#else
                return CARD_RESULT_READY;
#endif
            }
        }
    }

#if DOLPHIN_REVISION >= 45
    OSRestoreInterrupts(enabled);
    return result;
#else
    return CARD_RESULT_READY;
#endif
}

#define AD1(x) ((u8)(((x) >> 17) & 0x7f))
#define AD1EX(x) ((u8)(AD1(x) | 0x80));
#define AD2(x) ((u8)(((x) >> 9) & 0xff))
#define AD3(x) ((u8)(((x) >> 7) & 0x03))
#define BA(x) ((u8)((x)&0x7f))

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 55 : 0)

s32 __CARDReadSegment(s32 chan, CARDCallback callback) {
    CARDControl *card;
    s32 result;

    ASSERTLINE(0x2F9+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    ASSERTLINE(0x2FB+LINE_OFFSET, card->addr % CARD_SEG_SIZE == 0);
    ASSERTLINE(0x2FC+LINE_OFFSET, card->addr < (u32) card->size * 1024 * 1024 / 8);
    card->cmd[0] = 0x52;
    card->cmd[1] = AD1(card->addr);
    card->cmd[2] = AD2(card->addr);
    card->cmd[3] = AD3(card->addr);
    card->cmd[4] = BA(card->addr);
    card->cmdlen = 5;
    card->mode = 0;
    card->retry = 0;

    result = __CARDStart(chan, callback, 0);
    if (result == CARD_RESULT_BUSY)
    {
#if DOLPHIN_REVISION >= 45
        result = CARD_RESULT_READY;
#else
        return CARD_RESULT_READY;
#endif
    }
#if DOLPHIN_REVISION >= 45
    else if (result >= 0) {
#else
        if (result < 0) {
            return result;
        }
#endif
        if (!EXIImmEx(chan, card->cmd, card->cmdlen, 1) ||
            !EXIImmEx(chan, (u8 *)card->workArea + sizeof(CARDID), card->latency,
                      1) || // XXX use DMA if possible
            !EXIDma(chan, card->buffer, 512, card->mode, __CARDTxHandler))
        {
            card->txCallback = NULL;
            EXIDeselect(chan);
            EXIUnlock(chan);
#if DOLPHIN_REVISION >= 45
            result = CARD_RESULT_NOCARD;
#else
            return CARD_RESULT_NOCARD;
#endif
        }
#if DOLPHIN_REVISION >= 45
        else {
            result = CARD_RESULT_READY;
        }
    }
    return result;
#else
    return CARD_RESULT_READY;
#endif
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 56 : 0)

s32 __CARDWritePage(s32 chan, CARDCallback callback) {
    CARDControl *card;
    s32 result;

    ASSERTLINE(0x331+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    ASSERTLINE(0x333+LINE_OFFSET, card->addr % CARD_PAGE_SIZE == 0);
    ASSERTLINE(0x334+LINE_OFFSET, card->addr < (u32) card->size * 1024 * 1024 / 8);
    card->cmd[0] = 0xF2;
    card->cmd[1] = AD1(card->addr);
    card->cmd[2] = AD2(card->addr);
    card->cmd[3] = AD3(card->addr);
    card->cmd[4] = BA(card->addr);
    card->cmdlen = 5;
    card->mode = 1;
    card->retry = 3;

    result = __CARDStart(chan, 0, callback);
    if (result == CARD_RESULT_BUSY)
    {
#if DOLPHIN_REVISION >= 45
        result = CARD_RESULT_READY;
#else
        return CARD_RESULT_READY;
#endif
    }
#if DOLPHIN_REVISION >= 45
    else if (result >= 0) {
#else
        if (result < 0)
        {
            return result;
        }
#endif
        if (!EXIImmEx(chan, card->cmd, card->cmdlen, 1) ||
            !EXIDma(chan, card->buffer, 128, card->mode, __CARDTxHandler))
        {
            card->exiCallback = 0;
            EXIDeselect(chan);
            EXIUnlock(chan);
#if DOLPHIN_REVISION >= 45
            result = CARD_RESULT_NOCARD;
#else
            return CARD_RESULT_NOCARD;
#endif
        }
#if DOLPHIN_REVISION >= 45
        else {
            result = CARD_RESULT_READY;
        }
    }
    return result;
#else
    return CARD_RESULT_READY;
#endif
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 57 : 0)

long __CARDErase(long chan, void (* callback)(long, long)) {
    struct CARDControl * card;
    long result;

    ASSERTLINE(0x364+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    card->cmd[0] = 0xF4;
    card->cmd[1] = 0;
    card->cmd[2] = 0;
    card->cmdlen = 3;
    card->mode = -1;
    card->retry = 3;
    result = __CARDStart(chan, 0, callback);
    if (result == -1) {
#if DOLPHIN_REVISION >= 45
        result = 0;
#else
        return 0;
#endif
    }
#if DOLPHIN_REVISION >= 45
    else if (result >= 0) {
#else
        if (result < 0) {
            return result;
        }
        result = 0;
#endif
        if (EXIImmEx(chan, &card->cmd, card->cmdlen, 1) == 0) {
            result = -3;
            card->exiCallback = 0;
        }
#if DOLPHIN_REVISION >= 45
        else {
            result = 0;
        }
#endif
        EXIDeselect(chan);
        EXIUnlock(chan);
#if DOLPHIN_REVISION >= 45
    }
#endif
    return result;
}

s32 __CARDEraseSector(s32 chan, u32 addr, CARDCallback callback) {
    s32 result;
    CARDControl *card;

    ASSERTLINE(0x394+LINE_OFFSET, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    ASSERTLINE(0x396+LINE_OFFSET, addr % card->sectorSize == 0);
    ASSERTLINE(0x397+LINE_OFFSET, addr < (u32) card->size * 1024 * 1024 / 8);
    card->cmd[0] = 0xF1;
    card->cmd[1] = AD1(addr);
    card->cmd[2] = AD2(addr);
    card->cmdlen = 3;
    card->mode = -1;
    card->retry = 3;

    result = __CARDStart(chan, 0, callback);

    if (result == CARD_RESULT_BUSY)
    {
#if DOLPHIN_REVISION >= 45
        result = CARD_RESULT_READY;
#else
        return CARD_RESULT_READY;
#endif
    }
#if DOLPHIN_REVISION >= 45
    else if (result >= 0) {
#else
        if (result < 0)
        {
            return result;
        }
        result = CARD_RESULT_READY;
#endif
        if (!EXIImmEx(chan, card->cmd, card->cmdlen, 1))
        {
            result = CARD_RESULT_NOCARD;
            card->exiCallback = NULL;
        }
#if DOLPHIN_REVISION >= 45
        else {
            result = 0;
        }
#endif
        EXIDeselect(chan);
        EXIUnlock(chan);
#if DOLPHIN_REVISION >= 45
    }
#endif
    return result;
}

void CARDInit(void) {
    int chan;

    if (__CARDDiskID)
    {
        return;
    }

    DSPInit();
    OSInitAlarm();

    for (chan = 0; chan < 2; ++chan)
    {
        CARDControl *card = &__CARDBlock[chan];

        card->result = CARD_RESULT_NOCARD;
        OSInitThreadQueue(&card->threadQueue);
        OSCreateAlarm(&card->alarm);
    }
    __CARDSetDiskID((void*)OSPhysicalToCached(0));

    OSRegisterResetFunction(&ResetFunctionInfo);
}

void __CARDSetDiskID(DVDDiskID *id) {
    __CARDDiskID = id ? id : &__CARDDiskNone;
}

s32 __CARDGetControlBlock(s32 chan, CARDControl **pcard) {
    BOOL enabled;
    s32 result;
    CARDControl *card;

    if (chan < 0 || chan >= 2 || !__CARDDiskID)
    {
        return CARD_RESULT_FATAL_ERROR;
    }

    enabled = OSDisableInterrupts();
    card = &__CARDBlock[chan];
    if (!card->attached)
    {
        result = CARD_RESULT_NOCARD;
    }
    else if (card->result == CARD_RESULT_BUSY)
    {
        result = CARD_RESULT_BUSY;
    }
    else
    {
        card->result = CARD_RESULT_BUSY;
        result = CARD_RESULT_READY;
        card->apiCallback = NULL;
        *pcard = card;
    }
    OSRestoreInterrupts(enabled);
    return result;
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 43 : 0)

s32 __CARDPutControlBlock(CARDControl *card, s32 result) {
    BOOL enabled;

    ASSERTLINE(0x43A+LINE_OFFSET, result != CARD_RESULT_BUSY);

    enabled = OSDisableInterrupts();
    if (card->attached) {
        card->result = result;
    }
#if DOLPHIN_REVISION >= 45
    else if (card->result == -1) {
        card->result = result;
    }
#else
    else {
        ASSERTLINE(0x442, card->result == CARD_RESULT_NOCARD);
    }
#endif
    OSRestoreInterrupts(enabled);
    return result;
}

#undef LINE_OFFSET
#define LINE_OFFSET (DOLPHIN_REVISION >= 45 ? 49 : 0)

s32 CARDGetResultCode(s32 chan) {
    CARDControl *card;
    
    ASSERTLINE(0x455+LINE_OFFSET, 0 <= chan && chan < 2);

    if (chan < 0 || chan >= 2)
    {
        return CARD_RESULT_FATAL_ERROR;
    }
    card = &__CARDBlock[chan];
    return card->result;
}

s32 CARDFreeBlocks(s32 chan, s32 *byteNotUsed, s32 *filesNotUsed) {
    CARDControl *card;
    s32 result;
    u16 *fat;
    CARDDir *dir;
    CARDDir *ent;
    u16 fileNo;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0)
    {
        return result;
    }

    fat = __CARDGetFatBlock(card);
    dir = __CARDGetDirBlock(card);
    if (fat == 0 || dir == 0)
    {
        return __CARDPutControlBlock(card, CARD_RESULT_BROKEN);
    }

    if (DOLPHIN_REVISION < 37 || byteNotUsed != NULL) {
        *byteNotUsed = (s32)(card->sectorSize * fat[CARD_FAT_FREEBLOCKS]);
    }

    if (DOLPHIN_REVISION < 37 || filesNotUsed != NULL) {
        *filesNotUsed = 0;
        for (fileNo = 0; fileNo < CARD_MAX_FILE; fileNo++)
        {
            ent = &dir[fileNo];
            if (ent->fileName[0] == 0xff)
            {
                ++*filesNotUsed;
            }
        }
    }

    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

long CARDGetEncoding(long chan, unsigned short * encode) {
    struct CARDControl * card;
    struct CARDID * id;
    long result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }
    id = card->workArea;
    *encode = id->encode;
    return __CARDPutControlBlock(card, 0);
}

long CARDGetMemSize(long chan, unsigned short * size) {
    struct CARDControl * card;
    long result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }
    *size = card->size;
    return __CARDPutControlBlock(card, 0);
}

s32 CARDGetSectorSize(s32 chan, u32 *size) {
    struct CARDControl *card;
    long result;
#if DOLPHIN_REVISION == 37
    BOOL enabled;
    CARDControl *tmp;
#endif

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0)
    {
        return result;
    }
    *size = card->sectorSize;
#if DOLPHIN_REVISION == 37
    tmp = card;
    enabled = OSDisableInterrupts();
    if (tmp->attached) {
        tmp->result = 0;
    } else if (tmp->result == -1) {
        tmp->result = 0;
    }
    OSRestoreInterrupts(enabled);
    return 0;
#else
    return __CARDPutControlBlock(card, 0);
#endif
}

s32 __CARDSync(s32 chan) {
    CARDControl *block;
    s32 result;
    s32 enabled;

    block = &__CARDBlock[chan];
    enabled = OSDisableInterrupts();
    while ((result = CARDGetResultCode(chan)) == -1)
    {
        OSSleepThread(&block->threadQueue);
    }
    OSRestoreInterrupts(enabled);
    return result;
}

static BOOL OnReset(BOOL f) {
    if (!f)
    {
        if (CARDUnmount(0) == CARD_RESULT_BUSY || CARDUnmount(1) == CARD_RESULT_BUSY)
        {
            return FALSE;
        }
    }

    return TRUE;
}
