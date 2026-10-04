#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021e230/FUN_0021e230.s", FUN_0021e230);
#else
#include "types.h"
#include "qcopy.h"

struct PreviewMobyResource { u8 pad0[0xC]; u8 animation_count; };
struct ItemPreviewVars { void *owner; u8 pad4[8]; s32 item_index; };
struct ItemPreviewMoby {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[8];
    struct PreviewMobyResource *resource;
    u8 pad28[0xC];
    s16 flags;
    u8 pad36[0xA];
    f32 rotation_x;
    f32 rotation_y;
    f32 rotation_z;
    u8 pad4C[0x28];
    void (*update)();
    struct ItemPreviewVars *preview_vars;
    u8 pad7C[0x2A];
    s16 oclass;
};
struct ItemPreviewBinding {
    u8 pad0[0x14];
    s32 variant;
    u8 pad18[0x18];
    s32 flags;
    s32 state;
    f32 rotation_angle;
    u8 pad3C[8];
    struct ItemPreviewMoby *primary_moby;
    struct ItemPreviewMoby *secondary_moby;
};
struct PreviewItemDefinition { u8 pad0[8]; s32 item_type; u8 padC[4]; s32 oclass; u8 pad14[0x38]; };
struct ItemPreviewPlacement { f32 alternate_x; f32 normal_x; f32 y; f32 z; f32 rotation_x; f32 rotation_y; u8 pad18[8]; };
struct PreviewItemSelection { u8 pad0[0x3C]; s32 index; u8 pad40[8]; u8 *table; };
struct PreviewMenuGame { u8 pad0[0x40]; struct PreviewItemSelection *selection; };
struct ItemPreviewMenuState {
    u8 pad0[4];
    struct PreviewMenuGame *game;
    u8 pad8[0x110];
    s32 class_state;
    s32 active_class;
    s32 requested_class;
    u8 pad124[0x1C];
    s32 last_requested_class;
    s32 last_resource_request_state;
};
struct PreviewCamera { u8 pad0[0x140]; f32 x; f32 y; f32 z; };
struct PreviewClassResource { u8 pad0[0xD]; u8 state_0d; };

extern struct ItemPreviewMenuState preview_menu_state __asm__("D_001D5BF0");
extern struct PreviewItemDefinition preview_item_definitions[] __asm__("D_001863D0");
extern struct PreviewCamera preview_camera __asm__("D_00186F40");
extern struct ItemPreviewPlacement preview_placements[] __asm__("D_001E0408");
extern u8 gold_weapon_purchased[] __asm__("D_0013E520");
extern s32 active_preview_resource_class[] __asm__("D_00140408");
extern s32 resource_request_state __asm__("D_0015FF50");
extern u8 moby_class_slots[] __asm__("D_001B3AC0");
extern struct PreviewClassResource *moby_class_resources[] __asm__("D_001B3200");
extern void update_item_preview_transform() __asm__("func_0021E698");

extern void func_001E9470(s32, s32);
extern void func_001E9478(struct ItemPreviewMoby *, s32);
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern void select_world_object_resource_tables(s32, s32) __asm__("func_00204A40");
extern void set_moby_animation(struct ItemPreviewMoby *, s32, s32) __asm__("func_00212ED8");
extern struct ItemPreviewMoby *create_menu_preview_moby(s32) __asm__("func_00225490");
extern struct ItemPreviewMoby *delete_moby(struct ItemPreviewMoby *) __asm__("func_00225530");

s32 update_item_preview_binding(struct ItemPreviewBinding *preview) __asm__("FUN_0021e230");

