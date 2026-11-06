#include "types.h"
#include "rnc/rendering/object_render_class.h"

struct Pair {
    s16 tex_base; /* TBP0 for tex0 bits 0..13, 0 = keep */
    s16 mip_base; /* TBP1 for mip bits 0..13, 0 = keep */
};

extern s32         D_001E3000[];
extern struct Pair D_001E2A00[];

void patch_tie_texture_fields(void) __asm__("FUN_00235780");

void patch_tie_texture_fields(void) {
    s32                *active_class_index_cursor;
    s32                 i;
    ObjectRenderClass  *render_class_descriptor;
    ObjectRenderRecord *render_record;
    struct Pair        *render_address_offsets;

    for (active_class_index_cursor = D_001E3000; *active_class_index_cursor >= 0;
         active_class_index_cursor++) {
        render_class_descriptor = object_render_classes[*active_class_index_cursor];
        render_record = render_class_descriptor->records;
        for (i = 0; i < render_class_descriptor->record_count; i++, render_record++) {
            render_address_offsets = &D_001E2A00[render_record->clamp.b[3]];
            if (render_address_offsets->tex_base != 0) {
                render_record->tex0.w[0] =
                    (render_record->tex0.w[0] & 0xFFFFC000) | render_address_offsets->tex_base;
            }
            if (render_address_offsets->mip_base != 0) {
                render_record->mip.w[0] =
                    (render_record->mip.w[0] & 0xFFFFC000) | render_address_offsets->mip_base;
            }
        }
    }
}

extern __typeof__(patch_tie_texture_fields) func_00235780 __attribute__((alias("FUN_00235780")));
