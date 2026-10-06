#include "types.h"
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
typedef struct HandGadgetSelection {
    u8 pad00[0x1C];
    s32 current_gadget;
    u8 pad20[0x10];
    s32 selected_gadget;
    s32 attachment_gadget;
    s32 animation_gadget;
    s32 pose_gadget;
    u8 pad40[0x8C];
    s32 animation_base;
    u8 padD0[0x48];
    s32 resource_request_state;
    s32 active_resource_class;
    s32 requested_resource_class;
    u8 pad124[0x1C];
    s32 last_requested_resource_class;
    s32 last_resource_request_state;
} HandGadgetSelection;
typedef struct HandGadgetManipulator {
    u8 pad0;
    u8 active;
    u8 pad2[0x1E];
    float rotation_x;
    float rotation_y;
    float rotation_z;
} HandGadgetManipulator;
typedef struct HandGadgetAnimation {
    s32 resource_first;
    s32 resource_count;
    s32 primary_animation;
    s32 delay_frames;
    s32 item_animation;
    s32 secondary_animation;
    s32 attachment0_class;
    s32 attachment0_animation;
    s32 attachment1_class;
    s32 attachment1_animation;
    s32 attachment2_class;
    s32 attachment2_animation;
} HandGadgetAnimation;
extern u8 gadget_available[] __asm__("D_0013D4C0");
extern u8 gold_weapon_purchased[] __asm__("D_0013E520");
typedef struct HandGadgetPlayerState {
    u8 pad0[0x10B8];
    s32 equipped_gadget;
    u8 pad10BC[0xF3A];
    u8 ammo_used;
    u8 ammo_capacity;
} HandGadgetPlayerState;
extern HandGadgetPlayerState player_state __asm__("D_0013F350");
extern s32 resource_request_state __asm__("D_0015FF50");
extern HandGadgetDefinition gadget_definitions[] __asm__("D_001863D0");
extern u8 *moby_class_resources[] __asm__("D_001B3200");
extern u8 moby_class_slots[] __asm__("D_001B3AC0");
extern HandGadgetAnimation gadget_animations[] __asm__("D_001D52E8");
extern HandGadgetSelection gadget_selection __asm__("D_001D5BF0");
extern HandGadgetManipulator class_pose_manipulator __asm__("D_001D5DD0");
extern HandGadgetManipulator first_attachment_manipulator __asm__("D_001D5E10");
extern HandGadgetManipulator second_attachment_manipulator __asm__("D_001D5E50");
extern float ammo_preview_offsets[] __asm__("D_001D5E90");
extern s32 ammo_preview_velocities[] __asm__("D_001D5EA8");
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
    selected_gadget = gadget_selection.selected_gadget;
    selected_class = gadget_definitions[selected_gadget].oclass;
    selected_class_ready = selected_class == gadget_selection.requested_resource_class;
    if ((previous_selected_class != selected_class) && selected_class_ready) {
        delete_moby(moby);
        if (class_pose_manipulator.active) {
            detach_manipulator(hand->source_moby_address, &class_pose_manipulator);
        }
        if ((player_state.equipped_gadget != 0) &&
            (selected_gadget != player_state.equipped_gadget)) {
            func_001E9470(0, 0);
        }
        resource_request_state = gadget_selection.resource_request_state == 0;
        select_world_object_resource_tables(selected_class, -1);
        gadget_selection.active_resource_class = selected_class;
        gadget_selection.resource_request_state = resource_request_state;
        gadget_selection.last_requested_resource_class = selected_class;
        gadget_selection.last_resource_request_state = resource_request_state;
        moby_class_resources[moby_class_slots[selected_class]][0xD] = 0;
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
    requested_class = gadget_definitions[gadget_selection.animation_gadget].oclass;
    if (previous_animation_class != requested_class) {
        moby = delete_moby(moby);
        if (requested_class != (-1)) {
            moby = create_menu_preview_moby(requested_class);
            if (moby != 0) {
                loaded_gadget = gadget_selection.animation_gadget;
                *moby->vars = hand;
                moby->update = update_menu_preview_animation_transform;
                moby->update_kind = 4;
            }
        }
        hand->animation_moby = moby;
    }
    moby = hand->first_attachment_moby;
    previous_attachment_class = (moby != 0) ? (moby->oclass) : (-1);
    requested_class = gadget_definitions[gadget_selection.attachment_gadget].oclass;
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
                loaded_gadget = gadget_selection.attachment_gadget;
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
    requested_class = gadget_definitions[gadget_selection.pose_gadget].oclass;
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
        requested_class = (slot_index < player_state.ammo_capacity) ? (0x1DF) : (-1);
        if (previous_class != requested_class) {
            moby = delete_moby(moby);
            if (requested_class != (-1)) {
                moby = create_menu_preview_moby(requested_class);
                velocity_offset = slot_index * 4;
                *ammo_offset = (slot_index < player_state.ammo_used) ? (0.0f) : (3.0f);
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
    if (((loaded_gadget != gadget_selection.current_gadget) && (loaded_gadget > 0)) &&
        (loaded_gadget < 0x24)) {
        gadget_selection.current_gadget = loaded_gadget;
        clear_preview_animation_queue();
        queue_preview_animation(gadget_animations[loaded_gadget].primary_animation +
                                    gadget_selection.animation_base,
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
                                        gadget_selection.animation_base,
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
