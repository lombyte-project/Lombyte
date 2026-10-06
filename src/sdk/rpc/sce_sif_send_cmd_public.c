#include "types.h"

extern s32 SendSifCommand(void *packet, s32 mode, void *command, void *source, s32 source_size,
                          void *destination, s32 destination_size) __asm__("_sceSifSendCmd");

s32 sceSifSendCmd(void *packet, void *command, void *source, s32 source_size, void *destination,
                  s32 destination_size) {
    return SendSifCommand(packet, 0, command, source, source_size, destination, destination_size);
}
