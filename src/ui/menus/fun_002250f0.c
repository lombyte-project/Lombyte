/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_00226380). */
#include "qcopy.h"
#include "rnc/pause_moby_types.h"
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern int advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void refresh_moby_spatial_bounds(void *) __asm__("FUN_0020def8");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
/* Class 0x197 uses bone 0x1E; the other preview classes use bone 0x1D.
   Keep the comparison direction used by the retail conditional move. */
void update_menu_preview_class_transform(PauseMoby *moby) __asm__("FUN_002250f0");

void update_menu_preview_class_transform(PauseMoby *moby) {
    f32 transform[16];
    s32 source_moby_address = *(int *)(*moby->vars + 0x44);

    advance_moby_animation(moby);
    refresh_moby_spatial_bounds(moby);
    build_moby_bone_transform(source_moby_address, moby->oclass == 0x197 ? 0x1E : 0x1D, transform);
    qcopy(moby->pos, &transform[12]);
    copy_matrix3x4(moby->basis, transform);
    normalize_vector_triplet(moby->basis);
    refresh_moby_spatial_bounds_from_basis(moby);
}

extern __typeof__(update_menu_preview_class_transform) func_002250F0 __attribute__((alias("FUN_002250f0")));
