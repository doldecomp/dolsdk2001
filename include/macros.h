#ifndef _H_MACROS_
#define _H_MACROS_

#ifdef DEBUG
#define ASSERTLINE(file, line, cond) \
    ((cond) || (OSPanic(file, line, "Failed assertion " #cond), 0))

#define ASSERTMSGLINE(file, line, cond, msg) \
    ((cond) || (OSPanic(file, line, msg), 0))

// This is dumb but we dont have a Metrowerks way to do variadic macros in the macro to make this done in a not scrubby way.
#define ASSERTMSG1LINE(file, line, cond, msg, arg1) \
    ((cond) || (OSPanic(file, line, msg, arg1), 0))

#else
#define ASSERTLINE(file, line, cond) (void)0
#define ASSERTMSGLINE(file, line, cond, msg) (void)0
#define ASSERTMSG1LINE(file, line, cond, msg, arg1) (void)0
#endif
    
#define ASSERT(cond) ASSERTLINE(__FILE__, __LINE__, cond)

#endif // _H_MACROS_
