#include "types.h"
#include "rnc/sdk/rpc/cmd.h"

s32 IsceSifSendCmd(void *packet,
                   void *command,
                   void *source,
                   s32   source_size,
                   void *destination,
                   s32   destination_size) __asm__("isceSifSendCmd");

s32 IsceSifSendCmd(void *packet,
                   void *command,
                   void *source,
                   s32   source_size,
                   void *destination,

                   s32 destination_size) {
    return _sceSifSendCmd(packet, 1, command, source, source_size, destination, destination_size);
}
