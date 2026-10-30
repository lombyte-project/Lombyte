/* Shared moby classes: armed_transport, grindrail_barrier. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/data_labels.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E3740), where it is exact; names translated to the US level program. */

typedef union {
    unsigned int q __attribute__((mode(TI)));
    float        f[4];
} __attribute__((aligned(16))) L16TransportVector;

char *spawn_class_32_moby(char *owner, char *vec, void *pos, void *vel, int c) __asm__("FUN_L16_002a0dc0");
extern char *D_L16_0015FFD8;
extern char *D_L16_001B0930[];
extern f32   advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float D_L16_0015F580[] MACRO_ADDR;
extern float armed_transport_passenger_attachment_offset[4] __asm__("D_L16_001D9680");
extern int   D_L16_0015F5C4;
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
int          advance_armed_transport_along_path(char *transport_moby,
                                                char *path) __asm__("FUN_L16_002e2e88");

extern int   advance_armed_transport_along_path(char *, char *) __asm__("FUN_L16_002e2e88");
extern short armed_transport_spawn_direction_scale __asm__("D_L16_00161D7C") __attribute__((sda));
extern short D_L16_00161D80;
extern short armed_transport_bob_phase_speed_degrees __asm__("D_L16_00161D88") __attribute__((sda));
extern short armed_transport_bob_amplitude __asm__("D_L16_00161D8C") __attribute__((sda));
extern void *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void FUN_L00_00257470(void *, int, int);
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
void        update_armed_transport_damage_and_targeting(struct Moby *transport_moby) __asm__("FUN_L16_002e2b60");

extern void update_armed_transport_damage_and_targeting(struct Moby *) __asm__("FUN_L16_002e2b60");
void        compute_armed_transport_path_headings(void *path_data) __asm__("FUN_L16_002e2fa0");

extern void compute_armed_transport_path_headings(void *) __asm__("FUN_L16_002e2fa0");
void        update_armed_transport_joint_animation(char *moby) __asm__("FUN_L16_002e3050");

extern void update_armed_transport_joint_animation(char *) __asm__("FUN_L16_002e3050");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
void        set_blarg_elite_commando_parent_transform(char *moby,
                                                      void *owner,
                                                      void *position,
                                                      char *vector) __asm__("FUN_L16_002d3dc0");
void        start_blarg_elite_commando_return_to_initial_position(char *moby,
                                                                  int   arg) __asm__("FUN_L16_002d3de0");
extern int  func_0022ED80_update(int, int, void *) __asm__("FUN_0022da68");

void armed_transport_update(char *m) __asm__("FUN_L16_002e22d8");

