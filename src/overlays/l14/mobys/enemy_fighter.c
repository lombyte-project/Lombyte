/* Level 14 moby class enemy_fighter (908, "Enemy Fighter (moby908)"): flies its path in a loop, puffs exhaust, and respawns after it is shot down. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/math/random.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/l14/mobys/puff.h"
#include "rnc/overlay/l14/mobys/enemy_fighter.h"

extern void debug_print(const char *, short) __asm__("FUN_001e93b0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);

extern void cart_burst(float, float, struct Moby *, void *, int) __asm__("FUN_L00_0025f090");

extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");

void advance_enemy_fighter_along_flight_path(struct Moby *moby) __asm__("FUN_L14_002fc9a0");

extern void advance_enemy_fighter_along_flight_path(struct Moby *) __asm__("FUN_L14_002fc9a0");

extern void spawn_enemy_fighter_exhaust(struct Moby *) __asm__("FUN_L14_002fc3e8");

extern struct {
    char          pad[0x18];
    unsigned char flag;
} D_0013D408;

extern s32 enemy_fighter_respawn_delay_frames_gp __asm__("D_L14_00161F5C") __attribute__((sda));

char D_L14_001FCE10[] = "Enemy Fighter (moby908) with no flyPath. UID: %d - RAR\n";

char D_L14_001FCE48[] =
    "Enemy Fighter (moby908) with a flyPath that has no points. UID: %d - RAR\n";

typedef struct {
    u8  pad0[0x60];
    s32 segment; /* 0x60 */
    s32 along;   /* 0x64 */
    s32 path;    /* 0x68 */
    s32 sound;   /* 0x6C */
    f32 f70;     /* 0x70 */
    f32 f74;     /* 0x74 */
    f32 f78;     /* 0x78 */
    s32 timer;   /* 0x7C */
    f32 f80;     /* 0x80 */
    f32 f84;     /* 0x84 */
    f32 f88;     /* 0x88 */
} EnemyFighterVars;

void enemy_fighter_update(struct Moby *moby) __asm__("FUN_L14_002fc0f0");

void enemy_fighter_update(struct Moby *moby) {
    enum {
        ENEMY_FIGHTER_STATE_INITIALIZE = 0,
        ENEMY_FIGHTER_STATE_FOLLOW_PATH = 1,
        ENEMY_FIGHTER_STATE_WAIT_FOR_RESPAWN = 2
    };
    EnemyFighterVars *data = (EnemyFighterVars *)moby->pvars;
    int               hit = find_moby_hit(moby, 0x800000, 0);
    int               index;
    int               path_index;
    char             *path;
    moby->hit_slot = 0xff;
    if (hit && moby->state != 2) {
        if (current_level_index == 14) {
            enemy_fighter_destruction_count = enemy_fighter_destruction_count + 1;
            if (enemy_fighter_destruction_count > 2 && D_0013D408.flag == 0) {
                D_0013D408.flag = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53d6, -1);
            }
        }
        moby->state = 2;
        cart_burst(5.0f, 13.0f, moby, &moby->pos, -1);
        moby->collision = 0;
        moby->flags |= 0x41;
        index = data->sound;
        if (index != -1) {
            char *entry = ((char *)&voice_pool) + index * 0x70;
            if (*(struct Moby **)(entry + 0x88) == moby && *(unsigned char *)(entry + 0x74) != 0)
                release_voice_slot(index);
        }
        data->sound = -1;
    }
    switch (moby->state) {
    case ENEMY_FIGHTER_STATE_INITIALIZE:
        path_index = data->path;
        if (path_index == -1) {
            debug_print(D_L14_001FCE10, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)l14_paths[path_index] == 0) {
            debug_print(D_L14_001FCE48, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        initialize_flight_path_state(moby);
        moby->state = 1;
        moby->update_dist = 0xff;
        moby->draw_dist = 0xff;
        data->sound = -1;
        data->timer = scale_game_frames(enemy_fighter_respawn_delay_frames_gp);
        break;
    case ENEMY_FIGHTER_STATE_FOLLOW_PATH:
        advance_enemy_fighter_along_flight_path(moby);
        break;
    case ENEMY_FIGHTER_STATE_WAIT_FOR_RESPAWN:
        if (fast_dec_timer(&data->timer)) {
            path = ((char *)l14_paths[data->path]);
            moby->flags &= 0xffbe;
            moby->collision = moby->pclass->collision;
            data->timer = scale_game_frames(enemy_fighter_respawn_delay_frames_gp);
            moby->state = 1;
            data->segment = 0;
            data->along = 0;
            qcopy(&moby->pos, path + 0x10);
            moby->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                       *(float *)(path + 0x24) - *(float *)(path + 0x14));
            data->f80 = data->f84 = data->f88 = data->f70 = data->f74 = data->f78 = 0.0f;
        }
        break;
    }
    spawn_enemy_fighter_exhaust(moby);
    if (moby->state != 0 && moby->state != 2 && voice_is_playing(moby, data->sound) == 0)
        data->sound = allocate_voice_for_target_entry(0, 4, moby);
}

