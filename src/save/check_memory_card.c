#include "types.h"
#include "rnc/sdk/graph.h"
#include "rnc/sdk/storage/memory_card.h"

/* memory_card_state viewed from the sceMcSync outputs at +0xB8. */
struct McDirBlk {
    u8  pad_0[0x4];
    s32 slot; /* 0x04: memory_card_state.card[0].slot */
    u8  pad_8[0xB4];
    s32 sync_result; /* 0xBC: sceMcSync result of the sceMcGetInfo call */
};

#include "rnc/save/memory_card_state.h"

extern u8 memory_card_info_sync_outputs[] __asm__("D_0013D348");

s32 check_memory_card(void) __asm__("FUN_00209168");

s32 check_memory_card(void) {
    enum {
        MEMCARD_TYPE_SUPPORTED_FOR_STARTUP = 2
    };
    s32              card_type;
    s32              free_blocks;
    s32              format;
    s32              cmd_code;
    s32              card_command_result;
    s32             *blk;
    struct McDirBlk *card_state;

    blk = &memory_card_state.card[0].port;
    card_command_result = sceMcGetInfo(blk[0], blk[1], &card_type, &free_blocks, &format);

    while (sceMcSync(1, memory_card_info_sync_outputs, memory_card_info_sync_outputs + 4) == 0) {
        sceGsSyncV(0);
    }
    card_state = (struct McDirBlk *)((u8 *)memory_card_info_sync_outputs - 0xB8);
    if (card_state->sync_result == -5) {
        return 1;
    }
    if (card_state->sync_result < -9) {
        return 1;
    }
    if (card_state->sync_result == -2) {
        return 0;
    }
    if (card_type != MEMCARD_TYPE_SUPPORTED_FOR_STARTUP) {
        return 1;
    }
    if (format == 0) {
        return 0;
    }
    card_command_result = sceMcGetDir(
        memory_card_state.card[0].port, card_state->slot, save_directory_path, 0, -1, 0);
    while (sceMcSync(1, &cmd_code, &card_command_result) == 0) {
        sceGsSyncV(0);
    }
    if (card_command_result > 0) {
        return 0;
    }
    if (free_blocks < 0x15E) {
        return 2;
    }
    return 0;
}

extern __typeof__(check_memory_card) func_00209168 __attribute__((alias("FUN_00209168")));
