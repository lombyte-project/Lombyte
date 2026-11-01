#include "types.h"
#include "rnc/sdk/rpc/cmd/sce_sif_send_cmd.h"

s32 sceSifSendCmd(void *packet,
                  void *command,
                  void *source,
                  s32   source_size,
                  void *destination,

                  s32 destination_size) {
    return _sceSifSendCmd(packet, 0, command, source, source_size, destination, destination_size);
}
