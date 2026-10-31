/* Moby class 0x7A (debris_update): a sparking piece of debris thrown out by an explosion, and the function that spawns it. */

#include "types.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/globals.h"

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2af5b8;

typedef struct {
    int c[4];
} C_2af5b8;

typedef struct {
    float x0, x4, x8, xc;
    float x10, x14;
    short x18, x1a;
    float x1c;
} W_2af5b8;

extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void        set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
void        FUN_L00_00250df8_2af450(void *) __asm__("FUN_L00_00250df8");
int         FUN_L00_0025c0e8_c(int, int) __asm__("FUN_L00_0025c0e8");
void        spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");

extern C_2af5b8 D_L00_00161438 NOT_SDA;

C_2af5b8 D_L01_00161438 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L02_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L03_00161418 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L04_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L05_00161458 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L06_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L07_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L08_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L09_00161498 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L10_00161418 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L11_00161498 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L12_00161418 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L13_001614E8 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L14_00161620 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L15_00161548 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L16_001614C8 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L17_001614E0 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

C_2af5b8 D_L18_001614A8 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

unsigned char *
spawn_debris(void *pos, void *vel, int n, int flag, int g) __asm__("FUN_L00_002af450");

unsigned char *spawn_debris(void *spawn_position,
                            void *initial_velocity,
                            int   lifetime_frames,
                            int   flag,

                            int trail_color_mask) {
    unsigned char *debris_moby = create_moby(0x7A);
    if (debris_moby) {
        unsigned char *debris_data;
        float          rotation_x_increment, frame_dt;
        debris_data = ((unsigned char *)MOBY(debris_moby)->pvars);
        debris_moby[0x30] = 0xFF;
        MOBY(debris_moby)->draw_dist = 0xFF;
        debris_moby[0x31] = 1;
        debris_moby[0x23] = 0x80;
        set_moby_color(debris_moby, 0x7F, 0x7F, 0x7F);
        qcopy(debris_moby + 0x10, spawn_position);
        qcopy(debris_data, initial_velocity);
        rotation_x_increment =
            random_float_between(frame_time * 6.2831855f, frame_time * 12.566371f);
        frame_dt = frame_time;
        *(float *)(debris_data + 0x10) = rotation_x_increment;
        *(float *)(debris_data + 0x14) =
            random_float_between(frame_dt * 6.2831855f, frame_dt * 12.566371f);
        *(short *)(debris_data + 0x18) = lifetime_frames;
        *(short *)(debris_data + 0x1A) = lifetime_frames;
        if (flag == 0) {
            MOBY(debris_moby)->scale *= random_float_between(0.5f, 0.75f);
        }
        *(float *)(debris_data + 0x1C) = MOBY(debris_moby)->scale;
        debris_moby[0xBC] = flag | (trail_color_mask << 1);
        FUN_L00_00250df8_2af450(debris_moby);
    }
    return debris_moby;
}

void debris_update(unsigned char *m) __asm__("FUN_L00_002af5b8");

/* Moby class 122 */
void debris_update(unsigned char *debris_moby) {
    W_2af5b8 *debris_data = ((W_2af5b8 *)MOBY(debris_moby)->pvars);
    C_2af5b8  c;
    V_2af5b8  emission_direction;
    V_2af5b8  random_direction;
    int       i;
    float     f;
    if (debris_moby[0xBC] & 1) {
        for (i = 0; i < 1; i++) {
            c = D_L00_00161438;
            *(OvlQuad *)&random_direction = 0;
            random_direction.f[0] = random_float_between(-1.0f, 1.0f);
            random_direction.f[1] = random_float_between(-1.0f, 1.0f);
            random_direction.f[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)&emission_direction = *(OvlQuad *)&random_direction;
            fast_vec_normalize(&emission_direction, &emission_direction, frame_time);
            if (debris_moby[0xBC] & 0xFE) {
                int adjusted_color =
                    FUN_L00_0025c0e8_c(c.c[random_integer_below(4)], debris_moby[0xBC] >> 1);
                int adjusted_secondary_color = FUN_L00_0025c0e8_c(0x4FFF, debris_moby[0xBC] >> 1);
                spawn_effect(debris_moby + 0x10,
                             &emission_direction,
                             adjusted_color,
                             adjusted_secondary_color,
                             scale_game_frames(0x2D),
                             0x32,
                             0x78,
                             1);
            } else {
                spawn_effect(debris_moby + 0x10,
                             &emission_direction,
                             c.c[random_integer_below(4)],
                             0x4FFF,
                             scale_game_frames(0x2D),
                             0x32,
                             0x78,
                             1);
            }
        }
    }
    MOBY(debris_moby)->rot.x = fast_add_rotations(MOBY(debris_moby)->rot.x, debris_data->x10);
    MOBY(debris_moby)->rot.y = fast_add_rotations(MOBY(debris_moby)->rot.y, debris_data->x14);
    fast_vec_add(debris_moby + 0x10, debris_moby + 0x10, debris_data);
    debris_data->x8 -= frame_time_sq * 14.6f;
    if (MOBY(debris_moby)->pos.x < 0.0f || MOBY(debris_moby)->pos.y < 0.0f ||
        MOBY(debris_moby)->pos.z < 0.0f) {
        mark_moby_for_removal(debris_moby);
        return;
    }
    if (debris_data->x18 < debris_data->x1a / 4) {
        f = convert_integer_to_float(debris_data->x18);
        MOBY(debris_moby)->scale =
            debris_data->x1c * f / convert_integer_to_float(debris_data->x1a / 4);
    }
    if (fast_dec_timer_s16(&debris_data->x18))
        mark_moby_for_removal(debris_moby);
}
