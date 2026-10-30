/* Level 13 forcefield tower moby class: damage and destruction effects, and the voice position helper it uses. */
#include "types.h"
#include "rnc/data_labels.h"
#include "qzero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/l13/mobys/hit_proxy.h"

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");

extern float D_0015ED6C;

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

#include "rnc/overlay/l13/effect_suppression_levels.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern int FUN_L00_00257b90(int, int);

extern void FUN_L00_00257470(struct Moby *, int, int);

extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern float D_L13_001670C0[];

extern void DebugPrint(char *, ...);

extern void release_voice_slot(s32) __asm__("FUN_0022d798");

typedef int u128z __attribute__((mode(TI)));

typedef struct {
    unsigned char b[24];
} Blk24;

Blk24 D_L13_001F4950 = {
    {0xff, 0x8f, 0, 0x4f, 0xff, 0x8f, 0,    0x4f, 0xff, 0x7f, 0,    0x4f,
     0xff, 0x6f, 0, 0x4f, 0xff, 0xff, 0xff, 0x2f, 0xff, 0xff, 0xff, 0x2f}
};

Blk24 D_L13_001F4968 = {
    {0x7f, 0x5f, 0, 0x2f, 0x7f, 0x4f, 0, 0x2f, 0x7f, 0x3f, 0, 0x2f,
     0x4f, 0,    0, 0x2f, 0,    0,    0, 0x2f, 0,    0,    0, 0x3f}
};

extern float D_L13_0015F580;

extern int spawn_glow_particle(void *, void *, void *, float, int, int, int, int, int) __asm__("FUN_L00_002ac910");

extern s32 random_integer_below(s32) __asm__("FUN_00213260");

extern unsigned char *
spawn_spinning_fade_in_out_effect_particle(void *pos,
                                           int   spin,
                                           int   col,
                                           float range,
                                           float f1,
                                           float f2,
                                           float f3,
                                           float scale) __asm__("FUN_L00_0026b790");

extern void FUN_L00_0026a9f0(void *, void *, int, int, int, int, int, float, int, float);

extern void spawn_crate_debris(void *, void *, void *, void *, int, int, float, float) __asm__("FUN_L00_002af8b0");

