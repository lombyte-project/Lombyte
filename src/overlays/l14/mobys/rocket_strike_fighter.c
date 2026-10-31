/* Level 14 moby class rocket_strike_fighter (921, "Enemy Fighter (moby921)"): once three missions are done and the hero sees the target, it flies in during a flythrough and fires its rocket (class 922) at the target. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/overlay/level_path.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/math/random.h"
#include "rnc/hero/hero.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/camera/path_flythrough_camera.h"
#include "rnc/overlay/l14/mobys/puff.h"
#include "rnc/overlay/l14/mobys/rocket.h"
#include "rnc/overlay/l14/mobys/enemy_fighter.h"
#include "rnc/overlay/level_camera.h"

extern float vector_length_xy(void *);

extern void debug_print(const char *, short) __asm__("FUN_001e93b0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);

extern int FUN_L00_00257b90(int, int);

extern int
FUN_L00_0025d808_2f05d8(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

extern int D_L14_001BABD0[];

extern void FUN_L00_00263d40(int, int);

extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");

extern char D_L14_0015F580[] MACRO_ADDR;

extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);

float absolute_float(float input) __asm__("func_001F99C0");

extern struct Moby *D_L14_0015FFD8_t __asm__("D_L14_0015FFD8");

extern LevelSaveState D_L14_001BB930;

extern char rocket_strike_fighter_missing_flight_path_error_format[] __asm__("D_L14_001FCE98");

extern char rocket_strike_fighter_empty_flight_path_error_format[] __asm__("D_L14_001FCED0");

extern s32 rocket_strike_fighter_respawn_delay_frames_gp __asm__("D_L14_00161F8C")
    __attribute__((sda));

extern void set_moby_state_two(unsigned char *moby) __asm__("FUN_L06_00303150");

extern void FUN_L00_0025f090(void *, void *, int, float, float);

void spawn_rocket_strike_fighter_exhaust(struct Moby *m) __asm__("FUN_L14_002fd310");

int advance_rocket_strike_fighter_along_flight_path(struct Moby *moby) __asm__("FUN_L14_002fd918");

int advance_rocket_strike_fighter_along_flight_path(struct Moby *m) __asm__("FUN_L14_002fd918");

typedef struct {
    u8  pad0[0x60];
    s32 segment;    /* 0x60 */
    s32 along;      /* 0x64 */
    s32 path;       /* 0x68 */
    s32 sound;      /* 0x6C */
    f32 f70;        /* 0x70 */
    f32 f74;        /* 0x74 */
    f32 f78;        /* 0x78 */
    s32 timer;      /* 0x7C */
    f32 f80;        /* 0x80 */
    f32 f84;        /* 0x84 */
    f32 f88;        /* 0x88 */
    s32 rocket;     /* 0x8C: its class 922 rocket */
    s32 flythrough; /* 0x90: the flythrough camera descriptor it starts */
    s32 stage;      /* 0x94 */
    s32 mission[3]; /* 0x98: the missions that must be complete */
    s32 clip;       /* 0xA4 */
} RocketStrikeFighterVars;

void rocket_strike_fighter_update(struct Moby *moby) __asm__("FUN_L14_002fcbb0");

