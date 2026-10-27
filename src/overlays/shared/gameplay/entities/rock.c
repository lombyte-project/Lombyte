/* Shared moby classes: rock, the_plumber, spawn_point, gold_bolt, teleporter. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"

#include "qcopy.h"

/* A path of points: the point count, then 16-byte points whose w is the length of the segment to the next one */
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 pts[1];        /* 0x10 */
} RailPath;

/* pvars of the rail: paths[] are D_L01_001B0930 rows (paths[0] -1: none) */
typedef struct {
    s32 paths[0x18];
    f32 speed;          /* 0x60: speed it pulls the hero along with */
    f32 range;          /* 0x64: farthest the hero may be from it */
    s32 ready;          /* 0x68: segment lengths measured */
    u8 pad6C[4];
    s32 count;          /* 0x70 */
} RailVars;

extern RailPath *D_L01_001B0930[];
extern s32 D_0015ED84;
extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern s32 FUN_L00_0025df68(void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);
extern s32 sample_path(void *, void *, s32 *, f32 *, s32, f32) __asm__("FUN_L00_0025d808");
extern f32 vendor_spring(f32 *value, f32 target, f32 *speed, f32 accel, f32 decel, f32 max) __asm__("FUN_L00_0025b8c0");

/* Grind rail: measures its paths once, then while the hero is in control mode 0x10 snaps it to the nearest path
   point in range and pulls it along, faster on level 1 the more rail is left. */
void update_rail_hero_motion(struct Moby *rail_moby) __asm__("FUN_L01_002f6328");

void update_rail_hero_motion(struct Moby *rail_moby) {
    RailVars *rail_vars = (RailVars *)rail_moby->pvars;
    RailPath *path;
    RailPath *best_path;
    Vec4 best_pt;
    Vec4 pt;
    Vec4 rail_step_delta;
    Vec4 pull;
    s32 seg;
    f32 candidate_fraction;
    s32 best_seg;
    f32 best_t;
    f32 drag;
    f32 best;
    f32 candidate_path_distance;
    f32 left;
    f32 rate;
    s32 i, j, k, remaining_segment_index, end;

    if (rail_vars == 0 || rail_vars->paths[0] == -1 || rail_vars->count == 0) {
        return;
    }
    if (rail_vars->ready == 0) {
        rail_vars->ready = 1;
        for (i = 0; i < rail_vars->count; i++) {
            RailPath *measured_path = D_L01_001B0930[rail_vars->paths[i]];
            for (j = 0; j < measured_path->count - 1; j++) {
                measured_path->pts[j].f[3] = FUN_001f9b48(&measured_path->pts[j], &measured_path->pts[j + 1]);
            }
            measured_path->pts[j].f[3] = FUN_001f9b48(&measured_path->pts[j], &measured_path->pts[0]);
        }
        rail_moby->update_dist = 0xFF;
    }
    if (hero.state.control_mode != 0x10) {
        return;
    }
    best = 9999.0f;
    best_path = 0;
    for (k = 0; k < rail_vars->count; k++) {
        path = D_L01_001B0930[rail_vars->paths[k]];
        FUN_L00_0025df68(path, &hero.motion.pos, &pt, &seg, &candidate_fraction, 0, 999.0f, 5.0f, 0.0f);
        candidate_path_distance = vector_distance(&hero.motion.pos, &pt);
        if (candidate_path_distance < best) {
            best_path = path;
            qcopy(&best_pt, &pt);
            best = candidate_path_distance;
            best_seg = seg;
            best_t = candidate_fraction;
        }
    }
    if (best_path == 0) {
        return;
    }
    end = sample_path(best_path, &pt, &best_seg, &best_t, 0, 0.3f);
    if (rail_vars->range < best) {
        return;
    }
    if (D_0015ED84 == 1) {
        left = 0.0f;
        for (remaining_segment_index = best_seg; remaining_segment_index < best_path->count - 1; remaining_segment_index++) {
            left += best_path->pts[remaining_segment_index].f[3];
        }
        if (left < 8.0f) {
            rail_vars->speed = frame_time * 3.0f;
        } else if (left < 15.0f) {
            rail_vars->speed = frame_time * 4.0f;
        } else if (left < 30.0f) {
            rail_vars->speed = frame_time * 5.0f;
        } else {
            rail_vars->speed = frame_time * 9.0f;
        }
    }
    if (end == 0) {
        fast_vec_sub(&rail_step_delta, &pt, &best_pt);
        hero.rail_ride_pitch = fast_arc_tan(vector_length_xy(&rail_step_delta), rail_step_delta.f[2]);
        hero.rail_yaw = fast_arc_tan(rail_step_delta.f[0], rail_step_delta.f[1]);
        rail_step_delta.i[2] = 0;
        if (hero.unk9DC < rail_vars->speed) {
            approach_value(&hero.unk9DC, rail_vars->speed, frame_time_sq * 6.0f);
        } else {
            approach_value(&hero.unk9DC, rail_vars->speed, frame_time_sq * 6.0f);
        }
        if (0.001f < fast_vec_length(&rail_step_delta)) {
            fast_vec_normalize(&hero.motion.platform_delta, &rail_step_delta, hero.unk9DC);
        }
        fast_vec_sub(&pull, &best_pt, &hero.motion.pos);
        rate = frame_time * 5.0f;
        pull.f[2] = 0.0f;
        drag = -vector_length_xy(&pull);
        vendor_spring(&drag, 0.0f, &hero.rail_pull_step, frame_scale_sq * 0.008f, frame_scale_sq * 0.3f, rate);
        {
            f32 horizontal_pull_distance = fast_vec_length(&pull);
            if (horizontal_pull_distance < hero.rail_pull_step) {
                hero.rail_pull_step = horizontal_pull_distance;
            }
            if (hero.rail_pull_step < horizontal_pull_distance) {
                fast_vec_normalize(&pull, &pull, hero.rail_pull_step);
            }
        }
        fast_vec_add(&hero.motion.platform_delta, &hero.motion.platform_delta, &pull);
        hero.motion.platform_delta.f[3] = 0.0f;
        qcopy(&hero.motion.momentum, &hero.motion.platform_delta);
        if (hero.unk1E0 < scale_game_frames(30)) {
            hero.unk1E0 = scale_game_frames(30);
        }
    } else {
        fast_vec_scale(&hero.motion.momentum, &hero.motion.momentum, frame_scale * -0.0099999905f + 1.0f);
        qcopy(&hero.motion.platform_delta, &hero.motion.momentum);
        hero.motion.platform_delta.i[3] = 0;
    }
}
#include "rnc/math/vector.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_002F7E08), where it is exact; names translated to the US level program. */

typedef int u128_f6 __attribute__((mode(TI)));

typedef struct Hero Hero;

typedef struct { float x, y, z, w; } Vec_2F7E08 __attribute__((aligned(16)));

typedef struct { int c[6]; } Cols_2F7E08;

typedef struct {
    Vec_2F7E08 vel;
    Vec_2F7E08 target;
    int ids[4];
    int f30;
    int f34;
    float f38;
} Data_2F7E08;

typedef struct {
    int timer;
    short h4;
    int vol;
} Tgt_2F7E08;

typedef struct {
    Vec_2F7E08 dir;
    struct Moby *owner;
    int flags;
    unsigned char b28;
    unsigned char b29;
    short id;
    float scale;
    int count;
} Dmg_2F7E08;

