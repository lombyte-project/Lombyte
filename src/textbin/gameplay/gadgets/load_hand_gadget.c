#include "rnc/gameplay/entities/moby_class_tables.h"
#include "types.h"
#include "rnc/ui/menus/item_preview/preview_animation.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/gadgets/hand_gadget.h"
#include "asm.h"

#include "types.h"

typedef struct Moby {
    u8 pad00[0x20];
    u8 state;
    u8 pad21[0x13];
    s16 update_kind;
    u8 pad36[0x1D];
    u8 anim;
    u8 pad54[0x20];
    void *update;
    void **vars;
    u8 pad7C[0x2A];
    s16 oclass;
    u8 padA8[0x14];
    u8 ammo_moby_slot;
} Moby;
typedef struct HandGadgetState {
    u8 pad00[0x44];
    s32 source_moby_address;
    u8 pad48[4];
    Moby *pose_moby;
    Moby *animation_moby;
    Moby *class_pose_moby;
    Moby *first_attachment_moby;
    Moby *second_attachment_moby;
    s32 x60;
    s32 x64;
    s32 x68;
    Moby *class_0197_moby;
    Moby *class_0266_moby;
    Moby *class_026a_moby;
    Moby *ammo_mobys[8];
    u8 pad98[0xC];
    u8 timers[0x18];
} HandGadgetState;
typedef struct HandGadgetDefinition {
    u8 pad00[0x10];
    s32 oclass;
    u8 pad14[0x38];
} HandGadgetDefinition;
extern u8 gadget_available[] __asm__("D_0013D4C0");
extern u8 gold_weapon_purchased[] __asm__("D_0013E520");
extern s32 resource_request_state __asm__("D_0015FF50");
extern HandGadgetDefinition gadget_definitions[] __asm__("D_001863D0");
extern void func_001E9470(s32, s32);
extern void func_001E9478(Moby *, s32);
extern void select_world_object_resource_tables(s32, s32) __asm__("func_00204A40");
extern void attach_manipulator(s32, s32, HandGadgetManipulator *) __asm__("FUN_0020cb10");
extern void detach_manipulator(s32, HandGadgetManipulator *) __asm__("func_0020CB88");
extern void blend_moby_animation(void *, int, int, int) __asm__("func_00212F90");
extern Moby *create_menu_preview_moby(s32) __asm__("func_00225490");
extern Moby *delete_moby(Moby *) __asm__("FUN_00225530");
extern void update_preview_animation_and_attachments(s32, Moby *, Moby *, s32 *, s32 *,
                                                     s32 *) __asm__("func_00225E70");
extern s32 queue_preview_animation(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                                   s32) __asm__("func_002265D8");
extern s32 clear_preview_animation_queue(void) __asm__("func_00226718");
extern void update_menu_preview_class_pose() __asm__("func_00224B70");
extern void update_menu_preview_animation_transform() __asm__("func_00224D28");
extern void update_menu_preview_pose_and_attachments() __asm__("func_00224E18");
extern void update_menu_preview_animation_pose() __asm__("func_00224FC0");
extern void update_menu_preview_class_transform() __asm__("func_002250F0");
extern void update_ammo_preview_transform() __asm__("func_00225180");
s32 load_hand_gadget(HandGadgetState *hand) __asm__("FUN_00224368");

