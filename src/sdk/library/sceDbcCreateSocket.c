/* Ported from rac1-decomp (src/core/001236F0.c, func_00124858). */
#include "types.h"

extern s32 D_0015B080[];
extern u8 D_0015B008[];
extern u8 D_00153578[];
extern s32 sceSifCallRpc();
extern void debug_print_stub(u8 *) __asm__("func_00124A20");

/* The header words are copied as pointers: their alias set differs from
   the int port/mode stores, so the header loads need not wait for them. */
s32 sceDbcCreateSocket(void *params, s32 port, s32 mode) {
    s32 *socket_rpc_buffer = D_0015B080;
    u8 *name_source = (u8 *)params + 0x14;
    u8 *name_target = (u8 *)socket_rpc_buffer + 0x14;
    s32 i;

    socket_rpc_buffer[10] = port;
    socket_rpc_buffer[11] = mode;
    ((void **)socket_rpc_buffer)[0] = ((void **)params)[0];
    ((void **)socket_rpc_buffer)[1] = ((void **)params)[1];
    ((void **)socket_rpc_buffer)[2] = ((void **)params)[2];
    ((void **)socket_rpc_buffer)[3] = ((void **)params)[3];
    ((void **)socket_rpc_buffer)[4] = ((void **)params)[4];
    for (i = 0; i < 0x10; i++) {
        name_target[i] = name_source[i];
    }
    if (sceSifCallRpc(D_0015B008, 0x80000901, 0, D_0015B080, 0x400, D_0015B080, 0x400, 0, 0) < 0) {
        debug_print_stub(D_00153578);
        return 0;
    }
    return socket_rpc_buffer[9];
}
