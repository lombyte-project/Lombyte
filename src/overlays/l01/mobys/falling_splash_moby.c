/* Level 01 moby class falling_splash_moby. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/level_labels.h"

extern int FUN_L00_00257b90(int lo, int hi);

extern s32 collision_line(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");

#include "sda.h"

extern float fast_cos(float);
extern float fast_sin(float);
extern float random_float_between(float, float);

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002FABE8.c: func_L01_00301198), where it is exact; names translated to the US level program. */

extern char                *spawn_burst_moby(void *, float) __asm__("FUN_L00_002d7e90");
extern int                  D_L01_001612D0; /* no foreign declaration */
extern float D_L01_001742E8 NOT_SDA;        /* no foreign declaration */
extern int  collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int  collision_hit_material(void) __asm__("FUN_001f0b58");
extern int  allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int  tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern void FUN_L00_0026f548(void *, void *, int, int);
extern void FUN_L00_002a3ec8(int, int, int, float, float, float, float);

/* Per-frame update of a falling debris moby: moves it, bursts into particles when it hits the ground, and removes it when its timer runs out. */
void falling_splash_moby_update(struct Moby *moby) __asm__("FUN_L01_002ffdc0");

/* Moby class 787 */
void falling_splash_moby_update(struct Moby *moby) {
    float next_pos[4];
    float effect_pos[4];
    float particle_vel[4];
    char *falling_splash_vars = (char *)moby->pvars;
    *(float *)(falling_splash_vars + 8) -= *(float *)(falling_splash_vars + 0x10);
    moby->rot.x += *(float *)(falling_splash_vars + 0x1C);
    fast_vec_add(next_pos, &moby->pos, falling_splash_vars);
    if (collision_line(&moby->pos, next_pos, 2, 0, 0)) {
        int   i;
        char *effect;
        if (!collision_hit_material()) {
            if (*(int *)(falling_splash_vars + 0x18) == 0) {
                allocate_voice_for_target_entry(0, 0, (int)moby);
            }
            FUN_L00_002a3ec8(
                D_L01_001612D0, pool_draw_count, 1, moby->pos.x, moby->pos.y, 0.5f, -0.35f);
            {
                float pos_unit = frame_time;
                float pos_unit2 = frame_time_sq;
                *(int *)(falling_splash_vars + 0x18) = 1;
                *(int *)(falling_splash_vars + 0x14) = 0x78;
                *(int *)(falling_splash_vars + 0x1C) = 0;
                *(float *)(falling_splash_vars + 8) = pos_unit * -1.5f;
                *(float *)(falling_splash_vars + 0x10) = pos_unit2 * 0.8f;
            }
            effect_pos[0] = moby->pos.x;
            effect_pos[1] = moby->pos.y;
            effect_pos[2] = D_L01_001742E8;
            effect = spawn_burst_moby(effect_pos, 2.0f);
            if (effect != 0) {
                effect[0x23] = 0x70;
            }
            for (i = 0; i < 16; i++) {
                float particle_angle = random_angle_radians();
                float horizontal_velocity_magnitude =
                    random_float_between(frame_time * 0.0f, frame_time * 3.0f);
                particle_vel[0] = fast_cos(particle_angle) * horizontal_velocity_magnitude;
                particle_vel[1] = fast_sin(particle_angle) * horizontal_velocity_magnitude;
                particle_vel[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
                {
                    int lifetime = FUN_L00_00257b90(0x5A, 0x78);
                    FUN_L00_0026f548(effect_pos, particle_vel, random_integer_below(2), lifetime);
                }
            }
        } else {
            mark_moby_for_removal(moby);
            return;
        }
    }
    qcopy(&moby->pos, next_pos);
    if (*(int *)(falling_splash_vars + 0x18) != 0) {
        moby->scale *= 0.99f;
    }
    if (tick_countdown_32_alt((int *)(falling_splash_vars + 0x14))) {
        mark_moby_for_removal(moby);
    }
}