s32 load_hand_gadget(HandGadgetState *hand) {
    Moby *moby;
    s32 previous_selected_class;
    s32 previous_animation_class;
    s32 previous_attachment_class;
    s32 previous_pose_class;
    s32 previous_class_0197;
    s32 previous_class_0266;
    s32 previous_class;
    s32 requested_class;
    s32 selected_class;
    s32 selected_gadget;
    s32 loaded_gadget;
    s32 slot_index;
    s32 velocity_offset;
    HandGadgetAnimation *animation;
    float *ammo_offset;
    s32 selected_class_ready;
    Moby **ammo_moby_slot;
    loaded_gadget = 0;
    moby = hand->class_pose_moby;
    previous_selected_class = (moby != 0) ? (moby->oclass) : (-1);
    selected_gadget = menu_system.equipped[0];
    selected_class = gadget_definitions[selected_gadget].oclass;
    selected_class_ready = selected_class == menu_system.unk120;
    if ((previous_selected_class != selected_class) && selected_class_ready) {
        delete_moby(moby);
        if (class_pose_manipulator.active) {
            detach_manipulator(hand->source_moby_address, &class_pose_manipulator);
        }
        if ((hero.items[0].item_id != 0) &&
            (selected_gadget != hero.items[0].item_id)) {
            func_001E9470(0, 0);
        }
        resource_request_state = menu_system.resource_table_toggle == 0;
        select_world_object_resource_tables(selected_class, -1);
        menu_system.unk11C = selected_class;
        menu_system.resource_table_toggle = resource_request_state;
        menu_system.unk140 = selected_class;
        menu_system.last_resource_table_toggle = resource_request_state;
        ((u8 *)moby_class_resources[resident_class_slot_by_id[selected_class]])[0xD] = 0;
        moby = create_menu_preview_moby(selected_class);
        if (moby != 0) {
            loaded_gadget = selected_gadget;
            if (gold_weapon_purchased[loaded_gadget]) {
                func_001E9478(moby, hand->source_moby_address);
            }
            *moby->vars = hand;
            moby->update = update_menu_preview_class_pose;
            moby->update_kind = 4;
            if (loaded_gadget == 0x12) {
                attach_manipulator(hand->source_moby_address, 0, &class_pose_manipulator);
                class_pose_manipulator.rotation_x = 0;
                class_pose_manipulator.rotation_y = 0;
                class_pose_manipulator.rotation_z = 0;
            }
        }
        hand->class_pose_moby = moby;
    }
    moby = hand->animation_moby;
    previous_animation_class = (moby != 0) ? (moby->oclass) : (-1);
    requested_class = gadget_definitions[menu_system.equipped[2]].oclass;
    if (previous_animation_class != requested_class) {
        moby = delete_moby(moby);
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                loaded_gadget = menu_system.equipped[2];
                *moby->vars = hand;
                moby->update = update_menu_preview_animation_transform;
                moby->update_kind = 4;
            }
        }
        hand->animation_moby = moby;
    }
    moby = hand->first_attachment_moby;
    previous_attachment_class = (moby != 0) ? (moby->oclass) : (-1);
    requested_class = gadget_definitions[menu_system.equipped[1]].oclass;
    if (previous_attachment_class != requested_class) {
        moby = delete_moby(moby);
        if (first_attachment_manipulator.active) {
            detach_manipulator(hand->source_moby_address, &first_attachment_manipulator);
        }
        if (second_attachment_manipulator.active) {
            detach_manipulator(hand->source_moby_address, &second_attachment_manipulator);
        }
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                loaded_gadget = menu_system.equipped[1];
                *moby->vars = hand;
                moby->update = update_menu_preview_pose_and_attachments;
                moby->update_kind = 4;
                attach_manipulator(hand->source_moby_address, 0x16, &first_attachment_manipulator);
                attach_manipulator(hand->source_moby_address, 0x17, &second_attachment_manipulator);
                second_attachment_manipulator.rotation_z =
                    (first_attachment_manipulator.rotation_z =
                         (second_attachment_manipulator.rotation_y =
                              (second_attachment_manipulator.rotation_x =
                                   (first_attachment_manipulator.rotation_y =
                                        (first_attachment_manipulator.rotation_x = 0.01f)))));
            }
        }
        hand->first_attachment_moby = moby;
        moby = delete_moby(hand->second_attachment_moby);
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                *moby->vars = hand;
                moby->update = update_menu_preview_pose_and_attachments;
                moby->update_kind = 4;
            }
        }
        hand->second_attachment_moby = moby;
    }
    moby = hand->pose_moby;
    previous_pose_class = (moby != 0) ? (moby->oclass) : (-1);
    requested_class = gadget_definitions[menu_system.equipped[3]].oclass;
    if (previous_pose_class != requested_class) {
        moby = delete_moby(moby);
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                *moby->vars = hand;
                moby->update = update_menu_preview_animation_pose;
                moby->update_kind = 4;
                if (moby->oclass == 0x25F) {
                    if (moby->anim != 6) {
                        blend_moby_animation(moby, 6, 0, 10);
                    }
                    moby->state = 8;
                }
            }
        }
        hand->pose_moby = moby;
    }
    moby = hand->class_0197_moby;
    previous_class_0197 = (moby != 0) ? (moby->oclass) : (-1);
    requested_class = (gadget_available[0x23]) ? (0x197) : (-1);
    if (previous_class_0197 != requested_class) {
        moby = delete_moby(moby);
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                *moby->vars = hand;
                moby->update = update_menu_preview_class_transform;
                moby->update_kind = 4;
            }
        }
        hand->class_0197_moby = moby;
    }
    moby = hand->class_0266_moby;
    previous_class_0266 = (moby != 0) ? (moby->oclass) : (-1);
    requested_class = (gadget_available[0x21]) ? (0x266) : (-1);
    if (previous_class_0266 != requested_class) {
        moby = delete_moby(moby);
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                *moby->vars = hand;
                moby->update = update_menu_preview_class_transform;
                moby->update_kind = 4;
            }
        }
        hand->class_0266_moby = moby;
    }
    moby = hand->class_026a_moby;
    previous_class = (moby != 0) ? (moby->oclass) : (-1);
    requested_class = (gadget_available[0x22]) ? (0x26A) : (-1);
    if (previous_class != requested_class) {
        moby = delete_moby(moby);
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                *moby->vars = hand;
                moby->update = update_menu_preview_class_transform;
                moby->update_kind = 4;
            }
        }
        hand->class_026a_moby = moby;
    }
    slot_index = 0;
    ammo_offset = ammo_preview_offsets;
    ammo_moby_slot = hand->ammo_mobys;
    do {
        moby = *ammo_moby_slot;
        previous_class = (moby != 0) ? (moby->oclass) : (-1);
        requested_class = (slot_index < hero.ammo_capacity) ? (0x1DF) : (-1);
        if (previous_class != requested_class) {
            moby = delete_moby(moby);
            if (requested_class != (-1)) {
                moby = create_menu_preview_moby(requested_class);
                velocity_offset = slot_index * 4;
                *ammo_offset = (slot_index < hero.ammo_used) ? (0.0f) : (3.0f);
                *(s32 *)((u8 *)ammo_preview_velocities + velocity_offset) = 0;
                if (moby != 0) {
                    *moby->vars = hand;
                    moby->update = update_ammo_preview_transform;
                    moby->update_kind = 4;
                    moby->ammo_moby_slot = slot_index;
                }
            }
            *ammo_moby_slot = moby;
        }
        slot_index++;
        ammo_moby_slot++;
        ammo_offset++;
    } while (slot_index < 8);
    if (((loaded_gadget != menu_system.current_gadget) && (loaded_gadget > 0)) &&
        (loaded_gadget < 0x24)) {
        menu_system.current_gadget = loaded_gadget;
        clear_preview_animation_queue();
        queue_preview_animation(gadget_animations[loaded_gadget].primary_animation +
                                    menu_system.streamed_animation_base,
                                0, gadget_animations[loaded_gadget].delay_frames, loaded_gadget,
                                gadget_animations[loaded_gadget].item_animation,
                                gadget_animations[loaded_gadget].attachment0_class,
                                gadget_animations[loaded_gadget].attachment0_animation,
                                gadget_animations[loaded_gadget].attachment1_class,
                                gadget_animations[loaded_gadget].attachment1_animation,
                                gadget_animations[loaded_gadget].attachment2_class,
                                gadget_animations[loaded_gadget].attachment2_animation,
                                gadget_animations[loaded_gadget].resource_first,
                                gadget_animations[loaded_gadget].resource_count);
        if (gadget_animations[loaded_gadget].delay_frames != 0) {
            queue_preview_animation(gadget_animations[loaded_gadget].secondary_animation +
                                        menu_system.streamed_animation_base,
                                    2, 0, loaded_gadget, 1, -1, 0, -1, 0, -1, 0, 0, 0);
        }
    }
    for (slot_index = 0; slot_index < 0x18; slot_index++) {
        if (hand->timers[slot_index] != 0) {
            hand->timers[slot_index]--;
        }
    }

    update_preview_animation_and_attachments(hand->source_moby_address, hand->class_pose_moby,
                                             hand->animation_moby, &hand->x60, &hand->x64,
                                             &hand->x68);
    return 0;
}

