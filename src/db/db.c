#include <dolphin.h>
#include <dolphin/db.h>
#include <dolphin/os.h>

unsigned char DBStack[4096];
int DBVerbose;
struct DBInterface * __DBInterface;
unsigned char * DBStackEnd = DBStack;

void DBInit(void) {
    __DBInterface = (void *) OSPhysicalToCached(64);
    __DBInterface->ExceptionDestination = (void *) OSCachedToPhysical(__DBExceptionDestination);
    DBVerbose = 1;
}

int DBIsDebuggerPresent(void) {
    if (__DBInterface == NULL) {
        return FALSE;
    } else {
        return __DBInterface->bPresent;
    }
}

void __DBExceptionDestinationAux(void) {
    unsigned long * contextAddr = (void *) 0xC0;
    struct OSContext * context = (void *) OSPhysicalToCached(*contextAddr);
    OSReport("DBExceptionDestination\n");
    OSDumpContext(context);
    PPCHalt();
}

asm void __DBExceptionDestination(void) {
    nofralloc
    mfmsr r3
    ori r3, r3, MSR_IR | MSR_DR
    mtmsr r3
    b __DBExceptionDestinationAux
}

int __DBIsExceptionMarked(unsigned char exception) {
    unsigned long mask = (1 << exception);
    return __DBInterface->exceptionMask & mask;
}

void __DBMarkException(unsigned char exception, int value) {
    unsigned long mask = (1 << exception);
    if (value != 0) {
        __DBInterface->exceptionMask = __DBInterface->exceptionMask | mask;
    } else {
        __DBInterface->exceptionMask = __DBInterface->exceptionMask & ~mask;
    }
}

void __DBSetPresent(unsigned long value) {
    __DBInterface->bPresent = value;
}

void DBPrintf(char * x, ...) {
    (void) x;
    return;
}
