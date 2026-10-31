/* Level 05 moby class for a particle spray emitter that runs when triggered. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"

extern char D_0013E550[];
extern char D_L05_001671C0[];
extern int  allocate_voice_for_target_entry() __asm__("FUN_0022da68");

/* pvars of the 0x357 emitter: once woken it sprays particles along its facing (unkC0) that fall to the
   floor height of its D_L05_001612D0 row and splash there, and keeps the shared voice at the emitter nearest D_L05_001671C0. */
struct EmitterVars {
    s32  unk0;
    s32  floor;   /* row of D_L05_001612D0 */
    s32  trigger; /* D_L05_0015FFD8 index of the moby whose byte 0xBC wakes it, -1: none */
    s32 *voice;   /* voice slot shared by the emitters, -1: none */
    s32  age;     /* frames since it started spraying */
};

/* D_0013E550 seen from slot n (0x70 bytes per slot) */
struct VoiceSlot {
    u8           pad0[0x88];
    struct Moby *owner;
    u8           pad8C[4];
    Vec4f        pos;
};

/* 0x1190-byte rows; word 8 is the floor height the emitter particles land on */
struct FloorRow {
    u8  pad0[8];
    f32 z;
    u8  padC[0x1190 - 0xC];
};

extern void set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern void create_particle_with_velocity_and_randomized_parameters(
    float, float, float, void *, void *, int, int) __asm__("FUN_L05_0029be70");
extern void          spawn_splash(float, float, void *, void *, void *) __asm__("FUN_L00_002718d0");
extern unsigned char D_0014C050[][16];
extern struct Moby  *D_L05_0015FFD8;
extern struct FloorRow *D_L05_001612D0;
extern char             D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;

#include "rnc/overlay/l05/mobys/triggered_spray_emitter_data/triggered_spray_emitter_update.h"

void triggered_spray_emitter_update(struct Moby *emitter_moby) __asm__("FUN_L05_003150f0");

