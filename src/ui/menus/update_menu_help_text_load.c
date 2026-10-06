#include "types.h"

extern s16 D_001516D8[];
extern s32 D_00137B80[];
extern s32 D_001D5CF8[];
extern u8 D_001D5BF0[];
extern s32 D_0015ED88;
extern u8 D_001996D0[];

extern s32 D_0015F6A0;

extern s32 start_audio_stream_read(s32 menu, s32 arg1, s32 arg2) __asm__("FUN_00216788");
extern void FUN_001f9838(void *, void *, s32);

static inline int pauseSlotCount(void) {
    char *b = D_001996D0;
    return *(int *)(b + 0x2C);
}

int update_menu_help_text_load(char *menu) __asm__("FUN_0021d338");

int update_menu_help_text_load(char *menu) {
    switch (*(int *)(menu + 0x50)) {
    case 0:
        if (D_001516D8[0] == 0) {
            if (start_audio_stream_read(D_001D5CF8[0], D_00137B80[0x1528 / 4],
                                        D_00137B80[0x152C / 4]) != 0) {
                *(int *)(menu + 0x50) = 1;
            } else {
                *(int *)(menu + 0x50) = 3;
            }
        }
        break;
    case 1:
        if (D_001516D8[0] == 0) {
            char *g = D_001D5BF0;
            int *archive = *(int **)(g + 0x108);
            int *language_entries = (int *)((char *)archive + archive[D_0015ED88]);
            int entry_count = *language_entries++;
            int entry_bytes = *language_entries++;
            char *b;
            int *text_entries;
            int i;

            FUN_001f9838(archive, language_entries, ((entry_bytes + 3) & ~3) - 8);
            b = D_001996D0;
            *(int *)(menu + 0x54) = D_0015F6A0;
            *(int *)(menu + 0x38) = *(int *)(b + 0x2C);
            *(int *)(b + 0x2C) = entry_count;
            text_entries = *(int **)(g + 0x108);
            D_0015F6A0 = (int)text_entries;
            for (i = 0; i < pauseSlotCount(); i++) {
                int text_base = (int)text_entries - 8;
                *(int *)((char *)text_entries + i * 0x10) += text_base;
            }
            *(int *)(menu + 0x10) &= ~4;
            *(int *)(menu + 0x50) = 2;
        }
        break;
    case 2:
    case 3:
        break;
    }
    return 0;
}

extern __typeof__(update_menu_help_text_load) func_0021D338 __attribute__((alias("FUN_0021d338")));
