/* 00000CA4 00000CD8  7C 08 02 A6 */	mflr r0
/* 00000CA8 00000CDC  90 01 00 04 */	stw r0, 0x4(r1)
/* 00000CAC 00000CE0  94 21 FF C0 */	stwu r1, -0x40(r1)
/* 00000CB0 00000CE4  BF 61 00 2C */	stmw r27, 0x2c(r1)
/* 00000CB4 00000CE8  98 61 00 08 */	stb r3, 0x8(r1)
/* 00000CB8 00000CEC  7C 9E 23 78 */	mr r30, r4
/* 00000CBC 00000CF0  98 A1 00 10 */	stb r5, 0x10(r1)
/* 00000CC0 00000CF4  3B 66 00 00 */	addi r27, r6, 0x0
/* 00000CC4 00000CF8  3C 60 00 00 */	lis r3, ___data_0@ha
/* 00000CC8 00000CFC  3B E3 00 00 */	addi r31, r3, ___data_0@l
/* 00000CCC 00000D00  88 60 00 00 */	lbz r3, __GXinBegin
/* 00000CD0 00000D04  28 03 00 00 */	cmplwi r3, 0x0
/* 00000CD4 00000D08  41 82 00 18 */	beq L_00000CEC
/* 00000CD8 00000D0C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000CDC 00000D10  38 80 04 F1 */	li r4, 0x4f1
/* 00000CE0 00000D14  38 BF 08 6C */	addi r5, r31, 0x86c
/* 00000CE4 00000D18  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000CE8 00000D1C  48 00 00 01 */	bl OSPanic
L_00000CEC:
/* 00000CEC 00000D20  88 01 00 08 */	lbz r0, 0x8(r1)
/* 00000CF0 00000D24  28 00 00 00 */	cmplwi r0, 0x0
/* 00000CF4 00000D28  41 82 05 98 */	beq L_0000128C
/* 00000CF8 00000D2C  38 00 00 00 */	li r0, 0x0
/* 00000CFC 00000D30  90 01 00 18 */	stw r0, 0x18(r1)
/* 00000D00 00000D34  88 1E 00 00 */	lbz r0, 0x0(r30)
/* 00000D04 00000D38  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00000D08 00000D3C  41 82 00 18 */	beq L_00000D20
/* 00000D0C 00000D40  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000D10 00000D44  38 80 04 F5 */	li r4, 0x4f5
/* 00000D14 00000D48  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000D18 00000D4C  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000D1C 00000D50  48 00 00 01 */	bl OSPanic
L_00000D20:
/* 00000D20 00000D54  80 61 00 18 */	lwz r3, 0x18(r1)
/* 00000D24 00000D58  54 64 00 36 */	clrrwi r4, r3, 4
/* 00000D28 00000D5C  88 7E 00 00 */	lbz r3, 0x0(r30)
/* 00000D2C 00000D60  7C 80 1B 78 */	or r0, r4, r3
/* 00000D30 00000D64  90 01 00 18 */	stw r0, 0x18(r1)
/* 00000D34 00000D68  88 1E 00 01 */	lbz r0, 0x1(r30)
/* 00000D38 00000D6C  54 03 00 37 */	clrrwi. r3, r0, 4
/* 00000D3C 00000D70  41 82 00 18 */	beq L_00000D54
/* 00000D40 00000D74  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000D44 00000D78  38 80 04 F6 */	li r4, 0x4f6
/* 00000D48 00000D7C  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000D4C 00000D80  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000D50 00000D84  48 00 00 01 */	bl OSPanic
L_00000D54:
/* 00000D54 00000D88  80 81 00 18 */	lwz r4, 0x18(r1)
/* 00000D58 00000D8C  54 83 07 2E */	rlwinm r3, r4, 0, 28, 23
/* 00000D5C 00000D90  88 1E 00 01 */	lbz r0, 0x1(r30)
/* 00000D60 00000D94  54 00 20 36 */	slwi r0, r0, 4
/* 00000D64 00000D98  7C 63 03 78 */	or r3, r3, r0
/* 00000D68 00000D9C  90 61 00 18 */	stw r3, 0x18(r1)
/* 00000D6C 00000DA0  88 1E 00 02 */	lbz r0, 0x2(r30)
/* 00000D70 00000DA4  54 03 00 37 */	clrrwi. r3, r0, 4
/* 00000D74 00000DA8  41 82 00 18 */	beq L_00000D8C
/* 00000D78 00000DAC  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000D7C 00000DB0  38 80 04 F7 */	li r4, 0x4f7
/* 00000D80 00000DB4  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000D84 00000DB8  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000D88 00000DBC  48 00 00 01 */	bl OSPanic
L_00000D8C:
/* 00000D8C 00000DC0  80 61 00 18 */	lwz r3, 0x18(r1)
/* 00000D90 00000DC4  54 65 06 26 */	rlwinm r5, r3, 0, 24, 19
/* 00000D94 00000DC8  88 9E 00 02 */	lbz r4, 0x2(r30)
/* 00000D98 00000DCC  54 83 40 2E */	slwi r3, r4, 8
/* 00000D9C 00000DD0  7C A0 1B 78 */	or r0, r5, r3
/* 00000DA0 00000DD4  90 01 00 18 */	stw r0, 0x18(r1)
/* 00000DA4 00000DD8  88 7E 00 03 */	lbz r3, 0x3(r30)
/* 00000DA8 00000DDC  54 60 00 37 */	clrrwi. r0, r3, 4
/* 00000DAC 00000DE0  41 82 00 18 */	beq L_00000DC4
/* 00000DB0 00000DE4  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000DB4 00000DE8  38 80 04 F8 */	li r4, 0x4f8
/* 00000DB8 00000DEC  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000DBC 00000DF0  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000DC0 00000DF4  48 00 00 01 */	bl OSPanic
L_00000DC4:
/* 00000DC4 00000DF8  80 61 00 18 */	lwz r3, 0x18(r1)
/* 00000DC8 00000DFC  54 60 05 1E */	rlwinm r0, r3, 0, 20, 15
/* 00000DCC 00000E00  88 7E 00 03 */	lbz r3, 0x3(r30)
/* 00000DD0 00000E04  54 65 60 26 */	slwi r5, r3, 12
/* 00000DD4 00000E08  7C 04 2B 78 */	or r4, r0, r5
/* 00000DD8 00000E0C  90 81 00 18 */	stw r4, 0x18(r1)
/* 00000DDC 00000E10  88 1E 00 04 */	lbz r0, 0x4(r30)
/* 00000DE0 00000E14  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00000DE4 00000E18  41 82 00 18 */	beq L_00000DFC
/* 00000DE8 00000E1C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000DEC 00000E20  38 80 04 F9 */	li r4, 0x4f9
/* 00000DF0 00000E24  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000DF4 00000E28  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000DF8 00000E2C  48 00 00 01 */	bl OSPanic
L_00000DFC:
/* 00000DFC 00000E30  80 01 00 18 */	lwz r0, 0x18(r1)
/* 00000E00 00000E34  54 04 04 16 */	rlwinm r4, r0, 0, 16, 11
/* 00000E04 00000E38  88 7E 00 04 */	lbz r3, 0x4(r30)
/* 00000E08 00000E3C  54 60 80 1E */	slwi r0, r3, 16
/* 00000E0C 00000E40  7C 83 03 78 */	or r3, r4, r0
/* 00000E10 00000E44  90 61 00 18 */	stw r3, 0x18(r1)
/* 00000E14 00000E48  88 9E 00 05 */	lbz r4, 0x5(r30)
/* 00000E18 00000E4C  54 83 00 37 */	clrrwi. r3, r4, 4
/* 00000E1C 00000E50  41 82 00 18 */	beq L_00000E34
/* 00000E20 00000E54  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000E24 00000E58  38 80 04 FA */	li r4, 0x4fa
/* 00000E28 00000E5C  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000E2C 00000E60  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000E30 00000E64  48 00 00 01 */	bl OSPanic
L_00000E34:
/* 00000E34 00000E68  80 01 00 18 */	lwz r0, 0x18(r1)
/* 00000E38 00000E6C  54 03 03 0E */	rlwinm r3, r0, 0, 12, 7
/* 00000E3C 00000E70  88 1E 00 05 */	lbz r0, 0x5(r30)
/* 00000E40 00000E74  54 00 A0 16 */	slwi r0, r0, 20
/* 00000E44 00000E78  7C 60 03 78 */	or r0, r3, r0
/* 00000E48 00000E7C  90 01 00 18 */	stw r0, 0x18(r1)
/* 00000E4C 00000E80  80 01 00 18 */	lwz r0, 0x18(r1)
/* 00000E50 00000E84  54 00 02 3E */	clrlwi r0, r0, 8
/* 00000E54 00000E88  64 00 01 00 */	oris r0, r0, 0x100
/* 00000E58 00000E8C  90 01 00 18 */	stw r0, 0x18(r1)
/* 00000E5C 00000E90  38 00 00 00 */	li r0, 0x0
/* 00000E60 00000E94  90 01 00 1C */	stw r0, 0x1c(r1)
/* 00000E64 00000E98  88 7E 00 06 */	lbz r3, 0x6(r30)
/* 00000E68 00000E9C  54 60 00 37 */	clrrwi. r0, r3, 4
/* 00000E6C 00000EA0  41 82 00 18 */	beq L_00000E84
/* 00000E70 00000EA4  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000E74 00000EA8  38 80 04 FE */	li r4, 0x4fe
/* 00000E78 00000EAC  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000E7C 00000EB0  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000E80 00000EB4  48 00 00 01 */	bl OSPanic
L_00000E84:
/* 00000E84 00000EB8  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 00000E88 00000EBC  54 03 00 36 */	clrrwi r3, r0, 4
/* 00000E8C 00000EC0  88 1E 00 06 */	lbz r0, 0x6(r30)
/* 00000E90 00000EC4  7C 63 03 78 */	or r3, r3, r0
/* 00000E94 00000EC8  90 61 00 1C */	stw r3, 0x1c(r1)
/* 00000E98 00000ECC  88 1E 00 07 */	lbz r0, 0x7(r30)
/* 00000E9C 00000ED0  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00000EA0 00000ED4  41 82 00 18 */	beq L_00000EB8
/* 00000EA4 00000ED8  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000EA8 00000EDC  38 80 04 FF */	li r4, 0x4ff
/* 00000EAC 00000EE0  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000EB0 00000EE4  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000EB4 00000EE8  48 00 00 01 */	bl OSPanic
L_00000EB8:
/* 00000EB8 00000EEC  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 00000EBC 00000EF0  54 04 07 2E */	rlwinm r4, r0, 0, 28, 23
/* 00000EC0 00000EF4  88 7E 00 07 */	lbz r3, 0x7(r30)
/* 00000EC4 00000EF8  54 60 20 36 */	slwi r0, r3, 4
/* 00000EC8 00000EFC  7C 80 03 78 */	or r0, r4, r0
/* 00000ECC 00000F00  90 01 00 1C */	stw r0, 0x1c(r1)
/* 00000ED0 00000F04  88 1E 00 08 */	lbz r0, 0x8(r30)
/* 00000ED4 00000F08  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00000ED8 00000F0C  41 82 00 18 */	beq L_00000EF0
/* 00000EDC 00000F10  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000EE0 00000F14  38 80 05 00 */	li r4, 0x500
/* 00000EE4 00000F18  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000EE8 00000F1C  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000EEC 00000F20  48 00 00 01 */	bl OSPanic
L_00000EF0:
/* 00000EF0 00000F24  80 81 00 1C */	lwz r4, 0x1c(r1)
/* 00000EF4 00000F28  54 83 06 26 */	rlwinm r3, r4, 0, 24, 19
/* 00000EF8 00000F2C  88 1E 00 08 */	lbz r0, 0x8(r30)
/* 00000EFC 00000F30  54 00 40 2E */	slwi r0, r0, 8
/* 00000F00 00000F34  7C 60 03 78 */	or r0, r3, r0
/* 00000F04 00000F38  90 01 00 1C */	stw r0, 0x1c(r1)
/* 00000F08 00000F3C  88 1E 00 09 */	lbz r0, 0x9(r30)
/* 00000F0C 00000F40  54 03 00 37 */	clrrwi. r3, r0, 4
/* 00000F10 00000F44  41 82 00 18 */	beq L_00000F28
/* 00000F14 00000F48  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000F18 00000F4C  38 80 05 01 */	li r4, 0x501
/* 00000F1C 00000F50  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000F20 00000F54  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000F24 00000F58  48 00 00 01 */	bl OSPanic
L_00000F28:
/* 00000F28 00000F5C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 00000F2C 00000F60  54 03 05 1E */	rlwinm r3, r0, 0, 20, 15
/* 00000F30 00000F64  88 1E 00 09 */	lbz r0, 0x9(r30)
/* 00000F34 00000F68  54 00 60 26 */	slwi r0, r0, 12
/* 00000F38 00000F6C  7C 63 03 78 */	or r3, r3, r0
/* 00000F3C 00000F70  90 61 00 1C */	stw r3, 0x1c(r1)
/* 00000F40 00000F74  88 1E 00 0A */	lbz r0, 0xa(r30)
/* 00000F44 00000F78  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00000F48 00000F7C  41 82 00 18 */	beq L_00000F60
/* 00000F4C 00000F80  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000F50 00000F84  38 80 05 02 */	li r4, 0x502
/* 00000F54 00000F88  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000F58 00000F8C  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000F5C 00000F90  48 00 00 01 */	bl OSPanic
L_00000F60:
/* 00000F60 00000F94  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 00000F64 00000F98  54 04 04 16 */	rlwinm r4, r0, 0, 16, 11
/* 00000F68 00000F9C  88 7E 00 0A */	lbz r3, 0xa(r30)
/* 00000F6C 00000FA0  54 60 80 1E */	slwi r0, r3, 16
/* 00000F70 00000FA4  7C 80 03 78 */	or r0, r4, r0
/* 00000F74 00000FA8  90 01 00 1C */	stw r0, 0x1c(r1)
/* 00000F78 00000FAC  88 1E 00 0B */	lbz r0, 0xb(r30)
/* 00000F7C 00000FB0  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00000F80 00000FB4  41 82 00 18 */	beq L_00000F98
/* 00000F84 00000FB8  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000F88 00000FBC  38 80 05 03 */	li r4, 0x503
/* 00000F8C 00000FC0  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000F90 00000FC4  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000F94 00000FC8  48 00 00 01 */	bl OSPanic
L_00000F98:
/* 00000F98 00000FCC  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 00000F9C 00000FD0  54 63 03 0E */	rlwinm r3, r3, 0, 12, 7
/* 00000FA0 00000FD4  88 9E 00 0B */	lbz r4, 0xb(r30)
/* 00000FA4 00000FD8  54 80 A0 16 */	slwi r0, r4, 20
/* 00000FA8 00000FDC  7C 60 03 78 */	or r0, r3, r0
/* 00000FAC 00000FE0  90 01 00 1C */	stw r0, 0x1c(r1)
/* 00000FB0 00000FE4  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 00000FB4 00000FE8  54 60 02 3E */	clrlwi r0, r3, 8
/* 00000FB8 00000FEC  64 00 02 00 */	oris r0, r0, 0x200
/* 00000FBC 00000FF0  90 01 00 1C */	stw r0, 0x1c(r1)
/* 00000FC0 00000FF4  38 60 00 00 */	li r3, 0x0
/* 00000FC4 00000FF8  90 61 00 20 */	stw r3, 0x20(r1)
/* 00000FC8 00000FFC  88 9E 00 0C */	lbz r4, 0xc(r30)
/* 00000FCC 00001000  54 80 00 37 */	clrrwi. r0, r4, 4
/* 00000FD0 00001004  41 82 00 18 */	beq L_00000FE8
/* 00000FD4 00001008  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00000FD8 0000100C  38 80 05 07 */	li r4, 0x507
/* 00000FDC 00001010  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00000FE0 00001014  4C C6 31 82 */	crclr 4*cr1+eq
/* 00000FE4 00001018  48 00 00 01 */	bl OSPanic
L_00000FE8:
/* 00000FE8 0000101C  80 61 00 20 */	lwz r3, 0x20(r1)
/* 00000FEC 00001020  54 63 00 36 */	clrrwi r3, r3, 4
/* 00000FF0 00001024  88 1E 00 0C */	lbz r0, 0xc(r30)
/* 00000FF4 00001028  7C 63 03 78 */	or r3, r3, r0
/* 00000FF8 0000102C  90 61 00 20 */	stw r3, 0x20(r1)
/* 00000FFC 00001030  88 1E 00 0D */	lbz r0, 0xd(r30)
/* 00001000 00001034  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00001004 00001038  41 82 00 18 */	beq L_0000101C
/* 00001008 0000103C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 0000100C 00001040  38 80 05 08 */	li r4, 0x508
/* 00001010 00001044  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001014 00001048  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001018 0000104C  48 00 00 01 */	bl OSPanic
L_0000101C:
/* 0000101C 00001050  80 01 00 20 */	lwz r0, 0x20(r1)
/* 00001020 00001054  54 03 07 2E */	rlwinm r3, r0, 0, 28, 23
/* 00001024 00001058  88 1E 00 0D */	lbz r0, 0xd(r30)
/* 00001028 0000105C  54 00 20 36 */	slwi r0, r0, 4
/* 0000102C 00001060  7C 60 03 78 */	or r0, r3, r0
/* 00001030 00001064  90 01 00 20 */	stw r0, 0x20(r1)
/* 00001034 00001068  88 1E 00 0E */	lbz r0, 0xe(r30)
/* 00001038 0000106C  54 00 00 37 */	clrrwi. r0, r0, 4
/* 0000103C 00001070  41 82 00 18 */	beq L_00001054
/* 00001040 00001074  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00001044 00001078  38 80 05 09 */	li r4, 0x509
/* 00001048 0000107C  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 0000104C 00001080  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001050 00001084  48 00 00 01 */	bl OSPanic
L_00001054:
/* 00001054 00001088  80 01 00 20 */	lwz r0, 0x20(r1)
/* 00001058 0000108C  54 03 06 26 */	rlwinm r3, r0, 0, 24, 19
/* 0000105C 00001090  88 1E 00 0E */	lbz r0, 0xe(r30)
/* 00001060 00001094  54 00 40 2E */	slwi r0, r0, 8
/* 00001064 00001098  7C 60 03 78 */	or r0, r3, r0
/* 00001068 0000109C  90 01 00 20 */	stw r0, 0x20(r1)
/* 0000106C 000010A0  88 1E 00 0F */	lbz r0, 0xf(r30)
/* 00001070 000010A4  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00001074 000010A8  41 82 00 18 */	beq L_0000108C
/* 00001078 000010AC  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 0000107C 000010B0  38 80 05 0A */	li r4, 0x50a
/* 00001080 000010B4  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001084 000010B8  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001088 000010BC  48 00 00 01 */	bl OSPanic
L_0000108C:
/* 0000108C 000010C0  80 01 00 20 */	lwz r0, 0x20(r1)
/* 00001090 000010C4  54 03 05 1E */	rlwinm r3, r0, 0, 20, 15
/* 00001094 000010C8  88 1E 00 0F */	lbz r0, 0xf(r30)
/* 00001098 000010CC  54 00 60 26 */	slwi r0, r0, 12
/* 0000109C 000010D0  7C 60 03 78 */	or r0, r3, r0
/* 000010A0 000010D4  90 01 00 20 */	stw r0, 0x20(r1)
/* 000010A4 000010D8  88 1E 00 10 */	lbz r0, 0x10(r30)
/* 000010A8 000010DC  54 00 00 37 */	clrrwi. r0, r0, 4
/* 000010AC 000010E0  41 82 00 18 */	beq L_000010C4
/* 000010B0 000010E4  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 000010B4 000010E8  38 80 05 0B */	li r4, 0x50b
/* 000010B8 000010EC  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000010BC 000010F0  4C C6 31 82 */	crclr 4*cr1+eq
/* 000010C0 000010F4  48 00 00 01 */	bl OSPanic
L_000010C4:
/* 000010C4 000010F8  80 01 00 20 */	lwz r0, 0x20(r1)
/* 000010C8 000010FC  54 03 04 16 */	rlwinm r3, r0, 0, 16, 11
/* 000010CC 00001100  88 1E 00 10 */	lbz r0, 0x10(r30)
/* 000010D0 00001104  54 00 80 1E */	slwi r0, r0, 16
/* 000010D4 00001108  7C 60 03 78 */	or r0, r3, r0
/* 000010D8 0000110C  90 01 00 20 */	stw r0, 0x20(r1)
/* 000010DC 00001110  88 1E 00 11 */	lbz r0, 0x11(r30)
/* 000010E0 00001114  54 00 00 37 */	clrrwi. r0, r0, 4
/* 000010E4 00001118  41 82 00 18 */	beq L_000010FC
/* 000010E8 0000111C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 000010EC 00001120  38 80 05 0C */	li r4, 0x50c
/* 000010F0 00001124  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000010F4 00001128  4C C6 31 82 */	crclr 4*cr1+eq
/* 000010F8 0000112C  48 00 00 01 */	bl OSPanic
L_000010FC:
/* 000010FC 00001130  80 01 00 20 */	lwz r0, 0x20(r1)
/* 00001100 00001134  54 03 03 0E */	rlwinm r3, r0, 0, 12, 7
/* 00001104 00001138  88 1E 00 11 */	lbz r0, 0x11(r30)
/* 00001108 0000113C  54 00 A0 16 */	slwi r0, r0, 20
/* 0000110C 00001140  7C 60 03 78 */	or r0, r3, r0
/* 00001110 00001144  90 01 00 20 */	stw r0, 0x20(r1)
/* 00001114 00001148  80 01 00 20 */	lwz r0, 0x20(r1)
/* 00001118 0000114C  54 00 02 3E */	clrlwi r0, r0, 8
/* 0000111C 00001150  64 00 03 00 */	oris r0, r0, 0x300
/* 00001120 00001154  90 01 00 20 */	stw r0, 0x20(r1)
/* 00001124 00001158  38 00 00 00 */	li r0, 0x0
/* 00001128 0000115C  90 01 00 24 */	stw r0, 0x24(r1)
/* 0000112C 00001160  88 1E 00 12 */	lbz r0, 0x12(r30)
/* 00001130 00001164  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00001134 00001168  41 82 00 18 */	beq L_0000114C
/* 00001138 0000116C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 0000113C 00001170  38 80 05 10 */	li r4, 0x510
/* 00001140 00001174  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001144 00001178  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001148 0000117C  48 00 00 01 */	bl OSPanic
L_0000114C:
/* 0000114C 00001180  80 01 00 24 */	lwz r0, 0x24(r1)
/* 00001150 00001184  54 03 00 36 */	clrrwi r3, r0, 4
/* 00001154 00001188  88 1E 00 12 */	lbz r0, 0x12(r30)
/* 00001158 0000118C  7C 60 03 78 */	or r0, r3, r0
/* 0000115C 00001190  90 01 00 24 */	stw r0, 0x24(r1)
/* 00001160 00001194  88 1E 00 13 */	lbz r0, 0x13(r30)
/* 00001164 00001198  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00001168 0000119C  41 82 00 18 */	beq L_00001180
/* 0000116C 000011A0  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00001170 000011A4  38 80 05 11 */	li r4, 0x511
/* 00001174 000011A8  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001178 000011AC  4C C6 31 82 */	crclr 4*cr1+eq
/* 0000117C 000011B0  48 00 00 01 */	bl OSPanic
L_00001180:
/* 00001180 000011B4  80 01 00 24 */	lwz r0, 0x24(r1)
/* 00001184 000011B8  54 03 07 2E */	rlwinm r3, r0, 0, 28, 23
/* 00001188 000011BC  88 1E 00 13 */	lbz r0, 0x13(r30)
/* 0000118C 000011C0  54 00 20 36 */	slwi r0, r0, 4
/* 00001190 000011C4  7C 60 03 78 */	or r0, r3, r0
/* 00001194 000011C8  90 01 00 24 */	stw r0, 0x24(r1)
/* 00001198 000011CC  88 1E 00 14 */	lbz r0, 0x14(r30)
/* 0000119C 000011D0  54 00 00 37 */	clrrwi. r0, r0, 4
/* 000011A0 000011D4  41 82 00 18 */	beq L_000011B8
/* 000011A4 000011D8  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 000011A8 000011DC  38 80 05 12 */	li r4, 0x512
/* 000011AC 000011E0  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000011B0 000011E4  4C C6 31 82 */	crclr 4*cr1+eq
/* 000011B4 000011E8  48 00 00 01 */	bl OSPanic
L_000011B8:
/* 000011B8 000011EC  80 01 00 24 */	lwz r0, 0x24(r1)
/* 000011BC 000011F0  54 03 06 26 */	rlwinm r3, r0, 0, 24, 19
/* 000011C0 000011F4  88 1E 00 14 */	lbz r0, 0x14(r30)
/* 000011C4 000011F8  54 00 40 2E */	slwi r0, r0, 8
/* 000011C8 000011FC  7C 60 03 78 */	or r0, r3, r0
/* 000011CC 00001200  90 01 00 24 */	stw r0, 0x24(r1)
/* 000011D0 00001204  88 1E 00 15 */	lbz r0, 0x15(r30)
/* 000011D4 00001208  54 00 00 37 */	clrrwi. r0, r0, 4
/* 000011D8 0000120C  41 82 00 18 */	beq L_000011F0
/* 000011DC 00001210  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 000011E0 00001214  38 80 05 13 */	li r4, 0x513
/* 000011E4 00001218  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000011E8 0000121C  4C C6 31 82 */	crclr 4*cr1+eq
/* 000011EC 00001220  48 00 00 01 */	bl OSPanic
L_000011F0:
/* 000011F0 00001224  80 01 00 24 */	lwz r0, 0x24(r1)
/* 000011F4 00001228  54 03 05 1E */	rlwinm r3, r0, 0, 20, 15
/* 000011F8 0000122C  88 1E 00 15 */	lbz r0, 0x15(r30)
/* 000011FC 00001230  54 00 60 26 */	slwi r0, r0, 12
/* 00001200 00001234  7C 60 03 78 */	or r0, r3, r0
/* 00001204 00001238  90 01 00 24 */	stw r0, 0x24(r1)
/* 00001208 0000123C  88 1E 00 16 */	lbz r0, 0x16(r30)
/* 0000120C 00001240  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00001210 00001244  41 82 00 18 */	beq L_00001228
/* 00001214 00001248  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00001218 0000124C  38 80 05 14 */	li r4, 0x514
/* 0000121C 00001250  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001220 00001254  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001224 00001258  48 00 00 01 */	bl OSPanic
L_00001228:
/* 00001228 0000125C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 0000122C 00001260  54 03 04 16 */	rlwinm r3, r0, 0, 16, 11
/* 00001230 00001264  88 1E 00 16 */	lbz r0, 0x16(r30)
/* 00001234 00001268  54 00 80 1E */	slwi r0, r0, 16
/* 00001238 0000126C  7C 60 03 78 */	or r0, r3, r0
/* 0000123C 00001270  90 01 00 24 */	stw r0, 0x24(r1)
/* 00001240 00001274  88 1E 00 17 */	lbz r0, 0x17(r30)
/* 00001244 00001278  54 00 00 37 */	clrrwi. r0, r0, 4
/* 00001248 0000127C  41 82 00 18 */	beq L_00001260
/* 0000124C 00001280  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00001250 00001284  38 80 05 15 */	li r4, 0x515
/* 00001254 00001288  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001258 0000128C  4C C6 31 82 */	crclr 4*cr1+eq
/* 0000125C 00001290  48 00 00 01 */	bl OSPanic
L_00001260:
/* 00001260 00001294  80 01 00 24 */	lwz r0, 0x24(r1)
/* 00001264 00001298  54 03 03 0E */	rlwinm r3, r0, 0, 12, 7
/* 00001268 0000129C  88 1E 00 17 */	lbz r0, 0x17(r30)
/* 0000126C 000012A0  54 00 A0 16 */	slwi r0, r0, 20
/* 00001270 000012A4  7C 60 03 78 */	or r0, r3, r0
/* 00001274 000012A8  90 01 00 24 */	stw r0, 0x24(r1)
/* 00001278 000012AC  80 01 00 24 */	lwz r0, 0x24(r1)
/* 0000127C 000012B0  54 00 02 3E */	clrlwi r0, r0, 8
/* 00001280 000012B4  64 00 04 00 */	oris r0, r0, 0x400
/* 00001284 000012B8  90 01 00 24 */	stw r0, 0x24(r1)
/* 00001288 000012BC  48 00 00 34 */	b L_000012BC
L_0000128C:
/* 0000128C 000012C0  3C 60 01 66 */	lis r3, 0x166
/* 00001290 000012C4  38 63 66 66 */	addi r3, r3, 0x6666
/* 00001294 000012C8  90 61 00 18 */	stw r3, 0x18(r1)
/* 00001298 000012CC  3C 60 02 66 */	lis r3, 0x266
/* 0000129C 000012D0  38 63 66 66 */	addi r3, r3, 0x6666
/* 000012A0 000012D4  90 61 00 1C */	stw r3, 0x1c(r1)
/* 000012A4 000012D8  3C 80 03 66 */	lis r4, 0x366
/* 000012A8 000012DC  38 84 66 66 */	addi r4, r4, 0x6666
/* 000012AC 000012E0  90 81 00 20 */	stw r4, 0x20(r1)
/* 000012B0 000012E4  3C 60 04 66 */	lis r3, 0x466
/* 000012B4 000012E8  38 63 66 66 */	addi r3, r3, 0x6666
/* 000012B8 000012EC  90 61 00 24 */	stw r3, 0x24(r1)
L_000012BC:
/* 000012BC 000012F0  38 00 00 61 */	li r0, 0x61
/* 000012C0 000012F4  3C 60 CC 01 */	lis r3, 0xcc01
/* 000012C4 000012F8  98 03 80 00 */	stb r0, -0x8000(r3)
/* 000012C8 000012FC  80 01 00 18 */	lwz r0, 0x18(r1)
/* 000012CC 00001300  3C 60 CC 01 */	lis r3, 0xcc01
/* 000012D0 00001304  90 03 80 00 */	stw r0, -0x8000(r3)
/* 000012D4 00001308  80 A1 00 18 */	lwz r5, 0x18(r1)
/* 000012D8 0000130C  80 80 00 00 */	lwz r4, __gxVerif
/* 000012DC 00001310  80 61 00 18 */	lwz r3, 0x18(r1)
/* 000012E0 00001314  54 63 55 BA */	rlwinm r3, r3, 10, 22, 29
/* 000012E4 00001318  38 03 0C C8 */	addi r0, r3, 0xcc8
/* 000012E8 0000131C  7C A4 01 2E */	stwx r5, r4, r0
/* 000012EC 00001320  38 00 00 61 */	li r0, 0x61
/* 000012F0 00001324  3C 60 CC 01 */	lis r3, 0xcc01
/* 000012F4 00001328  98 03 80 00 */	stb r0, -0x8000(r3)
/* 000012F8 0000132C  80 01 00 1C */	lwz r0, 0x1c(r1)
/* 000012FC 00001330  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001300 00001334  90 03 80 00 */	stw r0, -0x8000(r3)
/* 00001304 00001338  80 A1 00 1C */	lwz r5, 0x1c(r1)
/* 00001308 0000133C  80 80 00 00 */	lwz r4, __gxVerif
/* 0000130C 00001340  80 61 00 1C */	lwz r3, 0x1c(r1)
/* 00001310 00001344  54 63 55 BA */	rlwinm r3, r3, 10, 22, 29
/* 00001314 00001348  38 03 0C C8 */	addi r0, r3, 0xcc8
/* 00001318 0000134C  7C A4 01 2E */	stwx r5, r4, r0
/* 0000131C 00001350  38 00 00 61 */	li r0, 0x61
/* 00001320 00001354  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001324 00001358  98 03 80 00 */	stb r0, -0x8000(r3)
/* 00001328 0000135C  80 01 00 20 */	lwz r0, 0x20(r1)
/* 0000132C 00001360  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001330 00001364  90 03 80 00 */	stw r0, -0x8000(r3)
/* 00001334 00001368  80 A1 00 20 */	lwz r5, 0x20(r1)
/* 00001338 0000136C  80 80 00 00 */	lwz r4, __gxVerif
/* 0000133C 00001370  80 61 00 20 */	lwz r3, 0x20(r1)
/* 00001340 00001374  54 63 55 BA */	rlwinm r3, r3, 10, 22, 29
/* 00001344 00001378  38 03 0C C8 */	addi r0, r3, 0xcc8
/* 00001348 0000137C  7C A4 01 2E */	stwx r5, r4, r0
/* 0000134C 00001380  38 80 00 61 */	li r4, 0x61
/* 00001350 00001384  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001354 00001388  98 83 80 00 */	stb r4, -0x8000(r3)
/* 00001358 0000138C  80 01 00 24 */	lwz r0, 0x24(r1)
/* 0000135C 00001390  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001360 00001394  90 03 80 00 */	stw r0, -0x8000(r3)
/* 00001364 00001398  80 A1 00 24 */	lwz r5, 0x24(r1)
/* 00001368 0000139C  80 80 00 00 */	lwz r4, __gxVerif
/* 0000136C 000013A0  80 61 00 24 */	lwz r3, 0x24(r1)
/* 00001370 000013A4  54 63 55 BA */	rlwinm r3, r3, 10, 22, 29
/* 00001374 000013A8  38 03 0C C8 */	addi r0, r3, 0xcc8
/* 00001378 000013AC  7C A4 01 2E */	stwx r5, r4, r0
/* 0000137C 000013B0  3B A0 00 00 */	li r29, 0x0
/* 00001380 000013B4  57 A0 02 3E */	clrlwi r0, r29, 8
/* 00001384 000013B8  64 1D 53 00 */	oris r29, r0, 0x5300
/* 00001388 000013BC  3B 80 00 00 */	li r28, 0x0
/* 0000138C 000013C0  57 80 02 3E */	clrlwi r0, r28, 8
/* 00001390 000013C4  64 1C 54 00 */	oris r28, r0, 0x5400
/* 00001394 000013C8  88 01 00 10 */	lbz r0, 0x10(r1)
/* 00001398 000013CC  28 00 00 00 */	cmplwi r0, 0x0
/* 0000139C 000013D0  41 82 01 50 */	beq L_000014EC
/* 000013A0 000013D4  88 1B 00 00 */	lbz r0, 0x0(r27)
/* 000013A4 000013D8  54 00 00 33 */	clrrwi. r0, r0, 6
/* 000013A8 000013DC  41 82 00 18 */	beq L_000013C0
/* 000013AC 000013E0  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 000013B0 000013E4  38 80 05 2E */	li r4, 0x52e
/* 000013B4 000013E8  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000013B8 000013EC  4C C6 31 82 */	crclr 4*cr1+eq
/* 000013BC 000013F0  48 00 00 01 */	bl OSPanic
L_000013C0:
/* 000013C0 000013F4  57 A3 00 32 */	clrrwi r3, r29, 6
/* 000013C4 000013F8  88 1B 00 00 */	lbz r0, 0x0(r27)
/* 000013C8 000013FC  7C 7D 03 78 */	or r29, r3, r0
/* 000013CC 00001400  88 1B 00 01 */	lbz r0, 0x1(r27)
/* 000013D0 00001404  54 03 00 33 */	clrrwi. r3, r0, 6
/* 000013D4 00001408  41 82 00 18 */	beq L_000013EC
/* 000013D8 0000140C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 000013DC 00001410  38 80 05 2F */	li r4, 0x52f
/* 000013E0 00001414  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000013E4 00001418  4C C6 31 82 */	crclr 4*cr1+eq
/* 000013E8 0000141C  48 00 00 01 */	bl OSPanic
L_000013EC:
/* 000013EC 00001420  57 A3 06 A6 */	rlwinm r3, r29, 0, 26, 19
/* 000013F0 00001424  88 1B 00 01 */	lbz r0, 0x1(r27)
/* 000013F4 00001428  54 00 30 32 */	slwi r0, r0, 6
/* 000013F8 0000142C  7C 7D 03 78 */	or r29, r3, r0
/* 000013FC 00001430  88 1B 00 02 */	lbz r0, 0x2(r27)
/* 00001400 00001434  54 00 00 33 */	clrrwi. r0, r0, 6
/* 00001404 00001438  41 82 00 18 */	beq L_0000141C
/* 00001408 0000143C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 0000140C 00001440  38 80 05 30 */	li r4, 0x530
/* 00001410 00001444  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001414 00001448  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001418 0000144C  48 00 00 01 */	bl OSPanic
L_0000141C:
/* 0000141C 00001450  57 A3 05 1A */	rlwinm r3, r29, 0, 20, 13
/* 00001420 00001454  88 1B 00 02 */	lbz r0, 0x2(r27)
/* 00001424 00001458  54 00 60 26 */	slwi r0, r0, 12
/* 00001428 0000145C  7C 7D 03 78 */	or r29, r3, r0
/* 0000142C 00001460  88 1B 00 03 */	lbz r0, 0x3(r27)
/* 00001430 00001464  54 00 00 33 */	clrrwi. r0, r0, 6
/* 00001434 00001468  41 82 00 18 */	beq L_0000144C
/* 00001438 0000146C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 0000143C 00001470  38 80 05 31 */	li r4, 0x531
/* 00001440 00001474  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001444 00001478  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001448 0000147C  48 00 00 01 */	bl OSPanic
L_0000144C:
/* 0000144C 00001480  57 A4 03 8E */	rlwinm r4, r29, 0, 14, 7
/* 00001450 00001484  88 7B 00 03 */	lbz r3, 0x3(r27)
/* 00001454 00001488  54 60 90 1A */	slwi r0, r3, 18
/* 00001458 0000148C  7C 9D 03 78 */	or r29, r4, r0
/* 0000145C 00001490  88 1B 00 04 */	lbz r0, 0x4(r27)
/* 00001460 00001494  54 00 00 33 */	clrrwi. r0, r0, 6
/* 00001464 00001498  41 82 00 18 */	beq L_0000147C
/* 00001468 0000149C  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 0000146C 000014A0  38 80 05 32 */	li r4, 0x532
/* 00001470 000014A4  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 00001474 000014A8  4C C6 31 82 */	crclr 4*cr1+eq
/* 00001478 000014AC  48 00 00 01 */	bl OSPanic
L_0000147C:
/* 0000147C 000014B0  57 83 00 32 */	clrrwi r3, r28, 6
/* 00001480 000014B4  88 1B 00 04 */	lbz r0, 0x4(r27)
/* 00001484 000014B8  7C 7C 03 78 */	or r28, r3, r0
/* 00001488 000014BC  88 7B 00 05 */	lbz r3, 0x5(r27)
/* 0000148C 000014C0  54 60 00 33 */	clrrwi. r0, r3, 6
/* 00001490 000014C4  41 82 00 18 */	beq L_000014A8
/* 00001494 000014C8  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 00001498 000014CC  38 80 05 33 */	li r4, 0x533
/* 0000149C 000014D0  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000014A0 000014D4  4C C6 31 82 */	crclr 4*cr1+eq
/* 000014A4 000014D8  48 00 00 01 */	bl OSPanic
L_000014A8:
/* 000014A8 000014DC  57 83 06 A6 */	rlwinm r3, r28, 0, 26, 19
/* 000014AC 000014E0  88 1B 00 05 */	lbz r0, 0x5(r27)
/* 000014B0 000014E4  54 00 30 32 */	slwi r0, r0, 6
/* 000014B4 000014E8  7C 7C 03 78 */	or r28, r3, r0
/* 000014B8 000014EC  88 1B 00 06 */	lbz r0, 0x6(r27)
/* 000014BC 000014F0  54 00 00 33 */	clrrwi. r0, r0, 6
/* 000014C0 000014F4  41 82 00 18 */	beq L_000014D8
/* 000014C4 000014F8  38 7F 05 A0 */	addi r3, r31, 0x5a0
/* 000014C8 000014FC  38 80 05 34 */	li r4, 0x534
/* 000014CC 00001500  38 BF 05 E8 */	addi r5, r31, 0x5e8
/* 000014D0 00001504  4C C6 31 82 */	crclr 4*cr1+eq
/* 000014D4 00001508  48 00 00 01 */	bl OSPanic
L_000014D8:
/* 000014D8 0000150C  57 83 05 1A */	rlwinm r3, r28, 0, 20, 13
/* 000014DC 00001510  88 1B 00 06 */	lbz r0, 0x6(r27)
/* 000014E0 00001514  54 00 60 26 */	slwi r0, r0, 12
/* 000014E4 00001518  7C 7C 03 78 */	or r28, r3, r0
/* 000014E8 0000151C  48 00 00 30 */	b L_00001518
L_000014EC:
/* 000014EC 00001520  57 BD 00 32 */	clrrwi r29, r29, 6
/* 000014F0 00001524  57 BD 06 A6 */	rlwinm r29, r29, 0, 26, 19
/* 000014F4 00001528  57 A0 05 1A */	rlwinm r0, r29, 0, 20, 13
/* 000014F8 0000152C  64 1D 00 01 */	oris r29, r0, 0x1
/* 000014FC 00001530  63 BD 50 00 */	ori r29, r29, 0x5000
/* 00001500 00001534  57 A0 03 8E */	rlwinm r0, r29, 0, 14, 7
/* 00001504 00001538  64 1D 00 58 */	oris r29, r0, 0x58
/* 00001508 0000153C  57 80 00 32 */	clrrwi r0, r28, 6
/* 0000150C 00001540  60 1C 00 15 */	ori r28, r0, 0x15
/* 00001510 00001544  57 9C 06 A6 */	rlwinm r28, r28, 0, 26, 19
/* 00001514 00001548  57 9C 05 1A */	rlwinm r28, r28, 0, 20, 13
L_00001518:
/* 00001518 0000154C  38 00 00 61 */	li r0, 0x61
/* 0000151C 00001550  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001520 00001554  98 03 80 00 */	stb r0, -0x8000(r3)
/* 00001524 00001558  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001528 0000155C  93 A3 80 00 */	stw r29, -0x8000(r3)
/* 0000152C 00001560  80 80 00 00 */	lwz r4, __gxVerif
/* 00001530 00001564  57 A3 55 BA */	rlwinm r3, r29, 10, 22, 29
/* 00001534 00001568  38 03 0C C8 */	addi r0, r3, 0xcc8
/* 00001538 0000156C  7F A4 01 2E */	stwx r29, r4, r0
/* 0000153C 00001570  38 00 00 61 */	li r0, 0x61
/* 00001540 00001574  3C 60 CC 01 */	lis r3, 0xcc01
/* 00001544 00001578  98 03 80 00 */	stb r0, -0x8000(r3)
/* 00001548 0000157C  3C 60 CC 01 */	lis r3, 0xcc01
/* 0000154C 00001580  93 83 80 00 */	stw r28, -0x8000(r3)
/* 00001550 00001584  80 80 00 00 */	lwz r4, __gxVerif
/* 00001554 00001588  57 83 55 BA */	rlwinm r3, r28, 10, 22, 29
/* 00001558 0000158C  38 63 0C C8 */	addi r3, r3, 0xcc8
/* 0000155C 00001590  7F 84 19 2E */	stwx r28, r4, r3
/* 00001560 00001594  38 00 00 01 */	li r0, 0x1
/* 00001564 00001598  80 60 00 00 */	lwz r3, gx
/* 00001568 0000159C  B0 03 00 02 */	sth r0, 0x2(r3)
/* 0000156C 000015A0  80 01 00 44 */	lwz r0, 0x44(r1)
/* 00001570 000015A4  BB 61 00 2C */	lmw r27, 0x2c(r1)
/* 00001574 000015A8  38 21 00 40 */	addi r1, r1, 0x40
/* 00001578 000015AC  7C 08 03 A6 */	mtlr r0
/* 0000157C 000015B0  4E 80 00 20 */	blr