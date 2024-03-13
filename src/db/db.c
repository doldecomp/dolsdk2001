#include <dolphin/os.h>

struct DBInterface
{
    u32 bPresent;
    u32 exceptionMask;
    void (*ExceptionDestination)(void);
    void *exceptionReturn;
};

u8 DBStack[4096];
u8 *DBStackEnd = DBStack + 4088;
BOOL DBVerbose;
struct DBInterface *__DBInterface;

static void __DBExceptionDestination(void);

void DBInit(void)
{
    __DBInterface = OSPhysicalToCached(0x40);
    __DBInterface->ExceptionDestination = (void *)OSCachedToPhysical(__DBExceptionDestination);
    DBVerbose = TRUE;
}

BOOL DBIsDebuggerPresent(void)
{
    if (__DBInterface == NULL)
        return FALSE;
    return __DBInterface->bPresent;
}

void __DBExceptionDestinationAux(void)
{
    u32 *contextAddr;
    OSContext *context;

    contextAddr = (void *)0xC0;
    context = OSPhysicalToCached(*contextAddr);
    OSReport("DBExceptionDestination\n");
    OSDumpContext(context);
    PPCHalt();
}

asm void __DBExceptionDestination(void)
{
    nofralloc
    mfmsr r3
    ori r3, r3, 0x30
    mtmsr r3
    b __DBExceptionDestinationAux
}

int __DBIsExceptionMarked(u8 exception)
{
    u32 mask = (1 << exception);
    return __DBInterface->exceptionMask & mask;
}

void __DBMarkException(u8 exception, int value)
{
    u32 mask = (1 << exception);

    if (value != 0)
        __DBInterface->exceptionMask = __DBInterface->exceptionMask | mask;
    else
        __DBInterface->exceptionMask = __DBInterface->exceptionMask & ~mask;
}

void __DBSetPresent(u32 value)
{
    __DBInterface->bPresent = value;
}

void DBPrintf(char *str, ...) {}