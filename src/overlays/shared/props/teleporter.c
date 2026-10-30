/* Shared moby class teleporter: activates, shows its children and draws its radius ring. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_labels.h"

typedef struct L01Moby L01Moby;

typedef struct {
    float v[16];
    int   c[4];
    float uv[8];
    long  g[4];
} Quad;

typedef struct {
    char     pad[0x14];
    L01Moby *slots[3];
} L01MobyData;

struct L01Moby {
    char           pad0[0x31];
    unsigned char  state;
    char           pad32[2];
    unsigned short flags;
    char           pad36[0x42];
    L01MobyData   *data;
    char           pad7c[0x18];
    int            field94;
};

extern void approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern int  FUN_00214720(void *, int);
extern void show_teleporter_and_children(struct Moby *) __asm__("FUN_L01_00309838");
extern void hide_teleporter_and_children(L01Moby *) __asm__("FUN_L01_003097e8");
extern void update_teleporter_child_transforms(char *) __asm__("FUN_L01_003092d0");
extern void try_set_teleporter_help_message(struct Moby *) __asm__("FUN_L01_00309430");
extern void draw_moby_radius_ring(char *) __asm__("FUN_L01_003094f0");
extern void request_hero_target_pose(float *, int, float) __asm__("FUN_L00_00233ee8");
extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern int  get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

extern s32           D_0015ED84;
extern int           D_0015ED84; /* no foreign declaration */
extern s32           D_0014C190[][64];
extern u8            D_0014C050[];
extern unsigned char D_0014C050[];
extern int           alternate_animation_sequence_gp __asm__("D_0015EE20") __attribute__((sda));
extern int           D_0013CAE4[];
extern char         *D_L01_0015FFD8;
extern int           D_L01_0015F594 __attribute__((sda));
extern float teleporter_child_motion_acceleration_gp __asm__("D_L01_00161F88") __attribute__((sda));
extern float teleporter_child_motion_deceleration_gp __asm__("D_L01_00161F8C") __attribute__((sda));
extern float teleporter_child_motion_max_speed_gp __asm__("D_L01_00161F90") __attribute__((sda));
extern float teleporter_child_position_z_offset_gp __asm__("D_L01_00161F78") __attribute__((sda));
extern float teleporter_child_displacement_angle_degrees_gp __asm__("D_L01_00161F80")
    __attribute__((sda));
extern int   shared_teleporter_help_message_ids[] __asm__("D_L01_00201600");
extern int   level_17_teleporter_help_message_ids[] __asm__("D_L01_00201618");
extern short level_8_teleporter_help_message_ids __asm__("D_L01_00161F98") __attribute__((sda));
extern short level_12_teleporter_help_message_ids __asm__("D_L01_00161FA0") __attribute__((sda));
extern short level_16_teleporter_help_message_ids __asm__("D_L01_00161FA8") __attribute__((sda));
extern short level_18_teleporter_help_message_ids __asm__("D_L01_00161FB0") __attribute__((sda));
extern float moby_radius_ring_quad_uvs[] __asm__("D_L01_00201630");
extern char  D_001413F5_lui __asm__("D_001413F5") NOT_SDA;
extern int   moby_radius_ring_blend_minuend_selector_gp __asm__("D_L01_00161FB8")
    __attribute__((sda));
extern int moby_radius_ring_blend_subtrahend_selector_gp __asm__("D_L01_00161FBC")
    __attribute__((sda));
extern int moby_radius_ring_blend_alpha_factor_selector_gp __asm__("D_L01_00161FC0")
    __attribute__((sda));
extern int moby_radius_ring_blend_addend_selector_gp __asm__("D_L01_00161FC4") __attribute__((sda));
extern int moby_radius_ring_texture_index_gp __asm__("D_L01_00161FC8") __attribute__((sda));
extern int moby_radius_ring_vertex_color_gp __asm__("D_L01_00161FCC") __attribute__((sda));
extern float moby_radius_ring_center_z_gp __asm__("D_L01_00161FD0") __attribute__((sda));