/* Moby class 855 */
void triggered_spray_emitter_update(struct Moby *emitter_moby) {
    enum {
        EMITTER_INITIALIZE = 0,
        EMITTER_WAIT_FOR_TRIGGER = 1,
        EMITTER_SPRAY_PARTICLES = 2
    };
    float               v[4];
    float               particle_velocity[4];
    float               u[4];
    struct EmitterVars *emitter_vars = (struct EmitterVars *)emitter_moby->pvars;
    float               floor;
    float               minimum_particle_speed;
    float               particle_speed, second_particle_speed_sample, third_particle_speed_sample;
    float               scale;
    int                 kind;
    int                 big;
    int                 i;
    unsigned char       state;

    if (emitter_vars == 0) {
        return;
    }
    switch (state = emitter_moby->state) {
    case EMITTER_INITIALIZE:
        if (D_0014C050[current_level_index][emitter_moby->mission] != 0xFF) {
            emitter_moby->state = 1;
        } else {
            emitter_moby->state = 3;
        }
        break;
    case EMITTER_WAIT_FOR_TRIGGER:
        if (emitter_vars->trigger >= 0 && D_L05_0015FFD8[emitter_vars->trigger].command != 0) {
            set_moby_list_state(emitter_moby->group, 2);
        }
        break;
    case EMITTER_SPRAY_PARTICLES:
        emitter_vars->age++;
        if (fast_vec_dist(&emitter_moby->pos, D_L05_001671C0) < 64.0f) {
            struct Moby *voice_owner;
            if (*emitter_vars->voice >= 0 &&
                (voice_owner =
                     ((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->owner) !=
                    0 &&
                voice_owner->oclass == 0x357) {
                if (voice_owner != emitter_moby &&
                    (voice_owner->state != state ||
                     fast_vec_dist(&emitter_moby->pos, D_L05_001671C0) <
                         fast_vec_dist(&voice_owner->pos, D_L05_001671C0))) {
                    fast_vec_normalize(v, &emitter_moby->rot_row0, 2.5f);
                    fast_vec_add(
                        &((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->pos,
                        v,
                        &emitter_moby->pos);
                    ((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->owner =
                        emitter_moby;
                }
            } else {
                *emitter_vars->voice = allocate_voice_for_target_entry(0, 0xD, emitter_moby);
                if (*emitter_vars->voice >= 0) {
                    fast_vec_normalize(v, &emitter_moby->rot_row0, 2.5f);
                    fast_vec_add(
                        &((struct VoiceSlot *)(D_0013E550 + *emitter_vars->voice * 0x70))->pos,
                        v,
                        &emitter_moby->pos);
                }
            }
        }
        floor = D_L05_001612D0[emitter_vars->floor].z;
        if (emitter_moby->pos.z < floor) {
            emitter_moby->state = 3;
        }
        for (i = 0; (float)i < spray_particle_count_limit_per_update_gp; i++) {
            big = 1;
            if (spray_big_particle_selection_threshold_percent_gp <
                random_float_between(0.0f, 100.0f)) {
                big = 0;
            }
            minimum_particle_speed = spray_big_particle_speed_lower_bound_gp;
            if (!big) {
                minimum_particle_speed = spray_small_particle_speed_lower_bound_gp;
            }
            qcopy(v, &emitter_moby->pos);
            fast_vec_normalize(
                u,
                &emitter_moby->rot_row1,
                random_float_between(-spray_particle_lateral_spread_and_height_offset_gp,
                                     spray_particle_lateral_spread_and_height_offset_gp));
            fast_vec_add(v, v, u);
            particle_speed =
                random_float_between(minimum_particle_speed, spray_particle_speed_upper_bound_gp);
            second_particle_speed_sample =
                random_float_between(minimum_particle_speed, spray_particle_speed_upper_bound_gp);
            third_particle_speed_sample =
                random_float_between(minimum_particle_speed, spray_particle_speed_upper_bound_gp);
            if (particle_speed < second_particle_speed_sample) {
                particle_speed = second_particle_speed_sample;
            }
            if (particle_speed < third_particle_speed_sample) {
                particle_speed = third_particle_speed_sample;
            }
            fast_vec_normalize(
                particle_velocity, &emitter_moby->rot_row0, particle_speed * frame_time);
            if (big) {
                scale = spray_big_particle_base_scale_gp;
                kind = spray_big_particle_kind_gp;
                v[2] += spray_particle_lateral_spread_and_height_offset_gp - scale +
                        random_float_between(-0.4f, 0.4f);
            } else {
                scale = spray_small_particle_base_scale_gp;
                kind = spray_small_particle_kind_gp;
                v[2] += spray_particle_lateral_spread_and_height_offset_gp - scale + 0.4f;
            }
            create_particle_with_velocity_and_randomized_parameters(
                scale * random_float_between(0.8f, 1.2f),
                floor,
                frame_time_sq * 10.0f,
                v,
                particle_velocity,
                kind,
                big);
            if (random_integer_below(2)) {
                /* fall time from the emitter to the floor */
                float fall_time_frames =
                    square_root(2.0f * (emitter_moby->pos.z - floor) / (frame_time_sq * 10.0f));
                if (fall_time_frames < (float)emitter_vars->age) {
                    float splash_forward_distance = fall_time_frames * (frame_time * 4.0f);
                    float splash_offset_angle = random_angle_radians();
                    float splash_offset_radius = random_float_between(0.0f, 1.25f);
                    fast_vec_scale(v, &emitter_moby->rot_row0, splash_forward_distance);
                    fast_vec_add(v, v, &emitter_moby->pos);
                    v[0] += fast_cos(splash_offset_angle) * splash_offset_radius;
                    v[1] += fast_sin(splash_offset_angle) * splash_offset_radius;
                    v[2] = floor + 0.05f;
                    spawn_splash(random_float_between(1.0f, 1.5f),
                                 random_integer_below(2) ? 2.0f : -2.0f,
                                 v,
                                 D_L05_0015F580_macro,
                                 &D_L05_001612D0[emitter_vars->floor].z);
                }
            }
        }
        break;
    }
}