extern Cols_2F7E08 D_L01_0020B470;
extern Cols_2F7E08 D_L01_0020B488;
extern struct Moby *D_L01_001742D8 __attribute__((section(".data")));
extern char *D_L01_0015FFD8_c __asm__("D_L01_0015FFD8") __attribute__((section(".sdata")));
extern char D_0013E533[];
extern char D_L01_00167240[];
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C __attribute__((section(".sdata")));
extern float D_L01_0015F5D0 __attribute__((section(".sdata")));
extern int cutscene_active_macro __asm__("D_L01_0015F404") __attribute__((section(".sdata")));
extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int FUN_L00_00257b90_c(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L00_002ac910(void *, void *, void *, float, int, int, int, int, int);
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int is_point_inside_clip_volume_c(void *arg0, int arg1) __asm__("func_00214720");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below_c2(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern void FUN_L00_0026a4c8(void *, void *, int, int, int, float);
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern void spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern char D_0013F3D0[];
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_explosive_projectile(struct Moby *moby) __asm__("FUN_L01_002f6a30");

void update_explosive_projectile(struct Moby *moby) {
    Vec_2F7E08 old_pos;
    Vec_2F7E08 smoke_pos;
    Data_2F7E08 *data;
    int rate_divisor;
    int i;
    int j;
    int flash;
    int spark;
    int link;
    int max_rings;
    int rings;
    float step_length;
    float size_scale;
    float camera_distance;
    float view_distance;
    float fade;

    rate_divisor = 2;
    if (cutscene_active_macro != 0) {
        rate_divisor = 4;
    }
    data = (*(void **)&moby->pvars);
    qcopy(&old_pos, &(*(Vec_2F7E08 *)&moby->pos));
    add_vector_xyz(&(*(Vec_2F7E08 *)&moby->pos), &(*(Vec_2F7E08 *)&moby->pos), data);
    step_length = vector_length_xyz(data);
    for (i = 0; cutscene_active_macro != 0 ? i < 2 : i < 4; i++) {
        Vec_2F7E08 smoke_vel;
        {
            Vec_2F7E08 random_dir;
            *(u128_f6 *)&random_dir = 0;
            random_dir.x = random_float_between_c(-1.0f, 1.0f);
            random_dir.y = random_float_between_c(-1.0f, 1.0f);
            random_dir.z = random_float_between_c(-1.0f, 1.0f);
            *(u128_f6 *)&smoke_vel = *(u128_f6 *)&random_dir;
        }
        {
            normalize_vector_xyz_c(&smoke_vel, &smoke_vel, random_float_between_c(0.5f, 1.0f) * D_0015ED6C);
            normalize_vector_xyz_c(&smoke_pos, data, -random_float_between_c(D_0015ED6C * 0.1f, D_0015ED6C));
            add_vector_xyz(&smoke_vel, &smoke_vel, &smoke_pos);
            normalize_vector_xyz_c(&smoke_pos, data, random_float_between_c(0.0f, 1.0f) * step_length);
            add_vector_xyz(&smoke_pos, &smoke_pos, &(*(Vec_2F7E08 *)&moby->pos));
            FUN_L00_00269958(&smoke_pos, &smoke_vel, 0x6F00AFFF, 0xFF, scale_game_frames_alt(FUN_L00_00257b90_c(0x1E, 0x2D)) / rate_divisor, 0x50,
                              FUN_L00_00257b90_c(0x50, 0x8C) / rate_divisor, 1);
            normalize_vector_xyz_c(&smoke_pos, data, random_float_between_c(0.0f, 1.0f) * step_length);
            add_vector_xyz(&smoke_pos, &smoke_pos, &(*(Vec_2F7E08 *)&moby->pos));
            FUN_L00_00269958(&smoke_pos, &smoke_vel, 0x1FFFFFFF, 0x4F4F4F, scale_game_frames_alt(FUN_L00_00257b90_c(0x3C, 0x78)) / rate_divisor, 0x50,
                              FUN_L00_00257b90_c(0xC8, 0x12C) / rate_divisor, 0);
        }
    }
    if ((*(Vec_2F7E08 *)&moby->pos).x < 0.0f || (*(Vec_2F7E08 *)&moby->pos).y < 0.0f || (*(Vec_2F7E08 *)&moby->pos).z < 0.0f) {
        mark_moby_for_removal_c(moby);
        return;
    }
    if (distance_xyz(&data->target, &(*(Vec_2F7E08 *)&moby->pos)) < data->f38) {
        qcopy(&(*(Vec_2F7E08 *)&moby->pos), &data->target);
        moby->state = 1;
    }
    if (moby->state != 0) {
        size_scale = 8.0f;
        camera_distance = distance_xyz(D_L01_00167240, &(*(Vec_2F7E08 *)&moby->pos));
        if (cutscene_active_macro != 0) {
            size_scale = 4.0f;
        }
        scale_vector_xyz_c(data, data, -0.55f);
        if (cutscene_active_macro != 0) {
            Dmg_2F7E08 damage;
            {
                damage.owner = moby;
                damage.flags = 0x810001;
                damage.scale = 0.0f;
                clear_u64_value_c(&damage);
                damage.count = 0;
                collect_mobies_in_sphere(1.0f, &(*(Vec_2F7E08 *)&moby->pos), 9, (void *)((Hero *)(D_0013E533 + 0xE1D))->moby, &damage);
                if (D_L01_001742D8 != 0 && D_L01_001742D8->oclass >= 0x2C5 && D_L01_001742D8->oclass < 0x2C8) {
                    allocate_voice_for_target_entry_alt(2, 0, (int)moby);
                } else {
                    allocate_voice_for_target_entry_alt(1, 0, (int)moby);
                }
            }
        }
        view_distance = distance_xyz(&(*(Vec_2F7E08 *)&moby->pos), D_L01_00167240);
        max_rings = 5;
        if (cutscene_active_macro != 0) {
            max_rings = 2;
        }
        if (view_distance < convert_integer_to_float_c(max_rings) * 2.0f) {
            rings = func_001FA898_r(view_distance) / 2;
        } else {
            rings = max_rings;
        }
        fade = 0.0f;
        if (view_distance < 7.0f) {
            fade = 7.0f - view_distance;
        }
        for (j = 0; j < rings; j++) {
            float speed = random_float_between_c(8.0f, 10.0f) * D_0015ED6C - fade * D_0015ED6C;
            Cols_2F7E08 start_colors = D_L01_0020B470;
            Cols_2F7E08 end_colors = D_L01_0020B488;
            speed = speed * size_scale;
            FUN_L00_0026a9f0(&(*(Vec_2F7E08 *)&moby->pos), data, start_colors.c[random_integer_below_c2(6)], end_colors.c[random_integer_below_c2(6)], size_scale * 400000.0f,
                              FUN_L00_00257b90_c(scale_game_frames_alt(0x1E), scale_game_frames_alt(0x28)) / rate_divisor,
                              FUN_L00_00257b90_c(scale_game_frames_alt(0x3C), scale_game_frames_alt(0x5A)) / rate_divisor, 0, 8, speed);
            FUN_L00_0026a9f0(&(*(Vec_2F7E08 *)&moby->pos), data, 0x7FFFFFFF, 0xFFFFFF, size_scale * 400000.0f,
                              FUN_L00_00257b90_c(scale_game_frames_alt(0xA), scale_game_frames_alt(0x14)) / rate_divisor,
                              FUN_L00_00257b90_c(scale_game_frames_alt(0x1E), scale_game_frames_alt(0x28)) / rate_divisor, 0, 8, speed * 0.5f);
        }
        for (flash = 0; flash < 5; flash++) {
            Cols_2F7E08 start_colors = D_L01_0020B470;
            Cols_2F7E08 end_colors = D_L01_0020B488;
            Vec_2F7E08 flash_emission_vector;
            {
                Vec_2F7E08 random_dir;
                *(u128_f6 *)&random_dir = 0;
                random_dir.x = random_float_between_c(-1.0f, 1.0f);
                random_dir.y = random_float_between_c(-1.0f, 1.0f);
                random_dir.z = random_float_between_c(-1.0f, 1.0f);
                *(u128_f6 *)&flash_emission_vector = *(u128_f6 *)&random_dir;
            }
            {
                normalize_vector_xyz_c(&flash_emission_vector, &flash_emission_vector, size_scale * (random_float_between_c(0.0f, 3.0f) * D_0015ED6C));
                FUN_L00_0026a4c8(&(*(Vec_2F7E08 *)&moby->pos), &flash_emission_vector, start_colors.c[random_integer_below_c2(6)], end_colors.c[random_integer_below_c2(6)],
                                  FUN_L00_00257b90_c(scale_game_frames_alt(0xF), scale_game_frames_alt(0x14)) / rate_divisor, 200000.0f);
            }
        }
        if (D_L01_0015F5D0 < 0.95f && view_distance > 9.0f) {
            FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, size_scale * 4.0f, scale_game_frames_alt(0xF), 0x7F, 0x7F, 0x7F, 0x20);
            FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, size_scale * 4.0f, scale_game_frames_alt(0x18), 0x7F, 0x7F, 0x7F, 0x20);
        }
        FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, size_scale * 4.0f, scale_game_frames_alt(0x14), 0x7F, 0x7F, 0, 0x30);
        FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, size_scale * 2.0f, scale_game_frames_alt(0x13), 0xFF, 0xFF, 0xFF, 0x20);
        if (camera_distance > 100.0f) {
            if (D_L01_0015F5D0 < 0.95f && view_distance > 9.0f) {
                FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, 30.0f, scale_game_frames_alt(0xF), 0x7F, 0x7F, 0x7F, 0x20);
                FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, 30.0f, scale_game_frames_alt(0x18), 0x7F, 0x7F, 0x7F, 0x20);
            }
            FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, 30.0f, scale_game_frames_alt(0x14), 0x7F, 0x7F, 0, 0x30);
            FUN_L00_002ac910(moby, &(*(Vec_2F7E08 *)&moby->pos), data, 15.0f, scale_game_frames_alt(0x13), 0xFF, 0xFF, 0xFF, 0x20);
        }
        for (spark = 0; cutscene_active_macro != 0 ? spark < 10 : spark < 40; spark++) {
            Vec_2F7E08 spark_vel;
            float speed;
            float size;
            {
                Vec_2F7E08 random_dir;
                *(u128_f6 *)&random_dir = 0;
                random_dir.x = random_float_between_c(-1.0f, 1.0f);
                random_dir.y = random_float_between_c(-1.0f, 1.0f);
                random_dir.z = random_float_between_c(-1.0f, 1.0f);
                *(u128_f6 *)&spark_vel = *(u128_f6 *)&random_dir;
            }
            speed = random_float_between_c(10.0f, 40.0f) * D_0015ED6C;
                normalize_vector_xyz_c(&spark_vel, &spark_vel, cutscene_active_macro != 0 ? speed * 0.2f : speed);
                size = 105000.0f;
                add_vector_xyz(&spark_vel, &spark_vel, data);
                if (cutscene_active_macro != 0) {
                    size = 21000.0f;
                }
                spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(&(*(Vec_2F7E08 *)&moby->pos), &spark_vel, 0x4F007FFF, 0x1F00007F, size,
                                  FUN_L00_00257b90_c(scale_game_frames_alt(0x5A), scale_game_frames_alt(0x96)) / rate_divisor, 1, -1, -1);
        }
        size_scale = (200.0f - camera_distance) / 200.0f;
        if (data->f34 == 0) {
            if (size_scale < 0.1f) {
                size_scale = 0.1f;
            }
            /* Never used again: the compiler drops this second test, but only after it has kept the first one. */
            if (size_scale > 1.0f) {
                size_scale = 1.0f;
            }
        }
        for (link = 0; link < 4; link++) {
            if (cutscene_active_macro == 0 && data->ids[link] != -1) {
                if (((struct Moby *)D_L01_0015FFD8_c)[data->ids[link]].oclass == 0x2BC) {
                    Tgt_2F7E08 *target = (*(void **)&((struct Moby *)D_L01_0015FFD8_c)[data->ids[link]].pvars);
                    if (target->h4 == 0 && target->timer == 0 && is_point_inside_clip_volume_c(((char *)&D_0013F3D0), target->vol) != 0
                        && is_point_inside_clip_volume_c(D_L01_00167240, target->vol) != 0) {
                        target->timer = scale_game_frames_alt(FUN_L00_00257b90_c(0x3C, 0xB4));
                    }
                }
            }
        }
        mark_moby_for_removal_c(moby);
    }
}
#include "qcopy.h"

/* pvars of the debris moby spawn_debris_chunk spawns */
typedef struct {
    Vec4f vel;      /* 0x00 */
    s32 unk10;      /* 0x10 */
    f32 scale;      /* 0x14 */
    f32 spin;       /* 0x18: random spin, random sign */
    s32 life;       /* 0x1C: frames */
    s32 unk20;      /* 0x20 */
    f32 gravity;    /* 0x24 */
    s32 unk28;      /* 0x28 */
} DebrisVars_2f8530;

extern f32 D_0015ED6C_2f8530 __asm__("D_0015ED6C");
extern struct Moby *create_moby_local(s32) __asm__("FUN_0020c4f8");
extern void copy_hero_moby_field_38(struct Moby *) __asm__("FUN_L00_0025d1b8");
extern f32 random_float_between_2f8530(f32, f32) __asm__("FUN_002132a8");
extern s32 rand_e50(void) __asm__("FUN_001160d8");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void upd_a(struct Moby *) __asm__("FUN_L00_00250df8");

struct Moby *spawn_debris_chunk(Vec4f *pos, Vec4f *vel, s32 oclass, s32 a3, s32 a4,
                              f32 scale, f32 gravity, f32 spin, f32 seconds) __asm__("FUN_L01_002f8530");
struct Moby *spawn_debris_chunk(Vec4f *pos, Vec4f *vel, s32 oclass, s32 initial_timer, s32 fade_out,
                              f32 scale, f32 gravity, f32 spin, f32 seconds) {
    u128 spawn_position_snapshot;
    u128 spawn_velocity_snapshot;
    u128 *pp = &spawn_position_snapshot;
    u128 *vp = &spawn_velocity_snapshot;
    struct Moby *debris_moby;
    DebrisVars_2f8530 *debris_vars;
    f32 random_spin_magnitude;
    f32 initial_spin;

    spawn_position_snapshot = *(u128 *)pos;
    spawn_velocity_snapshot = *(u128 *)vel;
    debris_moby = create_moby_local(oclass);
    if (debris_moby != 0) {
        debris_vars = (DebrisVars_2f8530 *)debris_moby->pvars;
        copy_hero_moby_field_38(debris_moby);
        debris_moby->scale *= scale;
        debris_moby->update_dist = 0xFF;
        debris_moby->draw_dist = 0xFF;
        debris_moby->visible = 1;
        debris_moby->rot.x = random_float_between_2f8530(-3.1415927f, 3.1415927f);
        debris_moby->rot.y = random_float_between_2f8530(-3.1415927f, 3.1415927f);
        debris_moby->rot.z = random_float_between_2f8530(-3.1415927f, 3.1415927f);
        qcopy(&debris_moby->pos, pp);
        qcopy(&debris_vars->vel, vp);
        random_spin_magnitude = random_float_between_2f8530(D_0015ED6C_2f8530 * 1.5707964f,
                                        D_0015ED6C_2f8530 * 6.2831855f);
        if (rand_e50() & 1) {
            initial_spin = -random_spin_magnitude * spin;
        } else {
            initial_spin = random_spin_magnitude * spin;
        }
        debris_vars->unk10 = initial_timer;
        debris_vars->unk28 = initial_timer;
        debris_vars->scale = scale;
        debris_vars->spin = initial_spin;
        debris_vars->life = truncate_float_to_s32(scale_time(seconds * 60.0f));
        debris_vars->unk20 = fade_out;
        debris_vars->gravity = gravity * 9.8f * frame_time_sq;
        upd_a(debris_moby);
    }
    return debris_moby;
}
#include "rnc/overlay/collision.h"