HandGadgetAnimation gadget_animations[37] = {
    {0, 0, 7, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 7, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 7, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 7, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 7, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 0x19, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 0x1a, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 0x15, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 7, 1, 1, 7, -1, 0, 0, 0, 0, 0},
    {0, 0, 0x16, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {1, 1, 4, 1, 1, 7, 0x291, 1, -1, 0, -1, 0},
    {0, 0, 0, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {0, 0, 8, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0xe, 1, 0xf, 1, 4, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 0xe, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 1, 3, 1, 5, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 0xa, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {2, 1, 5, 0, 1, 7, 0x4a, 3, -1, 0, -1, 0},
    {0, 0, 2, 0, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 1, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {7, 3, 0xd, 1, 1, 7, 0xba, 8, 0xba, 9, 0xba, 0xa},
    {0x10, 1, 0x17, 1, 1, 7, 0x10e, 4, -1, 0, -1, 0},
    {0, 0, 0x12, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {0, 0, 9, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {0, 0, 0x11, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {6, 1, 0x10, 1, 1, 7, 0xcb, 3, -1, 0, -1, 0},
    {0xc, 1, 0x18, 1, 3, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 0x1b, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {0, 0, 0xb, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 0xc, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 6, 1, 1, 6, -1, 0, 0, 0, 0, 0},
    {0xa, 2, 0x13, 1, 3, 7, 0x27a, 8, -1, 0, -1, 0},
    {0xf, 1, 0x14, 1, 1, 7, -1, 0, -1, 0, -1, 0},
    {0, 0, 6, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {0, 0, 6, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {0, 0, 6, 1, 1, 6, -1, 0, -1, 0, -1, 0},
    {0, 0, 6, 1, 1, 6, -1, 0, -1, 0, -1, 0},
};
