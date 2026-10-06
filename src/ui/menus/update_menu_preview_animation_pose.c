/* Ported from rac1-decomp (src/game/pause.c, func_00226250). */
#include "qcopy.h"
#include "rnc/ui/menus/pause_moby.h"
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern void blend_moby_animation(void *, int, int, int) __asm__("func_00212F90");
/* Class 0x25F counts down state byte 0x20 before blending selector 6
   back to selector 1. Publish bone 5's translation and normalized basis. */
void update_menu_preview_animation_pose(void *preview) __asm__("FUN_00224fc0");

void update_menu_preview_animation_pose(void *preview) {
    PauseMoby *moby = preview;
    unsigned char *moby_bytes = (unsigned char *)moby;
    f32 transform[16];
    s32 source_moby_address = *(int *)(*moby->vars + 0x44);

    if (moby_bytes[0x70] & 2) {
        if (moby->oclass == 0x25F) {
            if (moby_bytes[0x52] == 6) {
                if (--moby_bytes[0x20] == 0) {
                    if (moby_bytes[0x53] != 1) {
                        blend_moby_animation(moby, 1, 0, 10);
                    }
                } else if (moby_bytes[0x53] != 6) {
                    blend_moby_animation(moby, 6, 0, 0);
                }
            } else if (moby_bytes[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, 0);
            }
        } else if (moby_bytes[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 0);
        }
    }
    build_moby_bone_transform(source_moby_address, 5, transform);
    qcopy(moby->pos, &transform[12]);
    copy_matrix3x4(moby->basis, transform);
    normalize_vector_triplet(moby->basis);
    refresh_moby_spatial_bounds_from_basis(moby);
}

extern __typeof__(update_menu_preview_animation_pose) func_00224FC0
    __attribute__((alias("FUN_00224fc0")));
