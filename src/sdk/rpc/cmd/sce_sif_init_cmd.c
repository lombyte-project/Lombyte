/* Ported from rac1-decomp (src/core/00119D88.c: func_0011A780). */
#include "types.h"
#include "eeregs.h"

typedef void (*SifCmdHandler)(void *packet, void *argument);

typedef struct SifCmdHandlerEntry {
    SifCmdHandler handler;
    void         *argument;
} SifCmdHandlerEntry;

typedef struct SifCmdHeader {
    u32 packet_size      : 8;
    u32 destination_size : 24;
    u32 destination;
    s32 command_id;
    u32 option;
} SifCmdHeader;

/* The SIF command state of sce_sif_cmd_intr_hdlr.c. Typed pointers in the
   first and fourth fields reorder the stores below, so they stay void *. */
typedef struct SifCmdData {
    void               *packet_buffer;
    void               *unk4;
    s32                 unk8;
    void               *system_handlers;
    s32                 system_handler_count;
    SifCmdHandlerEntry *user_handlers;
    s32                 user_handler_count;
    s32                *sregs;
} SifCmdData;

/* Tells the IOP where the EE receives command packets. */
typedef struct SifCmdInitPacket {
    SifCmdHeader header;
    void        *packet_buffer;
} SifCmdInitPacket;

extern u32                D_0012FC04[];
extern u32                D_00154E54[];
extern int                sif_sregs[] __asm__("D_00154F80");
extern u8                 sif_cmd_packet_buffer[] __asm__("D_00154D80");
extern u8                 sif_cmd_send_buffer[] __asm__("D_00154E00");
extern SifCmdData         sif_cmd_data __asm__("D_00154E58");
extern SifCmdHandlerEntry sif_system_handlers[] __asm__("D_00154E80");
extern SifCmdInitPacket   sif_init_packet __asm__("D_00154E40");

extern int  DIntr(void);
extern void EnableInterrupts(void);
extern void FlushCache(int);
extern void sceSifSetDChain(void);
extern int  AddDmacHandler(int, void *, int);
extern s32  enable_dmac(s32 channel) __asm__("func_00119160");
extern void _sceSifCmdIntrHdlr(void);
extern u32  sceSifGetReg(u32 reg);
extern u32  sceSifSetReg(s32 reg, u32 value);
extern u32
sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 data_size);
extern s32  handle_sif_initialize_command(s32 *packet, s32 *command_state) __asm__("func_0011A448");
extern void handle_sif_set_system_register(s32 *packet,
                                           s32 *command_state) __asm__("func_0011A428");

void sceSifInitCmd(void) {
    int i;
    u32 uncached_packet_buffer, uncached_unk4;
    DIntr();
    if (D_0012FC04[0] != 0) {
        EnableInterrupts();
        return;
    }
    D_0012FC04[0] = 1;
    uncached_packet_buffer = (u32)sif_cmd_packet_buffer | 0x20000000;
    uncached_unk4 = (u32)sif_cmd_send_buffer | 0x20000000;
    sif_cmd_data.packet_buffer = (void *)uncached_packet_buffer;
    sif_cmd_data.unk4 = (void *)uncached_unk4;
    sif_cmd_data.unk8 = 0;
    sif_cmd_data.system_handlers = sif_system_handlers;
    sif_cmd_data.system_handler_count = 0x20;
    sif_cmd_data.user_handlers = 0;
    sif_cmd_data.user_handler_count = 0;
    sif_cmd_data.sregs = sif_sregs;
    for (i = 0; i < 32; i++) {
        sif_system_handlers[i].handler = 0;
        sif_system_handlers[i].argument = 0;
    }
    for (i = 0; i < 32; i++) {
        sif_sregs[i] = 0;
    }
    sif_system_handlers[0].handler = (SifCmdHandler)handle_sif_initialize_command;
    sif_system_handlers[1].handler = (SifCmdHandler)handle_sif_set_system_register;
    sif_system_handlers[0].argument = &sif_cmd_data;
    sif_system_handlers[1].argument = &sif_cmd_data;
    EnableInterrupts();
    FlushCache(0);
    if (*REG_DMAC_STAT & 0x20) {
        *REG_DMAC_STAT = 0x20;
    }
    if (!(*(vu32 *)0x1000C000 & 0x100)) {
        sceSifSetDChain();
    }
    D_00154E54[0] = AddDmacHandler(5, _sceSifCmdIntrHdlr, 0);
    enable_dmac(5);
    sif_cmd_data.unk8 = sceSifGetReg(0x80000000);
    if (sif_cmd_data.unk8 != 0) {
        sif_init_packet.packet_buffer = sif_cmd_packet_buffer;
        sceSifSendCmd(0x80000000, &sif_init_packet, 0x14, 0, 0, 0);
        return;
    }
    do {
    } while (!(sceSifGetReg(4) & 0x20000));
    sif_cmd_data.unk8 = sceSifGetReg(2);
    sceSifSetReg(0x80000000, sif_cmd_data.unk8);
    sceSifSetReg(0x80000001, (u32)&sif_cmd_data);
    sif_init_packet.packet_buffer = sif_cmd_packet_buffer;
    sif_init_packet.header.option = 0;
    sceSifSendCmd(0x80000002, &sif_init_packet, 0x14, 0, 0, 0);
}
