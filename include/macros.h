#ifndef _H_MACROS_
#define _H_MACROS_

#ifdef DEBUG
#define ASSERTLINE(file, line, cond) \
    ((cond) || (OSPanic(file, line, "Failed assertion " #cond), 0))
#else
#define ASSERTLINE(file, line, cond) (void)0
#endif
    
#define ASSERT(cond) ASSERTLINE(__FILE__, __LINE__, cond)

#endif // _H_MACROS_