extern CollisionHit D_L01_001742C0;
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern s32 coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void FUN_L00_001ff660(void *, void *, void *);
extern s32 FUN_L00_00257b90(s32, s32);
extern void spawn_effect_particle_with_target_scalar(void *, void *, s32, s32, f32, s32) __asm__("FUN_L00_0026ced0");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Debris update: falls and spins, bounces off what it hits with a random kick, bursts into 40 sparks once it
   comes to rest (or its unk10 timer runs out), or fades out over unk28 frames when unk20 is set. */
void rock_update(struct Moby *m) __asm__("FUN_L01_002f8718");
void rock_update(struct Moby *debris_moby) {
    DebrisVars_2f8530 *debris_vars = (DebrisVars_2f8530 *)debris_moby->pvars;
    Vec4 old;
    Vec4 bounce_or_particle_velocity;
    Vec4 particle_position;
    Vec4 particle_offset_direction;
    f32 velocity_heading;
    f32 spin;
    s32 i;

    qcopy(&old, &debris_moby->pos);
    debris_vars->vel.z -= debris_vars->gravity;
    fast_vec_add(&debris_moby->pos, &debris_moby->pos, &debris_vars->vel);
    debris_moby->rot.z = fast_add_rotations(debris_moby->rot.z, debris_vars->spin);
    if (debris_moby->pos.x < 2.0f || 1021.0f < debris_moby->pos.x || debris_moby->pos.y < 2.0f || 1021.0f < debris_moby->pos.y || debris_moby->pos.z < 2.0f ||
        1021.0f < debris_moby->pos.z) {
        mark_moby_for_removal(debris_moby);
        return;
    }
    if (fast_dec_timer(&debris_vars->life)) {
        if (collision_line(&old, &debris_moby->pos, 0, debris_moby, 0) != 0 || coll_sphere(debris_vars->scale, &debris_moby->pos, 0, debris_moby) != 0) {
            velocity_heading = fast_arc_tan(debris_vars->vel.x, debris_vars->vel.y);
            bounce_or_particle_velocity.f[0] = fast_cos(fast_add_rotations(velocity_heading, random_float_between(-3.1415927f, 3.1415927f))) *
                     random_float_between(0.0f, debris_vars->vel.z * 0.35f);
            bounce_or_particle_velocity.f[1] = fast_sin(fast_add_rotations(velocity_heading, random_float_between(-3.1415927f, 3.1415927f))) *
                     random_float_between(0.0f, debris_vars->vel.z * 0.35f);
            bounce_or_particle_velocity.f[2] = 0.0f;
            debris_moby->rot.x = random_float_between(-3.1415927f, 3.1415927f);
            debris_moby->rot.y = random_float_between(-3.1415927f, 3.1415927f);
            debris_moby->rot.z = random_float_between(-3.1415927f, 3.1415927f);
            FUN_L00_001ff660(&debris_vars->vel, &debris_vars->vel, &D_L01_001742C0.normal_x);
            fast_vec_scale(&debris_vars->vel, &debris_vars->vel, random_float_between(0.07f, 0.13f));
            fast_vec_add(&debris_vars->vel, &debris_vars->vel, &bounce_or_particle_velocity);
            fast_vec_normalize(&D_L01_001742C0.normal_x, &D_L01_001742C0.normal_x, 0.1f);
            fast_vec_add(&debris_moby->pos, &D_L01_001742C0.normal_x, &D_L01_001742C0.point);
            spin = random_float_between(frame_time * 1.5707964f, frame_time * 6.2831855f);
            if (0.0f < debris_vars->spin) {
                spin = -spin;
            }
            debris_vars->spin = spin;
        }
    }
    if (debris_vars->unk20 == 0) {
        if (fast_dec_timer(&debris_vars->unk10) == 0) {
            if (!(fast_vec_length(&debris_vars->vel) < frame_time * 0.5f)) {
                return;
            }
            if (debris_vars->life != 0) {
                return;
            }
        }
        for (i = 0; i < 40; i++) {
            particle_position.q = 0;
            particle_position.f[0] = random_float_between(-1.0f, 1.0f);
            particle_position.f[1] = random_float_between(-1.0f, 1.0f);
            particle_position.f[2] = random_float_between(-1.0f, 1.0f);
            particle_offset_direction.q = 0;
            bounce_or_particle_velocity.q = particle_position.q;
            particle_offset_direction.f[0] = random_float_between(-1.0f, 1.0f);
            particle_offset_direction.f[1] = random_float_between(-1.0f, 1.0f);
            particle_offset_direction.f[2] = random_float_between(-1.0f, 1.0f);
            particle_position.q = particle_offset_direction.q;
            fast_vec_normalize(&particle_position, &particle_position, random_float_between(0.0f, 0.5f));
            fast_vec_add(&particle_position, &particle_position, &debris_moby->pos);
            fast_vec_normalize(&bounce_or_particle_velocity, &bounce_or_particle_velocity, random_float_between(0.0f, 0.5f) * frame_time);
            fast_vec_add(&bounce_or_particle_velocity, &bounce_or_particle_velocity, &debris_vars->vel);
            spawn_effect_particle_with_target_scalar(&particle_position, &bounce_or_particle_velocity, 0x1F0C1820, 0x81020, random_float_between(70000.0f, 140000.0f),
                             scale_game_frames(FUN_L00_00257b90(20, 60)));
        }
        mark_moby_for_removal(debris_moby);
    } else if (fast_dec_timer(&debris_vars->unk10) != 0) {
        mark_moby_for_removal(debris_moby);
    } else {
        debris_moby->alpha = debris_vars->unk10 * 127 / debris_vars->unk28;
    }
}
typedef struct {
    u8 pad0[0xA6];
    s16 type;
} Moby;

typedef struct {
    u8 pad0[0x44];
    s16 count;
    u8 pad46[0x178 - 0x46];
    Moby *list[1];
} MobyList;

typedef struct {
    u8 b0;
    u8 active;
    u8 pad2[0x1E];
    f32 x;
    f32 y;
    f32 z;
} Effect;

extern s32 cutscene_mode __asm__("D_L01_0015F5C4");
extern u8 D_0015EDB0;
extern MobyList D_L01_0016CCE0;
extern Effect D_L01_0017C8C0;
extern void attach_manipulator(void *, int, void *) __asm__("FUN_0020cb10");

void attach_cutscene_fixed_scale_manipulator(void) __asm__("FUN_L01_002fb4b8");

void attach_cutscene_fixed_scale_manipulator(void) {
    s32 i;

    if (cutscene_mode == 2 && D_0015EDB0 != 0) {
        for (i = 0; i < D_L01_0016CCE0.count; i++) {
            int moby_type = D_L01_0016CCE0.list[i]->type;

            if ((moby_type == 0x398 || moby_type == 0x1BF) && D_L01_0017C8C0.active == 0) {
                attach_manipulator(D_L01_0016CCE0.list[i], 0, &D_L01_0017C8C0);
                D_L01_0017C8C0.z = D_L01_0017C8C0.y = D_L01_0017C8C0.x = 2.1f;
            }
        }
    }
}
#include "qcopy.h"

/* Cutscene trigger: unless already seen, starts its camera sequence when the hero enters the volume (or it is
 * triggered), then ends it after its timer and re-arms. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_002FC988), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x454];
    unsigned char collected[1];
} TpLevelState_fc988;

typedef struct {
    char pad0[0x454];
    unsigned char collected[1];
} TpLevelState;

extern char D_L01_00167240[];
extern char D_L01_0020B550[];
extern int D_0015ED84; /* no foreign declaration */
extern int cutscene_active __asm__("D_L01_0015F404");
extern int D_L01_001600EC_q __asm__("D_L01_001600EC");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 D_0014C190[][64];
extern void DebugPrint(char *, ...);
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern void request_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L01_002405a0");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
void FUN_L01_002fb4b8_c(void) __asm__("FUN_L01_002fb4b8");
extern void func_L01_002FC890_m(char *) __asm__("FUN_L01_002fb4b8");
extern TpLevelState_fc988 D_L01_001BB9C0_s __asm__("D_L01_001BB6B0");
extern char D_L01_001672C0_c[] __asm__("D_L01_00167240");
extern char D_0014171B_c[] __asm__("D_0014C190");
extern int stub_printf(void *, int, int) __asm__("FUN_001e93b0");

void update_cutscene_trigger(char *moby) __asm__("FUN_L01_002fb5b0");

