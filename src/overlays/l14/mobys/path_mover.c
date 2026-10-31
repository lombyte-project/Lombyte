/* Level 14 path mover moby class. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/level_path.h"

typedef struct {
    u8  pad0[0x20];
    s32 unk20;
    s16 unk24;
    u8  pad26[2];
    u8  unk28;
    u8  pad29[0x15];
    s16 unk3E;
    u8  pad40[0x20];
    u8  carry[0x40]; /* 0x60: carried-object state for update_entity_matrix */
    u8  start_end;   /* 0xA0: 0 starts at the far end */
    u8  padA1[3];
    f32 t;            /* 0xA4: position along the path, 0 to 1 */
    f32 speed;        /* 0xA8 */
    f32 target_speed; /* 0xAC: +-1 / travel frames */
    f32 travel_time;  /* 0xB0: seconds per trip */
    s32 path;         /* 0xB4: index into D_L14_001B0BB0, -1 for none */
    s16 retrigger;    /* 0xB8: set when the hero steps off while stopped */
    s16 wait;         /* 0xBA: frames before it may move again */
    f32 rise;         /* 0xBC: height gained this frame */
    s32 trigger;      /* 0xC0: clip volume that activates it, -1 once active */
    s32 voice;        /* 0xC4 */
    s32 silent;       /* 0xC8 */
    s32 show_volume;  /* 0xCC: clip volume that makes it visible, -1 once shown */
    f32 ease[3];      /* 0xD0: per-axis easing state for vendor_spring */
} PathMoverVars;

extern LevelSaveState D_L14_001BB930;

extern s32 D_L14_001BABD0[];

extern float scale_time(float) __asm__("FUN_001f96b0");

extern int is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");

extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");

void ease_path_mover_to_endpoint(struct Moby *moby) __asm__("FUN_L14_002ac2b8");

extern void ease_path_mover_to_endpoint(struct Moby *) __asm__("FUN_L14_002ac2b8");

extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");

void path_mover_update(struct Moby *moby) __asm__("FUN_L14_002aba80");

