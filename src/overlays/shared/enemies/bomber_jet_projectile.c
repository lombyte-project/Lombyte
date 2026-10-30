/* Shared moby class 0x2AE: an explosive projectile that flies at its target and bursts on impact. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

typedef int u128_f6 __attribute__((mode(TI)));

typedef struct Hero Hero;

typedef struct {
    float x, y, z, w;
} Vec_2F7E08 __attribute__((aligned(16)));

typedef struct {
    int c[6];
} Cols;

typedef struct {
    Vec_2F7E08 vel;
    Vec_2F7E08 target;
    int        ids[4];
    int        f30;
    int        f34;
    float      f38;
} Data;

typedef struct {
    int   timer;
    short h4;
    int   vol;
} Tgt;

typedef struct {
    Vec_2F7E08    dir;
    struct Moby  *owner;
    int           flags;
    unsigned char b28;
    unsigned char b29;
    short         id;
    float         scale;
    int           count;
} Dmg;

extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int spawn_glow_particle(void *, void *, void *, float, int, int, int, int, int) __asm__("FUN_L00_002ac910");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void FUN_L00_0026a4c8(void *, void *, int, int, int, float);
extern void spawn_particle(void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");
extern void spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
    void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");

extern Cols l01_explosive_projectile_initial_color_palette __asm__("D_L01_0020B470");
extern Cols l01_explosive_projectile_secondary_color_palette __asm__("D_L01_0020B488");
extern struct Moby *D_L01_001742D8 NOT_SDA;
extern char                       *ents_macro __asm__("D_L01_0015FFD8") MACRO_ADDR;
extern char                        D_0013E533[];
extern char                        D_L01_00167240[];
extern float D_0015ED6C            MACRO_ADDR;
extern float D_L01_0015F5D0        MACRO_ADDR;
extern int                         cutscene_active_macro __asm__("D_L01_0015F404") MACRO_ADDR;
extern char                        D_0013F3D0[];

int D_L02_0015F404 = 0;
int D_L03_0015F404 = 0;
int D_L04_0015F404 = 0;
int D_L05_0015F404 = 0;
int D_L07_0015F404 = 0;
int D_L08_0015F404 = 0;
int D_L09_0015F404 = 0;
int D_L11_0015F404 = 0;

Cols D_L14_001FCB50 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Cols D_L14_001FCB68 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int D_L15_0015F404 = 0;
int D_L16_0015F404 = 0;
int D_L17_0015F404 = 0;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_002F7E08), where it is exact; names translated to the US level program. */
void update_explosive_projectile(struct Moby *moby) __asm__("FUN_L01_002f6a30");