void update_cutscene_trigger(char *moby) {
    enum {
        CUTSCENE_TRIGGER_INITIALIZE = 0,
        CUTSCENE_TRIGGER_WAIT_FOR_TRIGGER = 1,
        CUTSCENE_TRIGGER_ACTIVE = 2,
        CUTSCENE_TRIGGER_WAIT_FOR_REARM = 3
    };
    char *trigger_vars = *(char **)(moby + 0x78);
    float saved_camera_position[4];
    float saved_camera_rotation[4];
    func_L01_002FC890_m(moby);
    switch (((unsigned char *)moby)[0x20]) {
    case CUTSCENE_TRIGGER_INITIALIZE:
        if (*(int *)(trigger_vars + 0x24) == -1) {
            stub_printf(D_L01_0020B550, *(short *)(moby + 0xA6), *(unsigned short *)(moby + 0xA8));
            mark_moby_for_removal(moby);
            return;
        }
        *(int *)(trigger_vars + 0x2C) = scale_game_frames(*(int *)(trigger_vars + 0x2C));
        moby[0x20] = 1;
        {
            int ff = 0xFF;
            *(short *)(moby + 0x32) = ff;
            ((unsigned char *)moby)[0x30] = ff;
        }
        break;
    case CUTSCENE_TRIGGER_WAIT_FOR_TRIGGER: {
        char *hero_position;
        if (D_L01_001BB9C0_s.collected[(short)*(unsigned short *)(moby + 0xB2)] != 0) {
            moby[0x20] = 1;
            break;
        }
        if ((*(int *)((((short)*(unsigned short *)(moby + 0xB2) >> 5) << 2) + (D_0015ED84 << 8) +
                      D_0014171B_c) >>
             (*(unsigned short *)(moby + 0xB2) & 0x1F)) &
            1) {
            moby[0x20] = 1;
            break;
        }
        hero_position = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(hero_position, *(int *)(trigger_vars + 0x20)) == 0 &&
            ((unsigned char *)moby)[0xBC] != 1)
            break;
        qcopy(saved_camera_position, D_L01_001672C0_c);
        qcopy(saved_camera_rotation, D_L01_001672C0_c + 0x10);
        if (*(int *)(trigger_vars + 0x28) == -1) {
            FUN_L00_00216f90(hero_position, hero_position + 0x10, 0x72, 1);
        } else {
            char *o = D_L01_001600EC_q + *(int *)(trigger_vars + 0x28) * 128;
            FUN_L00_00216f90(o + 0x30, o + 0x70, 0x72, 1);
        }
        if (*(int *)(trigger_vars + 0x30) != 0) {
            jet_mount(saved_camera_position, saved_camera_rotation, 2, *(int *)(trigger_vars + 0x34), 0);
        } else {
            char *o = D_L01_001600EC_q + *(int *)(trigger_vars + 0x24) * 128;
            jet_mount(o + 0x30, o + 0x70, 0, 0, 0);
        }
        cutscene_active = 1;
        moby[0x20] = 2;
        moby[0xBC] = 1;
        {
            int o = *(int *)(trigger_vars + 0x24) << 7;
            char *record_table_base = D_L01_001600EC_q;
            qcopy(trigger_vars, (char *)(o + (int)record_table_base) + 0x30);
            qcopy(trigger_vars + 0x10, (char *)(o + (int)record_table_base) + 0x70);
        }
        break;
    }
    case CUTSCENE_TRIGGER_ACTIVE:
        if (fast_dec_timer(trigger_vars + 0x2C) != 0 || ((unsigned char *)moby)[0xBC] != 1) {
            if (*(int *)(trigger_vars + 0x30) != 0) {
                FUN_L00_002eac18(2);
            } else {
                FUN_L00_002eac18(0);
            }
            request_hero_idle_state_for_vehicle_mode();
            cutscene_active = 0;
            moby[0x20] = 3;
            moby[0xBC] = 0;
        } else {
            FUN_L00_002ea9d8(trigger_vars);
            FUN_L00_002eaa30(trigger_vars + 0x10);
        }
        break;
    case CUTSCENE_TRIGGER_WAIT_FOR_REARM:
        if (((unsigned char *)moby)[0xBC] == 1)
            moby[0x20] = ((unsigned char *)moby)[0xBC];
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Register this level's vendor data set. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_002FE498), where it is exact; names translated to the US level program. */

extern char D_L01_001E34C0[];
extern void render_water_grids(char *, int) __asm__("FUN_L01_002b91c8");

void render_level_water_grids(void) __asm__("FUN_L01_002fd0c0");

void render_level_water_grids(void) {
    render_water_grids(D_L01_001E34C0, 0x15);
}
/* Resets the marker record's scale once, for mobys of the caller's class or class 0x32B. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00300400), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x44];
    short n;
    char pad1[0x132];
    char *m[1];
} G_q;

extern G_q D_L01_0016CCE0_q __asm__("D_L01_0016CCE0");
extern int cutscene_mode __asm__("D_L01_0015F5C4");
#include "rnc/gameplay/state/options.h"
extern unsigned char D_L01_0017C8C0_q[] __asm__("D_L01_0017C8C0") __attribute__((section(".data")));
extern void attach_manipulator_q(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");

#include "rnc/overlay/shared/gameplay/entities/rock_globals.h"
extern float cutscene_big_head_scale_gp __asm__("D_L01_00161D30") __attribute__((sda));
void attach_cutscene_big_head_manipulator(char *class_reference_moby) __asm__("FUN_L01_002ff028");

void attach_cutscene_big_head_manipulator(char *class_reference_moby) {
    if (cutscene_mode == 2 && big_head_cheat_enabled != 0) {
        int i;
        for (i = 0; i < D_L01_0016CCE0_q.n; i++) {
            char *moby = D_L01_0016CCE0_q.m[i];
            short c = *(short *)(moby + 0xA6);
            if (c == *(short *)(class_reference_moby + 0xA6) || c == 0x32B) {
                unsigned char *manipulator_record = D_L01_0017C8C0_q;
                if (manipulator_record[1] == 0) {
                    attach_manipulator_q(moby, 0, manipulator_record);
                    *(float *)(manipulator_record + 0x20) = cutscene_big_head_scale_gp;
                    *(float *)(manipulator_record + 0x24) = cutscene_big_head_scale_gp;
                    *(float *)(manipulator_record + 0x28) = cutscene_big_head_scale_gp;
                }
            }
        }
    }
}
extern float FUN_001fa580_c(float, float) __asm__("FUN_001fa580");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
#include "sda.h"
extern unsigned char D_0015EDB0_c __asm__("D_0015EDB0");
#include "qcopy.h"
typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L01WatchScratch;
typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0xF];
    unsigned char opacity;
    unsigned char near_flag;
    char pad32[0x16];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x24];
    char *data;
    char pad7C[3];
    unsigned char flag7F;
    char pad80[0x30];
    unsigned char bB0;
    char padB1;
    unsigned short hB2;
} L01WatchMoby;
typedef struct {
    char pad00[4];
    short sub;
    short h6;
    unsigned char b8;
    char pad09[0x17];
    void *p20;
    char pad24[0x20];
    int i44;
    char pad48[4];
    int i4C;
    char body[0x64];
    float pitch;
    float yaw;
    char padBC[4];
    float height;
    char padC4[0xC];
    char head[0x68];
    float head_yaw;
    char pad13C[0x14];
    float target[4];
    char pad160[4];
    int moving_timer;
    int target_timer;
} L01WatchData;
typedef struct {
    char pad00[0xD0];
    float aim[4];
} L01Player;
extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern int tick_countdown_32(int *) __asm__("FUN_001f9740");
extern int FUN_001fa6d0(float);
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void FUN_L00_0025a120(void *);
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern int D_0015ED84_c __asm__("D_0015ED84");
extern int D_0015ED84_m __asm__("D_0015ED84");
extern unsigned char D_0013DD42 __attribute__((section(".data")));
extern unsigned char D_0014C050_c[] __asm__("D_0014C050");
extern char D_L01_0020B600[];
extern char D_L01_00161D38[];
extern char D_L01_00161D48[];
extern char D_L01_00167240_c[] __asm__("D_L01_00167240");
extern char D_L01_001BAAA0[];
extern char *D_L01_001600EC_c __asm__("D_L01_001600EC");
extern int cutscene_mode __asm__("D_L01_0015F5C4");
extern struct {
    char pad[0x80];
    float f80;
    float f84;
    float f88;
} D_0013F350_w __asm__("D_0013F350");
extern void FUN_L01_002ff028_c(void *) __asm__("FUN_L01_002ff028");
extern void initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern int update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void unlock_planet(int) __asm__("FUN_L00_002607d0");
extern void queue_planet_unlock_message(int) __asm__("FUN_L00_00262d38");
extern void record_moby_context_bytes(void *, int, void *, int, void *) __asm__("FUN_L01_0029b0a0");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

/* Watcher moby update with a level-specific state script, then turns its head toward the hero or a random target. */
void the_plumber_update(L01WatchMoby *m) __asm__("FUN_L01_002ff118");
void the_plumber_update(L01WatchMoby *plumber_moby) {
    enum {
        PLUMBER_INITIALIZE = 0,
        PLUMBER_LEVEL_ONE_DIALOGUE = 1,
        PLUMBER_LEVEL_ONE_POST_DIALOGUE = 2,
        PLUMBER_REMOVE = 3,
        PLUMBER_LEVEL_EIGHT_DIALOGUE = 4,
        PLUMBER_LEVEL_EIGHT_POST_DIALOGUE = 5,
        PLUMBER_LEVEL_EIGHT_DIALOGUE_FINISHED = 6
    };
    L01WatchScratch scratch;
    L01WatchData *plumber_vars = (L01WatchData *)plumber_moby->data;
    float rate;
    float head_rate;
    int tracking;
    if (!plumber_vars)
        return;
    FUN_L01_002ff028_c(plumber_moby);
    if (plumber_moby->near_flag) {
        if (FUN_001f9b48(plumber_moby->position, D_L01_00167240_c) < 32.0f) {
            FUN_L00_0025a120(plumber_moby);
            plumber_moby->flag7F = 0x1A;
        }
    }
    switch (plumber_moby->state) {
    case PLUMBER_INITIALIZE:
        plumber_moby->opacity = 0xFF;
        plumber_vars->i44 = 0;
        if (D_0015ED84_m == 1) {
            if (D_0013DD42) {
                mark_moby_for_removal(plumber_moby);
                return;
            }
            plumber_vars->p20 = (void *)D_L01_0020B600;
            plumber_moby->state = 1;
        } else if (D_0015ED84_m == 8) {
            plumber_vars->p20 = (void *)D_L01_00161D38;
            plumber_moby->state = 4;
        } else {
            plumber_vars->p20 = (void *)D_L01_00161D48;
            plumber_moby->state = 7;
        }
        initialize_npc_dialogue(plumber_moby, plumber_vars);
        break;
    case PLUMBER_LEVEL_ONE_DIALOGUE:
        if (update_npc_dialogue(plumber_moby, plumber_vars)) {
            begin_dialogue_camera_transition(plumber_moby, 2.2f);
            plumber_moby->state = 2;
        }
        break;
    case PLUMBER_LEVEL_ONE_POST_DIALOGUE:
        if (cutscene_mode == 2)
            break;
        if (D_0014C050_c[plumber_moby->bB0 + D_0015ED84_c * 16] != 0xFF) {
            jet_clear_persistent(plumber_moby->bB0);
            if (plumber_vars->i4C != -1) {
                char *camera_record = D_L01_001600EC_c + plumber_vars->i4C * 128;
                jet_reset_camera(camera_record + 0x30, camera_record + 0x70);
            }
        }
        plumber_moby->state = 1;
        if (plumber_vars->sub == 4) {
            unlock_planet(2);
            queue_planet_unlock_message(2);
            plumber_moby->state = 3;
            record_moby_context_bytes((char *)plumber_moby + 0x20, 1, plumber_moby, 1, D_L01_001BAAA0);
            memcard_save_data(0, -1);
        }
        break;
    case PLUMBER_REMOVE:
        mark_moby_for_removal(plumber_moby);
        return;
    case PLUMBER_LEVEL_EIGHT_DIALOGUE:
        if (update_npc_dialogue(plumber_moby, plumber_vars)) {
            plumber_moby->state = 5;
            plumber_vars->b8 = 0;
        }
        break;
    case PLUMBER_LEVEL_EIGHT_POST_DIALOGUE:
        if (cutscene_mode == 2)
            break;
        if (plumber_vars->h6 == 2) {
            D_0013F350_w.f80 = 130.5f;
            D_0013F350_w.f84 = 299.5f;
            D_0013F350_w.f88 = 46.0f;
            plumber_moby->state = 6;
        } else {
            plumber_moby->state = 4;
        }
        break;
    case PLUMBER_LEVEL_EIGHT_DIALOGUE_FINISHED:
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (plumber_moby->animation == 0) {
        char *hero_position = (char *)&hero.motion.pos;
        L01Player *hero_aim_view;
        tracking = 1;
        if (vector_distance(plumber_moby->position, hero_position) < 8.0f &&
            (hero_aim_view = (L01Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 plumber_moby->yaw, fast_arc_tan(hero_aim_view->aim[0] - plumber_moby->position[0], hero_aim_view->aim[1] - plumber_moby->position[1]))) <
                1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                plumber_vars->moving_timer = scale_game_frames(120);
            else
                tick_countdown_32(&plumber_vars->moving_timer);
        } else if (plumber_vars->moving_timer) {
            plumber_vars->moving_timer = 0;
            qcopy(plumber_vars->target, hero.motion.body_center.f);
        }
        if (tick_countdown_32(&plumber_vars->target_timer)) {
            float heading;
            plumber_vars->target_timer = FUN_001fa6d0(multiply_global_factor_ed64(random_float_between(180.0f, 300.0f)));
            heading = FUN_001fa580_c(plumber_moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(plumber_vars->target, 6.0f, heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            add_vector_xyz(plumber_vars->target, plumber_vars->target, plumber_moby->position);
        }
        if (plumber_vars->moving_timer) {
            qcopy(scratch.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, plumber_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, plumber_moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), plumber_moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        plumber_vars->yaw = yaw * 0.5f;
        plumber_vars->pitch = pitch * 1.25f;
        plumber_vars->head_yaw = yaw * 0.5f;
    }
    if (D_0015EDB0_b)
        plumber_vars->height = 2.75f;
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, plumber_moby, plumber_vars->body, 0);
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, plumber_moby, plumber_vars->head, 1);
}

/* pvars of the respawning pickup spawn_point_update updates */
typedef struct {
    s32 volume;      /* 0x00: clip volume that arms it */
    u8 pad4[4];
    u32 flags;       /* 0x08: 2/4: needs a D_001413F4 mode; 8: disabled; 0x10: needs a D_001413DC state */
    s32 armed;       /* 0x0C */
    s32 rearm;       /* 0x10 */
    s32 in_volume;   /* 0x14 */
    s32 spawn_slot;  /* 0x18: slot in the level's D_0014C050 row, -1 for none */
} PickupVars;

/* Per-level pickup state; collected[] is indexed by the moby's unkB2 id. */
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L01LevelState;

extern L01LevelState D_L01_001BB6B0;
extern L01LevelState D_L01_001BAA50;
extern s32 moby_killed[] __asm__("D_L01_001BA950");
extern s32 D_0014C190[][64];
extern u8 D_0014C050[];
extern u8 D_001413F4 __attribute__((section(".data")));
extern u32 D_001413DC __attribute__((section(".data")));
extern f32 FUN_001f99c0(f32);
extern f32 probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");

void spawn_point_update(struct Moby *moby) __asm__("FUN_L01_00300220");
void spawn_point_update(struct Moby *moby) {
    PickupVars *vars = (PickupVars *)moby->pvars;
    s32 armed = 0;
    Vec4f pos;
    u32 state;

    if (vars->flags & 8) {
        return;
    }
    if (moby->command == 0) {
        moby->command = 1;
        vars->in_volume = 0;
        return;
    }
    if (vars->spawn_slot != -1 &&
        D_0014C050[vars->spawn_slot + current_level_index * 16] != 0xFF) {
        return;
    }
    if ((vars->flags & 2) && D_001413F4 != 1) {
        return;
    }
    if ((vars->flags & 4) && D_001413F4 != 3 && D_001413F4 != 0) {
        return;
    }
    if (vars->flags & 0x10) {
        state = D_001413DC;
        if (state > 1 && state != 9) {
            return;
        }
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, vars->volume)) {
        if (vars->in_volume == 0) {
            armed = 1;
        }
        vars->in_volume = 1;
    } else {
        vars->in_volume = 0;
    }
    if (armed || D_L01_001BB6B0.collected[(s16)moby->save_id] != 0 ||
        ((D_0014C190[current_level_index][(s16)moby->save_id >> 5] >> (moby->save_id & 0x1F)) & 1)) {
        if (vars->armed == 0 || vars->rearm != 0) {
            armed = 1;
            vars->armed = armed;
            jet_clear_persistent(moby->mission);
            qcopy(&pos, &moby->pos);
            pos.z = probe_ground_height(&pos, 0, 0.5f);
            if (FUN_001f99c0(moby->pos.z - pos.z) > 2.0f) {
                pos.z = moby->pos.z;
            }
            jet_reset_camera(&pos, &moby->rot);
            D_L01_001BAA50.collected[(s16)moby->save_id] = 0;
            D_L01_001BB6B0.collected[(s16)moby->save_id] = 0;
            D_0014C190[current_level_index][(s16)moby->save_id >> 5] &=
                ~(armed << (moby->save_id & 0x1F));
            moby_killed[(s16)moby->save_id >> 5] &= ~(armed << (moby->save_id & 0x1F));
        }
    } else {
        vars->armed = 0;
    }
    if (moby->state == 0 && D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
        moby->state = 1;
    }
}
#include "rnc/rendering/screen.h"
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, u64,
                               u64) __asm__("FUN_001f5450");

void draw_visibomb_outside_zone_overlay(struct Moby *moby) __asm__("FUN_L01_00302438");

void draw_visibomb_outside_zone_overlay(struct Moby *moby) {
    s32 *vars = (s32 *)moby->pvars;
    s32 row_count;
    s32 column_count;
    s32 row;
    s32 column;
    f32 screen_center[2];
    volatile f32 coordinate[2];
    f32 opacity;
    s32 color;
    s32 texture_x;
    s32 texture_y;
    s32 draw_x;
    s32 draw_y;
    s32 tile_size = 32;
    s32 tile_half;
    u64 texture;

    screen_center[0] = convert_integer_to_float(screen_extent.half_width);
    screen_center[1] = convert_integer_to_float(screen_extent.half_height);
    texture = get_effect_texture(30);
    row_count = screen_extent.width / 32 + 1;
    column_count = screen_extent.height / 32 + 1;
    for (row = 0; row < row_count; row++) {
        s32 base_color = 0xff7f7f;
        for (column = 0; column < column_count; column++) {
            coordinate[0] = (f32)(row * 32);
            coordinate[1] = (f32)(column * 32);
            tile_half = tile_size >> 1;
            opacity = convert_integer_to_float(vars[0x50]) * 127.5f /
                      convert_integer_to_float(scale_game_frames(90));
            color = (truncate_float_to_s32(opacity) << 24) + base_color;
            texture_x = random_integer_below(32);
            texture_y = random_integer_below(32);
            draw_x = truncate_float_to_s32(coordinate[0] - (f32)tile_half);
            draw_y = truncate_float_to_s32(coordinate[1] - (f32)tile_half);
            draw_textured_quad(draw_x, draw_y, 32, 32, texture_x, texture_y, 32, 32, color,
                               texture);
        }
    }
}
/* pvars of the moby run by FUN_L01_00302648: 20 trigger zones and a hold counter */
typedef struct {
    s32 path;   /* -1, or an index into D_L01_001B0930 */
    f32 zmin;
    f32 zmax;
    s32 volume; /* clip volume is_point_inside_clip_volume tests */
} Zone_302648;
typedef struct {
    Zone_302648 zones[20];
    s32 count;
} ZoneVars_302648;

extern s32 D_001413D4_302648[] __asm__("D_001413D4");
extern struct Moby *D_L01_0015FFE4_302648 __asm__("D_L01_0015FFE4");
extern s32 *D_L01_001B0930_302648[] __asm__("D_L01_001B0930");
extern s32 absolute_float_cf(void *, void *, s32) __asm__("FUN_L00_00259740");
extern s32 FUN_00214720_302648(void *, s32) __asm__("FUN_00214720");
extern void FUN_001f4600_302648(void (*)(char *), void *) __asm__("FUN_001f4600");
extern void FUN_L01_00302438_302648(char *) __asm__("FUN_L01_00302438");
extern void terminate_visibomb_missile(struct Moby *, void *) __asm__("FUN_L00_002b5b20");

extern s32 visibomb_outside_zone_overlay_enabled_gp __asm__("D_L01_0015F30C") __attribute__((sda));
/* While the camera is in mode 0x1D, counts the frames the class 0xAC moby spends outside every zone.
 * Entering a zone resets the count; past a random threshold FUN_L00_002b5b20 runs on that moby. */
void update_target_outside_zone_timeout(struct Moby *moby) __asm__("FUN_L01_00302648");

void update_target_outside_zone_timeout(struct Moby *moby) {
    ZoneVars_302648 *zone_vars = (ZoneVars_302648 *)moby->pvars;
    struct Moby *target_moby = 0;
    struct Moby *candidate_moby;
    s32 i;
    s32 *path;

    moby->update_dist = 0xFF;
    if (D_001413D4_302648[0] != 0x1D) {
        zone_vars->count = 0;
        return;
    }
    for (candidate_moby = D_L01_0015FFE4_302648; candidate_moby != 0; candidate_moby = candidate_moby->next) {
        if (candidate_moby->oclass == 0xAC) {
            target_moby = candidate_moby;
            break;
        }
    }
    if (target_moby == 0 || target_moby->oclass != 0xAC) {
        zone_vars->count = 0;
        return;
    }
    zone_vars->count++;
    for (i = 0; i < 20; i++) {
        if (zone_vars->zones[i].path != -1 && target_moby->pos.z > zone_vars->zones[i].zmin && target_moby->pos.z < zone_vars->zones[i].zmax) {
            path = D_L01_001B0930_302648[zone_vars->zones[i].path];
            if (absolute_float_cf(&target_moby->pos, path + 4, path[0])) {
                zone_vars->count = 0;
                break;
            }
        }
        if (FUN_00214720_302648(&target_moby->pos, zone_vars->zones[i].volume)) {
            zone_vars->count = 0;
            break;
        }
    }
    if (zone_vars->count > 0) {
        if (visibomb_outside_zone_overlay_enabled_gp) {
            FUN_001f4600_302648(FUN_L01_00302438_302648, moby);
        }
        if (scale_game_frames(90) < zone_vars->count) {
            terminate_visibomb_missile(target_moby, target_moby->pvars);
        }
    }
}

typedef struct {
    s32 id;                        /* collected flag index (D_0014BEC0 + level * 4), -1 none */
    u8 pad4[8];
    s16 unkC;                      /* nonzero: collect at once, no cutscene */
    u8 padE[2];
    Vec4f home;                    /* spawn position; w = spawn yaw */
    u8 pad20[0x40];
    f32 unk60;
    f32 unk64;
    f32 unk68;
    s32 timer;                     /* frames until the pickup finishes */
} Vars_307ca0;

extern u8 D_0014BEC0[];
/* MACRO_ADDR aliases: retail loads these with the one-register lui form */
extern f32 D_0015ED6C_f __asm__("D_0015ED6C") MACRO_ADDR;
extern s32 D_0015ED84_n __asm__("D_0015ED84") MACRO_ADDR;
extern u8 opt_mirror_anim __asm__("D_0015EDB5") MACRO_ADDR;
extern s32 D_L01_0015F640 MACRO_ADDR;
extern s32 cutscene_active __asm__("D_L01_0015F404") MACRO_ADDR;
extern f32 D_L01_00161F10_gp __asm__("D_L01_00161F10") __attribute__((sda));
extern f32 D_L01_00161F14_gp __asm__("D_L01_00161F14") __attribute__((sda));
extern f32 D_L01_00161F18_gp __asm__("D_L01_00161F18") __attribute__((sda));
extern f32 D_L01_00161F2C_gp __asm__("D_L01_00161F2C") __attribute__((sda));
extern s32 D_L01_00161F50_gp __asm__("D_L01_00161F50") __attribute__((sda));
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 FUN_001f99c0(f32);
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void FUN_001f99f8(void *);
extern void fade_to_black(s32) __asm__("FUN_001f4a58");
extern void exit_hero_vehicle_mode_and_restore_moby(void) __asm__("FUN_L01_00231450");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void blend_moby_animation(struct Moby *, s32, s32, s32) __asm__("FUN_00212f90");
extern void set_hero_animation(f32, s32, s32) __asm__("FUN_L00_002323b8");
extern void queue_dialogue_message(s32, s32) __asm__("FUN_L00_00263d40");
extern void initialize_gold_bolt_data(struct Moby *) __asm__("FUN_L01_00308470");
/* the unit defines these with a char * parameter further down */
extern void FUN_L01_00308550_f(struct Moby *) __asm__("FUN_L01_00308550");
extern void FUN_L01_00308380_f(struct Moby *) __asm__("FUN_L01_00308380");
extern void FUN_L01_003087e0_f(struct Moby *) __asm__("FUN_L01_003087e0");
extern void FUN_L01_003089f0_f(struct Moby *) __asm__("FUN_L01_003089f0");

extern f32 gold_bolt_spin_x_degrees_per_second_gp __asm__("D_L01_00161F20") __attribute__((sda));
extern f32 gold_bolt_spin_y_degrees_per_second_gp __asm__("D_L01_00161F24") __attribute__((sda));
extern f32 gold_bolt_spin_z_degrees_per_second_gp __asm__("D_L01_00161F28") __attribute__((sda));
void gold_bolt_update(struct Moby *moby) __asm__("FUN_L01_00307ca0");
void gold_bolt_update(struct Moby *moby) {
    enum {
        GOLD_BOLT_INITIALIZE = 0,
        GOLD_BOLT_WAIT_FOR_PICKUP = 1,
        GOLD_BOLT_PICKUP_CUTSCENE = 2
    };
    Vars_307ca0 *vars = (Vars_307ca0 *)moby->pvars;
    struct Hero *hero_ptr;
    Vec4f at;
    Vec4f look;
    Vec4f at2;
    Vec4f look2;
    int i;

    switch (moby->state) {
    case GOLD_BOLT_INITIALIZE:
        if (vars->id == -1 || D_0014BEC0[vars->id + D_0015ED84 * 4] != 0) {
            goto remove;
        }
        qcopy(&vars->home, &moby->pos);
        vars->home.w = moby->rot.z;
        moby->rot.x = random_angle_radians();
        moby->rot.y = random_angle_radians();
        moby->rot.z = random_angle_radians();
        moby->rot.w = random_angle_radians();
        vars->unk68 = gold_bolt_spin_x_degrees_per_second_gp * 0.017453292f * D_0015ED6C_f;
        vars->unk64 = gold_bolt_spin_y_degrees_per_second_gp * 0.017453292f * D_0015ED6C_f;
        vars->unk60 = gold_bolt_spin_z_degrees_per_second_gp * 0.017453292f * D_0015ED6C_f;
        moby->state = 1;
        initialize_gold_bolt_data(moby);
        return;
    case GOLD_BOLT_WAIT_FOR_PICKUP:
        moby->scale = moby->pclass->scale * D_L01_00161F10_gp;
        moby->pos.z = vars->home.z + D_L01_00161F14_gp + D_L01_00161F18_gp * fast_sin(moby->rot.w);
        moby->rot.x = FUN_001fa580_c(moby->rot.x, gold_bolt_spin_x_degrees_per_second_gp * 0.017453292f * D_0015ED6C_f);
        moby->rot.y = FUN_001fa580_c(moby->rot.y, gold_bolt_spin_y_degrees_per_second_gp * 0.017453292f * D_0015ED6C_f);
        moby->rot.z = FUN_001fa580_c(moby->rot.z, gold_bolt_spin_z_degrees_per_second_gp * 0.017453292f * D_0015ED6C_f);
        moby->rot.w = FUN_001fa580_c(moby->rot.w, D_L01_00161F2C_gp * 0.017453292f * D_0015ED6C_f);
        FUN_L01_00308550_f(moby);
        if (!random_integer_below(D_L01_00161F50_gp)) {
            FUN_L01_003089f0_f(moby);
        }
        if (!(vector_distance(&moby->pos, &hero.motion.pos) < 3.0f)) {
            return;
        }
        if (!(FUN_001f99c0(moby->pos.z - hero.motion.pos.f[2]) < 2.0f)) {
            return;
        }
        if (hero.health.hp == 0) {
            return;
        }
        if (hero.vehicle_mode != 3 && hero.vehicle_mode != 0) {
            return;
        }
        if (vars->unkC != 0) {
            D_L01_0015F640 = scale_game_frames(180);
            queue_dialogue_message(0x53B3, -1);
            D_0014BEC0[vars->id + D_0015ED84_n * 4] = 1;
            memcard_save_data(0, -1);
        remove:
            mark_moby_for_removal(moby);
            return;
        }
        at.x = fast_cos(vars->home.w) * 2.5f;
        at.y = fast_sin(vars->home.w) * 2.5f;
        at.z = 0.0f;
        add_vector_xyz(&at, &at, &vars->home);
        FUN_001f99f8(&look);
        look.z = FUN_001fa580_c(vars->home.w, 3.1415927f);
        fade_to_black(scale_game_frames(10));
        if (hero.vehicle_mode == 3) {
            exit_hero_vehicle_mode_and_restore_moby();
        }
        FUN_L00_00216f90(&at, &look, 0x72, 0);
        allocate_voice_for_target_entry(0, 0, moby);
        at2.x = fast_cos(vars->home.w) * 1.25f;
        at2.y = fast_sin(vars->home.w) * 1.25f;
        at2.z = 0.0f;
        at2.x += fast_cos(FUN_001fa580_c(vars->home.w, 1.5707964f)) * 4.0f;
        at2.y += fast_sin(FUN_001fa580_c(vars->home.w, 1.5707964f)) * 4.0f;
        add_vector_xyz(&at2, &at2, &vars->home);
        at2.z += 1.0f;
        FUN_001f99f8(&look2);
        look2.z = fast_subtract_rotations(vars->home.w, 1.5707964f);
        jet_mount(&at2, &look2, 1, 0, 0);
        FUN_L00_002ea9d8(&at2);
        FUN_L00_002eaa30(&look2);
        qcopy(&moby->pos, &hero.motion.pos);
        qcopy(&moby->rot, &hero.motion.rot);
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 0);
        }
        set_hero_animation(0.0f, 0x82, 0);
        if (opt_mirror_anim) {
            moby->flags |= 0x8000;
        }
        hero.unk20AF = 1;
        cutscene_active = 1;
        moby->state = 2;
        vars->timer = scale_game_frames(60);
        return;
    case GOLD_BOLT_PICKUP_CUTSCENE:
        FUN_L01_003087e0_f(moby);
        qcopy(&moby->pos, &hero.motion.pos);
        qcopy(&moby->rot, &hero.motion.rot);
        if (moby->seq == 1) {
            FUN_L01_00308380_f(moby);
            if (!random_integer_below(10)) {
                FUN_L01_003089f0_f(moby);
            }
            if (moby->anim_flags & 2) {
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, 0);
                }
                moby->visible = 0;
                moby->flags |= 1;
                for (i = 0; i < 4; i++) {
                    FUN_L01_003089f0_f(moby);
                }
            }
        }
        hero_ptr = &hero;
        if (hero_ptr->anim_flags & 2) {
            set_hero_animation(scale_game_frames(30), 0, 0);
        }
        if (moby->seq != 0 || !tick_countdown_32(&vars->timer)) {
            return;
        }
        D_L01_0015F640 = scale_game_frames(180);
        queue_dialogue_message(0x53B3, -1);
        D_0014BEC0[vars->id + D_0015ED84_n * 4] = 1;
        cutscene_active = 0;
        FUN_L00_00216f90(&hero_ptr->motion.pos, &hero_ptr->motion.rot, 0, 0);
        FUN_L00_002eac18(0);
        memcard_save_data(0, -1);
        mark_moby_for_removal(moby);
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309758), where it is exact; names translated to the US level program. */

