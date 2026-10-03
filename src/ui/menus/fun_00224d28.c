/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00225FB8). */
#include "qcopy.h"
#include "rnc/pause_moby_types.h"
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern char preview_binding_table[] __asm__("D_001863D0");
extern char preview_binding_table_alias[] __asm__("D_001863D0");
extern int advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void refresh_moby_spatial_bounds(void *) __asm__("FUN_0020def8");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern void noop_callback_s(void *, void *, int, int, int) __asm__("func_001E9480");
/* Advance animation and publish bone 4's pose. Class 0x1B1 selects the
   alternate arguments to the retail's empty pose hook. */
void update_menu_preview_animation_transform(PauseMoby *moby) __asm__("FUN_00224d28");

void update_menu_preview_animation_transform(PauseMoby *moby) {
    f32 transform[16];
    s32 source_moby_address = *(int *)(*moby->vars + 0x44);

    advance_moby_animation(moby);
    refresh_moby_spatial_bounds(moby);
    build_moby_bone_transform(source_moby_address, 4, transform);
    qcopy(moby->pos, &transform[12]);
    copy_matrix3x4(moby->basis, transform);
    normalize_vector_triplet(moby->basis);
    refresh_moby_spatial_bounds_from_basis(moby);
    if (moby->oclass == 0x1B1) {
        noop_callback_s(preview_binding_table_alias, preview_binding_table, moby->resource_address, 6, source_moby_address);
    } else {
        noop_callback_s(preview_binding_table_alias, preview_binding_table, moby->resource_address, 0, source_moby_address);
    }
    moby->binding_state = 0;
    moby->primary_binding = preview_binding_table;
    moby->binding_blend_word = 0;
    moby->secondary_binding = preview_binding_table;
}

extern __typeof__(update_menu_preview_animation_transform) func_00224D28 __attribute__((alias("FUN_00224d28")));
