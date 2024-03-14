/* gpioexi.c */

void __VIInitI2C(void);
void __VISetSCL(int value);
int __VIGetSCL(void);
void __VISetSDA(int value);
int __VIGetSDA(void);

/* i2c.c */

int __VISendI2CData(u8 slaveAddr, u8 *pData, int nBytes);

/* initphilips.c */

void __VIInitPhilips(void);
