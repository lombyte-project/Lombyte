/* Ported from rac1-decomp (src/game/pause.c, func_00225E00). */
#include "qcopy.h"
#include "rnc/ui/menus/pause_moby.h"
#include "rnc/gameplay/gadgets/hand_gadget.h"
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern char preview_binding_table[] __asm__("D_001863D0");
extern char preview_binding_table_alias[] __asm__("D_001863D0");
extern void refresh_moby_spatial_bounds(void *) __asm__("FUN_0020def8");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void normalize_vector_triplet(void *) __asm__("func_00214128");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern void noop_callback_s(void *, void *, int, int, int) __asm__("func_001E9480");
typedef struct {
    char pad00[0xC];
    s32 bone_index; /* 0x0C */
    s32 oclass;     /* 0x10 */
    char pad14[4];
    s32 normalize_basis; /* 0x18 */
    char pad1C[0x30];
} PauseClassRecord;
extern PauseClassRecord preview_class_records[] __asm__("D_001863D0");
extern void blend_moby_animation(void *, int, int, int) __asm__("func_00212F90");
extern void detach_manipulator(int, void *) __asm__("func_0020CB88");
extern void attach_manipulator(int, int, void *) __asm__("FUN_0020cb10");
/* Resolve the class-specific bone pose, publish translation and basis, and
   temporarily detach an active shared manipulator while resetting bindings.
   Store order follows the retail callback. */
void update_menu_preview_class_pose(void *preview) __asm__("FUN_00224b70");

void update_menu_preview_class_pose(void *preview) {
    PauseMoby *moby = preview;
    unsigned char *moby_bytes = preview;
    f32 transform[16];
    s32 source_moby_address = *(int *)(*moby->vars + 0x44);
    s32 i;
    s32 reset_bindings;
    s32 attachment_active;
    unsigned char *attachment;

    if ((moby_bytes[0x70] & 2) && moby_bytes[0x53] != 1) {
        blend_moby_animation(moby, 1, 0, 0);
    }
    for (i = 0; i < 0x25; i++) {
        if (preview_class_records[i].oclass == moby->oclass) {
            break;
        }
    }
    build_moby_bone_transform(source_moby_address, preview_class_records[i].bone_index, transform);
    qcopy(moby->pos, &transform[12]);
    refresh_moby_spatial_bounds(moby);
    reset_bindings = preview_class_records[i].normalize_basis == 0;
    copy_matrix3x4(moby->basis, transform);
    if (!reset_bindings) {
        normalize_vector_triplet(moby->basis);
    }
    refresh_moby_spatial_bounds_from_basis(moby);
    attachment_active = 0;
    attachment = (unsigned char *)&class_pose_manipulator;
    if (attachment[1] != 0) {
        attachment_active = 1;
        detach_manipulator(source_moby_address, attachment);
    }
    if (reset_bindings) {
        noop_callback_s(preview_binding_table_alias, preview_binding_table, moby->resource_address,
                        0, source_moby_address);
        moby->primary_binding = preview_binding_table;
        moby->binding_blend_word = 0;
        moby->secondary_binding = preview_binding_table;
        *(int *)(moby_bytes + 0x58) = 0;
        moby->binding_state = 0;
    }
    if (attachment_active) {
        attach_manipulator(source_moby_address, 0, attachment);
        *(int *)(attachment + 0x20) = 0;
        *(int *)(attachment + 0x24) = 0;
        *(int *)(attachment + 0x28) = 0;
    }
}

extern __typeof__(update_menu_preview_class_pose) func_00224B70
    __attribute__((alias("FUN_00224b70")));
