/* Level 15 moby class exploding_prop (0x9a): when hit, bursts into debris chunks, an explosion and smoke along its yaw, drops its bolts and deletes itself (it stays gone once destroyed). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"

extern char D_0014C190[];

extern unsigned char D_001413F4[];

extern unsigned char D_0013DD50 NOT_SDA;

extern char D_L15_00178700[];

extern u8 D_001413F4[];

extern u8 D_0013DD50;

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");

extern void *spawn_debris_chunk(void *, void *, int, float, int, float, float, float, int) __asm__("FUN_L01_002f8530");

extern void *mine_debris(void *, void *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");

extern char destructible_hit_debris_color_palette[] __asm__("D_L15_001615B8");

extern struct {
    u32 words[3];
} D_L15_001615C8;

extern int blend_color(float t, int from, int to) __asm__("FUN_L15_00249ce8");

extern void FUN_L00_00257470(void *, int, int);

extern void spawn_explosion(void         *m,
                            void         *unused,
                            void         *pos,
                            float         fA,
                            float         fB,
                            int           n,
                            int           k8,
                            int           k9,
                            float         fC,
                            float         fD,
                            float         fE,
                            float         fF,
                            int           k10,
                            float         fG,
                            int           k11,
                            int           s16,
                            int           s18,
                            unsigned char flag) __asm__("FUN_L00_0025e450");

extern void *spawn_debris_chunk(void *pos,
                                void *vel,
                                int   color,
                                float a,
                                int   life,
                                float size,
                                float b,
                                float c,
                                int   d) __asm__("FUN_L01_002f8530");

extern void *
mine_debris(void *pos, void *vel, int color, int color2, float size, int life, int tint) __asm__("FUN_L00_0026bed0");

void update_exploding_prop(struct Moby *moby) __asm__("FUN_L15_002aa280");

void update_exploding_prop(struct Moby *moby) {
    char *hit = 0;
    Vec4  velocity, hit_direction, push, offset, center;
    u32   palette[4];
    u32   colors[4];
    Vec4  delta;
    int   spark, puff, pick, hit_record_index;
    u32  *color;
    float size;
    void *debris;

    if (D_001413F4[0] != 2 && moby->state == 1)
        return;

    for (hit_record_index = 0; hit_record_index < 64; hit_record_index++) {
        char *entry = D_L15_00178700 + hit_record_index * 0x40;
        if (*(struct Moby **)(entry + 0x34) != moby)
            continue;
        if ((*(u32 *)(entry + 0x24) & 0x80000) == 0)
            continue;
        hit = entry;
        break;
    }
    moby->hit_slot = 0xff;
    if (hit != 0) {
        __builtin_memcpy(palette, &destructible_hit_debris_color_palette, 16);
        fast_vec_normalize(&hit_direction, hit + 0x10, frame_time * 10.0f);
        fast_vec_scale(&push, &hit_direction, 2.0f);
        if (fast_vec_length(hit) != 0.0f) {
            center.q = *(u128 *)hit;
        } else {
            center.q = *(u128 *)&moby->pos;
            center.f[2] += 6.0f;
        }

        for (spark = 0; spark < 25; spark++) {
            float radius, angle;
            int   life;
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_cos(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[0] = radius;
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_sin(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[1] = radius;
            offset.f[2] = 0.0f;
            offset.f[2] = random_float_between(1.0f, 7.0f);
            add_vectors(colors, &offset, &moby->pos);
            fast_vec_sub(colors, &center, &offset);
            velocity.q = *(u128 *)colors;
            fast_vec_normalize(&velocity, &velocity, frame_time * 5.0f);
            add_vectors(colors, &velocity, &push);
            velocity.f[2] += frame_time * 3.0f;
            size = random_float_between(0.75f, 1.25f);
            fast_vec_scale(&velocity, &velocity, size);
            pick = random_integer_below(4);
            color = &palette[pick];
            radius = random_float_between(0.1f, 0.3f);
            life = random_int_between(100, 160);
            size = random_float_between(3.0f, 7.0f);
            spawn_debris_chunk(&offset, &velocity, *color, radius, life, size, 0.15f, 2.0f, 1);
        }
        clear_u64_value(&velocity);
        spawn_explosion(moby,
                        &velocity,
                        &center,
                        0.0f,
                        0.0f,
                        20,
                        3,
                        4,
                        10.0f,
                        5.0f,
                        100000.0f,
                        3.0f,
                        0,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);

        for (puff = 0; puff < 20; puff++) {
            float radius, angle;
            int   color1, color2, life;
            __builtin_memcpy(colors, &D_L15_001615C8, 12);
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_cos(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[0] = radius;
            angle = random_float_between(moby->rot.z, 1.5707963267948966f);
            radius = fast_sin(angle);
            radius *= random_float_between(-3.0f, 3.0f);
            offset.f[1] = radius;
            offset.f[2] = 0.0f;
            fast_vec_add(&offset, &offset, &moby->pos);
            offset.f[2] += random_float_between(1.0f, 5.5f);
            fast_vec_sub(&delta, &center, &offset);
            velocity.q = delta.q;
            fast_vec_normalize(&velocity, &velocity, frame_time * 5.0f);
            add_vectors(&delta, &velocity, &hit_direction);
            size = random_float_between(0.75f, 1.25f);
            fast_vec_scale(&velocity, &velocity, size);
            size = random_float_between(0.25f, 1.0f);
            color1 = blend_color(size, 0x7f000000, 0x7f182030);
            size = random_float_between(0.5f, 1.0f);
            color2 = blend_color(size, 0, 0x5f5f5f);
            size = random_float_between(1.0f, 3.0f) * 210000.0f;
            life = scale_game_frames(180);
            pick = random_integer_below(3);
            debris = mine_debris(&offset, &velocity, color1, color2, size, life, colors[pick]);
            if (debris != 0)
                *(float *)((char *)debris + 0x2c) = moby->pos.z - 0.5f;
        }
        FUN_L00_00257470(moby, 0, -1);
        mark_moby_for_removal(moby);
    } else {
        moby->hit_slot = 0xff;
        if (moby->state != 0)
            return;
        moby->draw_dist = 0x80;
        moby->update_dist = 0xff;
        moby->state = 1;
        if (D_0013DD50 != 0) {
            u16 save_id = moby->save_id;
            if ((*(s32 *)(D_0014C190 + (((s16)save_id >> 5) * 4) + current_level_index * 0x100) >>
                 (save_id & 31)) &
                1)
                mark_moby_for_removal(moby);
        }
    }
}
