/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    s32 state;
    u8 pad[0x20];
    s32 cur;
    s32 idx;
} S00203908;
typedef struct {
    u16 id;
    u16 pad;
    s32 v;
} P00203908;
extern S00203908 D_L00_00179410_00203908 __asm__("D_L00_00179410");
extern P00203908 D_00141968_00203908[] __asm__("D_00141968");
void FUN_001fed30_00203908(s32) __asm__("FUN_001fed30");
/* Starts help entry b with message a when no help is showing; pushes a to the help history. */
s32 help_request(s32 a, s32 b) __asm__("FUN_L00_00203908");
s32 help_request(s32 message_id, s32 help_record_index) {
    S00203908 *help_state_ptr = &D_L00_00179410_00203908;
    if (help_state_ptr->state == 0 && help_state_ptr->cur == -1 && music_stream_state.secondary.handle == 0 &&
        music_stream_state.queued_secondary_track == help_state_ptr->cur && D_00141968_00203908[help_record_index].id != 0xFFFF) {
        help_state_ptr->idx = help_record_index;
        help_state_ptr->cur = message_id;
        FUN_001fed30_00203908(message_id);
        return 1;
    }
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/help_00203E98.c: func_L00_00203FB8), where it is exact; names translated to the US level program. */

extern char D_L00_00179410[];

/* When D_L00_00179410 is active, may set the 0x5A state to 5, then clears and resets the record. */
void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
void clear_active_help_record(void) {
    if (*(int *)(D_L00_00179410) != 0) {
        int help_entry_value = *(int *)((char *)(*(int *)(D_L00_00179410 + 0x20) << 4) + (int)D_L00_0015F660 + 8);
        if (help_entry_value != -1) {
            char *music_state_ptr = (char *)D_0014161B + 0x100B5;
            if (help_entry_value == *(short *)(music_state_ptr + 0x54) - 0x7530) {
                if ((unsigned)(*(unsigned short *)(music_state_ptr + 0x5A) - 6) >= 2)
                    *(short *)(music_state_ptr + 0x5A) = 5;
            }
        }
        *(int *)(D_L00_00179410) = 0;
        *(int *)(D_L00_00179410 + 0x24) = -1;
        *(int *)(D_L00_00179410 + 0x20) = -1;
        *(int *)(D_L00_00179410 + 4) = 0;
    }
}
typedef struct {
    s32 a;
    u32 b;
} P;
extern P D_00141968[];
/* Clears the top bit of each of the 0x94 help records. */
void clear_help_record_high_bits(void) __asm__("FUN_L00_00203a28");

void clear_help_record_high_bits(void) {
    s32 i;
    for (i = 0; i < 0x94; i++)
        D_00141968[i].b &= 0x7FFFFFFF;
}
