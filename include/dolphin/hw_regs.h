#ifndef _DOLPHIN_HW_REGS_H_
#define _DOLPHIN_HW_REGS_H_

#ifdef __MWERKS__
volatile u16 __VIRegs[59]     : 0xCC002000;
volatile u32 __PIRegs[12]     : 0xCC003000;
volatile u16 __MEMRegs[64]    : 0xCC004000;
volatile u16 __DSPRegs[]      : 0xCC005000;
volatile u32 __SIRegs[0x100]  : 0xCC006400;
volatile u32 __EXIRegs[0x40]  : 0xCC006800;
volatile u32 __AIRegs[8]      : 0xCC006C00;
#else
#define __VIRegs         ((volatile u16 *)0xCC002000)
#define __PIRegs         ((volatile u32 *)0xCC003000)
#define __MEMRegs        ((volatile u16 *)0xCC004000)
#define __DSPRegs        ((volatile u16 *)0xCC005000)
#define __SIRegs         ((volatile u32 *)0xCC006400)
#define __EXIRegs        ((volatile u32 *)0xCC006800)
#define __AIRegs         ((volatile u32 *)0xCC006C00)
#endif

#endif
