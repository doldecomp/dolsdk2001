#ifndef _DOLPHIN_OSIC_H_
#define _DOLPHIN_OSIC_H_

void ICFlashInvalidate(void);
void ICEnable(void);
void ICDisable(void);
void ICFreeze(void);
void ICUnfreeze(void);
void ICBlockInvalidate(void *addr);
void ICSync(void);

#endif
