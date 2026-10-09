#include "rnc/gameplay/entities/moby_class_tables.h"
#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/ui/menus/item_preview/item_preview_placement.h"

struct ItemPreviewVars {
    void *owner;
    u8 pad4[8];
    s32 item_index;
};
struct ItemPreviewBinding {
    u8 pad0[0x14];
    s32 variant;
    u8 pad18[0x18];
    s32 flags;
    s32 state;
    f32 rotation_angle;
    u8 pad3C[8];
    struct Moby *primary_moby;
    struct Moby *secondary_moby;
};
struct PreviewItemDefinition {
    u8 pad0[8];
    s32 item_type;
    u8 padC[4];
    s32 oclass;
    u8 pad14[0x38];
};
struct PreviewCamera {
    u8 pad0[0x140];
    f32 x;
    f32 y;
    f32 z;
};
struct PreviewClassResource {
    u8 pad0[0xD];
    u8 state_0d;
};

extern struct PreviewItemDefinition preview_item_definitions[] __asm__("D_001863D0");
extern struct PreviewCamera preview_camera __asm__("D_00186F40");
extern u8 gold_weapon_purchased[] __asm__("D_0013E520");
#include "rnc/gameplay/hero.h"
extern s32 resource_request_state __asm__("D_0015FF50");
extern void update_item_preview_transform() __asm__("FUN_0021e698");

extern void func_001E9470(s32, s32);
extern void func_001E9478(struct Moby *, s32);
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern void select_world_object_resource_tables(s32, s32) __asm__("func_00204A40");
extern void set_moby_animation(struct Moby *, s32, s32) __asm__("func_00212ED8");
extern struct Moby *create_menu_preview_moby(s32) __asm__("func_00225490");
extern struct Moby *delete_moby(struct Moby *) __asm__("FUN_00225530");

s32 update_item_preview_binding(struct ItemPreviewBinding *preview) __asm__("FUN_0021e230");

s32 update_item_preview_binding(struct ItemPreviewBinding *preview) {
    struct MenuScreen *grid;
    struct Moby *moby;
    struct Moby *secondary_moby;
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
    struct Moby *source_moby;
    s32 is_type2;
    struct ItemPreviewVars *preview_vars;
    s32 is_type1;
    s32 item_two_difference;
    f32 camera_x;
    struct ItemPreviewVars *secondary_vars;
    f32 position_x;

    grid = menu_system.current->focus;
    item_index = grid->data.grid.cells[grid->data.grid.selected_cell].id;
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
            if (hero.items[0].item_id != 0 &&
                oclass != hero.items[0].item_id) {
                func_001E9470(0, 0);
            }
            if (oclass != menu_system.unk11C) {
                select_world_object_resource_tables(oclass, menu_system.resource_table_toggle == 0);
                menu_system.last_resource_table_toggle = resource_request_state;
                menu_system.unk140 = oclass;
                menu_system.unk120 = oclass;
                ((struct PreviewClassResource *)moby_class_resources[resident_class_slot_by_id[oclass]])->state_0d = 0;
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
                position_x = camera_x + preview_placements[item_index].alternate_x;
            } else {
                position_x = camera_x + preview_placements[item_index].normal_x;
            }
            moby->pos.x = position_x;
            moby->pos.y = preview_camera.y + preview_placements[item_index].y;
            moby->pos.z = preview_camera.z + preview_placements[item_index].z;
            moby->rot.x = preview_placements[item_index].rotation_x;
            moby->rot.y = preview_placements[item_index].rotation_y;
            moby->rot.z = 3.1415927f;
            moby->update = update_item_preview_transform;
            preview_vars = (struct ItemPreviewVars *)moby->pvars;
            preview_vars->owner = preview;
            preview_vars->item_index = item_index;
            last_animation = moby->pclass->seq_count - 1;
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
                qcopy(&moby->pos, &source_moby->pos);
                qcopy(&moby->rot, &source_moby->rot);
                preview->secondary_moby = moby;
                moby->update = update_item_preview_transform;
                secondary_vars = (struct ItemPreviewVars *)moby->pvars;
                secondary_vars->item_index = item_index;
                secondary_vars->owner = preview;
                set_moby_animation(moby,
                                   (animation_index < moby->pclass->seq_count - 1)
                                       ? animation_index
                                       : moby->pclass->seq_count - 1,
                                   0);
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
