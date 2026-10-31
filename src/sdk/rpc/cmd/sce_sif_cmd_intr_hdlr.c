#include "types.h"
#include "eetypes.h"
#include "kernel.h"

typedef void (*SifCmdHandler)(void *packet, void *argument);

typedef struct SifCmdHandlerEntry {
    SifCmdHandler handler;
    void         *argument;
} SifCmdHandlerEntry;

/* Header of a SIF command packet from the IOP. */
typedef struct SifCmdHeader {
    u32 packet_size      : 8;
    u32 destination_size : 24;
    u32 destination;
    s32 command_id;
    u32 option;
} SifCmdHeader;

/* The SIF command state (sceSifInitCmd fills it). */
typedef struct SifCmdData {
    SifCmdHeader       *packet_buffer;
    void               *unk4;
    s32                 unk8;
    SifCmdHandlerEntry *system_handlers;
    s32                 system_handler_count;
    SifCmdHandlerEntry *user_handlers;
    s32                 user_handler_count;
    s32                *sregs;
} SifCmdData;

extern s32        EnableInterrupts(void);
extern s32        isceSifSetDChain(void);
extern SifCmdData sif_cmd_data __asm__("D_00154E58");

/* Sony libsifcmd: copies the received command packet out of the DMA buffer,
   re-arms the receive chain and runs the handler registered for the command. */
s32 _sceSifCmdIntrHdlr(void) {
    u128                   packet[7];
    SifCmdData            *cmd = &sif_cmd_data;
    volatile SifCmdHeader *header;
    SifCmdHandlerEntry    *handler_table;
    SifCmdHandler          handler;
    u128                  *receive_packet_cursor;
    s32 packet_count_or_handler_index; /* packet bytes, then quadwords, then handler index */
    s32 i;

    EnableInterrupts();
    header = cmd->packet_buffer;
    packet_count_or_handler_index = header->packet_size & 0xff;
    if (packet_count_or_handler_index == 0) {
        return 0;
    }
    packet_count_or_handler_index = (packet_count_or_handler_index + 15) / 16;
    header->packet_size = 0;
    receive_packet_cursor = (u128 *)header;
    for (i = 0; i < packet_count_or_handler_index; i++) {
        packet[i] = *receive_packet_cursor++;
    }
    isceSifSetDChain();
    header = (volatile SifCmdHeader *)packet;
    if (header->command_id < 0) {
        packet_count_or_handler_index = header->command_id & 0x7fffffff;
        if (packet_count_or_handler_index < cmd->system_handler_count) {
            handler_table = cmd->system_handlers;
            handler = handler_table[packet_count_or_handler_index].handler;
            if (handler) {
                handler(packet, handler_table[packet_count_or_handler_index].argument);
            }
        }
    } else {
        packet_count_or_handler_index = header->command_id;
        if (packet_count_or_handler_index < cmd->user_handler_count) {
            handler_table = cmd->user_handlers;
            handler = handler_table[packet_count_or_handler_index].handler;
            if (handler) {
                handler(packet, handler_table[packet_count_or_handler_index].argument);
            }
        }
    }
    CpuSync();
    CpuEnableInt();
    return 0;
}
