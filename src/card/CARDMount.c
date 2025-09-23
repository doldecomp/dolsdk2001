#include <dolphin.h>
#include <dolphin/card.h>
#include <dolphin/exi.h>

#include "os/__os.h"
#include "__card.h"

typedef void (*EXICallback)(s32 chan, OSContext *context);

static u32 SectorSizeTable[8] =
{
    8 * 1024, 16 * 1024, 32 * 1024, 64 * 1024, 128 * 1024, 256 * 1024, 0, 0,
};

static u32 LatencyTable[8] =
{
    4, 8, 16, 32, 64, 128, 256, 512,
};

// functions
static s32 DoMount(s32 chan);
#if DOLPHIN_REVISION >= 37
static void DoUnmount(s32 chan, s32 result);
#else
static void DoUnmount(s32 chan);
#endif

int CARDProbe(long chan) {
    EXIProbe(chan);
}

s32 CARDProbeEx(s32 chan, s32 *memSize, s32 *sectorSize) {
    u32 id;
    CARDControl *card;
    BOOL enabled;
    s32 result;
#if DOLPHIN_REVISION >= 37
    int r3;
#endif

    if (chan < 0 || 2 <= chan)
        return CARD_RESULT_FATAL_ERROR;

    card = &__CARDBlock[chan];
    enabled = OSDisableInterrupts();

#if DOLPHIN_REVISION >= 37
    r3 = EXIProbeEx(chan);
    if (r3 == -1)
        result = CARD_RESULT_NOCARD;
    else if (r3 == 0)
        result = CARD_RESULT_BUSY;
#else
    if (EXIProbe(chan) == 0)
        result = CARD_RESULT_NOCARD;
#endif
    else if (card->attached)
    {
        if (card->mountStep < 1)
            result = CARD_RESULT_BUSY;
        else
        {
            if (DOLPHIN_REVISION < 37 || memSize != NULL)
                *memSize = card->size;
            if (DOLPHIN_REVISION < 37 || sectorSize != NULL)
                *sectorSize = card->sectorSize;
            result = CARD_RESULT_READY;
        }
    }
    else if ((EXIGetState(chan) & 8))
        result = CARD_RESULT_WRONGDEVICE;
    else if (!EXIGetID(chan, 0, &id))
        result = CARD_RESULT_BUSY;
    else if ((id & 0xFFFF0000) || (id & 3))
        result = CARD_RESULT_WRONGDEVICE;
    else
    {
        if (DOLPHIN_REVISION < 37 || memSize != NULL)
            *memSize = (s32)(id & 0xfc);
        if (DOLPHIN_REVISION < 37 || sectorSize != NULL)
            *sectorSize = SectorSizeTable[(id & 0x00003800) >> 11];
        result = CARD_RESULT_READY;
    }

    OSRestoreInterrupts(enabled);
    return result;
}