float D_L07_00161C38 = 12.0f;
float D_L07_00161C3C = 12.0f;
float D_L07_00161C40 = 32.0f;
float D_L07_00161C28 = -0.4f;
float D_L07_00161C30 = -2e+01f;
short D_L07_00161C48 = 0x1f4f;
short D_L07_00161C50 = 0x2eea;
short D_L07_00161C58 = 0x53ef;
short D_L07_00161C60 = 0x4654;
int   D_L07_00161C68 = 0;
int   D_L07_00161C6C = 2;
int   D_L07_00161C70 = 0;
int   D_L07_00161C74 = 1;
int   D_L07_00161C78 = 0x13;
int   D_L07_00161C7C = 0x60408080;
float D_L07_00161C80 = 1.0f;
float D_L08_00162268 = 12.0f;
float D_L08_0016226C = 12.0f;
float D_L08_00162270 = 32.0f;
float D_L08_00162258 = -0.4f;
float D_L08_00162260 = -2e+01f;
short D_L08_00162278 = 0x1f4f;
short D_L08_00162280 = 0x2eea;
short D_L08_00162288 = 0x53ef;
short D_L08_00162290 = 0x4654;
int   D_L08_00162298 = 0;
int   D_L08_0016229C = 2;
int   D_L08_001622A0 = 0;
int   D_L08_001622A4 = 1;
int   D_L08_001622A8 = 0x13;
int   D_L08_001622AC = 0x60408080;
float D_L08_001622B0 = 1.0f;
float D_L11_00161900 = -0.4f;
float D_L11_00161908 = -2e+01f;
float D_L12_00161D58 = 12.0f;
float D_L12_00161D5C = 12.0f;
float D_L12_00161D60 = 32.0f;
float D_L12_00161D48 = -0.4f;
float D_L12_00161D50 = -2e+01f;
short D_L12_00161D68 = 0x1f4f;
short D_L12_00161D70 = 0x2eea;
short D_L12_00161D78 = 0x53ef;
short D_L12_00161D80 = 0x4654;
int   D_L12_00161D88 = 0;
int   D_L12_00161D8C = 2;
int   D_L12_00161D90 = 0;
int   D_L12_00161D94 = 1;
int   D_L12_00161D98 = 0x13;
int   D_L12_00161D9C = 0x60408080;
float D_L12_00161DA0 = 1.0f;
float D_L13_00161DB0 = 12.0f;
float D_L13_00161DB4 = 12.0f;
float D_L13_00161DB8 = 32.0f;
float D_L13_00161DA0 = -0.4f;
float D_L13_00161DA8 = -2e+01f;
short D_L13_00161DC0 = 0x1f4f;
short D_L13_00161DC8 = 0x2eea;
short D_L13_00161DD0 = 0x53ef;
short D_L13_00161DD8 = 0x4654;
int   D_L13_00161DE0 = 0;
int   D_L13_00161DE4 = 2;
int   D_L13_00161DE8 = 0;
int   D_L13_00161DEC = 1;
int   D_L13_00161DF0 = 0x13;
int   D_L13_00161DF4 = 0x60408080;
float D_L13_00161DF8 = 1.0f;
float D_L16_00161CF0 = 12.0f;
float D_L16_00161CF4 = 12.0f;
float D_L16_00161CF8 = 32.0f;
float D_L16_00161CE0 = -0.4f;
float D_L16_00161CE8 = -2e+01f;
short D_L16_00161D00 = 0x1f4f;
short D_L16_00161D08 = 0x2eea;
short D_L16_00161D10 = 0x53ef;
short D_L16_00161D18 = 0x4654;
int   D_L16_00161D20 = 0;
int   D_L16_00161D24 = 2;
int   D_L16_00161D28 = 0;
int   D_L16_00161D2C = 1;
int   D_L16_00161D30 = 0x13;
int   D_L16_00161D34 = 0x60408080;
float D_L16_00161D38 = 1.0f;
float D_L17_00161EB8 = 12.0f;
float D_L17_00161EBC = 12.0f;
float D_L17_00161EC0 = 32.0f;
float D_L17_00161EA8 = -0.4f;
float D_L17_00161EB0 = -2e+01f;
short D_L17_00161EC8 = 0x1f4f;
short D_L17_00161ED0 = 0x2eea;
short D_L17_00161ED8 = 0x53ef;
short D_L17_00161EE0 = 0x4654;
int   D_L17_00161EE8 = 0;
int   D_L17_00161EEC = 2;
int   D_L17_00161EF0 = 0;
int   D_L17_00161EF4 = 1;
int   D_L17_00161EF8 = 0x13;
int   D_L17_00161EFC = 0x60408080;
float D_L17_00161F00 = 1.0f;
float D_L18_00162188 = 12.0f;
float D_L18_0016218C = 12.0f;
float D_L18_00162190 = 32.0f;
float D_L18_00162178 = -0.4f;
float D_L18_00162180 = -2e+01f;
short D_L18_00162198 = 0x1f4f;
short D_L18_001621A0 = 0x2eea;
short D_L18_001621A8 = 0x53ef;
short D_L18_001621B0 = 0x4654;
int   D_L18_001621B8 = 0;
int   D_L18_001621BC = 2;
int   D_L18_001621C0 = 0;
int   D_L18_001621C4 = 1;
int   D_L18_001621C8 = 0x13;
int   D_L18_001621CC = 0x60408080;
float D_L18_001621D0 = 1.0f;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309F00), where it is exact; names translated to the US level program. */
void activate_teleporter_and_record_state(struct Moby *moby) __asm__("FUN_L01_00308b28");