/* Moby class 903 */
void path_mover_update(struct Moby *moby) {
    enum {
        PATH_MOVER_COMMAND_WAIT_AT_LAST_POINT = 0,
        PATH_MOVER_COMMAND_WAIT_AT_FIRST_POINT = 1,
        PATH_MOVER_COMMAND_TRAVERSE_PATH = 2
    };
    PathMoverVars *path_mover_vars = (PathMoverVars *)moby->pvars;
    LevelPath     *path;
    Vec4           orig;
    Vec4           delta;
    Vec4           step;
    Vec4           goal;
    s32            on = 0;

    if (path_mover_vars->path == -1) {
        return;
    }
    path = l14_paths[path_mover_vars->path];
    if (path_mover_vars->show_volume != -1) {
        if (is_point_inside_clip_volume(&hero.motion.pos, path_mover_vars->show_volume) == 0) {
            moby->update_dist = 0xFF;
            moby->collision = 0;
            moby->flags |= 0x41;
            return;
        }
        if (path_mover_vars->show_volume != -1 &&
            is_point_inside_clip_volume(&hero.motion.pos, path_mover_vars->show_volume)) {
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
            path_mover_vars->show_volume = -1;
        }
    }
    {
        u16 id;

        if (path_mover_vars->trigger == -1 ||
            is_point_inside_clip_volume(&hero.motion.pos, path_mover_vars->trigger) ||
            D_L14_001BB930.cleared[(s16)(id = moby->save_id)] != 0 ||
            (D_0014C190_257218[current_level_index][(s16)id >> 5] >> (id & 0x1F)) & 1) {
            if (path_mover_vars->trigger != -1) {
                D_0014C190_257218[current_level_index][(s16)moby->save_id >> 5] |=
                    1 << (moby->save_id & 0x1F);
                D_L14_001BABD0[(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                path_mover_vars->trigger = -1;
            }
            on = 1;
        }
    }
    qcopy(&orig, &moby->pos);
    fast_dec_timer_s16(&path_mover_vars->wait);
    if (moby->state == 0) {
        f32   t;
        Vec4 *points;

        path_mover_vars->unk28 = 4;
        path_mover_vars->unk3E = 5;
        path_mover_vars->unk20 = 0;
        path_mover_vars->unk24 = 0;
        t = 0.0f;
        if ((moby->command = path_mover_vars->start_end) == 0) {
            t = 1.0f;
        }
        path_mover_vars->t = t;
        points = path->points;
        /* the unsigned index keeps retail's (count - 1) << 4; a signed one folds the -1 into the pointer */
        qcopy(&moby->pos, truncate_float_to_s32(t) ? &points[(u32)(path->count - 1)] : points);
        path_mover_vars->wait = 0;
        path_mover_vars->speed = 0.0f;
        path_mover_vars->rise = 0.0f;
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        path_mover_vars->voice = -1;
    }
    switch (moby->command) {
    case PATH_MOVER_COMMAND_WAIT_AT_LAST_POINT:
        if (on) {
            f32 far;

            if ((path_mover_vars->retrigger != 0 && is_hero_on_moby(moby)) ||
                (far = fast_vec_dist(&hero.motion.pos, &path->points[path->count - 1]),
                 fast_vec_dist(&hero.motion.pos, &path->points[0]) < far)) {
                moby->command = 2;
                path_mover_vars->target_speed =
                    -1.0f / scale_time(path_mover_vars->travel_time * 60.0f);
                path_mover_vars->retrigger = 0;
            }
        }
        break;
    case PATH_MOVER_COMMAND_WAIT_AT_FIRST_POINT:
        if (on) {
            f32 near;

            if ((path_mover_vars->retrigger != 0 && is_hero_on_moby(moby)) ||
                (near = fast_vec_dist(&hero.motion.pos, &path->points[0]),
                 fast_vec_dist(&hero.motion.pos, &path->points[path->count - 1]) < near)) {
                moby->command = 2;
                path_mover_vars->target_speed =
                    1.0f / scale_time(path_mover_vars->travel_time * 60.0f);
                path_mover_vars->retrigger = 0;
            }
        }
        break;
    case PATH_MOVER_COMMAND_TRAVERSE_PATH:
        if (path_mover_vars->silent == 0 && is_hero_on_moby(moby)) {
            hero.jump_lock_timer = 4;
            hero.unk1F4 = 4;
        }
        if (path_mover_vars->wait != 0 ||
            (path_mover_vars->rise < 0.0f &&
             vector_distance_xy(&moby->pos, &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - moby->pos.z) < 4.0f &&
             moby->pos.z - hero.motion.pos.f[2] > 1.0f)) {
            s32 voice;

            if (path_mover_vars->wait == 0) {
                path_mover_vars->wait = scale_game_frames(30);
            }
            voice = path_mover_vars->voice;
            path_mover_vars->speed = 0.0f;
            if (voice != -1) {
                VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

                if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                    release_voice_slot(voice);
                }
            }
            path_mover_vars->voice = -1;
        } else {
            s32   i;
            f32   frac;
            f32   fi;
            Vec4 *from;

            if (!voice_is_playing(moby, path_mover_vars->voice)) {
                path_mover_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            }
            path_mover_vars->speed += path_mover_vars->target_speed * frame_time;
            if (absolute_float(path_mover_vars->speed) >
                absolute_float(path_mover_vars->target_speed)) {
                path_mover_vars->speed = path_mover_vars->target_speed;
            }
            path_mover_vars->t = path_mover_vars->t + path_mover_vars->speed;
            if (0.5f < absolute_float(path_mover_vars->t - 0.5f)) {
                s32 voice;

                if (0.0f < path_mover_vars->target_speed) {
                    moby->command = 0;
                    path_mover_vars->t = 1.0f;
                } else {
                    moby->command = 1;
                    path_mover_vars->t = 0.0f;
                }
                path_mover_vars->speed = 0.0f;
                path_mover_vars->wait = scale_game_frames(15);
                voice = path_mover_vars->voice;
                if (voice != -1) {
                    VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

                    if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                        release_voice_slot(voice);
                    }
                }
                path_mover_vars->voice = -1;
            }
            frac = convert_integer_to_float(path->count - 1);
            i = truncate_float_to_s32(frac * path_mover_vars->t);
            frac = convert_integer_to_float(path->count - 1);
            fi = convert_integer_to_float(i);
            frac *= path_mover_vars->t;
            frac -= fi;
            if (i == path->count - 1) {
                qcopy(&goal, &path->points[i]);
            } else {
                from = &path->points[i];
                fast_vec_sub(&step, &path->points[i + 1], from);
                fast_vec_scale(&step, &step, frac);
                fast_vec_add(&goal, &step, from);
            }
            vendor_spring(&moby->pos.x, &path_mover_vars->ease[0], goal.f[0], 0.005f, 0.2f, 0.0f);
            vendor_spring(&moby->pos.y, &path_mover_vars->ease[1], goal.f[1], 0.005f, 0.2f, 0.0f);
            vendor_spring(&moby->pos.z, &path_mover_vars->ease[2], goal.f[2], 0.005f, 0.2f, 0.0f);
            path_mover_vars->rise = moby->pos.z - orig.f[2];
        }
        break;
    }
    if (moby->command != 2) {
        ease_path_mover_to_endpoint(moby);
    }
    fast_vec_sub(&delta, &moby->pos, &orig);
    update_entity_matrix(path_mover_vars->carry, &delta, &moby->rot, &moby->rot);
    if (!is_hero_on_moby(moby)) {
        if (2.0f < vector_distance_xy(&moby->pos, &hero.motion.pos)) {
            if (path_mover_vars->wait == 0) {
                path_mover_vars->retrigger = 1;
            }
        }
    }
}

void ease_path_mover_to_endpoint(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float endpoint_target_position[4] __attribute__((aligned(16)));
    int  *path = ((int *)l14_paths[*(int *)(moby_vars + 0xB4)]);
    if (*(float *)(moby_vars + 0xA4) > 0.5f)
        qcopy(endpoint_target_position, (char *)path + path[0] * 16);
    else
        qcopy(endpoint_target_position, (char *)path + 0x10);
    vendor_spring(
        &moby->pos.x, (float *)(moby_vars + 0xD0), endpoint_target_position[0], 0.005f, 0.2f, 0.0f);
    vendor_spring(
        &moby->pos.y, (float *)(moby_vars + 0xD4), endpoint_target_position[1], 0.005f, 0.2f, 0.0f);
    vendor_spring(
        &moby->pos.z, (float *)(moby_vars + 0xD8), endpoint_target_position[2], 0.005f, 0.2f, 0.0f);
}

extern void release_voice_slot(s32) __asm__("FUN_0022d798");

extern int scale_game_frames(int);
