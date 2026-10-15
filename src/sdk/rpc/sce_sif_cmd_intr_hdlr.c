#include "types.h"
#include "eetypes.h"
#include "kernel.h"

typedef void (*SifCmdHandler)(void *packet, void *argument);

typedef struct SifCmdHandlerEntry {
    SifCmdHandler handler;
    void *argument;
} SifCmdHandlerEntry;

/* Header of a SIF command packet from the IOP. */
typedef struct SifCmdHeader {
    u32 packet_size : 8;
    u32 destination_size : 24;
    u32 destination;
    s32 command_id;
    u32 option;
} SifCmdHeader;

/* The SIF command state (sceSifInitCmd fills it). */
typedef struct SifCmdData {
    SifCmdHeader *packet_buffer;
    void *unk4;
    s32 unk8;
    SifCmdHandlerEntry *system_handlers;
    s32 system_handler_count;
    SifCmdHandlerEntry *user_handlers;
    s32 user_handler_count;
    s32 *unk1C;
} SifCmdData;

extern s32 EnableInterrupts(void);
extern s32 isceSifSetDChain(void);
extern SifCmdData sif_cmd_data __asm__("D_00154E58");

/* Sony libsifcmd: copies the received command packet out of the DMA buffer,
   re-arms the receive chain and runs the handler registered for the command. */
s32 _sceSifCmdIntrHdlr(void) {
    u128 packet[7];
    SifCmdData *cmd = &sif_cmd_data;
    volatile SifCmdHeader *header;
    SifCmdHandlerEntry *entry;
    SifCmdHandler handler;
    u128 *buffer;
    s32 n; /* packet bytes, then quadwords, then handler index */
    s32 i;

    EnableInterrupts();
    header = cmd->packet_buffer;
    n = header->packet_size & 0xff;
    if (n == 0) {
        return 0;
    }
    n = (n + 15) / 16;
    header->packet_size = 0;
    buffer = (u128 *)header;
    for (i = 0; i < n; i++) {
        packet[i] = *buffer++;
    }
    isceSifSetDChain();
    header = (volatile SifCmdHeader *)packet;
    if (header->command_id < 0) {
        n = header->command_id & 0x7fffffff;
        if (n < cmd->system_handler_count) {
            entry = cmd->system_handlers;
            handler = entry[n].handler;
            if (handler) {
                handler(packet, entry[n].argument);
            }
        }
    } else {
        n = header->command_id;
        if (n < cmd->user_handler_count) {
            entry = cmd->user_handlers;
            handler = entry[n].handler;
            if (handler) {
                handler(packet, entry[n].argument);
            }
        }
    }
    CpuSync();
    CpuEnableInt();
    return 0;
}
