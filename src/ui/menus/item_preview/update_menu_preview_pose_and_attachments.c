#include "types.h"

/* Ported from rac1-decomp (src/game/pause.c, func_002260A8). */
#include "qcopy.h"
#include "rnc/ui/menus/pause_moby.h"
#include "rnc/gameplay/gadgets/hand_gadget.h"
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern char preview_binding_table[] __asm__("D_001863D0");
/* Second name for the same table: the two arms below differ only in which name
   they pass, which keeps the compiler from merging them before the call. */
extern char preview_binding_table_alias[] __asm__("D_001863D0");
extern int advance_moby_animation(void *) __asm__("func_0020D580");
extern void refresh_moby_spatial_bounds(void *) __asm__("func_0020DEF8");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("func_0020E098");
extern void noop_callback_s(void *, void *, int, int, int) __asm__("func_001E9480");
extern void detach_manipulator(int, void *) __asm__("func_0020CB88");
extern void attach_manipulator(int, int, void *) __asm__("FUN_0020cb10");

void update_menu_preview_pose_and_attachments(void *preview) __asm__("FUN_00224e18");

void update_menu_preview_pose_and_attachments(void *preview) {
    PauseMoby *moby = preview;
    char **preview_vars = moby->vars;
    f32 transform[16];
    s32 source_moby_address = *(int *)(*preview_vars + 0x44);
    s32 is_second_preview_moby;
    s32 first_attachment_active;
    s32 second_attachment_active;

    advance_moby_animation(moby);
    refresh_moby_spatial_bounds(moby);
    is_second_preview_moby = moby == *(PauseMoby **)(*preview_vars + 0x5C);
    build_moby_bone_transform(source_moby_address, is_second_preview_moby ? 3 : 2, transform);
    qcopy(moby->pos, &transform[12]);
    copy_matrix3x4(moby->basis, transform);
    normalize_vector_triplet(moby->basis);
    refresh_moby_spatial_bounds_from_basis(moby);
    first_attachment_active = 0;
    second_attachment_active = 0;
    if (second_attachment_manipulator.active != 0) {
        first_attachment_active = 1;
        detach_manipulator(source_moby_address, &second_attachment_manipulator);
    }
    if (first_attachment_manipulator.active != 0) {
        second_attachment_active = 1;
        detach_manipulator(source_moby_address, &first_attachment_manipulator);
    }
    if (!is_second_preview_moby) {
        noop_callback_s(preview_binding_table_alias, preview_binding_table,
                        moby->resource_address, 0, source_moby_address);
        moby->primary_binding = preview_binding_table;
        moby->secondary_binding = preview_binding_table;
    } else {
        noop_callback_s(preview_binding_table, preview_binding_table_alias,
                        moby->resource_address, 0, source_moby_address);
        moby->primary_binding = preview_binding_table_alias;
        moby->secondary_binding = preview_binding_table_alias;
    }
    if (first_attachment_active) {
        attach_manipulator(source_moby_address, 0x17, &second_attachment_manipulator);
        *(int *)((char *)&second_attachment_manipulator + 0x20) = 0;
        *(int *)((char *)&second_attachment_manipulator + 0x24) = 0;
        *(int *)((char *)&second_attachment_manipulator + 0x28) = 0;
    }
    if (second_attachment_active) {
        attach_manipulator(source_moby_address, 0x16, &first_attachment_manipulator);
        *(int *)((char *)&first_attachment_manipulator + 0x20) = 0;
        *(int *)((char *)&first_attachment_manipulator + 0x24) = 0;
        *(int *)((char *)&first_attachment_manipulator + 0x28) = 0;
    }
    moby->binding_blend_word = 0;
    moby->binding_state = 0;
}

extern __typeof__(update_menu_preview_pose_and_attachments) func_00224E18
    __attribute__((alias("FUN_00224e18")));