static s32 DoMount(s32 chan) {
    CARDControl *card;
    u32 id;
    u8 status;
    s32 result;
    struct OSSramEx *sram;
    int i;
    u8 checkSum;
    int step;

    ASSERTLINE(0xFE, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (card->mountStep == 0)
    {
        result = __CARDReadNintendoID(chan, &id);
        if (result < 0)
            goto error;

        card->size = (u16)(id & 0xFC);
        card->sectorSize = SectorSizeTable[(id & 0x00003800) >> 11];
        card->cBlock = (u16)((card->size * 1024 * 1024 / 8) / card->sectorSize);
        card->latency = LatencyTable[(id & 0x00000700) >> 8];

        if (card->sectorSize == 0 || card->cBlock < 8)
        {
            result = -2;
            goto error;
        }
        result = __CARDClearStatus(chan);
        if (result < 0)
            goto error;
        result = __CARDReadStatus(chan, &status);
        if (result < 0)
            goto error;
        if (!EXIProbe(chan))
        {
            result = CARD_RESULT_NOCARD;
            goto error;
        }

        if (!(status & 0x40))
        {
            result = __CARDUnlock(chan, card->id);
            if (result < 0)
                goto error;

            checkSum = 0;
            sram = __OSLockSramEx();
            for (i = 0; i < 12; i++)
            {
                sram->flashID[chan][i] = card->id[i];
                checkSum += card->id[i];
            }
            sram->flashIDCheckSum[chan] = (u8)~checkSum;
            __OSUnlockSramEx(TRUE);

            return result;
        }
        else
        {
            card->mountStep = 1;

            checkSum = 0;
            sram = __OSLockSramEx();
            for (i = 0; i < 12; i++)
                checkSum += sram->flashID[chan][i];

            __OSUnlockSramEx(FALSE);
            if (sram->flashIDCheckSum[chan] != (u8)~checkSum)
            {
                result = CARD_RESULT_IOERROR;
                goto error;
            }
        }
    }

    if (card->mountStep == 1)
    {
        card->mountStep = 2;

        result = __CARDEnableInterrupt(chan, TRUE);
        if (result < 0)
            goto error;

        EXISetExiCallback(chan, __CARDExiHandler);
        EXIUnlock(chan);
        DCInvalidateRange(card->workArea, CARD_WORKAREA_SIZE);
    }

    step = card->mountStep - 2;
    result = __CARDRead(chan, (u32)card->sectorSize * step, CARD_SYSTEM_BLOCK_SIZE,
                        (u8 *)card->workArea + (CARD_SYSTEM_BLOCK_SIZE * step), __CARDMountCallback);
    if (result < 0)
        __CARDPutControlBlock(card, result);
    return result;

error:
    EXIUnlock(chan);
#if DOLPHIN_REVISION >= 37
    DoUnmount(chan, result);
#else
    DoUnmount(chan);
#endif
    return result;
}

void __CARDMountCallback(s32 chan, s32 result) {
    CARDControl *card;
    CARDCallback callback;

    ASSERTLINE(0x181, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    switch (result)
    {
    case CARD_RESULT_READY:
        if (++card->mountStep < CARD_MAX_MOUNT_STEP)
        {
            result = DoMount(chan);
            if (0 <= result)
                return;
        }
        else
            result = __CARDVerify(card);
        break;
    case CARD_RESULT_UNLOCKED:
        result = DoMount(chan);
        if (result >= 0)
            return;
        break;
    case CARD_RESULT_IOERROR:
    case CARD_RESULT_NOCARD:
#if DOLPHIN_REVISION >= 37
        DoUnmount(chan, result);
#else
        DoUnmount(chan);
#endif
        break;
    }

    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    ASSERTLINE(0x1AB, callback);
    callback(chan, result);
}

s32 CARDMountAsync(s32 chan, void *workArea, CARDCallback detachCallback, CARDCallback attachCallback) {
    CARDControl *card;
    BOOL enabled;

    ASSERTLINE(0x1CB, workArea && ((u32) workArea % 32 == 0));
    ASSERTLINE(0x1CC, 0 <= chan && chan < 2);

    if (chan < 0 || 2 <= chan)
        return CARD_RESULT_FATAL_ERROR;

    card = &__CARDBlock[chan];

    enabled = OSDisableInterrupts();
    if (card->result == CARD_RESULT_BUSY)
    {
        OSRestoreInterrupts(enabled);
        return CARD_RESULT_BUSY;
    }

    if (!card->attached && (EXIGetState(chan) & 0x08))
    {
        OSRestoreInterrupts(enabled);
        return CARD_RESULT_WRONGDEVICE;
    }

    card->result = CARD_RESULT_BUSY;
    card->workArea = workArea;
    card->extCallback = detachCallback;
    card->apiCallback = attachCallback ? attachCallback : __CARDDefaultApiCallback;
    card->exiCallback = 0;

    if (!card->attached && !EXIAttach(chan, __CARDExtHandler))
    {
        card->result = CARD_RESULT_NOCARD;
        OSRestoreInterrupts(enabled);
        return CARD_RESULT_NOCARD;
    }

    card->mountStep = 0;
    card->attached = TRUE;
    EXISetExiCallback(chan, 0);
#if DOLPHIN_REVISION >= 37
    OSCancelAlarm(&card->alarm);
#endif

    card->currentDir = 0;
    card->currentFat = 0;

    OSRestoreInterrupts(enabled);

    card->unlockCallback = __CARDMountCallback;
    if (!EXILock(chan, 0, __CARDUnlockedHandler))
        return CARD_RESULT_READY;

    card->unlockCallback = 0;
    return DoMount(chan);
}

s32 CARDMount(s32 chan, void *workArea, CARDCallback detachCallback) {
    s32 result = CARDMountAsync(chan, workArea, detachCallback, __CARDSyncCallback);

    if (result < 0)
        return result;
    return __CARDSync(chan);
}

#if DOLPHIN_REVISION >= 37
static void DoUnmount(s32 chan, s32 result)
#else
static void DoUnmount(s32 chan)
#endif
{
    CARDControl *card;
    BOOL enabled;

    ASSERTLINE(0x22F, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    enabled = OSDisableInterrupts();
    if (card->attached)
    {
        EXISetExiCallback(chan, 0);
        EXIDetach(chan);
#if DOLPHIN_REVISION >= 37
        OSCancelAlarm(&card->alarm);
#endif
        card->attached = FALSE;
#if DOLPHIN_REVISION >= 37
        card->result = result;
#else
        card->result = CARD_RESULT_NOCARD;
#endif
        card->mountStep = 0;
    }
    OSRestoreInterrupts(enabled);
}

s32 CARDUnmount(s32 chan) {
    CARDControl *card;
    s32 result;

    ASSERTLINE(0x252, 0 <= chan && chan < 2);

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0)
        return result;
#if DOLPHIN_REVISION >= 37
    DoUnmount(chan, CARD_RESULT_NOCARD);
#else
    DoUnmount(chan);
#endif
    return CARD_RESULT_READY;
}
