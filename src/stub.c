/* symbols just to satisfy the linker*/
#define TEXT_STUB(name) void name(void) {}
#define DATA_STUB(name) int name;

TEXT_STUB(_start)

TEXT_STUB(__ArenaHi)
TEXT_STUB(__ArenaLo)
TEXT_STUB(__DBIsExceptionMarked)
TEXT_STUB(__cvt_fp2unsigned)
TEXT_STUB(__div2i)
TEXT_STUB(__init_registers)
TEXT_STUB(__mod2i)
TEXT_STUB(__shr2i)
TEXT_STUB(_ctors)
TEXT_STUB(_dtors)
TEXT_STUB(_bss_init_info)
TEXT_STUB(_rom_copy_info)
TEXT_STUB(_stack_addr)
TEXT_STUB(_stack_end)
TEXT_STUB(DBInit)
TEXT_STUB(DBPrintf)
TEXT_STUB(DSPAddTask)
TEXT_STUB(DSPCheckMailToDSP)
TEXT_STUB(DSPInit)
TEXT_STUB(DSPSendMailToDSP)
TEXT_STUB(EnableMetroTRKInterrupts)
TEXT_STUB(GXBegin)
TEXT_STUB(GXLoadPosMtxImm)
TEXT_STUB(GXSetChanMatColor)
TEXT_STUB(GXSetLineWidth)
TEXT_STUB(GXSetNumChans)
TEXT_STUB(GXSetNumTexGens)
TEXT_STUB(GXSetTevOp)
TEXT_STUB(GXSetTevOrder)
TEXT_STUB(GXSetZCompLoc)
TEXT_STUB(InitMetroTRK)
TEXT_STUB(PPCHalt)
TEXT_STUB(PPCMfdec)
TEXT_STUB(PPCMfhid0)
TEXT_STUB(PPCMfhid2)
TEXT_STUB(PPCMfl2cr)
TEXT_STUB(PPCMfmsr)
TEXT_STUB(PPCMtdec)
TEXT_STUB(PPCMtdmaL)
TEXT_STUB(PPCMtdmaU)
TEXT_STUB(PPCMthid2)
TEXT_STUB(PPCMtl2cr)
TEXT_STUB(PPCMtmsr)
TEXT_STUB(PPCSync)
TEXT_STUB(VIGetTvFormat)
TEXT_STUB(cosf)
TEXT_STUB(main)
TEXT_STUB(memcmp)
TEXT_STUB(memcpy)
TEXT_STUB(memmove)
TEXT_STUB(memset)
TEXT_STUB(rand)
TEXT_STUB(srand)
TEXT_STUB(strlen)
TEXT_STUB(strncpy)
TEXT_STUB(vprintf)

DATA_STUB(__DVDLongFileNameFlag)