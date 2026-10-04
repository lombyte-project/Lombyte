#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00224e18/FUN_00224e18.s", FUN_00224e18);
#else
/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_002260A8). */
#include "qcopy.h"
#include "rnc/pause_moby_types.h"
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern char preview_binding_table[] __asm__("D_001863D0");
extern char preview_binding_table_alias[] __asm__("D_001863D0");
extern char first_preview_manipulator[] __asm__("D_001D5E50");
extern char second_preview_manipulator[] __asm__("D_001D5E10");
extern int advance_moby_animation(void *) __asm__("func_0020D580");
extern void refresh_moby_spatial_bounds(void *) __asm__("func_0020DEF8");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("func_0020E098");
extern void noop_callback_s(void *, void *, int, int, int) __asm__("func_001E9480");
extern void detach_manipulator(int, void *) __asm__("func_0020CB88");
extern void attach_manipulator(int, int, void *) __asm__("func_0020CB10");

void update_menu_preview_pose_and_attachments(void *preview) __asm__("FUN_00224e18");

void update_menu_preview_pose_and_attachments(void *preview) {
    PauseMoby *moby = preview;
    char **preview_vars = moby->vars;
    f32 transform[16];
    s32 source_moby_address = *(int *)(*preview_vars + 0x44);
    s32 is_second_preview_moby;
    s32 first_attachment_active;
    s32 second_attachment_active;
    char *binding_table;
    char *binding_table_alias;
    s32 resource_address;

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
    if ((unsigned char)first_preview_manipulator[1] != 0) {
        first_attachment_active = 1;
        detach_manipulator(source_moby_address, first_preview_manipulator);
    }
    if ((unsigned char)second_preview_manipulator[1] != 0) {
        second_attachment_active = 1;
        detach_manipulator(source_moby_address, second_preview_manipulator);
    }
    if (!is_second_preview_moby) {
        binding_table_alias = preview_binding_table_alias;
        binding_table = preview_binding_table;
        resource_address = moby->resource_address;
    } else {
        binding_table = preview_binding_table;
        binding_table_alias = preview_binding_table_alias;
        resource_address = moby->resource_address;
    }
    noop_callback_s(binding_table_alias, binding_table, resource_address, 0, source_moby_address);
    moby->primary_binding = binding_table;
    moby->secondary_binding = binding_table;
    if (first_attachment_active) {
        attach_manipulator(source_moby_address, 0x17, first_preview_manipulator);
        *(int *)(first_preview_manipulator + 0x20) = 0;
        *(int *)(first_preview_manipulator + 0x24) = 0;
        *(int *)(first_preview_manipulator + 0x28) = 0;
    }
    if (second_attachment_active) {
        attach_manipulator(source_moby_address, 0x16, second_preview_manipulator);
        *(int *)(second_preview_manipulator + 0x20) = 0;
        *(int *)(second_preview_manipulator + 0x24) = 0;
        *(int *)(second_preview_manipulator + 0x28) = 0;
    }
    moby->binding_blend_word = 0;
    moby->binding_state = 0;
}
#endif /* NON_MATCHING */
