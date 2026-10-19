/* Ported from rac1-decomp (src/core/00119D88.c: func_0011A780). */
#include "types.h"
#include "sda.h"

extern int D_0012FC04 NOT_SDA;
extern int D_00154E54 NOT_SDA;
extern int D_00154F80[];
typedef struct { void (*f)(void *, void *); void *arg; } Hnd;
typedef struct {
    void *a0;
    void *a1;
    int a2;
    void *a3;
    int a4;
    int a5;
    int a6;
    int *a7;
} CmdData;
typedef struct { char pad[0xC]; int pad2; void *u10; } CmdPkt;
extern int DIntr(void);
extern void EnableInterrupts(void);
extern void FlushCache(int);
extern void sceSifSetDChain(void);
extern int AddDmacHandler(int, void *, int);
extern void func_00119160(int);
extern void _sceSifCmdIntrHdlr(void);
extern int sceSifGetReg(int);
extern int sceSifSetReg(int, int);
extern int sceSifSendCmd(int, int, int, int, int, int);
extern int handle_sif_initialize_command() __asm__("func_0011A448");
extern void handle_sif_set_system_register() __asm__("func_0011A428");
extern unsigned char D_00154D80[];
extern unsigned char D_00154E00[];
extern CmdData D_00154E58;
extern Hnd D_00154E80[];
extern CmdPkt D_00154E40;

void sceSifInitCmd(void) {
    int i;
    unsigned t5, t6;
    DIntr();
    if (D_0012FC04 != 0) {
        EnableInterrupts();
        return;
    }
    D_0012FC04 = 1;
    t6 = (unsigned)D_00154D80 | 0x20000000;
    t5 = (unsigned)D_00154E00 | 0x20000000;
    D_00154E58.a0 = (void *)t6;
    D_00154E58.a1 = (void *)t5;
    D_00154E58.a2 = 0;
    D_00154E58.a3 = D_00154E80;
    D_00154E58.a4 = 0x20;
    D_00154E58.a5 = 0;
    D_00154E58.a6 = 0;
    D_00154E58.a7 = D_00154F80;
    for (i = 0; i < 32; i++) {
        D_00154E80[i].f = 0;
        D_00154E80[i].arg = 0;
    }
    for (i = 0; i < 32; i++) {
        D_00154F80[i] = 0;
    }
    D_00154E80[0].f = (void (*)(void *, void *))handle_sif_initialize_command;
    D_00154E80[1].f = (void (*)(void *, void *))handle_sif_set_system_register;
    D_00154E80[0].arg = &D_00154E58;
    D_00154E80[1].arg = &D_00154E58;
    EnableInterrupts();
    FlushCache(0);
    if (*(volatile unsigned *)0x1000E010 & 0x20) {
        *(volatile unsigned *)0x1000E010 = 0x20;
    }
    if (!(*(volatile unsigned *)0x1000C000 & 0x100)) {
        sceSifSetDChain();
    }
    D_00154E54 = AddDmacHandler(5, _sceSifCmdIntrHdlr, 0);
    func_00119160(5);
    D_00154E58.a2 = ((int (*)(unsigned))sceSifGetReg)(0x80000000);
    if (D_00154E58.a2 != 0) {
        D_00154E40.u10 = D_00154D80;
        sceSifSendCmd(0x80000000, (int)&D_00154E40, 0x14, 0, 0, 0);
        return;
    }
    do {
    } while (!(sceSifGetReg(4) & 0x20000));
    D_00154E58.a2 = sceSifGetReg(2);
    sceSifSetReg(0x80000000, D_00154E58.a2);
    sceSifSetReg(0x80000001, (int)&D_00154E58);
    D_00154E40.u10 = D_00154D80;
    D_00154E40.pad2 = 0;
    sceSifSendCmd(0x80000002, (int)&D_00154E40, 0x14, 0, 0, 0);
}
