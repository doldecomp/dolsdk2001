#ifndef _DOLPHIN_DBINTERFACE_H_
#define _DOLPHIN_DBINTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DBInterface
{
    u32     bPresent;
    u32     exceptionMask;
    void    (*ExceptionDestination) ( void );
    void    *exceptionReturn;
} DBInterface;

extern DBInterface* __DBInterface;

void DBInit(void);
void DBInitComm(int* inputFlagPtr, int* mtrCallback);
static void __DBExceptionDestination(void);

int __DBIsExceptionMarked(u8 exception);

#ifdef __cplusplus
}
#endif

#endif
