#include "types.h"
struct Part {
    s32 unk0;
    u8 pad_4[0x1C];
    s32 unk20;
    u8 pad_24[0xF];
    u8 unk33;
    u8 pad_34[0x1C];
};
struct Obj {
    u8 pad_0[0x23];
    u8 unk23;
    u8 pad_24[0x8];
    struct Part *unk2C;
};
struct Pair {
    s16 unk0;
    s16 unk2;
};
extern s32 D_001E3000[];
extern struct Obj *D_001E1700[];
extern struct Pair D_001E2A00[];

void patch_tie_texture_fields(void) __asm__("FUN_00235780");

void patch_tie_texture_fields(void) {
    s32 *active_class_index_cursor;
    s32 i;
    struct Obj *render_class_descriptor;
    struct Part *render_record;
    struct Pair *render_address_offsets;

    for (active_class_index_cursor = D_001E3000; *active_class_index_cursor >= 0; active_class_index_cursor++) {
        render_class_descriptor = D_001E1700[*active_class_index_cursor];
        render_record = render_class_descriptor->unk2C;
        for (i = 0; i < render_class_descriptor->unk23; i++, render_record++) {
            render_address_offsets = &D_001E2A00[render_record->unk33];
            if (render_address_offsets->unk0 != 0) {
                render_record->unk0 = (render_record->unk0 & 0xFFFFC000) | render_address_offsets->unk0;
            }
            if (render_address_offsets->unk2 != 0) {
                render_record->unk20 = (render_record->unk20 & 0xFFFFC000) | render_address_offsets->unk2;
            }
        }
    }
}

extern __typeof__(patch_tie_texture_fields) func_00235780 __attribute__((alias("FUN_00235780")));
