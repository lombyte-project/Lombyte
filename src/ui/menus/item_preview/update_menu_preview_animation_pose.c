/* Ported from rac1-decomp (src/game/pause.c, func_00226250). */
#include "qcopy.h"
#include "rnc/ui/menus/pause_moby.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"

extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");
extern void blend_moby_animation(void *, int, int, int) __asm__("func_00212F90");
/* Class 0x25F counts down Moby.state before blending selector 6
   back to selector 1. Publish bone 5's translation and normalized basis. */
void update_menu_preview_animation_pose(void *preview) __asm__("FUN_00224fc0");

void update_menu_preview_animation_pose(void *preview) {
    enum {
        MENU_PREVIEW_ANIMATION_COUNTDOWN_EXPIRED = 0
    };
    PauseMoby   *moby = preview;
    struct Moby *anim = (struct Moby *)moby;
    f32          transform[16];
    s32          source_moby_address = (s32)((struct MenuScreen *)*moby->vars)->data.preview.moby;

    if (anim->anim_flags & 2) {
        if (moby->oclass == 0x25F) {
            if (anim->seq == 6) {
                if (--anim->state == MENU_PREVIEW_ANIMATION_COUNTDOWN_EXPIRED) {
                    if (anim->prev_seq != 1) {
                        blend_moby_animation(moby, 1, 0, 10);
                    }
                } else if (anim->prev_seq != 6) {
                    blend_moby_animation(moby, 6, 0, 0);
                }
            } else if (anim->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, 0);
            }
        } else if (anim->prev_seq != 1) {
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
