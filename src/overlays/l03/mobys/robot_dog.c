/* Level 03 moby class robot_dog. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l03/mobys/robot_dog.h"

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C6F40), where it is exact; names translated to the US level program. */

int is_robot_dog_in_state_5(struct Moby *moby) __asm__("FUN_L03_002c5b78");

int is_robot_dog_in_state_5(struct Moby *moby) {
    if (moby && moby->state != 0xFE && moby->state != 0xFD && moby->oclass == 0x23D &&
        moby->state == 5) {
        return 1;
    }
    return 0;
}

extern char  D_L03_00166EC0[];
int          can_release_linked_robot_dog(struct Moby *moby) __asm__("FUN_L03_002c6ca0");
extern int   FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");

typedef struct {
    float pos[4];
    float v10[4];
    float dir[4];
    float v30[4];
    void *obj;
    int   pad[3];
} Tgt2c5bb8;

typedef struct {
    char  p00[0x20];
    float f20;
    short s24;
    char  p26[3];
    u8    b29;
    char  p2A[0x16];
    float v40[4];
    char  p50[8];
    u8    b58;
    char  p59;
    u8    b5A;
    char  p5B[0x6D];
    short sC8;
    char  pCA[6];
    void *pD0;
    char  pD4[0x3C];
    char  m110[0x10];
    char  m120[0x60];
    int   i180;
    char  p184[4];
    float f188;
    float f18C;
    char  p190[0x14];
    float f1A4;
    float f1A8;
    float f1AC;
    char  p1B0[8];
    int   i1B8;
    float f1BC;
    float f1C0;
    float f1C4;
    char  p1C8[0x38];
    float home[4];
    char  p210[0x10];
    float v220[4];
    float f230;
    float f234;
    char  p238[4];
    int   i23C;
    int   i240;
    int   i244;
    int   i248;
    char  p24C[0x14];
    int   i260;
    float f264;
    char  p268[4];
    int   i26C;
    int   i270;
    float f274;
    int   i278;
    int   i27C;
    char  m280[0x10];
} Data2c5bb8;

