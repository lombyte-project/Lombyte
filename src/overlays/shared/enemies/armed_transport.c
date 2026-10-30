/* Shared moby class: armed_transport. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

typedef union {
    unsigned int q __attribute__((mode(TI)));
    float        f[4];
} __attribute__((aligned(16))) L16TransportVector;

typedef struct {
    char  pad[0x74];
    u8    active;
    char  pad75[0x13];
    char *owner;
} L16Slot_e;

typedef struct {
    s32   status;
    float amount;
} L16Hit_e;

typedef struct {
    float x, y, z, heading;
} PathPoint2fa0;

typedef struct {
    int           count;
    int           pad[3];
    PathPoint2fa0 pts[1];
} Path2fa0;

char *spawn_armed_transport_rocket(char *owner, char *vec, void *pos, void *vel, int c) __asm__("FUN_L16_002a0dc0");
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
void        set_blarg_elite_commando_parent_transform(char *moby,
                                                      void *owner,
                                                      void *position,
                                                      char *vector) __asm__("FUN_L16_002d3dc0");
void        start_blarg_elite_commando_return_to_initial_position(char *moby,
                                                                  int   arg) __asm__("FUN_L16_002d3de0");
extern int  func_0022ED80_update(int, int, void *) __asm__("FUN_0022da68");

extern char *D_L16_0015FFD8_sda __asm__("D_L16_0015FFD8");

extern int  FUN_0022da68_i(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern int
hit_reaction(void *, char *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);

extern float D_L16_00161D78_gp __asm__("D_L16_00161D78") __attribute__((sda));

extern short D_L16_00161E44, D_L16_00161D80_gp __asm__("D_L16_00161D80") __attribute__((sda));
extern void func_00215CA8_path42_q(float, void *, int, void *, void *, int) __asm__("FUN_00214e58");
extern float
func_00214D88_path42_q(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

extern float armed_transport_path_acceleration_coefficient_gp __asm__("D_L16_00161D84")
    __attribute__((sda));

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E3740), where it is exact; names translated to the US level program. */

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
        spawn_armed_transport_rocket(transport_moby,
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

float D_L18_001DA310[4] = {1.0f, 0.0f, -3.0f, 0.0f};

short D_L18_00162274 = 0;
short D_L18_00162278 = 0;
short D_L18_00162280 = 0;
short D_L18_00162284 = -26214;
float D_L18_00162270 = 1.0f;
float D_L18_0016227C = 15.0f;

float armed_transport_passenger_attachment_offset[4] __asm__("D_L16_001D9680") = {
    1.0f, 0.0f, -3.0f, 0.0f};