#include "rnc/overlay/l14/mobys/enemy_fighter_data/spawn_enemy_fighter_exhaust.h"

extern char D_L14_0015F580[] MACRO_ADDR;

extern int exhaust_ftoi(float) __asm__("FUN_001fa6d0");

void spawn_enemy_fighter_exhaust(struct Moby *emitter_moby) {
    Vec4     step;
    Vec4     pos;
    Vec4     base;
    L14Puff *puff;
    int      i;
    int      j;
    int      r;
    int      spin;
    int      life;
    float    size;

    if (emitter_moby->state == 0 || emitter_moby->state == 2 || emitter_moby->visible == 0)
        return;
    fast_vec_scale(&step, &emitter_moby->rot_row0, enemy_fighter_exhaust_base_row0_offset_gp);
    fast_vec_add(&pos, &emitter_moby->pos, &step);
    fast_vec_scale(&step, &emitter_moby->rot_row2, enemy_fighter_exhaust_base_row2_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    qcopy(&base, &pos);
    fast_vec_scale(&step, &emitter_moby->rot_row1, enemy_fighter_exhaust_paired_emitter_offset_gp);
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
                          enemy_fighter_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = scale_game_frames(12);
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    spin = 16;
    size = 100000.0f;
    fast_vec_scale(&step, &emitter_moby->rot_row0, enemy_fighter_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
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
    fast_vec_scale(&step, &emitter_moby->rot_row1, enemy_fighter_exhaust_paired_emitter_offset_gp);
    fast_vec_sub(&pos, &base, &step);
    for (j = 0; j < 2; j++) {
        spin = random_integer_below(16);
        puff = spawn_puff(&pos,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? spin : -spin,
                          D_L14_0015F580,
                          200000.0f,
                          enemy_fighter_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            puff->life = scale_game_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    spin = 16;
    size = 100000.0f;
    fast_vec_scale(&step, &emitter_moby->rot_row0, enemy_fighter_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
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

void initialize_flight_path_state(struct Moby *moby) __asm__("FUN_L14_002fc890");

void initialize_flight_path_state(struct Moby *moby) {
    char  *data = (char *)moby->pvars;
    float *path = (float *)l14_paths[*(int *)(data + 0x68)];
    int    i;
    for (i = 0; i < *(int *)path - 1; i++) {
        path[i * 4 + 7] = fast_vec_dist(&path[i * 4 + 4], &path[i * 4 + 8]);
    }
    *(int *)(data + 0x60) = 0;
    *(int *)(data + 0x64) = 0;
    qcopy(&moby->pos, path + 4);
    moby->rot.z = fast_arc_tan(path[8] - path[4], path[9] - path[5]);
    *(float *)(data + 0x80) = *(float *)(data + 0x84) = *(float *)(data + 0x88) =
        *(float *)(data + 0x70) = *(float *)(data + 0x74) = *(float *)(data + 0x78) = 0.0f;
}

extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);

extern void
FUN_L00_0025d808_c(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");

float absolute_float(float input) __asm__("func_001F99C0");

extern short enemy_fighter_path_step_rate_gp __asm__("D_L14_00161F48") __attribute__((sda));

extern short enemy_fighter_flight_spring_strength_gp __asm__("D_L14_00161F4C") __attribute__((sda));

extern short enemy_fighter_flight_spring_damping_gp __asm__("D_L14_00161F50") __attribute__((sda));

extern short enemy_fighter_bank_angle_scale_gp __asm__("D_L14_00161F54") __attribute__((sda));

extern short enemy_fighter_bank_yaw_error_scale_gp __asm__("D_L14_00161F58") __attribute__((sda));

void advance_enemy_fighter_along_flight_path(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float path_target_position[3];
    float path_position_delta[3];
    float target_yaw, yaw_error_factor, path_vertical_angle;
    FUN_L00_0025d808_c((int)((char *)l14_paths[*(int *)(moby_vars + 0x68)]),
                       (char *)path_target_position,
                       moby_vars + 0x60,
                       moby_vars + 0x64,
                       1,
                       (*(float *)&enemy_fighter_path_step_rate_gp) * frame_time);
    FUN_L00_0025b8c0(&moby->pos.x,
                     (float *)(moby_vars + 0x70),
                     path_target_position[0],
                     (*(float *)&enemy_fighter_flight_spring_strength_gp),
                     (*(float *)&enemy_fighter_flight_spring_damping_gp),
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.y,
                     (float *)(moby_vars + 0x74),
                     path_target_position[1],
                     (*(float *)&enemy_fighter_flight_spring_strength_gp),
                     (*(float *)&enemy_fighter_flight_spring_damping_gp),
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.z,
                     (float *)(moby_vars + 0x78),
                     path_target_position[2],
                     (*(float *)&enemy_fighter_flight_spring_strength_gp),
                     (*(float *)&enemy_fighter_flight_spring_damping_gp),
                     0.0f);
    fast_vec_sub(path_position_delta, path_target_position, &moby->pos);
    if (absolute_float(path_position_delta[0]) > 0.01f) {
        if (absolute_float(path_position_delta[1]) > 0.01f) {
            target_yaw = fast_arc_tan(path_position_delta[0], path_position_delta[1]);
            yaw_error_factor = fast_subtract_rotations(moby->rot.z, target_yaw);
            FUN_L00_00258278(moby,
                             (float *)(moby_vars + 0x88),
                             target_yaw,
                             (*(float *)&enemy_fighter_flight_spring_strength_gp),
                             (*(float *)&enemy_fighter_flight_spring_damping_gp),
                             0.0f);
            path_vertical_angle =
                fast_arc_tan(vector_length_xy(path_position_delta), path_position_delta[2]);
            FUN_L00_0025b8c0(&moby->rot.y,
                             (float *)(moby_vars + 0x84),
                             -path_vertical_angle,
                             (*(float *)&enemy_fighter_flight_spring_strength_gp),
                             (*(float *)&enemy_fighter_flight_spring_damping_gp),
                             0.0f);
            yaw_error_factor =
                yaw_error_factor / (*(float *)&enemy_fighter_bank_yaw_error_scale_gp);
            if (yaw_error_factor > 1.0f)
                yaw_error_factor = 1.0f;
            if (yaw_error_factor < -1.0f)
                yaw_error_factor = -1.0f;
            FUN_L00_0025b8c0(&moby->rot.x,
                             (float *)(moby_vars + 0x80),
                             yaw_error_factor * (*(float *)&enemy_fighter_bank_angle_scale_gp),
                             0.003f,
                             (*(float *)&enemy_fighter_flight_spring_damping_gp),
                             0.0f);
        }
    }
}