extern char *D_L03_0015FFD8_c5bb8 __asm__("D_L03_0015FFD8");
char         D_L03_001618D0 = 3;
extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") MACRO_ADDR;
extern float FUN_L04_001f1978(float);
extern int   rand(void) __asm__("FUN_001160d8");
extern int   FUN_L00_00258b50(void *, void *, void *, void *, float);
extern int   FUN_L00_00259028(char *m, char *p, char *pos, char *v);
extern int   FUN_L00_002dc9f0(void *, void *);
extern int select_moby_target_with_hero_fallback(char *, char *, float) __asm__("FUN_L03_0024e830");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_0025a120(void *);
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00263ac8_c5bb8(void *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void
spawn_moby_break_fragments(void *, int, int, int, int, int, int) __asm__("FUN_L00_00263e30");
extern void FUN_L01_0026d930(char *p, void *);

extern void update_robot_dog_hit_reaction(struct Moby *moby) __asm__("FUN_L03_002c6a20");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void spawn_debris_c5bb8(char *,
                               int,
                               float *,
                               void *,
                               int,
                               int,
                               float,
                               float *,
                               float *,
                               float *) __asm__("FUN_L00_00263fd8");

/* Kerwan enemy update: waits at home, walks its path or chases the target picked by FUN_L03_0024e830
 * (its partner moby data->0x248 is a class 0x23E), attacks in range, returns home, dies (states 8, 9, 21). */
void robot_dog_update(char *moby) __asm__("FUN_L03_002c5bb8");

void robot_dog_update(char *moby) {
    enum {
        ROBOT_DOG_INITIALIZE = 0,
        ROBOT_DOG_WAIT_FOR_ACTIVATION = 1,
        ROBOT_DOG_WAIT_FOR_PARTNER_RELEASE = 2,
        ROBOT_DOG_TURN_TO_TARGET = 3,
        ROBOT_DOG_CHASE_TARGET = 4,
        ROBOT_DOG_FOLLOW_PATH = 5,
        ROBOT_DOG_MELEE_ATTACK = 6,
        ROBOT_DOG_RETURN_HOME = 7,
        ROBOT_DOG_FALL_RECOVERY = 8,
        ROBOT_DOG_WAIT_TO_REINITIALIZE = 9,
        ROBOT_DOG_DEATH_FALL = 21
    };
    Tgt2c5bb8   selected_target;
    float       forward_vector_or_path_point[4];
    float       push[4];
    float       at[4];
    int         seg;
    float       frac;
    Data2c5bb8 *data;
    int        *path;
    char       *activation_path;
    char       *partner_moby;
    int         partner_moby_index;
    float       target_distance;
    float       initialization_multiplier_or_desired_heading;

    data = ((Data2c5bb8 *)MOBY(moby)->pvars);
    select_moby_target_with_hero_fallback(moby, (char *)&selected_target, data->f230);
    FUN_L00_00263ac8_c5bb8(moby, 0, data->m280, 2.1f);
    update_robot_dog_hit_reaction((struct Moby *)moby);
    fast_dec_timer(&data->i244);
    switch (((unsigned char *)moby)[0x20]) {
    case ROBOT_DOG_INITIALIZE:
        MOBY(moby)->flags |= 0x1000;
        qcopy(data->home, moby + 0x10);
        moby[0x20] = 1;
        moby[0x7F] = 0x18;
        data->f20 = 1.0f;
        data->b29 = 0;
        data->i240 = 0;
        data->s24 = 1;
        FUN_L01_0026d930((char *)&data->i180, data->home);
        data->f1BC = frame_time_sq * 6.2831855f;
        data->f1C0 = frame_time_sq * 12.566371f;
        data->f1C4 = frame_time * 3.1415927f;
        data->f188 = data->f18C = 2.0f;
        data->i180 = 0x38D;
        data->f1A4 = data->f234 * frame_time;
        data->i1B8 |= 0x20;
        data->f1A8 = 2.0954423f;
        initialization_multiplier_or_desired_heading = FUN_L04_001f1978(6.0f);
        data->f264 = 1.0f;
        data->i244 = 0;
        data->i26C = 0;
        data->i260 = 0;
        data->f1AC = data->f1A4 * initialization_multiplier_or_desired_heading;
        data->b58 = 8;
        data->b5A = 7;
        if (rand() & 1) {
            MOBY(moby)->flags |= 0x8000;
        }
        data->pD0 = &D_L03_001618D0;
        if (data->i248 != -1 && data->i270 == -1) {
            if (((unsigned char *)moby)[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            moby[0x20] = 2;
        } else if (((unsigned char *)moby)[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 0);
        }
        break;
    case ROBOT_DOG_WAIT_FOR_ACTIVATION:
        if (data->i270 != -1) {
            activation_path = ((char *)l03_paths[data->i270]);
            *(OvlQuad *)&MOBY(moby)->pos = *(OvlQuad *)(activation_path + 0x10);
            MOBY(moby)->rot.z = fast_arc_tan(
                *(float *)(activation_path + 0x20) - *(float *)(activation_path + 0x10),
                *(float *)(activation_path + 0x24) - *(float *)(activation_path + 0x14));
            if ((random_integer_below(14) == 0 &&
                 is_point_inside_clip_volume_cf(((unsigned char *)moby)[0x21])) ||
                is_point_inside_clip_volume(&selected_target, data->i27C) ||
                (fast_vec_dist(&selected_target, moby + 0x10) < data->f274 &&
                 absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) < 4.0f)) {
                data->i278 = 0;
                moby[0x20] = 5;
                if (((unsigned char *)moby)[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            }
        } else if (random_integer_below(14) == 0 &&
                   fast_vec_dist(&selected_target, data->home) < data->f230 &&
                   absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) < 3.0f) {
            moby[0x20] = 3;
            if (((unsigned char *)moby)[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
            MOBY(moby)->anim_speed = 0.8780488f;
        }
        break;
    case ROBOT_DOG_WAIT_FOR_PARTNER_RELEASE:
        if (random_integer_below(14) == 0) {
            partner_moby_index = data->i248;
            if (partner_moby_index == -1 ||
                (partner_moby = D_L03_0015FFD8_c5bb8 + (partner_moby_index << 8)) == 0 ||
                ((unsigned char *)partner_moby)[0x20] == 0xFE ||
                ((unsigned char *)partner_moby)[0x20] == 0xFD ||
                can_release_linked_robot_dog((struct Moby *)partner_moby)) {
                data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
                moby[0x20] = 4;
                if (((unsigned char *)moby)[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case ROBOT_DOG_TURN_TO_TARGET:
        initialization_multiplier_or_desired_heading = fast_arc_tan(
            selected_target.pos[0] - MOBY(moby)->pos.x, selected_target.pos[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.02f, 0.3f, 0.1f);
        if (((unsigned char *)moby)[0x70] & 2) {
            data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby[0x20] = 4;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case ROBOT_DOG_CHASE_TARGET:
        initialization_multiplier_or_desired_heading = fast_arc_tan(
            selected_target.pos[0] - MOBY(moby)->pos.x, selected_target.pos[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.05f, 0.3f, 0.2f);
        forward_vector_or_path_point[0] = 2.0f * fast_cos(MOBY(moby)->rot.z);
        forward_vector_or_path_point[1] = 2.0f * fast_sin(MOBY(moby)->rot.z);
        forward_vector_or_path_point[2] = 0.0f;
        FUN_L00_00258b50(moby, &data->i180, forward_vector_or_path_point, push, 1.0f);
        if (fast_dec_timer(&data->i23C)) {
            data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
        }
        target_distance = vector_distance_xy(moby + 0x10, &selected_target);
        if (target_distance < data->f234 * frame_time * 0.5f * FUN_L04_001f1978(40.0f) +
                                  data->f234 * frame_time * FUN_L04_001f1978(10.0f) + 1.5f) {
            moby[0x20] = 6;
            if (((unsigned char *)moby)[0x53] != 5) {
                blend_moby_animation(moby, 5, 0, truncate_float_to_s32(FUN_L04_001f1978(10.0f)));
            }
        } else if (data->f230 < fast_vec_dist(&selected_target, data->home) ||
                   absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) > 3.0f) {
            moby[0x20] = 7;
        }
        break;
    case ROBOT_DOG_FOLLOW_PATH:
        path = (int *)l03_paths[data->i270];
        FUN_L00_0025df68(
            path, moby + 0x10, forward_vector_or_path_point, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
        if (data->i278 < seg) {
            data->i278 = seg;
        }
        if (FUN_L00_00259028(moby,
                             (char *)&data->i180,
                             (char *)path + (data->i278 * 0x10 + 0x10),
                             (char *)data->v40) &
            0x14) {
            if (data->i278 < *path - 1) {
                data->i278++;
            } else {
                data->i270 = -1;
                moby[0x20] = 1;
                if (((unsigned char *)moby)[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
                if (data->i248 != -1) {
                    if (((unsigned char *)moby)[0x53] != 0) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                    }
                    moby[0x20] = 2;
                }
            }
        }
        if (random_integer_below(14) == 0) {
            partner_moby_index = data->i248;
            if (partner_moby_index == -1 ||
                (partner_moby = D_L03_0015FFD8_c5bb8 + (partner_moby_index << 8)) == 0 ||
                ((unsigned char *)partner_moby)[0x20] == 0xFE ||
                ((unsigned char *)partner_moby)[0x20] == 0xFD ||
                can_release_linked_robot_dog((struct Moby *)partner_moby)) {
                data->i270 = -1;
                data->i23C = scale_game_frames((int)random_float_between(30.0f, 90.0f));
                moby[0x20] = 4;
                if (((unsigned char *)moby)[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case ROBOT_DOG_MELEE_ATTACK:
        initialization_multiplier_or_desired_heading = fast_arc_tan(
            selected_target.pos[0] - MOBY(moby)->pos.x, selected_target.pos[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.05f, 0.3f, 0.2f);
        forward_vector_or_path_point[0] = 2.0f * fast_cos(MOBY(moby)->rot.z);
        forward_vector_or_path_point[1] = 2.0f * fast_sin(MOBY(moby)->rot.z);
        forward_vector_or_path_point[2] = 0.0f;
        FUN_L00_00258b50(moby, &data->i180, forward_vector_or_path_point, data->v40, 1.0f);
        if (((unsigned char *)moby)[0x52] == 5) {
            approach_value(&data->f1A4, 0.0f, data->f234 * frame_time / FUN_L04_001f1978(40.0f));
        }
        if (absolute_float(compute_interpolated_record_value(moby) - 25.0f) <= 0.25f &&
            absolute_float(selected_target.pos[2] - MOBY(moby)->pos.z) < 1.0f &&
            fast_difference_between_rotations(
                fast_arc_tan(selected_target.pos[0] - MOBY(moby)->pos.x,
                             selected_target.pos[1] - MOBY(moby)->pos.y),
                MOBY(moby)->rot.z) < 0.2617994f &&
            selected_target.obj != 0 && vector_distance_xy(moby + 0x10, &selected_target) < 2.0f) {
            push[0] = fast_cos(MOBY(moby)->rot.z) * 0.2f;
            push[1] = fast_sin(MOBY(moby)->rot.z) * 0.2f;
            push[2] = 0.0f;
            qcopy(at, &selected_target);
            at[2] += 0.75f;
            spawn_nearby_effect(selected_target.obj, moby, 1, at, push, 1.0f);
        } else if (((unsigned char *)moby)[0x70] & 2) {
            data->f1A4 = data->f234 * frame_time;
            moby[0x20] = 7;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(3));
            }
            data->i244 = scale_game_frames(120);
        }
        break;
    case ROBOT_DOG_RETURN_HOME:
        initialization_multiplier_or_desired_heading =
            fast_arc_tan(data->home[0] - MOBY(moby)->pos.x, data->home[1] - MOBY(moby)->pos.y);
        FUN_L00_00258278(
            moby, data->v220, initialization_multiplier_or_desired_heading, 0.02f, 0.3f, 0.1f);
        forward_vector_or_path_point[0] = 2.0f * fast_cos(MOBY(moby)->rot.z);
        forward_vector_or_path_point[1] = 2.0f * fast_sin(MOBY(moby)->rot.z);
        forward_vector_or_path_point[2] = 0.0f;
        FUN_L00_00258b50(moby, &data->i180, forward_vector_or_path_point, push, 1.0f);
        if (vector_distance_xy(moby + 0x10, data->home) < 1.0f) {
            moby[0x20] = 1;
            if (((unsigned char *)moby)[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(3));
            }
        } else if (data->i244 == 0) {
            target_distance = vector_distance_xy(moby + 0x10, &selected_target);
            if (target_distance < data->f234 * frame_time * 0.5f * FUN_L04_001f1978(40.0f) +
                                      data->f234 * frame_time * FUN_L04_001f1978(10.0f) + 1.5f) {
                moby[0x20] = 6;
                if (((unsigned char *)moby)[0x53] != 5) {
                    blend_moby_animation(
                        moby, 5, 0, truncate_float_to_s32(FUN_L04_001f1978(10.0f)));
                }
            }
        }
        break;
    case ROBOT_DOG_FALL_RECOVERY:
        if (step_moby_motion(moby, data->m120) & 0x60) {
            moby[0x20] = 4;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
            break;
        }
        goto low;
    case ROBOT_DOG_WAIT_TO_REINITIALIZE:
        if (FUN_L00_002dc9f0(moby, data->m120)) {
            moby[0x20] = 0;
            data->sC8 = 0;
        }
        break;
    case ROBOT_DOG_DEATH_FALL:
        MOBY(moby)->flags &= 0xEFFF;
        if (step_moby_motion(moby, data->m120) & 0x60) {
            clear_u64_value(forward_vector_or_path_point);
            cart_burst(moby, moby + 0x10, -1, 0.5f, 13.0f);
            spawn_debris_c5bb8(moby,
                               0x6CE,
                               (float *)&MOBY(moby)->pos,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro);
            spawn_debris_c5bb8(moby,
                               0x6CF,
                               (float *)&MOBY(moby)->pos,
                               moby + 0x40,
                               0,
                               0,
                               0.0f,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro,
                               D_L03_0015F580_macro);
            spawn_moby_break_fragments(moby, 0x77D, 1, 0x77D, 1, 3, 2);
            FUN_L00_00257470(moby, 0, -1);
            mark_moby_for_removal(moby);
            return;
        }
    low:
        if (MOBY(moby)->pos.z < 2.0f) {
            FUN_L00_00257470(moby, 0, -1);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    if (((unsigned char *)moby)[0x31] != 0 && fast_vec_dist(moby + 0x10, D_L03_00166EC0) < 32.0f) {
        FUN_L00_0025a120(moby);
    }
    trooper_anim_finish(moby, data->m110);
    ((unsigned char *)moby)[0xBC] = 0;
}
/* Applies damage reaction, knockback parameters and animation state. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C7DE8), where it is exact; names translated to the US level program. */

extern char *D_L03_0015FFD8_c __asm__("D_L03_0015FFD8");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void FUN_L01_0026e090(int, int);
extern void trooper_set_knockback(float *, float, float) __asm__("FUN_L03_00250a78");

void update_robot_dog_hit_reaction(struct Moby *moby) {
    float vector[4];
    int   result;
    float damage;
    float angle;
    char *data = (char *)moby->pvars;
    char *hit;
    if (*(int *)(data + 0x38) != 0 ||
        (*(int *)(data + 0x248) != -1 &&
         can_release_linked_robot_dog(
             (unsigned char *)(D_L03_0015FFD8_c + (*(int *)(data + 0x248) << 8)))))
        *(int *)(data + 0x240) = scale_game_frames(240);
    if (fast_dec_timer((int *)(data + 0x240)))
        *(float *)(data + 0x230) = *(float *)(data + 0x22C);
    else
        *(float *)(data + 0x230) = *(float *)(data + 0x22C) * 2.0f;
    damage = 0.0f;
    hit = find_moby_hit(moby, 0x330000, 0);
    hit_reaction(moby, hit, data + 0x20, 0, &result, &damage, 0, 4);
    if (result != 1 && moby->state != 21) {
        char *other;
        if (moby->group != 255)
            FUN_L01_0026e090(moby->group, 1);
        {
            float health = *(float *)(data + 0x20) - damage;
            float speed = frame_time_sq * 50.0f;
            *(int *)(data + 0x144) = 9;
            data[0x15D] = 0;
            *(float *)(data + 0x130) = speed;
            *(float *)(data + 0x20) = health;
        }
        other = *(char **)(hit + 0x20);
        angle = fast_arc_tan(moby->pos.x - *(float *)(other + 0x10),
                             moby->pos.y - *(float *)(other + 0x14));
        {
            float rotationSpeed = frame_time * 10.0f;
            moby->flags &= 0xEFFF;
            *(float *)(data + 0x13C) = rotationSpeed;
        }
        trooper_set_knockback((float *)(data + 0x120), 4.5f, 2.0f);
        *(float *)(data + 0x170) = 5.0f;
        *(float *)(data + 0x174) = 12.0f;
        *(OvlQuad *)vector = *(OvlQuad *)(hit + 0x10);
        trooper_knockback_arc(vector, &angle, data + 0x138, data + 0x13C);
        trooper_start_knockback(angle, moby, data + 0x120, 4, 1, 0);
        moby->state = 21;
        *(unsigned char *)(data + 0x117) = 240;
        trooper_anim_update(moby, (short *)(data + 0x110));
    }
    moby->hit_slot = 255;
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C8068), where it is exact; names translated to the US level program. */

int can_release_linked_robot_dog(struct Moby *moby) {
    unsigned char state;
    char         *data;
    data = (char *)moby->pvars;
    if (!moby)
        return 0;
    state = moby->state;
    if (state == 0xFE)
        return 0;
    if (state == 0xFD)
        return 0;
    if (moby->oclass != 0x23E)
        return 0;
    if (moby->seq == 10 && compute_interpolated_record_value(moby) >= 10.0f)
        return 1;
    if (moby->state == 10 || moby->state == 13 || *(int *)(data + 0x284) == -1)
        return 1;
    return 0;
}

extern int get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
/* scans class instances for either predicate */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C8160), where it is exact; names translated to the US level program. */

extern int is_kerwan_path_enemy_in_state_9(unsigned char *) __asm__("FUN_L03_002c6d58");

int is_point_inside_clip_volume_cf(int id) __asm__("FUN_L03_002c6d98");

int is_point_inside_clip_volume_cf(int id) {
    unsigned char *item;
    if (id == 0xFF)
        goto absent;
    get_first_moby_in_group((int *)&item, id, 0, 0);
    while (item != 0) {
        if (is_kerwan_path_enemy_in_state_9(item))
            return 1;
        if (is_robot_dog_in_state_5(item))
            return 1;
        get_next_moby_in_group((int *)&item, (int)item, 0, 0);
    }
absent:
    return 0;
}