s32 update_item_preview_binding(struct ItemPreviewBinding *preview) {
    struct PreviewItemSelection *selection;
    struct ItemPreviewMoby *moby;
    struct ItemPreviewMoby *secondary_moby;
    struct PreviewItemDefinition *definition;
    s32 item_index;
    s32 previous_class;
    s32 oclass;
    s32 item_type;
    s32 has_secondary_moby;
    s32 load_class;
    s32 use_alternate_x;
    s32 animation_index;
    s32 last_animation;
    struct ItemPreviewMoby *source_moby;
    s32 is_type2;
    struct ItemPreviewVars *preview_vars;
    s32 is_type1;
    s32 item_two_difference;
    f32 offset;
    f32 camera_x;

    selection = preview_menu_state.game->selection;
    item_index = *(s16 *)(selection->table + selection->index * 10 + 6);
    if (preview->primary_moby != 0) {
        previous_class = preview->primary_moby->oclass;
    } else {
        previous_class = -1;
    }
    load_class = 0;
    oclass = preview_item_definitions[item_index].oclass;
    if (item_index == 0x18) {
        oclass = 0x1DF;
    }
    preview->rotation_angle = fast_add_rotations(preview->rotation_angle, 0.01f);
    item_type = preview_item_definitions[item_index].item_type;
    is_type2 = item_type == 2;
    has_secondary_moby = item_type == 3;
    is_type1 = item_type == 1;
    if (!is_type2 && !has_secondary_moby && !is_type1) {
        load_class = 1;
    }
    if (item_index == 0x18) {
        load_class = 0;
    }
    use_alternate_x = preview->flags & 1;
    switch (preview->state) {
    case 0:
        preview->state = 1;
        break;
    case 1:
        if (oclass != -1) {
            preview->state = 2;
        }
        break;
    case 2:
        if (oclass == -1) {
            preview->state = 1;
            break;
        }
        if (load_class) {
            if (active_preview_resource_class[0] != 0 && oclass != active_preview_resource_class[0]) {
                func_001E9470(0, 0);
            }
            if (oclass != preview_menu_state.active_class) {
                select_world_object_resource_tables(oclass, preview_menu_state.class_state == 0);
                preview_menu_state.last_resource_request_state = resource_request_state;
                preview_menu_state.last_requested_class = oclass;
                preview_menu_state.requested_class = oclass;
                moby_class_resources[moby_class_slots[oclass]]->state_0d = 0;
            }
        }
        moby = create_menu_preview_moby(oclass);
        item_two_difference = item_index ^ 2;
        if (item_two_difference == 0) {
            animation_index = 6;
        } else {
            animation_index = 1;
        }
        if (moby != 0) {
            if (load_class && gold_weapon_purchased[item_index] != 0) {
                func_001E9478(moby, preview->variant);
            }
            preview->primary_moby = moby;
            moby->flags = 0;
            camera_x = preview_camera.x;
            if (use_alternate_x) {
                moby->x = camera_x + preview_placements[item_index].alternate_x;
            } else {
                moby->x = camera_x + preview_placements[item_index].normal_x;
            }
            moby->y = preview_camera.y + preview_placements[item_index].y;
            moby->z = preview_camera.z + preview_placements[item_index].z;
            moby->rotation_x = preview_placements[item_index].rotation_x;
            moby->rotation_y = preview_placements[item_index].rotation_y;
            moby->rotation_z = 3.1415927f;
            moby->update = update_item_preview_transform;
            preview_vars = moby->preview_vars;
            preview_vars->owner = preview;
            preview_vars->item_index = item_index;
            last_animation = moby->resource->animation_count - 1;
            if (animation_index < last_animation) {
                last_animation = animation_index;
            }
            set_moby_animation(moby, last_animation, 0);
        }
        if (has_secondary_moby && moby != 0) {
            moby = create_menu_preview_moby(preview_item_definitions[1].oclass);
            if (moby != 0) {
                moby->flags = 0;
                source_moby = preview->primary_moby;
                qcopy(&moby->x, &source_moby->x);
                qcopy(&moby->rotation_x, &source_moby->rotation_x);
                preview->secondary_moby = moby;
                moby->update = update_item_preview_transform;
                preview_vars = moby->preview_vars;
                preview_vars->item_index = item_index;
                preview_vars->owner = preview;
                set_moby_animation(moby, (animation_index < moby->resource->animation_count - 1) ? animation_index : moby->resource->animation_count - 1, 0);
            }
        }
        preview->state = 3;
        break;
    case 3:
        if (previous_class != oclass) {
            preview->primary_moby = delete_moby(preview->primary_moby);
            preview->secondary_moby = delete_moby(preview->secondary_moby);
            preview->state = 2;
        }
        break;
    }
    return 0;
}
#endif /* NON_MATCHING */