/* Moby class 921 */
void rocket_strike_fighter_update(struct Moby *moby) {
    enum {
        ROCKET_STRIKE_FIGHTER_STATE_INITIALIZE = 0,
        ROCKET_STRIKE_FIGHTER_STATE_WAIT_FOR_MISSIONS = 1,
        ROCKET_STRIKE_FIGHTER_STATE_FOLLOW_PATH = 2,
        ROCKET_STRIKE_FIGHTER_STATE_WAIT_FOR_RESPAWN = 3
    };
    RocketStrikeFighterVars *moby_vars = (RocketStrikeFighterVars *)moby->pvars;
    Vec4                     target_view_point_or_rocket_offset;
    Vec4                     rocket_position;

    if (moby->state == 0) {
        u16 id = moby->save_id;
        if (D_L14_001BB930.cleared[(short)id] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            mark_moby_and_linked_moby_for_removal(&D_L14_0015FFD8_t[moby_vars->rocket]);
            mark_moby_for_removal(moby);
            return;
        }
    }
    {
        void *hit = find_moby_hit(moby, 0x800000, 0);
        moby->hit_slot = 0xFF;
        if (hit != 0 && moby_vars->rocket == -1 && moby->state != 3) {
            if (current_level_index == 14) {
                enemy_fighter_destruction_count = enemy_fighter_destruction_count + 1;
                if (enemy_fighter_destruction_count >= 3 && item_unlocked[0x18] == 0) {
                    item_unlocked[0x18] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    FUN_L00_00263d40(0x53D6, -1);
                }
            }
            moby->state = 3;
            FUN_L00_0025f090(moby, &moby->pos, -1, 5.0f, 13.0f);
            moby->collision = 0;
            moby->flags |= 0x41;
            if (moby_vars->sound != -1) {
                char *voice_entry = ((char *)&voice_pool) + moby_vars->sound * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby && ((u8 *)voice_entry)[0x74] != 0)
                    release_voice_slot(moby_vars->sound);
            }
            moby_vars->sound = -1;
        }
    }
    switch (moby->state) {
    case ROCKET_STRIKE_FIGHTER_STATE_INITIALIZE:
        if (moby_vars->path == -1) {
            debug_print(rocket_strike_fighter_missing_flight_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)l14_paths[moby_vars->path] == 0) {
            debug_print(rocket_strike_fighter_empty_flight_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby_vars->mission[0] < 0 || moby_vars->mission[1] < 0 || moby_vars->mission[2] < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        initialize_flight_path_state(moby);
        if (moby_vars->rocket != 0) {
            struct Moby *c;
            moby->state = 1;
            moby->update_dist = 0xFF;
            moby->flags |= 0x41;
            moby->collision = 0;
            c = &D_L14_0015FFD8_t[moby_vars->rocket];
            c->flags |= 0x41;
            moby_vars->stage = 0;
        } else {
            moby->state = 2;
            moby->update_dist = 0x80;
        }
        moby->draw_dist = 0x80;
        moby_vars->sound = -1;
        moby_vars->timer = scale_game_frames(rocket_strike_fighter_respawn_delay_frames_gp);
        break;
    case ROCKET_STRIKE_FIGHTER_STATE_WAIT_FOR_MISSIONS: {
        int lvl = current_level_index * 16;
        u8  first_mission_state = visited52[moby_vars->mission[0] + lvl];
        if (first_mission_state == 0xFF) {
            u8 second_mission_state = visited52[moby_vars->mission[1] + lvl];
            if (second_mission_state == first_mission_state) {
                u8 x = visited52[moby_vars->mission[2] + lvl];
                if (x == second_mission_state && hero.frames_off_ground.s == 0 &&
                    hero.ground_moby == 0 &&
                    is_point_inside_clip_volume(&hero.motion.pos, moby_vars->clip) != 0) {
                    struct Moby *rocket = &D_L14_0015FFD8_t[moby_vars->rocket];
                    struct Moby *target = &D_L14_0015FFD8_t[((int *)rocket->pvars)[1]];
                    float        pitch;
                    qcopy(&target_view_point_or_rocket_offset, &target->pos);
                    target_view_point_or_rocket_offset.f[2] += 4.0f;
                    pitch = fast_difference_between_rotations(
                        l14_camera.pitch,
                        -fast_arc_tan(vector_distance_xy(&l14_camera.pos,
                                                         &target_view_point_or_rocket_offset),
                                      target_view_point_or_rocket_offset.f[2] -
                                          l14_camera.pos.f[2]));
                    if (fast_difference_between_rotations(
                            l14_camera.yaw,
                            fast_arc_tan(target->pos.x - l14_camera.pos.f[0],
                                         target->pos.y - l14_camera.pos.f[1])) < 0.4886922f &&
                        pitch < 0.34906584f &&
                        collision_line(&l14_camera.pos,
                                       &target_view_point_or_rocket_offset,
                                       0x12,
                                       target,
                                       0) == 0) {
                        D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |=
                            1 << (moby->save_id & 0x1F);
                        D_L14_001BABD0[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                        moby->flags &= 0xFFBE;
                        moby->collision = moby->pclass->collision;
                        rocket->flags &= 0xFFBE;
                        moby->state = 2;
                        activate_descriptor(moby_vars->flythrough);
                        target->draw_dist = 0xFF;
                        target->update_dist = 0xFF;
                    }
                }
            }
        }
        break;
    }
    case ROCKET_STRIKE_FIGHTER_STATE_FOLLOW_PATH:
        if (advance_rocket_strike_fighter_along_flight_path(moby) != 0)
            return;
        if (moby_vars->rocket != 0 && moby_vars->stage < 2) {
            struct Moby *c;
            if (moby_vars->stage == 0 && moby_vars->segment >= 0x38)
                moby_vars->stage = 1;
            fast_vec_scale(&rocket_position, &moby->rot_row0, 0.21f);
            fast_vec_scale(&target_view_point_or_rocket_offset, &moby->rot_row2, -1.24f);
            fast_vec_add(&rocket_position, &rocket_position, &target_view_point_or_rocket_offset);
            fast_vec_add(&rocket_position, &moby->pos, &rocket_position);
            c = &D_L14_0015FFD8_t[moby_vars->rocket];
            qcopy(&c->pos, &rocket_position);
            qcopy(&c->rot, &moby->rot);
            if (moby_vars->stage == 1) {
                set_moby_state_two((unsigned char *)c);
                moby_vars->stage = 2;
            }
        }
        break;
    case ROCKET_STRIKE_FIGHTER_STATE_WAIT_FOR_RESPAWN:
        if (fast_dec_timer(&moby_vars->timer) != 0) {
            char *path = ((char *)l14_paths[moby_vars->path]);
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
            moby_vars->timer = scale_game_frames(rocket_strike_fighter_respawn_delay_frames_gp);
            moby->state = 2;
            moby_vars->segment = 0;
            moby_vars->along = 0;
            qcopy(&moby->pos, path + 0x10);
            moby->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                       *(float *)(path + 0x24) - *(float *)(path + 0x14));
            moby_vars->f80 = moby_vars->f84 = moby_vars->f88 = moby_vars->f70 = moby_vars->f74 =
                moby_vars->f78 = 0.0f;
        }
        break;
    }
    spawn_rocket_strike_fighter_exhaust(moby);
    if (moby->state != 0 && moby->state != 3 && moby->state != 1 &&
        voice_is_playing(moby, moby_vars->sound) == 0)
        moby_vars->sound = allocate_voice_for_target_entry(0, 4, moby);
}

extern f32 rocket_strike_fighter_exhaust_base_row0_offset_gp __asm__("D_L14_00161F60")
    __attribute__((sda));

extern f32 rocket_strike_fighter_exhaust_paired_emitter_offset_gp __asm__("D_L14_00161F64")
    __attribute__((sda));

extern f32 rocket_strike_fighter_exhaust_base_row2_offset_gp __asm__("D_L14_00161F68")
    __attribute__((sda));

extern f32 rocket_strike_fighter_exhaust_ring_row0_offset_gp __asm__("D_L14_00161F6C")
    __attribute__((sda));

extern f32 rocket_strike_fighter_exhaust_grey_puff_row0_offset_gp __asm__("D_L14_00161F70")
    __attribute__((sda));

#include "rnc/overlay/l14/mobys/rocket_strike_fighter_data/spawn_rocket_strike_fighter_exhaust.h"

void spawn_rocket_strike_fighter_exhaust(struct Moby *moby) {
    Vec4     step;
    Vec4     pos;
    Vec4     common_exhaust_origin;
    L14Puff *puff;
    int      i;
    int      j;
    int      spin;
    int      life;
    int      grey;
    float    size;

    if (moby->state == 0 || moby->state == 3 || moby->state == 1)
        return;
    fast_vec_scale(&step, &moby->rot_row0, rocket_strike_fighter_exhaust_base_row0_offset_gp);
    fast_vec_add(&pos, &moby->pos, &step);
    fast_vec_scale(&step, &moby->rot_row2, rocket_strike_fighter_exhaust_base_row2_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    qcopy(&common_exhaust_origin, &pos);
    fast_vec_scale(&step, &moby->rot_row1, rocket_strike_fighter_exhaust_paired_emitter_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    for (j = 0; j < 2; j++) {
        spin = random_integer_below(16);
        puff = spawn_puff(&pos,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? spin : -spin,
                          D_L14_0015F580,
                          200000.0f,
                          rocket_strike_fighter_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = scale_game_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    size = 100000.0f;
    fast_vec_scale(&step, &moby->rot_row0, rocket_strike_fighter_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    spin = 16;
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = life;
            puff->shade = random_integer_below(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
    size = 300000.0f;
    fast_vec_scale(&step, &moby->rot_row0, rocket_strike_fighter_exhaust_grey_puff_row0_offset_gp);
    fast_vec_add(&pos, &common_exhaust_origin, &step);
    grey = FUN_L00_00257b90(0x40, 0x80);
    grey = 0x7F000000 | grey << 16 | grey << 8 | grey;
    if (random_integer_below(3) != 0) {
        L14Puff *grey_puff;
        spin = random_integer_below(4);
        grey_puff = spawn_puff(&pos,
                               0.05f,
                               1.0f,
                               1.0f,
                               random_integer_below(2) == 0 ? spin : -spin,
                               D_L14_0015F580,
                               size,
                               grey);
        if (grey_puff != 0) {
            L14PuffTail *tail = &grey_puff->tail;
            grey_puff->life = scale_game_frames(60);
            if (random_integer_below(2) != 0)
                grey_puff->alpha = 0x44;
            tail->tint = 0x40;
            tail->mode = 2;
            tail->fade = scale_game_frames(60);
        }
    }
    fast_vec_scale(&step, &moby->rot_row1, rocket_strike_fighter_exhaust_paired_emitter_offset_gp);
    fast_vec_sub(&pos, &common_exhaust_origin, &step);
    for (j = 0; j < 2; j++) {
        spin = random_integer_below(16);
        puff = spawn_puff(&pos,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? spin : -spin,
                          D_L14_0015F580,
                          200000.0f,
                          rocket_strike_fighter_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = scale_game_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    size = 100000.0f;
    fast_vec_scale(&step, &moby->rot_row0, rocket_strike_fighter_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    spin = 16;
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = life;
            puff->shade = random_integer_below(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
}

extern u8 *D_L14_00167500 NOT_SDA;

extern float vector_length_xy(void *);

extern float rocket_strike_fighter_flight_spring_strength_gp __asm__("D_L14_00161F7C")
    __attribute__((sda));

extern float rocket_strike_fighter_flight_spring_damping_gp __asm__("D_L14_00161F80")
    __attribute__((sda));

extern float rocket_strike_fighter_bank_angle_scale_gp __asm__("D_L14_00161F84")
    __attribute__((sda));

extern float rocket_strike_fighter_bank_yaw_error_scale_gp __asm__("D_L14_00161F88")
    __attribute__((sda));

extern float rocket_strike_fighter_path_step_rate_gp __asm__("D_L14_00161F78") __attribute__((sda));

extern void flag_vendor_item(int) __asm__("FUN_L13_00316f48");

int advance_rocket_strike_fighter_along_flight_path(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float path_target_position[4];
    float path_position_delta[4];
    float yaw;
    float yaw_error_factor;
    int   path_boundary_reached;
    path_boundary_reached =
        FUN_L00_0025d808_2f05d8(((int *)l14_paths[*(int *)(moby_vars + 0x68)]),
                                path_target_position,
                                moby_vars + 0x60,
                                moby_vars + 0x64,
                                (unsigned int)*(int *)(moby_vars + 0x8C) >> 31,
                                rocket_strike_fighter_path_step_rate_gp * frame_time);
    if (*(int *)(moby_vars + 0x8C) != 0 &&
        (path_boundary_reached != 0 || *(short *)(D_L14_00167500 + 0x86) != 0x13)) {
        flag_vendor_item(*(int *)(moby_vars + 0x90));
        if (*(int *)(moby_vars + 0x6C) != -1) {
            char *voice_entry = ((char *)&voice_pool) + *(int *)(moby_vars + 0x6C) * 0x70;
            if (*(char **)(voice_entry + 0x88) == moby &&
                ((unsigned char *)voice_entry)[0x74] != 0) {
                release_voice_slot(*(int *)(moby_vars + 0x6C));
            }
        }
        *(int *)(moby_vars + 0x6C) = -1;
        mark_moby_for_removal(moby);
        return 1;
    }
    FUN_L00_0025b8c0(&moby->pos.x,
                     (float *)(moby_vars + 0x70),
                     path_target_position[0],
                     rocket_strike_fighter_flight_spring_strength_gp,
                     rocket_strike_fighter_flight_spring_damping_gp,
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.y,
                     (float *)(moby_vars + 0x74),
                     path_target_position[1],
                     rocket_strike_fighter_flight_spring_strength_gp,
                     rocket_strike_fighter_flight_spring_damping_gp,
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.z,
                     (float *)(moby_vars + 0x78),
                     path_target_position[2],
                     rocket_strike_fighter_flight_spring_strength_gp,
                     rocket_strike_fighter_flight_spring_damping_gp,
                     0.0f);
    fast_vec_sub(path_position_delta, path_target_position, &moby->pos);
    if (0.01f < absolute_float(path_position_delta[0])) {
        if (0.01f < absolute_float(path_position_delta[1])) {
            yaw = fast_arc_tan(path_position_delta[0], path_position_delta[1]);
            yaw_error_factor = fast_subtract_rotations(moby->rot.z, yaw);
            FUN_L00_00258278(moby,
                             (float *)(moby_vars + 0x88),
                             yaw,
                             rocket_strike_fighter_flight_spring_strength_gp,
                             rocket_strike_fighter_flight_spring_damping_gp,
                             0.0f);
            yaw = fast_arc_tan(vector_length_xy(path_position_delta), path_position_delta[2]);
            FUN_L00_0025b8c0(&moby->rot.y,
                             (float *)(moby_vars + 0x84),
                             -yaw,
                             rocket_strike_fighter_flight_spring_strength_gp,
                             rocket_strike_fighter_flight_spring_damping_gp,
                             0.0f);
            yaw_error_factor = yaw_error_factor / rocket_strike_fighter_bank_yaw_error_scale_gp;
            if (1.0f < yaw_error_factor)
                yaw_error_factor = 1.0f;
            if (yaw_error_factor < -1.0f)
                yaw_error_factor = -1.0f;
            FUN_L00_0025b8c0(&moby->rot.x,
                             (float *)(moby_vars + 0x80),
                             yaw_error_factor * rocket_strike_fighter_bank_angle_scale_gp,
                             0.003f,
                             rocket_strike_fighter_flight_spring_damping_gp,
                             0.0f);
        }
    }
    return 0;
}