void update_explosive_projectile(struct Moby *moby) {
    Vec_2F7E08 old_pos;
    Vec_2F7E08 smoke_pos;
    Data      *data;
    int        rate_divisor;
    int        i;
    int        j;
    int        flash;
    int        spark;
    int        link;
    int        max_rings;
    int        rings;
    float      step_length;
    float      size_scale;
    float      camera_distance;
    float      view_distance;
    float      fade;

    rate_divisor = 2;
    if (cutscene_active_macro != 0) {
        rate_divisor = 4;
    }
    data = (*(void **)&moby->pvars);
    qcopy(&old_pos, &(*(Vec_2F7E08 *)&moby->pos));
    fast_vec_add(&(*(Vec_2F7E08 *)&moby->pos), &(*(Vec_2F7E08 *)&moby->pos), data);
    step_length = fast_vec_length(data);
    for (i = 0; cutscene_active_macro != 0 ? i < 2 : i < 4; i++) {
        Vec_2F7E08 smoke_vel;
        {
            Vec_2F7E08 random_dir;
            *(u128_f6 *)&random_dir = 0;
            random_dir.x = random_float_between(-1.0f, 1.0f);
            random_dir.y = random_float_between(-1.0f, 1.0f);
            random_dir.z = random_float_between(-1.0f, 1.0f);
            *(u128_f6 *)&smoke_vel = *(u128_f6 *)&random_dir;
        }
        {
            fast_vec_normalize(
                &smoke_vel, &smoke_vel, random_float_between(0.5f, 1.0f) * D_0015ED6C);
            fast_vec_normalize(
                &smoke_pos, data, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
            fast_vec_add(&smoke_vel, &smoke_vel, &smoke_pos);
            fast_vec_normalize(&smoke_pos, data, random_float_between(0.0f, 1.0f) * step_length);
            fast_vec_add(&smoke_pos, &smoke_pos, &(*(Vec_2F7E08 *)&moby->pos));
            spawn_effect(&smoke_pos,
                         &smoke_vel,
                         0x6F00AFFF,
                         0xFF,
                         scale_game_frames(random_int_between(0x1E, 0x2D)) / rate_divisor,
                         0x50,
                         random_int_between(0x50, 0x8C) / rate_divisor,
                         1);
            fast_vec_normalize(&smoke_pos, data, random_float_between(0.0f, 1.0f) * step_length);
            fast_vec_add(&smoke_pos, &smoke_pos, &(*(Vec_2F7E08 *)&moby->pos));
            spawn_effect(&smoke_pos,
                         &smoke_vel,
                         0x1FFFFFFF,
                         0x4F4F4F,
                         scale_game_frames(random_int_between(0x3C, 0x78)) / rate_divisor,
                         0x50,
                         random_int_between(0xC8, 0x12C) / rate_divisor,
                         0);
        }
    }
    if ((*(Vec_2F7E08 *)&moby->pos).x < 0.0f || (*(Vec_2F7E08 *)&moby->pos).y < 0.0f ||
        (*(Vec_2F7E08 *)&moby->pos).z < 0.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_vec_dist(&data->target, &(*(Vec_2F7E08 *)&moby->pos)) < data->f38) {
        qcopy(&(*(Vec_2F7E08 *)&moby->pos), &data->target);
        moby->state = 1;
    }
    if (moby->state != 0) {
        size_scale = 8.0f;
        camera_distance = fast_vec_dist(D_L01_00167240, &(*(Vec_2F7E08 *)&moby->pos));
        if (cutscene_active_macro != 0) {
            size_scale = 4.0f;
        }
        fast_vec_scale(data, data, -0.55f);
        if (cutscene_active_macro != 0) {
            Dmg damage;
            {
                damage.owner = moby;
                damage.flags = 0x810001;
                damage.scale = 0.0f;
                clear_u64_value(&damage);
                damage.count = 0;
                collect_mobies_in_sphere(1.0f,
                                         &(*(Vec_2F7E08 *)&moby->pos),
                                         9,
                                         (void *)((Hero *)(D_0013E533 + 0xE1D))->moby,
                                         &damage);
                if (D_L01_001742D8 != 0 && D_L01_001742D8->oclass >= 0x2C5 &&
                    D_L01_001742D8->oclass < 0x2C8) {
                    allocate_voice_for_target_entry(2, 0, (int)moby);
                } else {
                    allocate_voice_for_target_entry(1, 0, (int)moby);
                }
            }
        }
        view_distance = fast_vec_dist(&(*(Vec_2F7E08 *)&moby->pos), D_L01_00167240);
        max_rings = 5;
        if (cutscene_active_macro != 0) {
            max_rings = 2;
        }
        if (view_distance < convert_integer_to_float(max_rings) * 2.0f) {
            rings = truncate_float_to_s32(view_distance) / 2;
        } else {
            rings = max_rings;
        }
        fade = 0.0f;
        if (view_distance < 7.0f) {
            fade = 7.0f - view_distance;
        }
        for (j = 0; j < rings; j++) {
            float speed = random_float_between(8.0f, 10.0f) * D_0015ED6C - fade * D_0015ED6C;
            Cols  start_colors = l01_explosive_projectile_initial_color_palette;
            Cols  end_colors = l01_explosive_projectile_secondary_color_palette;
            speed = speed * size_scale;
            spawn_particle(
                &(*(Vec_2F7E08 *)&moby->pos),
                data,
                start_colors.c[random_integer_below(6)],
                end_colors.c[random_integer_below(6)],
                size_scale * 400000.0f,
                random_int_between(scale_game_frames(0x1E), scale_game_frames(0x28)) / rate_divisor,
                random_int_between(scale_game_frames(0x3C), scale_game_frames(0x5A)) / rate_divisor,
                0,
                8,
                speed);
            spawn_particle(
                &(*(Vec_2F7E08 *)&moby->pos),
                data,
                0x7FFFFFFF,
                0xFFFFFF,
                size_scale * 400000.0f,
                random_int_between(scale_game_frames(0xA), scale_game_frames(0x14)) / rate_divisor,
                random_int_between(scale_game_frames(0x1E), scale_game_frames(0x28)) / rate_divisor,
                0,
                8,
                speed * 0.5f);
        }
        for (flash = 0; flash < 5; flash++) {
            Cols       start_colors = l01_explosive_projectile_initial_color_palette;
            Cols       end_colors = l01_explosive_projectile_secondary_color_palette;
            Vec_2F7E08 flash_emission_vector;
            {
                Vec_2F7E08 random_dir;
                *(u128_f6 *)&random_dir = 0;
                random_dir.x = random_float_between(-1.0f, 1.0f);
                random_dir.y = random_float_between(-1.0f, 1.0f);
                random_dir.z = random_float_between(-1.0f, 1.0f);
                *(u128_f6 *)&flash_emission_vector = *(u128_f6 *)&random_dir;
            }
            {
                fast_vec_normalize(&flash_emission_vector,
                                   &flash_emission_vector,
                                   size_scale * (random_float_between(0.0f, 3.0f) * D_0015ED6C));
                FUN_L00_0026a4c8(
                    &(*(Vec_2F7E08 *)&moby->pos),
                    &flash_emission_vector,
                    start_colors.c[random_integer_below(6)],
                    end_colors.c[random_integer_below(6)],
                    random_int_between(scale_game_frames(0xF), scale_game_frames(0x14)) /
                        rate_divisor,
                    200000.0f);
            }
        }
        if (D_L01_0015F5D0 < 0.95f && view_distance > 9.0f) {
            spawn_glow_particle(moby,
                                &(*(Vec_2F7E08 *)&moby->pos),
                                data,
                                size_scale * 4.0f,
                                scale_game_frames(0xF),
                                0x7F,
                                0x7F,
                                0x7F,
                                0x20);
            spawn_glow_particle(moby,
                                &(*(Vec_2F7E08 *)&moby->pos),
                                data,
                                size_scale * 4.0f,
                                scale_game_frames(0x18),
                                0x7F,
                                0x7F,
                                0x7F,
                                0x20);
        }
        spawn_glow_particle(moby,
                            &(*(Vec_2F7E08 *)&moby->pos),
                            data,
                            size_scale * 4.0f,
                            scale_game_frames(0x14),
                            0x7F,
                            0x7F,
                            0,
                            0x30);
        spawn_glow_particle(moby,
                            &(*(Vec_2F7E08 *)&moby->pos),
                            data,
                            size_scale * 2.0f,
                            scale_game_frames(0x13),
                            0xFF,
                            0xFF,
                            0xFF,
                            0x20);
        if (camera_distance > 100.0f) {
            if (D_L01_0015F5D0 < 0.95f && view_distance > 9.0f) {
                spawn_glow_particle(moby,
                                    &(*(Vec_2F7E08 *)&moby->pos),
                                    data,
                                    30.0f,
                                    scale_game_frames(0xF),
                                    0x7F,
                                    0x7F,
                                    0x7F,
                                    0x20);
                spawn_glow_particle(moby,
                                    &(*(Vec_2F7E08 *)&moby->pos),
                                    data,
                                    30.0f,
                                    scale_game_frames(0x18),
                                    0x7F,
                                    0x7F,
                                    0x7F,
                                    0x20);
            }
            spawn_glow_particle(moby,
                                &(*(Vec_2F7E08 *)&moby->pos),
                                data,
                                30.0f,
                                scale_game_frames(0x14),
                                0x7F,
                                0x7F,
                                0,
                                0x30);
            spawn_glow_particle(moby,
                                &(*(Vec_2F7E08 *)&moby->pos),
                                data,
                                15.0f,
                                scale_game_frames(0x13),
                                0xFF,
                                0xFF,
                                0xFF,
                                0x20);
        }
        for (spark = 0; cutscene_active_macro != 0 ? spark < 10 : spark < 40; spark++) {
            Vec_2F7E08 spark_vel;
            float      speed;
            float      size;
            {
                Vec_2F7E08 random_dir;
                *(u128_f6 *)&random_dir = 0;
                random_dir.x = random_float_between(-1.0f, 1.0f);
                random_dir.y = random_float_between(-1.0f, 1.0f);
                random_dir.z = random_float_between(-1.0f, 1.0f);
                *(u128_f6 *)&spark_vel = *(u128_f6 *)&random_dir;
            }
            speed = random_float_between(10.0f, 40.0f) * D_0015ED6C;
            fast_vec_normalize(
                &spark_vel, &spark_vel, cutscene_active_macro != 0 ? speed * 0.2f : speed);
            size = 105000.0f;
            fast_vec_add(&spark_vel, &spark_vel, data);
            if (cutscene_active_macro != 0) {
                size = 21000.0f;
            }
            spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
                &(*(Vec_2F7E08 *)&moby->pos),
                &spark_vel,
                0x4F007FFF,
                0x1F00007F,
                size,
                random_int_between(scale_game_frames(0x5A), scale_game_frames(0x96)) / rate_divisor,
                1,
                -1,
                -1);
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
                if (((struct Moby *)ents_macro)[data->ids[link]].oclass == 0x2BC) {
                    Tgt *target = (*(void **)&((struct Moby *)ents_macro)[data->ids[link]].pvars);
                    if (target->h4 == 0 && target->timer == 0 &&
                        is_point_inside_clip_volume(((char *)&D_0013F3D0), target->vol) != 0 &&
                        is_point_inside_clip_volume(D_L01_00167240, target->vol) != 0) {
                        target->timer = scale_game_frames(random_int_between(0x3C, 0xB4));
                    }
                }
            }
        }
        mark_moby_for_removal(moby);
    }
}

Cols l01_explosive_projectile_initial_color_palette __asm__("D_L01_0020B470") = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Cols l01_explosive_projectile_secondary_color_palette __asm__("D_L01_0020B488") = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};
