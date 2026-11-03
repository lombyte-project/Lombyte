/* Level 11 moby class teleporter: the three-part switch, its help message and ring. */
#include "rnc/input/pad_globals.h"
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/item_state.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/ui/help/try_set_help_message.h"

#include "qcopy.h"
#include "rnc/game/game_globals.h"

extern float         D_0013F3D8_lui __asm__("D_0013F3D8") NOT_SDA; /* hero z */
extern int           D_L11_0015F594 __asm__("D_L11_0015F594");
extern char         *D_L11_0015FFD8_2518 __asm__("D_L11_0015FFD8");
extern unsigned char D_001413F5_lui __asm__("D_001413F5") NOT_SDA;
extern void          FUN_L11_002f3040_2518(char *) __asm__("FUN_L11_002f3040");
extern void          FUN_L11_002f2cd0_2518(char *) __asm__("FUN_L11_002f2cd0");
extern void          FUN_L11_002f2d58_2518(char *) __asm__("FUN_L11_002f2d58");
extern void          update_teleporter_child_transforms(char *) __asm__("FUN_L01_003092d0");
extern void          request_hero_target_pose(float *, int, float) __asm__("FUN_L00_00233ee8");
extern void FUN_L00_002598b0_2518(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern int get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void FUN_L00_00216f90_2518(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L01_0027a248_2518(int, int) __asm__("FUN_L01_0027a248");
extern int  add_draw_callback(void (*)(char *), char *) __asm__("FUN_001f4600");

#include "rnc/overlay/l11/mobys/teleporter_globals.h"

float teleporter_parts_acceleration_gp __asm__("D_L11_00161910") = 12.0f;
float teleporter_parts_deceleration_gp __asm__("D_L11_00161914") = 12.0f;
float teleporter_parts_max_speed_gp __asm__("D_L11_00161918") = 32.0f;
/* L11 twin of teleporter_update: a three-part switch moby that, once the hero
 * stands on it in mission state 0xB and presses the button, swings its parts
 * open and moves the linked moby (or its fallback) along. */
void teleporter_update(struct Moby *moby) __asm__("FUN_L11_002f2518");

typedef struct TeleporterVars {
    s32   destination;
    s32   alternate_destination;
    s32   mission;
    s32   alternate_destination_mission;
    s32   help_message_index;
    char *parts[3];
    f32   part_extension;
    f32   part_tilt;
    s32   part_extension_speed; /* f32 bits */
    s32   part_tilt_speed;      /* f32 bits */
    s32   music_track;          /* request_music_track (FUN_L01_0027a248) on arrival, -1: none */
    s32   music_transition;
    u8    pad_38[0x4];
    f32   ring_radius;
} TeleporterVars;

/* Moby class 318 */
void teleporter_update(struct Moby *moby) {
    enum {
        TELEPORTER_INITIALIZE = 0,
        TELEPORTER_WAIT_FOR_ACTIVATION = 1,
        TELEPORTER_OPEN_PARTS = 2,
        TELEPORTER_CONTRACT_RING = 3,
        TELEPORTER_RESET = 4
    };
    enum {
        HERO_TARGET_POSE_INACTIVE = 0
    };
    TeleporterVars *teleporter_instance_vars = (TeleporterVars *)moby->pvars;

    switch (moby->state) {
    case TELEPORTER_INITIALIZE: {
        char **slots = teleporter_instance_vars->parts;
        int    i;
        int    j;
        moby->state = TELEPORTER_WAIT_FOR_ACTIVATION;
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
        if (teleporter_instance_vars->mission >= 0 &&
            visited52[teleporter_instance_vars->mission + current_level_index * 16] != 0xFF) {
            moby->visible = 0;
            moby->flags |= 1;
            moby->collision = 0;
            for (j = 0; j < 3; j++) {
                *(unsigned short *)(slots[j] + 0x34) |= 1;
                slots[j][0x31] = 0;
                *(int *)(slots[j] + 0x94) = 0;
            }
        }
        update_teleporter_child_transforms(moby);
        break;
    }
    case TELEPORTER_WAIT_FOR_ACTIVATION: {
        struct Hero *g;
        if (teleporter_instance_vars->destination == -1)
            break;
        if ((moby->flags & 1) && teleporter_instance_vars->mission >= 0 &&
            visited52[teleporter_instance_vars->mission + current_level_index * 16] == 0xFF)
            FUN_L11_002f3040_2518(moby);
        g = &hero;
        if (((char *)g->ground_moby) != moby)
            break;
        if (g->air_frames.s != 0)
            break;
        FUN_L11_002f2cd0_2518(moby);
        if (!(pad_pressed_lui & 0x10))
            break;
        if (D_L11_0015F594 != 0xB)
            break;
        allocate_voice_for_target_entry(0, 0, (void *)moby);
        moby->state = TELEPORTER_OPEN_PARTS;
        teleporter_instance_vars->part_extension_speed = 0;
        teleporter_instance_vars->part_tilt_speed = 0;
        request_hero_target_pose(&moby->pos.x, 0, moby->rot.z);
        FUN_L00_002598b0_2518((int)moby, 1.5f, &moby->pos, 0x10000, 20.0f, 1.0f, 0, 1, 0);
        break;
    }
    case TELEPORTER_OPEN_PARTS: {
        int teleport_transition_status = get_hero_target_pose_status();
        if (teleporter_instance_vars->part_extension < 1.0f) {
            advance_accelerated_scalar(&teleporter_instance_vars->part_extension,
                                       (float *)&teleporter_instance_vars->part_extension_speed,
                                       1.0f,
                                       teleporter_parts_acceleration_gp * frame_time_sq,
                                       teleporter_parts_deceleration_gp * frame_time_sq,
                                       teleporter_parts_max_speed_gp * frame_time);
        } else if (teleporter_instance_vars->part_tilt < 1.0f) {
            advance_accelerated_scalar(&teleporter_instance_vars->part_tilt,
                                       (float *)&teleporter_instance_vars->part_tilt_speed,
                                       1.0f,
                                       teleporter_parts_acceleration_gp * frame_time_sq,
                                       teleporter_parts_deceleration_gp * frame_time_sq,
                                       teleporter_parts_max_speed_gp * frame_time);
        } else if (D_L11_0015F594 != 0xB || !(teleporter_instance_vars->part_extension > 0.25f)) {
            if (teleport_transition_status == 2) {
                teleporter_instance_vars->ring_radius = 1.95f;
                moby->state = TELEPORTER_CONTRACT_RING;
            } else if (teleport_transition_status == HERO_TARGET_POSE_INACTIVE) {
                moby->state = TELEPORTER_RESET;
            }
        }
        update_teleporter_child_transforms(moby);
        break;
    }
    case TELEPORTER_CONTRACT_RING:
        if (teleporter_instance_vars->ring_radius != 0.0f) {
            add_draw_callback(FUN_L11_002f2d58_2518, moby);
            approach_value(&teleporter_instance_vars->ring_radius, 0.0f, frame_time * 4.0f);
        } else {
            char *destination_moby =
                D_L11_0015FFD8_2518 + (teleporter_instance_vars->destination << 8);
            char *td;
            float destination_position[4];
            if (teleporter_instance_vars->alternate_destination != -1 &&
                visited52[teleporter_instance_vars->alternate_destination_mission +
                          current_level_index * 16] == 0xFF)
                destination_moby =
                    D_L11_0015FFD8_2518 + (teleporter_instance_vars->alternate_destination << 8);
            td = ((char *)MOBY(destination_moby)->pvars);
            qcopy(destination_position, destination_moby + 0x10);
            destination_position[2] += 0.2f;
            FUN_L00_00216f90_2518(destination_position, destination_moby + 0x40, 0, 1);
            D_001413F5_lui = 1;
            if (teleporter_instance_vars->music_track != -1 &&
                teleporter_instance_vars->music_transition != -1) {
                FUN_L01_0027a248_2518(teleporter_instance_vars->music_track,
                                      teleporter_instance_vars->music_transition);
            }
            FUN_L11_002f3040_2518(destination_moby);
            MOBY(destination_moby)->state = TELEPORTER_RESET;
            *(float *)(td + 0x3C) = 0.0f;
            *(float *)(td + 0x28) = 0.0f;
            *(float *)(td + 0x2C) = 0.0f;
            *(float *)(td + 0x20) = 1.0f;
            *(float *)(td + 0x24) = 1.0f;
            allocate_voice_for_target_entry(1, 0, (void *)destination_moby);
            moby->state = TELEPORTER_WAIT_FOR_ACTIVATION;
            teleporter_instance_vars->part_extension = 0.0f;
            teleporter_instance_vars->part_tilt = 0.0f;
            *(float *)&teleporter_instance_vars->part_extension_speed = 0.0f;
            *(float *)&teleporter_instance_vars->part_tilt_speed = 0.0f;
            teleporter_instance_vars->ring_radius = 1.95f;
            update_teleporter_child_transforms(moby);
        }
        break;
    case TELEPORTER_RESET:
        if (D_0013F3D8_lui < 138.0f)
            D_0013D3DD = 1;
        if (teleporter_instance_vars->ring_radius < 1.95f) {
            add_draw_callback(FUN_L11_002f2d58_2518, moby);
            approach_value(&teleporter_instance_vars->ring_radius, 1.95f, frame_time * 4.0f);
        } else if (0.0f < teleporter_instance_vars->part_tilt) {
            advance_accelerated_scalar(&teleporter_instance_vars->part_tilt,
                                       (float *)&teleporter_instance_vars->part_extension_speed,
                                       0.0f,
                                       teleporter_parts_acceleration_gp * frame_time_sq,
                                       teleporter_parts_deceleration_gp * frame_time_sq,
                                       teleporter_parts_max_speed_gp * frame_time);
        } else if (0.0f < teleporter_instance_vars->part_extension) {
            advance_accelerated_scalar(&teleporter_instance_vars->part_extension,
                                       (float *)&teleporter_instance_vars->part_tilt_speed,
                                       0.0f,
                                       teleporter_parts_acceleration_gp * frame_time_sq,
                                       teleporter_parts_deceleration_gp * frame_time_sq,
                                       teleporter_parts_max_speed_gp * frame_time);
        } else {
            moby->state = TELEPORTER_WAIT_FOR_ACTIVATION;
        }
        update_teleporter_child_transforms(moby);
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002F4040), where it is exact; names translated to the US level program. */

void request_teleporter_help_message(struct Moby *moby) __asm__("FUN_L11_002f2cd0");

void request_teleporter_help_message(struct Moby *moby) {
    TeleporterVars *teleporter_private_vars = (TeleporterVars *)moby->pvars;
    switch (teleporter_private_vars->help_message_index) {
    case 0:
        try_set_help_message(0xB, 0x2B0D);
        break;
    case 3:
        try_set_help_message(0xB, 0x2B0B);
        break;
    case 1:
    case 2:
    case 4:
        try_set_help_message(0xB, 0x2B0C);
        break;
    default:
        try_set_help_message(0xB, 0);
        break;
    }
}

typedef struct {
    float              corner[4][4];
    unsigned int       color[4];
    float              uv[4][2];
    unsigned long long tag[4];
} QuadPacket;

extern unsigned long long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void               draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

extern float teleporter_ring_quad_uvs[] __asm__("D_L11_001D3530");

int          l11_teleporter_ring_blend_a_selector __asm__("D_L11_00161920") = 0;
int          l11_teleporter_ring_blend_b_selector __asm__("D_L11_00161924") = 2;
int          l11_teleporter_ring_blend_c_selector __asm__("D_L11_00161928") = 0;
int          l11_teleporter_ring_blend_d_selector __asm__("D_L11_0016192C") = 1;
int          l11_teleporter_ring_effect_texture_index __asm__("D_L11_00161930") = 0x13;
unsigned int l11_teleporter_ring_vertex_color __asm__("D_L11_00161934") = 0x60408080;
float        teleporter_ring_local_z_center_gp __asm__("D_L11_00161938") = 1.0f;
/* Draws a ring of 32 textured quads around the moby; the band widens as the radius at pvars+0x3C shrinks below 1.15. */
void draw_teleporter_ring(struct Moby *moby) __asm__("FUN_L11_002f2d58");

void draw_teleporter_ring(struct Moby *moby) {
    QuadPacket      quad;
    float           matrix[16];
    TeleporterVars *teleporter_ring_vars = (TeleporterVars *)moby->pvars;
    float           width;
    float           angle;
    int             i, j;

    copy_matrix_rotation(matrix, &moby->rot_row0);
    qcopy(matrix + 12, &moby->pos);
    quad.tag[1] = get_effect_texture(l11_teleporter_ring_effect_texture_index);
    quad.tag[3] = (unsigned long long)l11_teleporter_ring_blend_a_selector |
                  (unsigned long long)l11_teleporter_ring_blend_b_selector << 2 |
                  (unsigned long long)l11_teleporter_ring_blend_c_selector << 4 |
                  (unsigned long long)l11_teleporter_ring_blend_d_selector << 6 | 0x8000000000ULL;
    quad.tag[2] = 0xff9000000260ULL;
    quad.tag[0] = 0;
    {
        float        *uv = teleporter_ring_quad_uvs;
        unsigned int *colors = quad.color;
        float        *dest1 = &quad.uv[0][1];
        float        *dest0 = &quad.uv[0][0];
        int           k = 3;
        do {
            float u = uv[0];
            float v = uv[1];
            *dest0 = u;
            uv += 2;
            *dest1 = v;
            dest0 += 2;
            *colors++ = l11_teleporter_ring_vertex_color;
            dest1 += 2;
            k--;
        } while (k >= 0);
    }
    width = 0.1f;
    if (teleporter_ring_vars->ring_radius < 1.15f) {
        float v = (1.15f - teleporter_ring_vars->ring_radius) +
                  (1.15f - teleporter_ring_vars->ring_radius);
        if (v > 1.0f)
            v = 1.0f;
        else if (v < 0.0f)
            v = 0.0f;
        width = v + 0.1f;
        if (teleporter_ring_vars->ring_radius < 0.65f)
            D_001413F5_lui = 1;
        else
            D_001413F5_lui = 0;
    }
    i = 0;
    do {
        j = 0;
        do {
            float *corner = quad.corner[j];
            angle = (float)(i + j / 2) * 6.28318f * 0.03125f - 3.14159f;
            corner[0] = fast_cos(angle) * teleporter_ring_vars->ring_radius;
            corner[1] = fast_sin(angle) * teleporter_ring_vars->ring_radius;
            corner[2] = 0.0f;
            if (j & 1)
                corner[2] = teleporter_ring_local_z_center_gp + width + 0.0f;
            else
                corner[2] = teleporter_ring_local_z_center_gp - width + 0.0f;
            corner[3] = 1.0f;
            j++;
        } while (j < 4);
        draw_geometry_quad(&quad, matrix, 0);
        i++;
    } while (i < 32);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002F43B0), where it is exact; names translated to the US level program. */

void restore_teleporter_visibility_and_collision(struct Moby *moby) __asm__("FUN_L11_002f3040");

void restore_teleporter_visibility_and_collision(struct Moby *moby) {
    TeleporterVars *teleporter_vars = (TeleporterVars *)moby->pvars;
    char          **child = teleporter_vars->parts;
    int             count;
    int             on;
    moby->visible = 1;
    moby->flags &= 0xFFFE;
    moby->collision = moby->pclass->collision;
    on = 1;
    child_slot_countdown = 2;
    do {
        if (*child) {
            *(unsigned short *)(*child + 0x34) &= 0xFFFE;
            (*child)[0x31] = on;
            *(int *)(*child + 0x94) = *(int *)(*(char **)(*child + 0x24) + 0x10);
        }
        child++;
        child_slot_countdown--;
    } while (child_slot_countdown >= 0);
}