/* Sets flag 0x40 on the voice and copies its 16-byte position. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/sound_002994D0.c: func_L13_002994D0), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x75];
    unsigned char flags;
} Level13SoundEffectTarget;

int set_voice_position(int index, const void *vector) __asm__("FUN_L13_002982e8");

int set_voice_position(int index, const void *vector) {
    int                       offset = index * 0x70;
    char                     *base = D_0013E533 + 0x1D;
    char                     *vector_base = base + 0xA0;
    Level13SoundEffectTarget *effect = (Level13SoundEffectTarget *)(base + offset);
    effect->flags |= 0x40;
    qcopy(vector_base + offset, vector);
    return 1;
}

void emit_forcefield_tower_destruction_effects(unsigned char *source_moby,
                                               void          *effect_position,
                                               int emit_extra_effects) __asm__("FUN_L13_002ce688");

void emit_forcefield_tower_destruction_effects(unsigned char *source_moby,
                                               void          *effect_position,

                                               int emit_extra_effects) {
    int            k;
    int            j;
    float          vA[4];
    float          vB[4];
    float          vE[4];
    float          vD[4];
    float          vC[4];
    Blk24          vG;
    Blk24          vH;
    int            extra_emission_count;
    int            debris_variant_index;
    int            glow_frame_count;
    float          emission_reference_distance;
    float          emission_distance_shortfall;
    float          random_emission_base;
    float          yv;
    unsigned char *emission_table_bytes;
    int           *pa;
    int           *pb;
    int            ra;
    int            rb;
    int            A;
    int            B;
    int            C;
    int            D;
    int            E;
    int            F;

    char *pA;
    pA = (char *)vA;
    *(u128z *)vA = *(u128z *)effect_position;
    qcopy(vB, pA);
    vB[2] = vB[2] + 0.3f;
    spawn_spinning_fade_in_out_effect_particle(
        vB, 1, 0x40808080, 0.3f, 1.01f, 1.205f, 0.05f, 150000.0f);
    for (k = 0; k < 5; k++) {
        spawn_spinning_fade_in_out_effect_particle(
            vB, 1, 0x40808080, 0.3f, 1.01f, 1.1800001f, 0.05f, 150000.0f);
    }
    for (k = 0; k < 4; k++) {
        *(u128z *)vC = 0;
        vC[0] = random_float_between(-1.875f, 1.875f);
        vC[1] = random_float_between(-1.875f, 1.875f);
        vC[2] = random_float_between(0.0f, 3.75f);
        *(u128z *)vD = *(u128z *)vC;
        fast_vec_add(vD, vD, vB);
        fast_vec_sub(vE, vD, pA);
        fast_vec_normalize(vE, vE, random_float_between(0.025f, 0.23f));
        spawn_crate_debris(source_moby, vE, vD, &D_L13_0015F580, 0x162, 0, 0.90000004f, 2.1f);
    }
    for (k = 0; k < 4; k++) {
        *(u128z *)vC = 0;
        vC[0] = random_float_between(-1.875f, 1.875f);
        vC[1] = random_float_between(-1.875f, 1.875f);
        vC[2] = random_float_between(0.0f, 3.75f);
        *(u128z *)vD = *(u128z *)vC;
        fast_vec_add(vD, vD, vB);
        fast_vec_sub(vE, vD, pA);
        fast_vec_normalize(vE, vE, random_float_between(0.025f, 0.23f));
        debris_variant_index = random_integer_below(2);
        spawn_crate_debris(
            source_moby, vE, vD, &D_L13_0015F580, debris_variant_index + 0x163, 0, 1.2f, 2.4f);
    }
    if (emit_extra_effects == 0) {
        return;
    }
    fast_vec_sub(vD, D_L13_001670C0, pA);
    clear_u64_value(vC);
    emission_reference_distance = fast_vec_length(vD);
    extra_emission_count = (emission_reference_distance < 8.0f)
                               ? truncate_float_to_s32(emission_reference_distance) + 2
                               : 10;
    emission_distance_shortfall =
        (emission_reference_distance < 7.0f) ? 7.0f - emission_reference_distance : 0.0f;
    if (extra_emission_count > 0) {
        emission_table_bytes = vG.b;
        j = extra_emission_count;
        do {
            random_emission_base = random_float_between(8.0f, 10.0f);
            j--;
            *(Blk24 *)vG.b = *(Blk24 *)&D_L13_001F4950;
            yv = random_emission_base * D_0015ED6C;
            *(Blk24 *)vH.b = *(Blk24 *)&D_L13_001F4968;
            yv = yv - emission_distance_shortfall * D_0015ED6C;
            ra = random_integer_below(6);
            pa = (int *)(emission_table_bytes + ra * 4);
            rb = random_integer_below(6);
            pb = (int *)(vH.b + rb * 4);
            A = scale_game_frames(15);
            B = scale_game_frames(20);
            C = FUN_L00_00257b90(A, B);
            D = scale_game_frames(25);
            E = scale_game_frames(30);
            F = FUN_L00_00257b90(D, E);
            FUN_L00_0026a9f0(pA, vC, *pa, *pb, C, F, 0, 1200000.0f, 0, yv);
        } while (j != 0);
    }
    if (optional_glow_suppression_level < 0.95f) {
        glow_frame_count = scale_game_frames(15);
        spawn_glow_particle(source_moby, pA, vC, 9.0f, glow_frame_count, 0x7F, 0x7F, 0x7F, 0x20);
        glow_frame_count = scale_game_frames(24);
        spawn_glow_particle(source_moby, pA, vC, 6.0f, glow_frame_count, 0x7F, 0x20, 0, 0x20);
    }
    glow_frame_count = scale_game_frames(20);
    spawn_glow_particle(source_moby, pA, vC, 9.0f, glow_frame_count, 0x7F, 0x40, 0, 0x30);
    glow_frame_count = scale_game_frames(27);
    spawn_glow_particle(source_moby, pA, vC, 6.0f, glow_frame_count, 0x60, 0x10, 0, 0x40);
    glow_frame_count = scale_game_frames(29);
    spawn_glow_particle(source_moby, pA, vC, 3.0f, glow_frame_count, 0x20, 0, 0, 0x20);
}

extern int fptodp(float) __asm__("FUN_00120478");

extern int
fun_00213928(struct Moby *, void *, float *, int, int *, int, int, int) __asm__("FUN_00213928");

extern void release_voice_slot(int) __asm__("FUN_0022d798");

extern void emit_effect_l13(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

extern char D_L13_001f4980[];

extern char D_L13_001f49a8[];

void process_forcefield_tower_damage(struct Moby   *moby,
                                     unsigned char *moby_data,
                                     float         *health) __asm__("FUN_L13_002cecb8");

void process_forcefield_tower_damage(struct Moby *moby, unsigned char *moby_data, float *health) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_REMOVE = 2,
        HIT_REACTION_LOW_INTERPOLATION_TARGET = 3,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE = 4,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE = 5,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE = 6,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE = 7,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE = 8,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET = 9,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE = 10,
        HIT_REACTION_NO_HIT = 11
    };
    char            *hit_record;
    int              hit;
    float            position[4];
    float            velocity[4];
    int              link;
    VoicePoolWindow *record;

    if (moby->state != 2) {
        hit_record = find_moby_hit(moby, 0x330000, 0);
        if (hit_record != 0) {
            DebugPrint(D_L13_001f4980, moby->oclass, fptodp(*(float *)(hit_record + 0x2c)));
            DebugPrint(D_L13_001f49a8,
                       *(int *)(hit_record + 0x38),
                       fptodp(*(float *)(hit_record + 0)),
                       fptodp(*(float *)(hit_record + 4)),
                       fptodp(*(float *)(hit_record + 8)));
        }
        switch (fun_00213928(moby, hit_record, health, 0, &hit, 0, 0, 4)) {
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_REMOVE:
            *health = 0.0f;
            break;
        case HIT_REACTION_NONE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE:
        case HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET:
        case HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE:
        case HIT_REACTION_NO_HIT:
            break;
        }
        if (hit >= 2) {
            if (hit_record == 0) {
                goto mark;
            }
            if (*(char **)(hit_record + 0x20) != 0 &&
                moby->pos.z + 8.7f < *(float *)(*(char **)(hit_record + 0x20) + 0x18)) {
                if (*health <= *(float *)(hit_record + 0x2c)) {
                    *(OvlQuad *)velocity = 0;
                    velocity[2] = frame_time * 8.0f;
                    *health = 0.0f;
                    moby->flags &= 0xefff;
                    *(OvlQuad *)position = *(OvlQuad *)velocity;
                    FUN_L00_00257470(moby, 0, -1);
                    moby_data[0x67] = 0x78;
                    trooper_anim_update(moby, (short *)(moby_data + 0x60));
                    moby_point_position(moby, 0, velocity);
                    emit_effect_l13(moby,
                                    position,
                                    velocity,
                                    0.0f,
                                    0.0f,
                                    10,
                                    3,
                                    16,
                                    4.0f,
                                    2.0f,
                                    0.0f,
                                    1.0f,
                                    1,
                                    20.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
                    if (moby->prev_seq != 1) {
                        blend_moby_animation((MobyAnim *)moby, 1, 0, 0);
                    }
                    link = *(int *)(moby_data + 0x70);
                    if (link != -1) {
                        record = (VoicePoolWindow *)((u8 *)&voice_pool + link * 0x70);
                        if ((struct Moby *)record->voice.owner == moby &&
                            record->voice.state != 0) {
                            release_voice_slot(link);
                        }
                    }
                    *(int *)(moby_data + 0x70) = -1;
                    moby->state = 2;
                } else {
                    *health -= *(float *)(hit_record + 0x2c);
                    moby_data[0x67] = 0xfa;
                    *(short *)(moby_data + 0x26) = scale_game_frames(0x3c);
                    trooper_anim_update(moby, (short *)(moby_data + 0x60));
                }
            }
        }
    mark:
        moby->hit_slot = 0xff;
    }
    trooper_anim_finish(moby, moby_data + 0x60);
}

extern int FUN_001160d8(void);

extern int start_moby_voice(int, int, void *) __asm__("FUN_L13_00297fa0");

extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");

extern int is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");

extern void spawn_particle(void *, int, int, int, float, int, float) __asm__("FUN_L00_00269b70");

extern int D_0014C190[];

extern int linked_state_flags[] __asm__("D_L13_001847A8");

void forcefield_tower_update(struct Moby *moby) __asm__("FUN_L13_002cefc0");

void forcefield_tower_update(struct Moby *moby) {
    enum {
        TOWER_STATE_INITIALIZE = 0,
        TOWER_STATE_ACTIVE = 1,
        TOWER_STATE_DESTRUCTION = 2
    };
    enum {
        TOWER_DESTRUCTION_WAIT_FOR_POINT_1_EFFECT = 0,
        TOWER_DESTRUCTION_WAIT_FOR_POINT_2_EFFECT = 1,
        TOWER_DESTRUCTION_WAIT_FOR_POINT_3_EFFECT = 2
    };
    unsigned char *moby_bytes = (unsigned char *)moby;
    int           *data = *(int **)(moby_bytes + 0x78);
    float          position[4];
    float          velocity[4];
    float          rotated[4];
    int            linked_index;
    int            i;
    int            red;
    int            green;
    int            blue;
    float          scale;
    float          threshold;
    int            state = moby_bytes[0x20];

    switch (state) {
    case TOWER_STATE_INITIALIZE: {
        moby_bytes[0xBC] = 0;
        moby_bytes[0x20] = 1;
        if (moby_bytes[0x30] < *(short *)(moby_bytes + 0x32))
            moby_bytes[0x30] = moby_bytes[0x32];
        data[0x70 / 4] = -1;
        if ((*(int *)((char *)D_0014C190 + (current_level_index << 8) +
                      (((short)*(unsigned short *)(moby_bytes + 0xB2) >> 5) << 2)) >>
             (*(unsigned short *)(moby_bytes + 0xB2) & 31)) &
            1) {
            linked_index = data[0x74 / 4];
            if (linked_index != -1)
                linked_state_flags[linked_index] = 1;
            mark_moby_for_removal(moby);
            return;
        }
        moby_point_position(moby, 0, position);
        position[2] += 1.1f;
        spawn_hit_proxy((char *)moby, position, 2.7f);
        *(unsigned short *)(moby_bytes + 0x34) &= 0xEFFF;
        return;
    }
    case TOWER_STATE_ACTIVE: {
        linked_index = data[0x74 / 4];
        if (linked_index != -1)
            linked_state_flags[linked_index] = 0;
        if (data[0x70 / 4] == -1)
            goto allocate_voice;
        if (voice_is_playing(moby, data[0x70 / 4]))
            goto voice_done;
        if (data[0x70 / 4] != -1)
            goto play_voice;
    allocate_voice:
        data[0x70 / 4] = start_moby_voice(0, 4, moby);
        if (data[0x70 / 4] == -1)
            goto voice_done;
    play_voice:
        qzero(position);
        position[2] += 8.7f;
        set_voice_position(data[0x70 / 4], position);
    voice_done:
        if (moby_bytes[0x31] != 0 && random_integer_below(9) == 0) {
            red = (FUN_001160d8() + 0x30) & 0x3F;
            green = (FUN_001160d8() + 0x20) & 0x3F;
            blue = FUN_001160d8() & 0x2F;
            moby_point_position(moby, 0, position);
            position[2] += 0.5f;
            scale = random_float_between(900000.0f, 1800000.0f);
            spawn_particle(position,
                           red,
                           green,
                           blue,
                           scale,
                           scale_game_frames(FUN_001160d8() % 40 + 40),
                           0.0f);
        }
        process_forcefield_tower_damage(
            moby, (unsigned char *)data, (float *)((char *)data + 0x20));
        return;
    }
    case TOWER_STATE_DESTRUCTION: {
        *(u128z *)velocity = 0;
        velocity[2] = D_0015ED6C * 8.0f;
        *(u128z *)position = *(u128z *)velocity;
        threshold = 1000.0f;
        switch ((int)moby_bytes[0xBC]) {
        case TOWER_DESTRUCTION_WAIT_FOR_POINT_1_EFFECT:
            threshold = 8.5f;
            break;
        case TOWER_DESTRUCTION_WAIT_FOR_POINT_2_EFFECT:
            threshold = 17.0f;
            break;
        case TOWER_DESTRUCTION_WAIT_FOR_POINT_3_EFFECT:
            threshold = 17.0f;
            break;
        }
        if (threshold < 100.0f && is_value_within_interpolated_window(moby, threshold)) {
            moby_point_position(moby, moby_bytes[0xBC] + 1, velocity);
            emit_effect_l13(moby,
                            position,
                            velocity,
                            0.0f,
                            0.0f,
                            10,
                            3,
                            16,
                            4.0f,
                            2.0f,
                            0.0f,
                            1.0f,
                            -1,
                            20.0f,
                            1,
                            1,
                            -1,
                            0);
            moby_bytes[0xBC]++;
        }
        if ((moby_bytes[0x70] & 2) == 0)
            return;
        emit_effect_l13(moby,
                        position,
                        0,
                        0.0f,
                        0.0f,
                        10,
                        3,
                        16,
                        4.0f,
                        2.0f,
                        0.0f,
                        1.0f,
                        -1,
                        20.0f,
                        1,
                        1,
                        -1,
                        0);
        moby_point_position(moby, 3, velocity);
        fast_vec_sub(velocity, velocity, moby_bytes + 0x10);
        i = 4;
        do {
            i--;
            scale = random_float_between(0.0f, 3.0f);
            fast_vec_scale(rotated, velocity, scale);
            fast_vec_add(rotated, rotated, moby_bytes + 0x10);
            emit_forcefield_tower_destruction_effects(moby_bytes, rotated, 1);
        } while (i >= 0);
        linked_index = data[0x74 / 4];
        if (linked_index != -1)
            linked_state_flags[linked_index] = 1;
        mark_moby_for_removal(moby);
        return;
    }
    default:
        return;
    }
}

extern int random_integer_below(int) __asm__("FUN_00213260");