void armed_transport_update(char *transport_moby) {
    enum {
        ARMED_TRANSPORT_STATE_INITIALIZE = 0,
        ARMED_TRANSPORT_STATE_WAIT_FOR_ACTIVATION = 1,
        ARMED_TRANSPORT_STATE_FOLLOW_FIRST_PATH = 2,
        ARMED_TRANSPORT_STATE_RELEASE_PASSENGERS = 3,
        ARMED_TRANSPORT_STATE_ALIGN_SECOND_PATH = 4,
        ARMED_TRANSPORT_STATE_FOLLOW_SECOND_PATH = 5,
        ARMED_TRANSPORT_STATE_TRACK_TARGET = 6,
        ARMED_TRANSPORT_STATE_SPAWN_DIRECTED_MOBY = 7,
        ARMED_TRANSPORT_STATE_DYING = 8
    };
    L16TransportVector vector, rotation;
    char              *transport_data = ((char *)MOBY(transport_moby)->pvars);

    update_armed_transport_damage_and_targeting(transport_moby);
    switch (MOBY(transport_moby)->state) {
    case ARMED_TRANSPORT_STATE_INITIALIZE: {
        char *path;
        float speed;

        MOBY(transport_moby)->draw_dist = 255;
        MOBY(transport_moby)->update_dist = 255;
        *(float *)(transport_data + 0x20) = 12.0f;
        *(short *)(transport_data + 0x24) = 12;
        transport_data[0x28] = 3;
        if (*(int *)(transport_data + 0xD8) != -1)
            compute_armed_transport_path_headings(D_L16_001B0930[*(int *)(transport_data + 0xD8)]);
        if (*(int *)(transport_data + 0xDC) != -1)
            compute_armed_transport_path_headings(D_L16_001B0930[*(int *)(transport_data + 0xDC)]);
        speed = *(float *)&D_L16_00161D80 * frame_time;
        *(int *)(transport_data + 0x16C) = 0;
        *(int *)(transport_data + 0x168) = 0;
        *(float *)(transport_data + 0x170) = speed;
        *(float *)(transport_data + 0x194) = MOBY(transport_moby)->pos.z;
        if (*(int *)(transport_data + 0xD8) == -1) {
            transport_moby[0x20] = 6;
            break;
        }
        path = D_L16_001B0930[*(int *)(transport_data + 0xD8)];
        *(float *)(transport_data + 0x174) = fast_vec_dist(path + 0x10, path + 0x20);
        qcopy(transport_moby + 0x10, path + 0x10);
        transport_moby[0x20] = 1;
        transport_moby[0x31] = 0;
        MOBY(transport_moby)->collision = 0;
        MOBY(transport_moby)->flags = (MOBY(transport_moby)->flags | 1) & 0xEFFF;
        break;
    }
    case ARMED_TRANSPORT_STATE_WAIT_FOR_ACTIVATION: {
        int i;

        if (D_L16_0015F5C4 == 0 &&
            (*(int *)(transport_data + 0xD4) == -1 ||
             point_in_path_xy(hero.motion.pos.f,
                              D_L16_001B0930[*(int *)(transport_data + 0xD4)] + 0x10,
                              *(int *)D_L16_001B0930[*(int *)(transport_data + 0xD4)]))) {
            transport_moby[0x20] = 2;
            MOBY(transport_moby)->flags &= 0xFFFE;
            transport_moby[0x31] = 1;
            MOBY(transport_moby)->collision =
                *(int *)(((char *)MOBY(transport_moby)->pclass) + 0x10);
            MOBY(transport_moby)->flags |= 0x1000;
            vector.q = 0;
            vector.f[2] = 3.14159f;
            for (i = 0; i < 4; i++) {
                if (((int *)(transport_data + 0xC0))[i] >= 0) {
                    set_blarg_elite_commando_parent_transform(
                        D_L16_0015FFD8 + (((int *)(transport_data + 0xC0))[i] << 8),
                        transport_moby,
                        armed_transport_passenger_attachment_offset,
                        (char *)vector.f);
                }
            }
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_FOLLOW_FIRST_PATH: {
        int   i, loaded = 0;
        char *path;

        for (i = 0; i < 4; i++) {
            if (((int *)(transport_data + 0xC0))[i] >= 0)
                loaded = 1;
        }
        path = D_L16_001B0930[*(int *)(transport_data + 0xD8)];
        if (loaded && (float)(*(int *)path - *(int *)(transport_data + 0x168)) *
                              *(float *)(transport_data + 0x174) <
                          16.0f) {
            advance_accelerated_scalar((float *)(transport_data + 0x17C),
                                       (float *)(transport_data + 0x180),
                                       1.5707964f,
                                       frame_time_sq * 12.566371f,
                                       frame_time_sq * 12.566371f,
                                       frame_time * 6.2831855f);
        }
        if (advance_armed_transport_along_path(transport_moby, path)) {
            if (loaded) {
                transport_moby[0x20] = 3;
            } else if (*(int *)(transport_data + 0xDC) == -1) {
                transport_moby[0x20] = 6;
            } else {
                transport_moby[0x20] = 4;
            }
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_RELEASE_PASSENGERS: {
        int i, *slot;

        if (!fast_dec_timer((int *)(transport_data + 0x184)))
            break;
        slot = (int *)(transport_data + 0xC0);
        for (i = 0; i < 4; i++, slot++) {
            if (((int *)(transport_data + 0xC0))[i] >= 0) {
                char *passenger = D_L16_0015FFD8 + (((int *)(transport_data + 0xC0))[i] << 8);

                clear_u64_value(rotation.f);
                rotation.f[2] = fast_add_rotations(MOBY(transport_moby)->rot.z, 3.14159f);
                transform_vector_by_basis(
                    vector.f, armed_transport_passenger_attachment_offset, transport_moby + 0xC0);
                fast_vec_add(vector.f, vector.f, transport_moby + 0x10);
                start_blarg_elite_commando_return_to_initial_position(passenger,
                                                                      scale_game_frames(90));
                *slot = -1;
                *(int *)(transport_data + 0x184) = scale_game_frames(30);
                break;
            }
            if (i == 3) {
                if (*(int *)(transport_data + 0xDC) != -1) {
                    transport_moby[0x20] = 4;
                } else {
                    transport_moby[0x20] = 6;
                }
            }
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_ALIGN_SECOND_PATH: {
        float heading = *(float *)(D_L16_001B0930[*(int *)(transport_data + 0xDC)] + 0x1C);

        advance_accelerated_scalar((float *)&MOBY(transport_moby)->rot.z,
                                   (float *)(transport_data + 0x178),
                                   heading,
                                   frame_time_sq * 6.2831855f,
                                   frame_time_sq * 6.2831855f,
                                   frame_time * 6.2831855f);
        if (fast_difference_between_rotations(MOBY(transport_moby)->rot.z, heading) < 0.01f) {
            transport_moby[0x20] = 5;
            *(int *)(transport_data + 0x168) = 0;
            *(int *)(transport_data + 0x16C) = 0;
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_FOLLOW_SECOND_PATH:
        advance_accelerated_scalar((float *)(transport_data + 0x17C),
                                   (float *)(transport_data + 0x180),
                                   0.0f,
                                   frame_time_sq * 12.566371f,
                                   frame_time_sq * 12.566371f,
                                   frame_time * 6.2831855f);
        if (advance_armed_transport_along_path(transport_moby,
                                               D_L16_001B0930[*(int *)(transport_data + 0xDC)])) {
            if (*(int *)(transport_data + 0xD0) == -1) {
                mark_moby_for_removal(transport_moby);
                return;
            }
            transport_moby[0x20] = 6;
        }
        break;
    case ARMED_TRANSPORT_STATE_TRACK_TARGET: {
        float *yaw = (float *)&MOBY(transport_moby)->rot.z;
        float  bearing;

        advance_accelerated_scalar((float *)(transport_data + 0x17C),
                                   (float *)(transport_data + 0x180),
                                   0.0f,
                                   frame_time_sq * 12.566371f,
                                   frame_time_sq * 12.566371f,
                                   frame_time * 6.2831855f);
        bearing = fast_arc_tan(*(float *)(transport_data + 0x70) - MOBY(transport_moby)->pos.x,
                               *(float *)(transport_data + 0x74) - MOBY(transport_moby)->pos.y);
        advance_accelerated_scalar(yaw,
                                   (float *)(transport_data + 0x178),
                                   bearing,
                                   frame_time_sq * 3.1415927f,
                                   frame_time_sq * 3.1415927f,
                                   frame_time * 6.2831855f);
        advance_accelerated_scalar((float *)&MOBY(transport_moby)->rot,
                                   (float *)(transport_data + 0x188),
                                   0.0f,
                                   frame_time_sq * 1.5707964f,
                                   frame_time_sq * 1.5707964f,
                                   frame_time * 6.2831855f);
        if (fast_dec_timer((int *)(transport_data + 0x164)) && *(int *)(transport_data + 0xB4) != 2)
            transport_moby[0x20] = 7;
        break;
    }
    case ARMED_TRANSPORT_STATE_SPAWN_DIRECTED_MOBY:
        moby_point_position(transport_moby, 1, vector.f);
        fast_vec_sub(rotation.f, transport_data + 0x70, vector.f);
        fast_vec_normalize(
            rotation.f, rotation.f, *(float *)&armed_transport_spawn_direction_scale * frame_time);
        func_0022ED80_update(1, 0, transport_moby);
        spawn_class_32_moby(transport_moby,
                            (char *)rotation.f,
                            vector.f,
                            transport_data + 0x70,
                            scale_game_frames(45));
        transport_moby[0x20] = 6;
        *(int *)(transport_data + 0x164) = scale_game_frames(180);
        break;
    case ARMED_TRANSPORT_STATE_DYING: {
        float dt = frame_time;

        *(float *)(transport_data + 0x194) -= dt + dt;
        MOBY(transport_moby)->rot.x += dt * 0.2617994f;
        if (fast_dec_timer((int *)(transport_data + 0x160)) || hero.vehicle_mode == 2) {
            spawn_explosion(transport_moby,
                            D_L16_0015F580,
                            transport_moby + 0x10,
                            0.0f,
                            0.0f,
                            20,
                            12,
                            8,
                            4.0f,
                            2.5f,
                            9.0f,
                            -1,
                            2.0f,
                            0.0f,
                            0,
                            0,
                            -1,
                            0);
            FUN_L00_00263fd8(transport_moby,
                             0x634,
                             (float *)&MOBY(transport_moby)->pos,
                             transport_moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L16_0015F580,
                             D_L16_0015F580,
                             D_L16_0015F580);
            FUN_L00_00263fd8(transport_moby,
                             0x635,
                             (float *)&MOBY(transport_moby)->pos,
                             transport_moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L16_0015F580,
                             D_L16_0015F580,
                             D_L16_0015F580);
            FUN_L00_00263fd8(transport_moby,
                             0x636,
                             (float *)&MOBY(transport_moby)->pos,
                             transport_moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L16_0015F580,
                             D_L16_0015F580,
                             D_L16_0015F580);
            FUN_L00_00257470(transport_moby, 0, -1);
            mark_moby_for_removal(transport_moby);
            return;
        }
        break;
    }
    }
    *(float *)(transport_data + 0x18C) = fast_add_rotations(
        *(float *)(transport_data + 0x18C),
        *(float *)&armed_transport_bob_phase_speed_degrees * DEG_TO_RAD * frame_time);
    MOBY(transport_moby)->pos.z =
        *(float *)(transport_data + 0x194) +
        *(float *)&armed_transport_bob_amplitude * fast_sin(*(float *)(transport_data + 0x18C));
    update_armed_transport_joint_animation(transport_moby);
}

#include "qcopy.h"

extern char *D_L16_0015FFD8_sda __asm__("D_L16_0015FFD8");

typedef struct {
    char  pad[0x74];
    u8    active;
    char  pad75[0x13];
    char *owner;
} L16Slot_e;

extern int  FUN_0022da68_i(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern int
hit_reaction(void *, char *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);

typedef struct {
    s32   status;
    float amount;
} L16Hit_e;

extern float D_L16_00161D78_gp __asm__("D_L16_00161D78") __attribute__((sda));

/* Update interaction ownership, damage responses and pursuit tracking. */
void update_armed_transport_damage_and_targeting(struct Moby *transport_moby) {
    L16Hit_e hit;
    char    *transport_data = (char *)transport_moby->pvars;
    float    zero;
    char    *record;
    int      response;

    transport_moby->scale = transport_moby->pclass->scale * D_L16_00161D78_gp;
    if ((unsigned int)(transport_moby->state - 2) < 4) {
        if (!voice_is_playing(transport_moby, *(int *)(transport_data + 0x190)))
            *(int *)(transport_data + 0x190) = FUN_0022da68_i(0, 4, transport_moby);
    } else if (voice_is_playing(transport_moby, *(int *)(transport_data + 0x190))) {
        int slot = *(int *)(transport_data + 0x190);
        if (slot != -1) {
            L16Slot_e *voice_slot = (L16Slot_e *)(((u8 *)&voice_pool) + slot * 0x70);
            if (voice_slot->owner == transport_moby && voice_slot->active != 0)
                release_voice_slot(slot);
        }
        *(int *)(transport_data + 0x190) = -1;
    }
    hit.amount = zero = 0.0f;
    record = find_moby_hit(transport_moby, 0x330000, 0);
    response = hit_reaction(
        transport_moby, record, transport_data + 0x20, 0, &hit.status, &hit.amount, 0, 4);
    if (hit.status != 1 && transport_moby->state != 8) {
        *(float *)(transport_data + 0x20) -= hit.amount;
        if (*(float *)(transport_data + 0x20) <= zero ||
            (hero.vehicle_mode == 2 && hit.amount >= 2.0f))
            response = 1;
        if (response > 0) {
            if (response >= 3) {
                if (response < 9) {
                    FUN_0022da68_i(3, 0, transport_moby);
                    *(unsigned char *)(transport_data + 0x67) = 120;
                    *(float *)(transport_data + 0x188) =
                        randf_sym(30.0f, 50.0f) * DEG_TO_RAD * frame_time;
                }
            } else {
                FUN_0022da68_i(3, 0, transport_moby);
                if (hero.vehicle_mode != 2)
                    FUN_0022da68_i(2, 0, transport_moby);
                {
                    int *child = (int *)(transport_data + 0xC0), count = 3;
                    do {
                        char *pool = D_L16_0015FFD8_sda;
                        if (*child >= 0)
                            mark_moby_for_removal(pool + (*child << 8));
                        child++;
                    } while (--count >= 0);
                }
                transport_moby->state = 8;
                *(int *)(transport_data + 0x160) = scale_game_frames(90);
                *(unsigned char *)(transport_data + 0x67) = 120;
            }
        }
        trooper_anim_update(transport_moby, transport_data + 0x60);
    }
    transport_moby->hit_slot = 255;
    trooper_anim_finish(transport_moby, transport_data + 0x60);
    if (*(int *)(transport_data + 0xD0) != -1 && D_001413DC[0] != 22) {
        char *path = D_L16_001B0930[*(int *)(transport_data + 0xD0)];
        FUN_L00_0025ff38(
            128.0f, transport_moby, transport_data + 0x70, 0, 0, path + 0x10, *(int *)path);
    } else
        *(int *)(transport_data + 0xB4) = 2;
    if (*(int *)(transport_data + 0xB0) == 0) {
        struct Hero *player = &hero;
        *(int *)(transport_data + 0xB0) = ((int)player->moby);
        qcopy(transport_data + 0x70, &player->motion.pos);
    }
}

/* Advance a path segment and ease the remaining travel distance. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E42F0), where it is exact; names translated to the US level program. */

extern short D_L16_00161E44, D_L16_00161D80_gp __asm__("D_L16_00161D80") __attribute__((sda));
extern void  sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("FUN_00214e58");
extern void func_00215CA8_path42_q(float, void *, int, void *, void *, int) __asm__("FUN_00214e58");
extern float
func_00214D88_path42_q(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

#include "rnc/overlay/shared/enemies/armed_transport_globals.h"

extern float armed_transport_path_acceleration_coefficient_gp __asm__("D_L16_00161D84")
    __attribute__((sda));

int advance_armed_transport_along_path(char *transport_moby, char *path) {
    char *transport_data = ((char *)MOBY(transport_moby)->pvars);
    float progress = (float)*(int *)(transport_data + 0x168) + *(float *)(transport_data + 0x16C) +
                     *(float *)(transport_data + 0x170) / *(float *)(transport_data + 0x174);
    int   index = truncate_float_to_s32(progress);
    float speed;
    float distance, rate;
    char *position;
    *(int *)(transport_data + 0x168) = index;
    *(float *)(transport_data + 0x16C) = progress - (float)index;
    if (index < *(int *)path - 1) {
        position = transport_moby + 0x10;
        func_00215CA8_path42_q(progress, path, 0, position, transport_moby + 0x40, 0);
        *(float *)(transport_data + 0x194) = MOBY(transport_moby)->pos.z;
        speed = 0.0f;
        distance = fast_vec_dist(position, path + (*(int *)path << 4));
        rate = armed_transport_path_acceleration_coefficient_gp * *(float *)&frame_time_sq;
        func_00214D88_path42_q(distance,
                               rate,
                               rate,
                               *(float *)&D_L16_00161D80_gp * frame_time,
                               &speed,
                               (float *)(transport_data + 0x170));
        return 0;
    }
    return 1;
}
/* Sets each path point's heading toward the next point; the last copies the one before. */

typedef struct {
    float x, y, z, heading;
} PathPoint2fa0;

typedef struct {
    int           count;
    int           pad[3];
    PathPoint2fa0 pts[1];
} Path2fa0;

void compute_armed_transport_path_headings(void *path_data) {
    Path2fa0 *path = path_data;
    int       i;
    int       point_count;
    for (i = 0; i < path->count - 1; i++) {
        PathPoint2fa0 *points = path->pts;
        path->pts[i].heading =
            fast_arc_tan(path->pts[i + 1].x - path->pts[i].x, points[i + 1].y - path->pts[i].y);
    }
    point_count = path->count;
    if (point_count >= 2) {
        Path2fa0 *heading_source = (Path2fa0 *)((char *)path + (point_count - 2) * 16);
        Path2fa0 *heading_destination = (Path2fa0 *)((char *)path + (point_count - 1) * 16);
        heading_destination->pts[0].heading = heading_source->pts[0].heading;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E44B8), where it is exact; names translated to the US level program. */

void update_armed_transport_joint_animation(char *moby) {
    float ticks = frame_scale_sq;
    float joint_animation_smoothing_factor = ticks * 0.03f;
    float joint_animation_secondary_control = ticks * 0.3f;
    char *data = ((char *)MOBY(moby)->pvars);
    *(float *)(data + 0x144) = *(float *)(data + 0x17C);
    update_joint_animation(
        moby, data + 0xE0, 2, joint_animation_smoothing_factor, joint_animation_secondary_control);
}
/* Updates a moby effect and advances it when its linked object is ready. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E6B70), where it is exact; names translated to the US level program. */

int check_grindrail_barrier_release_condition(int moby_list_index) __asm__("FUN_L16_002e5cd0");

int         check_grindrail_barrier_release_condition(int idx) __asm__("FUN_L16_002e5cd0");
extern void FUN_L16_002e58d8_u(void) __asm__("FUN_L16_002e58d8");
extern void group_trigger_nearest(int) __asm__("FUN_L16_002e5d30");
extern int  func_0022ED80_6B70(int, int, void *) __asm__("FUN_0022da68");

extern short grindrail_barrier_ring_rotation_speed __asm__("D_L16_00161E00") __attribute__((sda));
extern short grindrail_barrier_ring_phase_speed __asm__("D_L16_00161E04") __attribute__((sda));
extern short grindrail_barrier_accent_rotation_speeds __asm__("D_L16_00161E08")
    __attribute__((sda));
void grindrail_barrier_update(struct Moby *m) __asm__("FUN_L16_002e5708");

void grindrail_barrier_update(struct Moby *barrier_moby) {
    enum {
        GRINDRAIL_BARRIER_STATE_INITIALIZE = 0,
        GRINDRAIL_BARRIER_STATE_COLLISION_ENABLED = 1,
        GRINDRAIL_BARRIER_STATE_COLLISION_DISABLED = 2
    };
    char  *barrier_data = (char *)barrier_moby->pvars;
    float  cycle;
    int    i;
    float *cardinal_angles;
    switch (barrier_moby->state) {
    case GRINDRAIL_BARRIER_STATE_INITIALIZE:
        barrier_moby->state = 1;
        barrier_moby->update_dist = 0xFF;
        break;
    case GRINDRAIL_BARRIER_STATE_COLLISION_ENABLED:
        cycle = *(float *)(barrier_data + 0xC) +
                *(float *)&grindrail_barrier_ring_phase_speed * frame_time;
        *(float *)(barrier_data + 0xC) = cycle;
        if (cycle > 1.0f)
            *(float *)(barrier_data + 0xC) = cycle - 1.0f;
        if (voice_is_playing(barrier_moby, *(int *)(barrier_data + 0x24)) == 0) {
            *(int *)(barrier_data + 0x24) = func_0022ED80_6B70(0, 4, barrier_moby);
        }
        *(float *)(barrier_data + 8) = fast_add_rotations(
            *(float *)(barrier_data + 8),
            *(float *)&grindrail_barrier_ring_rotation_speed * DEG_TO_RAD * frame_time);
        cardinal_angles = (float *)(barrier_data + 0x10);
        for (i = 0; i < 4; i++) {
            cardinal_angles[i] = fast_add_rotations(
                cardinal_angles[i],
                ((float *)&grindrail_barrier_accent_rotation_speeds)[i] * DEG_TO_RAD * frame_time);
        }
        if (barrier_moby->visible)
            add_draw_callback(FUN_L16_002e58d8_u, barrier_moby);
        if (*(int *)(barrier_data + 0x20) != -1 &&
            check_grindrail_barrier_release_condition(*(int *)(barrier_data + 0x20))) {
            barrier_moby->collision = 0;
            barrier_moby->state = 2;
            group_trigger_nearest(*(int *)(barrier_data + 0x20));
        }
        break;
    case GRINDRAIL_BARRIER_STATE_COLLISION_DISABLED:
        if (!check_grindrail_barrier_release_condition(*(int *)(barrier_data + 0x20))) {
            barrier_moby->state = 1;
            barrier_moby->collision = barrier_moby->pclass->collision;
        }
        break;
    }
}
/* Draw the animated ring quads and four rotating accents around a moby. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E6D40), where it is exact; names translated to the US level program. */

typedef struct {
    float         position[4][4];
    int           color[4];
    float         uv[4][2];
    unsigned long flags, texture, giftag, primitive;
} RingQuad;

typedef struct {
    char  pad00[8];
    float yaw, phase;
    float cardinal_angles[4];
} RingData;

extern float grindrail_barrier_ring_uvs[4][2] __asm__("D_L16_001D9780");
extern float D_L16_001D9A70[4][2];
extern float grindrail_barrier_ring_vertex_positions[4][4] __asm__("D_L16_001D97C0");
extern float D_L16_001D9AD0[4][4];
extern float D_L16_001D9A50[4][2];
extern float grindrail_barrier_accent_uvs[4][2] __asm__("D_L16_001D97A0");
extern float D_L16_001D9A90[4][4];
extern float grindrail_barrier_accent_vertex_positions[4][4] __asm__("D_L16_001D9800");

#include "rnc/overlay/shared/enemies/armed_transport_data/draw_grindrail_barrier_effect.h"

extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

void draw_grindrail_barrier_effect(char *barrier_moby) __asm__("FUN_L16_002e58d8");

void draw_grindrail_barrier_effect(char *barrier_moby) {
    RingQuad  quad;
    float     transform[4][4], rotation[4];
    RingData *data = ((RingData *)MOBY(barrier_moby)->pvars);
    float     scale =
        MOBY(barrier_moby)->scale / *(float *)(((char *)MOBY(barrier_moby)->pclass) + 0x24);
    float *u, *v;
    float *angles;
    int   *color_base;
    float  phase, angle_step, phase_step;
    int    i;
    qcopy(rotation, barrier_moby + 0x40);
    qcopy(transform[3], barrier_moby + 0x10);
    quad.texture = get_effect_texture(*(int *)&grindrail_barrier_ring_texture_id);
    u = &quad.uv[0][0];
    v = &quad.uv[0][1];
    quad.giftag = 0xFF9000000260UL;
    quad.primitive = *(int *)&grindrail_barrier_blend_minuend_selector;
    quad.primitive |= (unsigned long)*(int *)&grindrail_barrier_blend_subtrahend_selector << 2;
    quad.primitive |= (unsigned long)*(int *)&grindrail_barrier_blend_multiplier_selector << 4;
    quad.primitive |= (unsigned long)*(int *)&grindrail_barrier_blend_addend_selector << 6;
    quad.primitive |= 0x8000000000UL;
    quad.flags = 0;
    color_base = quad.color;
    {
        int k;
        for (k = 0; k < 4; ++k) {
            u[k * 2] = grindrail_barrier_ring_uvs[k][0];
            v[k * 2] = grindrail_barrier_ring_uvs[k][1];
            color_base[k] = *(int *)&grindrail_barrier_ring_base_color;
            qcopy(quad.position[k], grindrail_barrier_ring_vertex_positions[k]);
            fast_vec_scale(quad.position[k], quad.position[k], scale);
        }
    }
    angles = data->cardinal_angles;
    angle_step = 6.2831855f / convert_integer_to_float(*(int *)&grindrail_barrier_ring_quad_count);
    phase_step = 3.0f / convert_integer_to_float(*(int *)&grindrail_barrier_ring_quad_count);
    phase = data->phase;
    while (phase < 0.0f)
        phase += 1.0f;
    while (phase > 1.0f)
        phase -= 1.0f;
    for (i = 0; i < *(int *)&grindrail_barrier_ring_quad_count;) {
        float blend;
        int   color, j;
        rotation[0] = fast_add_rotations(data->yaw, (float)i * angle_step);
        euler_rotation_basis(transform, rotation);
        if (phase > 0.5f)
            blend = (-phase + 1.0f) + (-phase + 1.0f);
        else
            blend = phase + phase;
        ++i;
        color = fast_tween_color(*(int *)&grindrail_barrier_ring_blend_start_color,
                                 *(int *)&grindrail_barrier_ring_base_color,
                                 blend);
        for (j = 1; j >= 0; --j)
            color_base[j] = color;
        phase += phase_step;
        if (phase > 1.0f)
            phase -= 1.0f;
        draw_geometry_quad(&quad, transform, 0);
    }
    quad.texture = get_effect_texture(*(int *)&grindrail_barrier_accent_texture_id);
    {
        int k;
        int solid_color = *(int *)&grindrail_barrier_accent_color;
        for (k = 0; k < 4; ++k) {
            u[k * 2] = grindrail_barrier_accent_uvs[k][0];
            v[k * 2] = grindrail_barrier_accent_uvs[k][1];
            color_base[k] = solid_color;
            qcopy(quad.position[k], grindrail_barrier_accent_vertex_positions[k]);
        }
    }
    {
        int count;
        for (count = 3; count >= 0; --count) {
            rotation[0] = angles[3 - count];
            euler_rotation_basis(transform, rotation);
            draw_geometry_quad(&quad, transform, 0);
        }
    }
}

/* Returns 1 when every moby in the list is active, 0 if any is not or the list is empty. */
extern short *D_L16_001ABCC0_5cd0[] __asm__("D_L16_001ABCC0");

typedef struct {
    char          pad[0xBC];
    unsigned char act;
    char          pad2[0x43];
} M_5cd0;

int check_grindrail_barrier_release_condition(int moby_list_index) {
    unsigned short *moby_index_entry = (unsigned short *)D_L16_001ABCC0_5cd0[moby_list_index];
    if (moby_index_entry == 0) {
        return 0;
    }
    do {
        if (((M_5cd0 *)D_L16_0015FFD8_sda)[*moby_index_entry & 0x7FFF].act == 0) {
            return 0;
        }
    } while ((short)*moby_index_entry++ >= 0);
    return 1;
}
/* picks the nearest moby in a list (skipping class 0x5A3) and acts on it */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E7198), where it is exact; names translated to the US level program. */

/* D_L16_0015FFD8 is declared sda above (FUN_L16_002e2b60 loads it off $gp), but
   here retail loads it absolute: reach it from a neighbour, past whose size the
   assembler no longer uses $gp. */
#define D_L16_0015FFD8 (((char **)&D_L16_00161D78)[-(0x161D78 - 0x15FFD8) / 4])
extern short *D_L16_001ABCC0[];

extern float D_L16_00161D78;
void         group_trigger_nearest(int moby_list_index) __asm__("FUN_L16_002e5d30");

void group_trigger_nearest(int moby_list_index) {
    short *moby_list_entry = D_L16_001ABCC0[moby_list_index];
    char  *best = 0;
    float  bestd = 50000.0f;
    if (moby_list_entry != 0) {
        do {
            char *candidate_moby =
                D_L16_0015FFD8 + ((*(unsigned short *)moby_list_entry & 0x7FFF) << 8);
            if (MOBY(candidate_moby)->oclass != 0x5A3) {
                float candidate_distance = fast_vec_dist(D_0013E533 + 0xE9D, candidate_moby + 0x10);
                if (candidate_distance < bestd) {
                    bestd = candidate_distance;
                    best = candidate_moby;
                }
            }
        } while (*moby_list_entry++ >= 0);
        if (best != 0) {
            allocate_voice_for_target_entry(0, 0, (int)best);
        }
    }
}

extern void draw_level16_reflection_mesh_batch(void) __asm__("FUN_L16_002e9ce0");

void moby_update_draw_only(struct Moby *moby) __asm__("FUN_L16_002e9df0");

void moby_update_draw_only(struct Moby *moby) {
    enum {
        MOBY_DRAW_ONLY_STATE_INITIALIZE = 0,
        MOBY_DRAW_ONLY_STATE_ENQUEUE_DRAW_CALLBACK = 1
    };
    switch (moby->state) {
    case MOBY_DRAW_ONLY_STATE_INITIALIZE:
        moby->state = 1;
        break;
    case MOBY_DRAW_ONLY_STATE_ENQUEUE_DRAW_CALLBACK:
        add_draw_callback((int)draw_level16_reflection_mesh_batch, (int)moby);
        break;
    }
}

float D_L18_001DA310[4] = {1.0f, 0.0f, -3.0f, 0.0f};

short D_L18_00162274 = 0;
short D_L18_00162278 = 0;
short D_L18_00162280 = 0;
short D_L18_00162284 = -26214;
float D_L18_00162270 = 1.0f;
float D_L18_0016227C = 15.0f;
short D_L18_00162588 = 0;
short D_L18_0016258C = 0;
short D_L18_00162590 = 0;

float D_L18_001EFC10[4][2] = {
    {0.5f, 0.5f},
    {0.5f, 1.0f},
    {0.5f, 0.5f},
    {0.5f, 1.0f},
};

float D_L18_001EFC50[4][4] = {
    {0.0f, -4.0f, 0.9f, 1.0f},
    {0.0f, -4.0f, 0.4f, 1.0f},
    {0.0f,  4.0f, 0.9f, 1.0f},
    {0.0f,  4.0f, 0.4f, 1.0f},
};

float D_L18_001EFC30[4][2] = {
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
};

float D_L18_001EFC90[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

float armed_transport_passenger_attachment_offset[4] __asm__("D_L16_001D9680") = {
    1.0f, 0.0f, -3.0f, 0.0f};

float grindrail_barrier_ring_uvs[4][2] __asm__("D_L16_001D9780") = {
    {0.5f, 0.5f},
    {0.5f, 1.0f},
    {0.5f, 0.5f},
    {0.5f, 1.0f},
};

float grindrail_barrier_ring_vertex_positions[4][4] __asm__("D_L16_001D97C0") = {
    {0.0f, -4.0f, 0.9f, 1.0f},
    {0.0f, -4.0f, 0.4f, 1.0f},
    {0.0f,  4.0f, 0.9f, 1.0f},
    {0.0f,  4.0f, 0.4f, 1.0f},
};

float grindrail_barrier_accent_uvs[4][2] __asm__("D_L16_001D97A0") = {
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
};

float grindrail_barrier_accent_vertex_positions[4][4] __asm__("D_L16_001D9800") = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};