extern f32 D_L01_00161F10_gp __asm__("D_L01_00161F10") __attribute__((sda));
extern f32 D_L01_00161F14_gp __asm__("D_L01_00161F14") __attribute__((sda));
extern f32 D_L01_00161F18_gp __asm__("D_L01_00161F18") __attribute__((sda));
extern f32 D_L01_00161F2C_gp __asm__("D_L01_00161F2C") __attribute__((sda));
extern s32 D_L01_00161F50_gp __asm__("D_L01_00161F50") __attribute__((sda));
void update_gold_bolt_glow_rings(char *m) __asm__("FUN_L01_00308550");
extern void FUN_L01_003089f0_u(char *) __asm__("FUN_L01_003089f0");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

void update_gold_bolt_visuals(struct Moby *moby) __asm__("FUN_L01_00308380");

void update_gold_bolt_visuals(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float updated_rotation_z;
    float bobbing_sine;
    float k;
    moby->scale =
        moby->pclass->scale * *(float *)&D_L01_00161F10_gp;
    if (moby->state == 1) {
        bobbing_sine = fast_sin(moby->rot.w);
        moby->pos.z =
            *(float *)(data + 0x18) + *(float *)&D_L01_00161F14_gp + *(float *)&D_L01_00161F18_gp * bobbing_sine;
        moby->rot.x =
            fast_add_rotations(moby->rot.x, *(float *)(data + 0x68));
        moby->rot.y =
            fast_add_rotations(moby->rot.y, *(float *)(data + 0x64));
        updated_rotation_z = fast_add_rotations(moby->rot.z, *(float *)(data + 0x60));
        k = *(float *)&D_L01_00161F2C_gp * DEG_TO_RAD * frame_time;
        moby->rot.z = updated_rotation_z;
        moby->rot.w = fast_add_rotations(moby->rot.w, k);
    }
    update_gold_bolt_glow_rings(moby);
    if (random_integer_below(*(int *)&D_L01_00161F50_gp) == 0) {
        FUN_L01_003089f0_u(moby);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Draws the moby's four pulsing glow rings between it and the camera, fading with distance. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309928), where it is exact; names translated to the US level program. */

extern char D_L01_00167240[];
extern float compute_interpolated_record_value_alt(void) __asm__("FUN_0020c9e0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern int gold_bolt_glow_ring_tween_start_color_gp __asm__("D_L01_00161F3C") __attribute__((sda));
extern int gold_bolt_glow_ring_tween_end_color_gp __asm__("D_L01_00161F40") __attribute__((sda));
extern int gold_bolt_glow_ring_particle_use_flags_44_gp __asm__("D_L01_00161F48") __attribute__((sda));
extern int gold_bolt_glow_ring_particle_texture_index_gp __asm__("D_L01_00161F4C") __attribute__((sda));
extern unsigned char *FUN_L00_00272f68(void *, int, unsigned char, int, int, int, int, float);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_vector_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
float absolute_float(float input) __asm__("func_001F99C0");
extern float func_0020D830_m(void *) __asm__("FUN_0020c9e0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_gold_bolt_glow_rings(char *moby) {
    char *moby_vars = *(char **)(moby + 0x78);
    float glow_particle_position[4];
    float glow_origin_position[4];
    float step[4];
    float scale = 1.0f;
    int i;
    float *ring_rotations;
    float *spd;
    int *tm;
    float *sz;
    if (((unsigned char *)moby)[0x20] == 1) {
        scale_vector_xyz(glow_origin_position, moby, 0.0009765625f);
    } else {
        moby_point_position(moby, 0, glow_origin_position);
        scale = (160.0f - func_0020D830_m(moby)) / 160.0f * 16.0f;
        if (1.0f < scale)
            scale = 1.0f;
    }
    subtract_vector_xyz(glow_particle_position, D_L01_00167240, glow_origin_position);
    normalize_vector_xyz(glow_particle_position, glow_particle_position, -0.3f);
    normalize_vector_xyz(step, glow_particle_position, 0.1f);
    add_vector_xyz(glow_particle_position, glow_particle_position, glow_origin_position);
    ring_rotations = (float *)(moby_vars + 0x20);
    spd = (float *)(moby_vars + 0x30);
    tm = (int *)(moby_vars + 0x40);
    sz = (float *)(moby_vars + 0x50);
    for (i = 0; i < 4; i++) {
        float unwrapped_angle = ring_rotations[i] + spd[i];
        float pulse_progress;
        int packed_color;
        ring_rotations[i] = unwrapped_angle;
        if (255.0f <= unwrapped_angle) {
            ring_rotations[i] = unwrapped_angle - 255.0f;
        } else if (unwrapped_angle <= 0.0f) {
            ring_rotations[i] = unwrapped_angle + 255.0f;
        }
        if (fast_dec_timer((int *)(moby_vars + 0x40) + i)) {
            tm[i] = scale_game_frames(0xFF);
        }
        pulse_progress = convert_integer_to_float(scale_game_frames(0xFF) - tm[i]) / (float)scale_game_frames(0xFF);
        packed_color = fast_tween_color(gold_bolt_glow_ring_tween_start_color_gp, gold_bolt_glow_ring_tween_end_color_gp, absolute_float(0.5f - pulse_progress));
        FUN_L00_00272f68(glow_particle_position, packed_color, func_001FA898_r(ring_rotations[i]), gold_bolt_glow_ring_particle_texture_index_gp, gold_bolt_glow_ring_particle_use_flags_44_gp, 2, 0,
                         sz[i] * scale);
        add_vector_xyz(glow_particle_position, glow_particle_position, step);
    }
}
typedef struct {
    f32 x, y, z, w;
} V4_3087e0;

#include "qcopy.h"

extern f32 compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern f32 FUN_001fa580_c(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern void clear_u64_value(f32 *) __asm__("FUN_001f99f8");
extern void lerp_vector(f32 *, void *, void *, f32) __asm__("FUN_001f9a40");
extern void FUN_L00_002ea9d8(void *);
extern u8 *D_L01_001600EC;

void update_gold_bolt_pickup_camera(char *moby) __asm__("FUN_L01_003087e0");

void update_gold_bolt_pickup_camera(char *moby) {
    char *data = *(char **)(moby + 0x78);
    V4_3087e0 output_position;
    V4_3087e0 output_rotation;
    V4_3087e0 start_rotation;
    V4_3087e0 end_rotation;
    f32 record_value;
    f32 interpolation_factor;

    if (*(unsigned char *)(moby + 0x52) == 1) {
        record_value = compute_interpolated_record_value(moby);
    } else {
        record_value = 170.0f;
    }
    interpolation_factor = record_value / 170.0f;
    if (*(s32 *)(data + 4) == -1 || *(s32 *)(data + 8) == -1) {
        f32 position_offset_heading;
        f32 five = 5.0f;
        struct Hero *hero_ptr = &hero;
        position_offset_heading = FUN_001fa580_c(interpolation_factor * 0.6981316804885864f - 0.3490658402442932f, hero_ptr->motion.rot.f[2]);
        output_position.x = fast_cos(position_offset_heading) * five;
        output_position.y = fast_sin(position_offset_heading) * five;
        output_position.z = 0.0f;
        add_vector_xyz(&output_position.x, &output_position.x, (f32 *)(&hero_ptr->motion.pos));
        output_position.z += 1.0f;
        clear_u64_value(&output_rotation.x);
        output_rotation.z = FUN_001fa580_c(position_offset_heading, 3.141590118408203f);
        output_rotation.y = interpolation_factor * -0.17000000178813934f;
    } else {
        lerp_vector(&output_position.x, D_L01_001600EC + *(s32 *)(data + 4) * 128 + 0x30,
                     D_L01_001600EC + *(s32 *)(data + 8) * 128 + 0x30, interpolation_factor);
        {
            u8 *base = D_L01_001600EC;

            qcopy(&start_rotation, base + *(s32 *)(data + 4) * 128 + 0x70);
            qcopy(&end_rotation, base + *(s32 *)(data + 8) * 128 + 0x70);
        }
        output_rotation.z = FUN_001fa580_c(fast_subtract_rotations(end_rotation.z, start_rotation.z) * interpolation_factor, start_rotation.z);
        output_rotation.y = FUN_001fa580_c(fast_subtract_rotations(end_rotation.y, start_rotation.y) * interpolation_factor, start_rotation.y);
        output_rotation.x = FUN_001fa580_c(fast_subtract_rotations(end_rotation.x, start_rotation.x) * interpolation_factor, start_rotation.x);
    }
    FUN_L00_002ea9d8(&output_position.x);
    FUN_L00_002eaa30(&output_rotation.x);
}
#include "sda.h"

/* builds three jittered offsets and fires the same effect at each accumulated point */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309DC8), where it is exact; names translated to the US level program. */

extern int random_integer_below_c(int) __asm__("FUN_00213260");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void spawn_moving_hero_relative_alpha_fade_particle(void *, void *, int, int, int, int, float) __asm__("FUN_L00_002730e0");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");

extern float gold_bolt_particle_initial_speed_min_gp __asm__("D_L01_00161F54") __attribute__((sda));
extern float gold_bolt_particle_initial_speed_max_gp __asm__("D_L01_00161F58") __attribute__((sda));
extern float gold_bolt_particle_velocity_jitter_speed_min_gp __asm__("D_L01_00161F5C") __attribute__((sda));
extern float gold_bolt_particle_velocity_jitter_speed_max_gp __asm__("D_L01_00161F60") __attribute__((sda));
extern short D_L01_00161F64 __attribute__((sda));
extern int gold_bolt_particle_initial_color_gp __asm__("D_L01_00161F68") __attribute__((sda));
extern int gold_bolt_particle_loop_color_gp __asm__("D_L01_00161F6C") __attribute__((sda));
extern float gold_bolt_particle_initial_scale_gp __asm__("D_L01_00161F70") __attribute__((sda));
extern float gold_bolt_particle_loop_scale_gp __asm__("D_L01_00161F74") __attribute__((sda));
void spawn_gold_bolt_particles(char *moby) __asm__("FUN_L01_003089f0");

void spawn_gold_bolt_particles(char *moby) {
    float v[4];
    float particle_velocity[4];
    float u[4];
    float frame_duration;
    int i;
    int particle_lifetime;
    int loop_particle_lifetime;
    if (*(unsigned char *)(moby + 0x20) == 1) {
        scale_vector_xyz(v, moby, 0.0009765625f);
    } else {
        moby_point_position(moby, 0, v);
    }
    frame_duration = frame_time;
    random_vector(particle_velocity, gold_bolt_particle_initial_speed_min_gp * frame_duration, gold_bolt_particle_initial_speed_max_gp * frame_duration);
    particle_lifetime = *(unsigned char *)&D_L01_00161F64;
    spawn_moving_hero_relative_alpha_fade_particle(v, particle_velocity, gold_bolt_particle_initial_color_gp, particle_lifetime, random_integer_below_c(0xFF) & 0xFF, 0,
                     gold_bolt_particle_initial_scale_gp);
    for (i = 2; i >= 0; i--) {
        frame_duration = frame_time;
        random_vector(u, gold_bolt_particle_velocity_jitter_speed_min_gp * frame_duration, gold_bolt_particle_velocity_jitter_speed_max_gp * frame_duration);
        add_vector_xyz(particle_velocity, particle_velocity, u);
        loop_particle_lifetime = *(unsigned char *)&D_L01_00161F64;
        spawn_moving_hero_relative_alpha_fade_particle(v, particle_velocity, gold_bolt_particle_loop_color_gp, loop_particle_lifetime, random_integer_below_c(0xFF) & 0xFF,
                         0, gold_bolt_particle_loop_scale_gp);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309F00), where it is exact; names translated to the US level program. */

extern int D_0015ED84; /* no foreign declaration */
extern int moby_killed[] __asm__("D_L01_001BA950");
extern s32 D_0014C190[][64];
extern void set_moby_list_state(char *arg, int val) __asm__("FUN_L01_0026e0e0");

void activate_teleporter_and_record_state(char *moby) __asm__("FUN_L01_00308b28");

void activate_teleporter_and_record_state(char *moby) {
    int level_bitset_byte_offset = D_0015ED84 << 8;
    unsigned char moby_group_index;
    *(int *)((u8 *)D_0014C190 + (((short)*(unsigned short *)(moby + 0xB2) >> 5) * 4 + level_bitset_byte_offset)) |=
        1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
    moby_killed[(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
        1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
    moby_group_index = *(unsigned char *)(moby + 0x21);
    if (moby_group_index != 0xFF)
        set_moby_list_state((char *)moby_group_index, 1);
    else
        moby[0x20] = 1;
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00309FB0). */

typedef struct L01Moby L01Moby;


extern L01LevelState D_L01_001BB6B0;
extern int D_0015ED84_c __asm__("D_0015ED84");
extern s32 D_0014C190_c[][64] __asm__("D_0014C190");
extern unsigned char D_0014C050[];
extern int alternate_animation_sequence_gp __asm__("D_0015EE20") __attribute__((sda));
extern int D_0013CAE4[];
extern char *D_L01_0015FFD8;
extern int D_L01_0015F594 __attribute__((sda));

extern int FUN_00214720(void *, int);
extern void activate_teleporter_and_record_state(char *) __asm__("FUN_L01_00308b28");
extern void show_teleporter_and_children(char *) __asm__("FUN_L01_00309838");
extern void FUN_L01_003097e8_c(L01Moby *) __asm__("FUN_L01_003097e8");
extern void update_teleporter_child_transforms(char *) __asm__("FUN_L01_003092d0");
extern void try_set_teleporter_help_message(char *) __asm__("FUN_L01_00309430");
extern void draw_moby_radius_ring(char *) __asm__("FUN_L01_003094f0");
extern void set_moby_list_state(char *, int) __asm__("FUN_L01_0026e0e0");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void request_hero_target_pose(float *, int, float) __asm__("FUN_L00_00233ee8");
extern void play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void advance_accelerated_scalar(float *, float *, float, float, float,
                                       float) __asm__("FUN_00213f38");
extern int get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern int enqueue_callback_list_1(void (*)(char *), char *) __asm__("FUN_001f4600");
extern char *create_moby(int) __asm__("FUN_0020c4f8");

extern float teleporter_child_motion_acceleration_gp __asm__("D_L01_00161F88") __attribute__((sda));
extern float teleporter_child_motion_deceleration_gp __asm__("D_L01_00161F8C") __attribute__((sda));
extern float teleporter_child_motion_max_speed_gp __asm__("D_L01_00161F90") __attribute__((sda));
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
    int active = 0;

    if (D_L01_001BB6B0.collected[(short)moby->save_id] != 0 ||
        (D_0014C190_c[D_0015ED84_c][((short)moby->save_id) >> 5] >>
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
        int i;
        moby->update_dist = 0xFF;
        for (i = 0; i < 3; i++) {
            struct Hero *g = &hero;
            char *child_moby;
            slots[i] = create_moby(0x13B);
            *(short *)(slots[i] + 0x32) = 0x40;
            slots[i][0x31] = 1;
            *(long *)(slots[i] + 0x38) = *(long *)(((char *)g->moby) + 0x38);
            *(unsigned short *)(slots[i] + 0x34) = moby->flags;
            child_moby = slots[i];
            qcopy(child_moby + 0x10, &moby->pos);
            qcopy(child_moby + 0x40, &moby->rot);
            *(float *)(slots[i] + 0x48) =
                fast_add_rotations(*(float *)(child_moby + 0x48), (float)i * 2.0943952f);
        }
        if (*(int *)(data + 0x38) != 0 || active) {
            moby->state = 6;
        } else {
            moby->state = 5;
            FUN_L01_003097e8_c((L01Moby *)moby);
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
        if (*(int *)(data + 8) >= 0 && D_0014C050[*(int *)(data + 8) + D_0015ED84_c * 16] != 0xFF)
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
            advance_accelerated_scalar((float *)(data + 0x20), (float *)(data + 0x28), 1.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else if (*(float *)(data + 0x24) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x24), (float *)(data + 0x2C), 1.0f,
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
            advance_accelerated_scalar((float *)(data + 0x20), (float *)(data + 0x28), 1.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else if (*(float *)(data + 0x24) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x24), (float *)(data + 0x2C), 1.0f,
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
            enqueue_callback_list_1(draw_moby_radius_ring, moby);
            approach_value((float *)(data + 0x34), 0.0f, frame_time * 4.0f);
        } else {
            char *destination_moby = D_L01_0015FFD8 + (*(int *)data << 8);
            char *td = *(char **)(destination_moby + 0x78);
            float destination_position[4];
            qcopy(destination_position, destination_moby + 0x10);
            destination_position[2] += 0.2f;
            FUN_L00_00216f90(destination_position, destination_moby + 0x40, 0, 1);
            hero.unk20A5 = 1;
            if (*(int *)(data + 0x40) != -1 && *(int *)(data + 0x44) != -1) {
                jet_game_state(*(int *)(data + 0x40), *(int *)(data + 0x44));
            }
            *(unsigned char *)(destination_moby + 0x20) = 4;
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
            enqueue_callback_list_1(draw_moby_radius_ring, moby);
            approach_value((float *)(data + 0x34), 1.95f, frame_time * 4.0f);
        } else if (0.0f < *(float *)(data + 0x24)) {
            advance_accelerated_scalar((float *)(data + 0x24), (float *)(data + 0x28), 0.0f,
                                       teleporter_child_motion_acceleration_gp * frame_time_sq,
                                       teleporter_child_motion_deceleration_gp * frame_time_sq,
                                       teleporter_child_motion_max_speed_gp * frame_time);
        } else if (0.0f < *(float *)(data + 0x20)) {
            advance_accelerated_scalar((float *)(data + 0x20), (float *)(data + 0x2C), 0.0f,
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
#include "qcopy.h"

/* Places the three child mobys at this moby's position and orients each from its data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030A6A8), where it is exact; names translated to the US level program. */

extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void FUN_L00_00250df8(void *);

extern float teleporter_child_position_z_offset_gp __asm__("D_L01_00161F78") __attribute__((sda));
extern float teleporter_child_displacement_angle_degrees_gp __asm__("D_L01_00161F80") __attribute__((sda));
void update_teleporter_child_transforms(char *parent_moby) {
    char *parent_data = *(char **)(parent_moby + 0x78);
    char **child_slot = (char **)(parent_data + 0x14);
    float v[4];
    int i;
    for (i = 2; i >= 0; i--) {
        float child_offset_length;
        float u;
        char *child_moby = *child_slot;
        qcopy(child_moby + 0x10, parent_moby + 0x10);
        u = teleporter_child_displacement_angle_degrees_gp * DEG_TO_RAD;
        *(float *)(child_moby + 0x18) += teleporter_child_position_z_offset_gp;
        *(float *)(*child_slot + 0x44) = u;
        child_offset_length = *(float *)(parent_data + 0x20) * 2.4f - 1.2f;
        euler_rotation_basis(*child_slot + 0xC0, *child_slot + 0x40);
        fast_vec_normalize(v, *child_slot + 0xC0, child_offset_length);
        fast_vec_add(*child_slot + 0x10, *child_slot + 0x10, v);
        *(float *)(*child_slot + 0x44) = *(float *)(parent_data + 0x24) * -1.0471976f - 0.34906584f;
        euler_rotation_basis(*child_slot + 0xC0, *child_slot + 0x40);
        FUN_L00_00250df8(*child_slot);
        child_slot++;
    }
}
/* Picks a per-stage table by the current stage number and passes the entry for the moby's index on. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_0030A808), where it is exact; names translated to the US level program. */

extern int D_0015ED84; /* no foreign declaration */
extern int D_L01_00201600[];
extern int D_L01_00201618[];
s32 try_set_help_message(s32 arg0, s32 arg1) __asm__("FUN_00215130");

extern short D_L01_00161F98 __attribute__((sda));
extern short D_L01_00161FA0 __attribute__((sda));
extern short D_L01_00161FA8 __attribute__((sda));
extern short D_L01_00161FB0 __attribute__((sda));
void try_set_teleporter_help_message(char *moby) {
    int *data = *(int **)(moby + 0x78);
    switch (D_0015ED84) {
    case 12:
        try_set_help_message(4, ((int *)&D_L01_00161FA0)[data[4]]);
        break;
    case 17:
        try_set_help_message(4, D_L01_00201618[data[4]]);
        break;
    case 1:
    case 7:
    case 13:
        try_set_help_message(4, D_L01_00201600[data[4]]);
        break;
    case 8:
        try_set_help_message(4, ((int *)&D_L01_00161F98)[data[4]]);
        break;
    case 16:
        try_set_help_message(4, ((int *)&D_L01_00161FA8)[data[4]]);
        break;
    case 18:
        try_set_help_message(4, ((int *)&D_L01_00161FB0)[data[4]]);
        break;
    }
}
/* Ported from rac1-decomp src/overlays/shared/vendor_002F7700.c (func_L01_0030A8C8) */

typedef struct {
    float v[16];
    int c[4];
    float uv[8];
    long g[4];
} Quad_30a8c8;

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern float D_L01_00201630[];
extern char D_001413F5_c __asm__("D_001413F5") __attribute__((section(".data")));
extern int moby_radius_ring_blend_minuend_selector_gp __asm__("D_L01_00161FB8") __attribute__((sda));
extern int moby_radius_ring_blend_subtrahend_selector_gp __asm__("D_L01_00161FBC") __attribute__((sda));
extern int moby_radius_ring_blend_alpha_factor_selector_gp __asm__("D_L01_00161FC0") __attribute__((sda));
extern int moby_radius_ring_blend_addend_selector_gp __asm__("D_L01_00161FC4") __attribute__((sda));
extern int moby_radius_ring_texture_index_gp __asm__("D_L01_00161FC8") __attribute__((sda));
extern int moby_radius_ring_vertex_color_gp __asm__("D_L01_00161FCC") __attribute__((sda));
extern float moby_radius_ring_center_z_gp __asm__("D_L01_00161FD0") __attribute__((sda));
extern void copy_matrix_rotation(void *, void *) __asm__("FUN_001fa298");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

void draw_moby_radius_ring(char *moby) {
    Quad_30a8c8 quad;
    float rotation_matrix[12];
    float moby_position_snapshot[4];
    char *moby_data;
    float ring_half_height;
    int i, j, nj;
    moby_data = *(char **)(moby + 0x78);
    copy_matrix_rotation(rotation_matrix, moby + 0xC0);
    qcopy(moby_position_snapshot, moby + 0x10);
    quad.g[1] = get_effect_texture(moby_radius_ring_texture_index_gp);
    quad.g[3] = (long)moby_radius_ring_blend_minuend_selector_gp | ((long)moby_radius_ring_blend_subtrahend_selector_gp << 2) | ((long)moby_radius_ring_blend_alpha_factor_selector_gp << 4) | ((long)moby_radius_ring_blend_addend_selector_gp << 6) | (0x8000L << 24);
    quad.g[2] = 0xFF9000000260UL;
    quad.g[0] = 0;
    for (i = 0; i < 4; i++) {
        quad.uv[i * 2] = D_L01_00201630[i * 2];
        *(&quad.uv[i * 2] + 1) = D_L01_00201630[i * 2 + 1];
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
                D_001413F5_c = 1;
            } else {
                D_001413F5_c = 0;
            }
        }
    }
    for (j = 0; j < 32; j = nj) {
        int k;
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

typedef struct {
    char pad[0x14];
    L01Moby *slots[3];
} L01MobyData;

struct L01Moby {
    char pad0[0x31];
    unsigned char state;
    char pad32[2];
    unsigned short flags;
    char pad36[0x42];
    L01MobyData *data;
    char pad7c[0x18];
    int field94;
};

void hide_teleporter_and_children(L01Moby *moby) __asm__("FUN_L01_003097e8");

void hide_teleporter_and_children(L01Moby *moby) {
    L01Moby **slots = moby->data->slots;
    int i;
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
void show_teleporter_and_children(char *parent_moby) {
    char *parent_record = *(char **)(parent_moby + 0x24);
    char **child_slot;
    s32 i;
    s8 one;
    child_slot = (char **)(*(char **)(parent_moby + 0x78) + 0x14);
    *(s32 *)(parent_moby + 0x94) = *(s32 *)(parent_record + 0x10);
    *(u16 *)(parent_moby + 0x34) &= 0xFFFE;
    *(s8 *)(parent_moby + 0x31) = 1;
    one = 1;
    for (i = 2; i >= 0; i--) {
        if (*child_slot != 0) {
            *(u16 *)(*child_slot + 0x34) &= 0xFFFE;
            *(s8 *)(*child_slot + 0x31) = one;
        }
        child_slot++;
    }
}
