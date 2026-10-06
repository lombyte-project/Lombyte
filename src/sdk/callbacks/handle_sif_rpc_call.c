#include "types.h"
#include "asm.h"

#include "types.h"

/* Packet, server, and queue are handled as words to preserve the retail
 * copy order. The indices below are byte offsets divided by four. */
extern void iWakeupThread(s32 thread_id);

void handle_sif_rpc_call(u32 *packet, void *state) __asm__("FUN_0011b138");

void handle_sif_rpc_call(u32 *packet, void *state) {
    u32 *server = (u32 *)packet[13]; /* packet + 0x34 */
    u32 *queue = (u32 *)server[16];  /* server + 0x40 */

    if (queue[3] == 0) {
        queue[3] = (u32)server;
    } else {
        ((u32 *)queue[4])[15] = (u32)server;
    }
    queue[4] = (u32)server;
    {
        u32 packet_address = packet[5];
        u32 client = packet[7];
        server[8] = packet_address;
        server[7] = client;
    }
    server[9] = packet[8];
    server[3] = packet[9];
    server[10] = packet[10];
    server[11] = packet[11];
    server[12] = packet[12];
    server[13] = packet[4];
    if ((s32)queue[0] >= 0 && queue[1] == 0) {
        iWakeupThread(queue[0]);
    }
}

extern __typeof__(handle_sif_rpc_call) D_0011B138 __attribute__((alias("FUN_0011b138")));
