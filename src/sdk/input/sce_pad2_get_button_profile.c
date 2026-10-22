#include "types.h"
#include "rnc/data_labels.h"
typedef struct {
    u64 size : 14;
    u64 mode : 2;
    u64 cmd : 8;
    u64 ver : 8;
    u64 pad : 32;
} DbcCommand;

typedef union {
    DbcCommand bits;
    u64 word;
} DbcCommandWord;

typedef struct {
    u8 pad0[0x4];
    s32 linked;
    u8 pad8[0x8];
    u8 order[0x320];
} Pad2Info;

extern Pad2Info D_0015B540[];
extern s32 sceDbcReceiveData(s32 port, s32 cmd, void *buf, void *data);
extern s32 scePad2LinkDriver(s32 socket);
extern void scePad2SetButtonOrder(u8 *profile, u8 *order);

s32 scePad2GetButtonProfile(s32 socket, u8 *profile) {
    DbcCommandWord cmd;
    s32 result;
    s32 profile_byte_count_or_error;

    if (D_0015B540[socket].linked == 0) {
        if (scePad2LinkDriver(socket) < 0) {
            return -1;
        }
    }
    cmd.bits.size = 2;
    cmd.bits.mode = 3;
    cmd.bits.cmd = 2;
    cmd.bits.ver = 1;
    profile_byte_count_or_error = sceDbcReceiveData(socket, cmd.word, &result, profile);
    if (profile_byte_count_or_error >= 0) {
        scePad2SetButtonOrder(profile, (u8 *)&((Pad2Info *)D_0015B550)[socket]);
        profile_byte_count_or_error = result;
    }
    return profile_byte_count_or_error;
}
