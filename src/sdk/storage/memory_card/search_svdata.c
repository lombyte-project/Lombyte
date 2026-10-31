#include "types.h"

u8 *SearchSvdata(u32 key, u8 *ctx) {
    u8 *server_queue = *(u8 **)(ctx + 0x28);
    u8 *rpc_server;

    if (server_queue != 0) {
        do {
            rpc_server = *(u8 **)(server_queue + 8);
            while (rpc_server != 0) {
                if (*(u32 *)rpc_server == key) {
                    return rpc_server;
                }
                rpc_server = *(u8 **)(rpc_server + 0x38);
            }
            server_queue = *(u8 **)(server_queue + 0x14);
        } while (server_queue != 0);
    }
    return 0;
}