void activate_teleporter_and_record_state(struct Moby *moby) {
    int           level_bitset_byte_offset = D_0015ED84 << 8;
    unsigned char moby_group_index;
    *(int *)((u8 *)D_0014C190 + (((short)moby->save_id >> 5) * 4 + level_bitset_byte_offset)) |=
        1 << (moby->save_id & 0x1F);
    moby_killed[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
    moby_group_index = moby->group;
    if (moby_group_index != 0xFF)
        set_moby_list_state((char *)moby_group_index, 1);
    else
        moby->state = 1;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309FB0). */
void teleporter_update(struct Moby *moby) __asm__("FUN_L01_00308bd8");

void teleporter_update(struct Moby *moby) {
    enum {
        TELEPORTER_INITIALIZE = 0,
        TELEPORTER_WAIT_FOR_USE = 1,
        TELEPORTER_PREPARE_TRANSFER = 2,
        TELEPORTER_TRANSFER_TO_DESTINATION = 3,
        TELEPORTER_RETURN_TO_IDLE = 4,
        TELEPORTER_ENTER_WAIT_FOR_USE = 6,
        TELEPORTER_EFFECT_CYCLE_PREPARE = 7,
        TELEPORTER_EFFECT_CYCLE_RETURN_TO_IDLE = 8
    };
    char *data = (char *)moby->pvars;
    int   active = 0;

    if (l01_save_state.cleared[(short)moby->save_id] != 0 ||
        (D_0014C190_257218[current_level_index][((short)moby->save_id) >> 5] >>
         (moby->save_id & 0x1F)) &
            1) {
        active = 1;
    }
    active = active != 0;
    if (moby->state != 0 && *(int *)(data + 4) != -1 && !active &&
        FUN_00214720(&hero.motion.pos, *(int *)(data + 4)) != 0) {
        active = 1;
        activate_teleporter_and_record_state(moby);
        show_teleporter_and_children(moby);
        if (moby->state == 5)
            moby->state = active;
    }
    if (*(int *)(data + 0x3C) != 0) {
        if (alternate_animation_sequence_gp == 0) {
            moby->collision = 0;
            moby->flags |= 0x41;
            return;
        }
        moby->collision = moby->pclass->collision;
        moby->flags &= ~0x41;
    }
    switch (moby->state) {
    case TELEPORTER_INITIALIZE: {
        char **slots = (char **)(data + 0x14);
        int    i;
        moby->update_dist = 0xFF;
        for (i = 0; i < 3; i++) {
            struct Hero *g = &hero;
            char        *child_moby;
            slots[i] = create_moby(0x13B);
            *(short *)(slots[i] + 0x32) = 0x40;
            slots[i][0x31] = 1;
            *(long *)(slots[i] + 0x38) = *(long *)(((char *)g->moby) + 0x38);
            *(unsigned short *)(slots[i] + 0x34) = moby->flags;
            child_moby = slots[i];
            qcopy(child_moby + 0x10, &moby->pos);
            qcopy(child_moby + 0x40, &moby->rot);
            *(float *)(slots[i] + 0x48) =
                fast_add_rotations(MOBY(child_moby)->rot.z, (float)i * 2.0943952f);
        }
        if (*(int *)(data + 0x38) != 0 || active) {
            moby->state = 6;
        } else {
            moby->state = 5;
            hide_teleporter_and_children((L01Moby *)moby);
        }
        update_teleporter_child_transforms(moby);
        break;
    }
    case TELEPORTER_ENTER_WAIT_FOR_USE:
        if (moby->group != 0xFF) {
            set_moby_list_state((char *)moby->group, 1);
        }
        moby->state = 1;
    case TELEPORTER_WAIT_FOR_USE: {
        struct Hero *g;
        if (*(int *)(data + 8) >= 0 &&
            D_0014C050[*(int *)(data + 8) + current_level_index * 16] != 0xFF)
            break;
        if (*(int *)data == -1)
            break;
        g = &hero;
        if (g->vehicle_mode == 2)
            break;
        if (((char *)g->ground_moby) != moby)
            break;
        if (g->air_frames.s != 0)
            break;
        try_set_teleporter_help_message(moby);
        if (!(D_0013CAE4[0] & 0x10))
            break;
        allocate_voice_for_target_entry(0, 0, (void *)moby);
        moby->state = 2;
        *(int *)(data + 0x30) = -1;
        *(int *)(data + 0x2C) = 0;
        *(int *)(data + 0x28) = 0;
        request_hero_target_pose(&moby->pos.x, 0, moby->rot.z);
        play_entity_sound((int)moby, 1.5f, &moby->pos, 0x10000, 20.0f, 1.0f, 0, 1, 0);
        break;
    }
    case TELEPORTER_EFFECT_CYCLE_PREPARE:
        if (*(float *)(data + 0x20) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x20),
                                       (float *)(data + 0x28),
                                       1.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else if (*(float *)(data + 0x24) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x24),
                                       (float *)(data + 0x2C),
                                       1.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else {
            allocate_voice_for_target_entry(1, 0, (void *)moby);
            moby->state = 8;
            *(float *)(data + 0x34) = 0.0f;
        }
        update_teleporter_child_transforms(moby);
        break;
    case TELEPORTER_PREPARE_TRANSFER: {
        int transfer_preparation_status = get_hero_target_pose_status();
        if (*(float *)(data + 0x20) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x20),
                                       (float *)(data + 0x28),
                                       1.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else if (*(float *)(data + 0x24) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x24),
                                       (float *)(data + 0x2C),
                                       1.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else if (D_L01_0015F594 != 4 || !(*(float *)(data + 0x20) > 0.25f)) {
            if (transfer_preparation_status == 2) {
                *(float *)(data + 0x34) = 1.95f;
                moby->state = 3;
            } else if (transfer_preparation_status == 0) {
                moby->state = 4;
            }
        }
        update_teleporter_child_transforms(moby);
        break;
    }
    case TELEPORTER_TRANSFER_TO_DESTINATION:
        if (*(float *)(data + 0x34) != 0.0f) {
            add_draw_callback(draw_moby_radius_ring, moby);
            approach_value((float *)(data + 0x34), 0.0f, frame_time * 4.0f);
        } else {
            char *destination_moby = D_L01_0015FFD8 + (*(int *)data << 8);
            char *td = ((char *)MOBY(destination_moby)->pvars);
            float destination_position[4];
            qcopy(destination_position, destination_moby + 0x10);
            destination_position[2] += 0.2f;
            set_race_camera(destination_position, destination_moby + 0x40, 0, 1);
            hero.unk20A5 = 1;
            if (*(int *)(data + 0x40) != -1 && *(int *)(data + 0x44) != -1) {
                request_music_track(*(int *)(data + 0x40), *(int *)(data + 0x44));
            }
            MOBY(destination_moby)->state = 4;
            *(float *)(td + 0x34) = 0.0f;
            *(float *)(td + 0x28) = 0.0f;
            *(float *)(td + 0x2C) = 0.0f;
            *(float *)(td + 0x20) = 1.0f;
            *(float *)(td + 0x24) = 1.0f;
            allocate_voice_for_target_entry(1, 0, (void *)destination_moby);
            moby->state = 1;
            *(float *)(data + 0x20) = 0.0f;
            *(float *)(data + 0x24) = 0.0f;
            *(float *)(data + 0x28) = 0.0f;
            *(float *)(data + 0x2C) = 0.0f;
            *(float *)(data + 0x34) = 1.95f;
            update_teleporter_child_transforms(moby);
        }
        break;
    case TELEPORTER_RETURN_TO_IDLE:
    case TELEPORTER_EFFECT_CYCLE_RETURN_TO_IDLE:
        if (*(float *)(data + 0x34) < 1.95f) {
            add_draw_callback(draw_moby_radius_ring, moby);
            approach_value((float *)(data + 0x34), 1.95f, frame_time * 4.0f);
        } else if (0.0f < *(float *)(data + 0x24)) {
            advance_accelerated_scalar((float *)(data + 0x24),
                                       (float *)(data + 0x28),
                                       0.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else if (0.0f < *(float *)(data + 0x20)) {
            advance_accelerated_scalar((float *)(data + 0x20),
                                       (float *)(data + 0x2C),
                                       0.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else {
            moby->state = 1;
        }
        update_teleporter_child_transforms(moby);
        break;
    }
}

/* Places the three child mobys at this moby's position and orients each from its data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030A6A8), where it is exact; names translated to the US level program. */
void update_teleporter_child_transforms(char *parent_moby) {
    char  *parent_data = ((char *)MOBY(parent_moby)->pvars);
    char **child_slot = (char **)(parent_data + 0x14);
    float  v[4];
    int    i;
    for (i = 2; i >= 0; i--) {
        float child_offset_length;
        float u;
        char *child_moby = *child_slot;
        qcopy(child_moby + 0x10, parent_moby + 0x10);
        u = teleporter_child_displacement_angle_degrees_gp * DEG_TO_RAD;
        MOBY(child_moby)->pos.z += teleporter_child_position_z_offset_gp;
        *(float *)(*child_slot + 0x44) = u;
        child_offset_length = *(float *)(parent_data + 0x20) * 2.4f - 1.2f;
        euler_rotation_basis(*child_slot + 0xC0, *child_slot + 0x40);
        fast_vec_normalize(v, *child_slot + 0xC0, child_offset_length);
        fast_vec_add(*child_slot + 0x10, *child_slot + 0x10, v);
        *(float *)(*child_slot + 0x44) = *(float *)(parent_data + 0x24) * -1.0471976f - 0.34906584f;
        euler_rotation_basis(*child_slot + 0xC0, *child_slot + 0x40);
        update_entity(*child_slot);
        child_slot++;
    }
}

/* Picks a per-stage table by the current stage number and passes the entry for the moby's index on. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030A808), where it is exact; names translated to the US level program. */
void try_set_teleporter_help_message(struct Moby *moby) {
    int *data = ((int *)moby->pvars);
    switch (D_0015ED84) {
    case 12:
        try_set_help_message(4, ((int *)&level_12_teleporter_help_message_ids)[data[4]]);
        break;
    case 17:
        try_set_help_message(4, level_17_teleporter_help_message_ids[data[4]]);
        break;
    case 1:
    case 7:
    case 13:
        try_set_help_message(4, shared_teleporter_help_message_ids[data[4]]);
        break;
    case 8:
        try_set_help_message(4, ((int *)&level_8_teleporter_help_message_ids)[data[4]]);
        break;
    case 16:
        try_set_help_message(4, ((int *)&level_16_teleporter_help_message_ids)[data[4]]);
        break;
    case 18:
        try_set_help_message(4, ((int *)&level_18_teleporter_help_message_ids)[data[4]]);
        break;
    }
}

/* Ported from rac1-decomp src/overlays/shared/vendor_002F7700.c (func_L01_0030A8C8) */
void draw_moby_radius_ring(char *moby) {
    Quad  quad;
    float rotation_matrix[12];
    float moby_position_snapshot[4];
    char *moby_data;
    float ring_half_height;
    int   i, j, nj;
    moby_data = ((char *)MOBY(moby)->pvars);
    copy_matrix_rotation(rotation_matrix, moby + 0xC0);
    qcopy(moby_position_snapshot, moby + 0x10);
    quad.g[1] = get_effect_texture(moby_radius_ring_texture_index_gp);
    quad.g[3] = (long)moby_radius_ring_blend_minuend_selector_gp |
                ((long)moby_radius_ring_blend_subtrahend_selector_gp << 2) |
                ((long)moby_radius_ring_blend_alpha_factor_selector_gp << 4) |
                ((long)moby_radius_ring_blend_addend_selector_gp << 6) | (0x8000L << 24);
    quad.g[2] = 0xFF9000000260UL;
    quad.g[0] = 0;
    for (i = 0; i < 4; i++) {
        quad.uv[i * 2] = moby_radius_ring_quad_uvs[i * 2];
        *(&quad.uv[i * 2] + 1) = moby_radius_ring_quad_uvs[i * 2 + 1];
        quad.c[i] = moby_radius_ring_vertex_color_gp;
    }
    ring_half_height = 0.1f;
    if (*(float *)(moby_data + 0x34) < 1.15f) {
        float ring_half_height_increase = (1.15f - *(float *)(moby_data + 0x34)) * 2.0f;
        if (1.0f < ring_half_height_increase) {
            ring_half_height_increase = 1.0f;
        } else if (ring_half_height_increase < 0.0f) {
            ring_half_height_increase = 0.0f;
        }
        ring_half_height = ring_half_height_increase + 0.1f;
        if (((unsigned char *)moby)[0x20] != 7 && ((unsigned char *)moby)[0x20] != 8) {
            if (*(float *)(moby_data + 0x34) < 0.65f) {
                D_001413F5_lui = 1;
            } else {
                D_001413F5_lui = 0;
            }
        }
    }
    for (j = 0; j < 32; j = nj) {
        int    k;
        float *vertex;
        k = 0;
        nj = j + 1;
        vertex = quad.v;
        for (; k < 4;) {
            float vertex_angle = (float)(j + k / 2) * 6.28318f * 0.03125f - 3.14159f;
            vertex[0] = fast_cos(vertex_angle) * *(float *)(moby_data + 0x34);
            vertex[1] = fast_sin(vertex_angle) * *(float *)(moby_data + 0x34);
            vertex[2] = 0.0f;
            if (k++ & 1) {
                vertex[2] = moby_radius_ring_center_z_gp + ring_half_height + vertex[2];
            } else {
                vertex[2] = moby_radius_ring_center_z_gp - ring_half_height + vertex[2];
            }
            vertex[3] = 1.0f;
            vertex += 4;
        }
        draw_geometry_quad(&quad, rotation_matrix, 0);
    }
}

/* Mark the parent and its three attached objects active. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030ABC0), where it is exact; names translated to the US level program. */
void hide_teleporter_and_children(L01Moby *moby) __asm__("FUN_L01_003097e8");

void hide_teleporter_and_children(L01Moby *moby) {
    L01Moby **slots = moby->data->slots;
    int       i;
    moby->field94 = 0;
    moby->flags |= 1;
    moby->state = 0;
    for (i = 2; i >= 0; i--, slots++) {
        if (*slots != 0) {
            (*slots)->flags |= 1;
            (*slots)->state = 0;
        }
    }
}

void show_teleporter_and_children(struct Moby *parent_moby) {
    char  *parent_record = ((char *)parent_moby->pclass);
    char **child_slot;
    s32    i;
    s8     one;
    child_slot = (char **)(((char *)parent_moby->pvars) + 0x14);
    parent_moby->collision = *(s32 *)(parent_record + 0x10);
    parent_moby->flags &= 0xFFFE;
    parent_moby->visible = 1;
    one = 1;
    for (i = 2; i >= 0; i--) {
        if (*child_slot != 0) {
            *(u16 *)(*child_slot + 0x34) &= 0xFFFE;
            *(s8 *)(*child_slot + 0x31) = one;
        }
        child_slot++;
    }
}
