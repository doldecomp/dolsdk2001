#ifndef _DOLPHIN_AXFX_H_
#define _DOLPHIN_AXFX_H_

struct AXFX_REVSTD_DELAYLINE {
    /* 0x00 */ long inPoint;
    /* 0x04 */ long outPoint;
    /* 0x08 */ long length;
    /* 0x0C */ float * inputs;
    /* 0x10 */ float lastOutput;
};

struct AXFX_REVSTD_WORK {
    /* 0x000 */ struct AXFX_REVSTD_DELAYLINE AP[6];
    /* 0x078 */ struct AXFX_REVSTD_DELAYLINE C[6];
    /* 0x0F0 */ float allPassCoeff;
    /* 0x0F4 */ float combCoef[6];
    /* 0x10C */ float lpLastout[3];
    /* 0x118 */ float level;
    /* 0x11C */ float damping;
    /* 0x120 */ long preDelayTime;
    /* 0x124 */ float * preDelayLine[3];
    /* 0x130 */ float * preDelayPtr[3];
};

struct AXFX_REVERBSTD {
    /* 0x000 */ struct AXFX_REVSTD_WORK rv;
    /* 0x13C */ u8 tempDisableFX;
    /* 0x140 */ float coloration;
    /* 0x144 */ float mix;
    /* 0x148 */ float time;
    /* 0x14C */ float damping;
    /* 0x150 */ float preDelay;
};

struct AXFX_BUFFERUPDATE {
    /* 0x00 */ long * left;
    /* 0x04 */ long * right;
    /* 0x08 */ long * surround;
};

// REVHI Structs

struct AXFX_REVHI_DELAYLINE {
    /* 0x00 */ long inPoint;
    /* 0x04 */ long outPoint;
    /* 0x08 */ long length;
    /* 0x0C */ float * inputs;
    /* 0x10 */ float lastOutput;
};

struct AXFX_REVHI_WORK {
    /* 0x000 */ struct AXFX_REVHI_DELAYLINE AP[9];
    /* 0x0B4 */ struct AXFX_REVHI_DELAYLINE C[9];
    /* 0x168 */ float allPassCoeff;
    /* 0x16C */ float combCoef[9];
    /* 0x190 */ float lpLastout[3];
    /* 0x19C */ float level;
    /* 0x1A0 */ float damping;
    /* 0x1A4 */ long preDelayTime;
    /* 0x1A8 */ float crosstalk;
    /* 0x1AC */ float * preDelayLine[3];
    /* 0x1B8 */ float * preDelayPtr[3];
};

struct AXFX_REVERBHI {
    /* 0x000 */ struct AXFX_REVHI_WORK rv;
    /* 0x1C4 */ u8 tempDisableFX;
    /* 0x1C8 */ float coloration;
    /* 0x1CC */ float mix;
    /* 0x1D0 */ float time;
    /* 0x1D4 */ float damping;
    /* 0x1D8 */ float preDelay;
    /* 0x1DC */ float crosstalk;
};

// reverb_std.c
int AXFXReverbStdInit(struct AXFX_REVERBSTD * rev);
int AXFXReverbStdShutdown(struct AXFX_REVERBSTD * rev);
int AXFXReverbStdSettings(struct AXFX_REVERBSTD * rev);
void AXFXReverbStdCallback(struct AXFX_BUFFERUPDATE * bufferUpdate, struct AXFX_REVERBSTD * reverb);

#endif // _DOLPHIN_AXFX_H_
