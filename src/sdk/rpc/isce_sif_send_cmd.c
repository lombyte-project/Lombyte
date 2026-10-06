#include "types.h"

extern s32 SendSifCommand(void *packet, s32 mode, void *command, void *source, s32 source_size,
                          void *destination, s32 destination_size) __asm__("_sceSifSendCmd");

s32 IsceSifSendCmd(void *packet, void *command, void *source, s32 source_size, void *destination,
                   s32 destination_size) __asm__("isceSifSendCmd");

s32 IsceSifSendCmd(void *packet, void *command, void *source, s32 source_size, void *destination,
                   s32 destination_size) {
    return SendSifCommand(packet, 1, command, source, source_size, destination, destination_size);
}
