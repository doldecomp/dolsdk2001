#ifndef _DOLPHIN_LIBC_
#define _DOLPHIN_LIBC_

#ifdef __cplusplus
extern "C" {
#endif

u32 strlen(const char *str);
void memcpy(void *s1, void *s2, int n);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_LIBC_
