/* Level 02 moby class sand_shark. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

#include "rnc/overlay/quad.h"
#include "rnc/overlay/l02/mobys/sand_shark_globals.h"

extern float sand_shark_dust_vertical_velocity_scale_gp __asm__("D_L02_001619D4")
    __attribute__((sda));
extern float sand_shark_dust_position_jitter_radius_gp __asm__("D_L02_001619DC")
    __attribute__((sda));
unsigned short D_L02_001619E0 = 0;
extern int     random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float   randf_sym(float, float) __asm__("FUN_L00_00257c48");
/* Dust puff emitter, declared in the argument order the other levels use (pos, three floats, n, vel, size, colour). */
extern unsigned char *
spawn_puff(void *pos, float r, float lo, float hi, int n, void *vel, float size, int color) __asm__("FUN_L00_0026d000");

/* Emits n dust puffs evenly spaced along the segment from a to b. */
void emit_sand_shark_dust_puffs_along_segment(void *moby,
                                              void *segment_start,
                                              void *b,
                                              int   puff_count,
                                              float sa,
                                              float sb) __asm__("FUN_L02_002d3a50");

void emit_sand_shark_dust_puffs_along_segment(

    void *moby, void *segment_start, void *b, int puff_count, float sa, float sb) {
    float          step[4];
    float          dust_puff_position[4];
    float          dust_puff_velocity[4];
    int            i;
    int            r, g, color;
    int            spin;
    float          scale;
    float          size_delta_per_puff;
    unsigned char *dust_puff;
    unsigned char *dust_puff_extension;

    fast_vec_sub(step, b, segment_start);
    fast_vec_scale(step, step, 1.0f / (float)puff_count);
    *(OvlQuad *)dust_puff_velocity = (OvlQuad)0;
    dust_puff_velocity[2] = 0.01f;
    dust_puff_velocity[3] = 1.0f;
    size_delta_per_puff = (sb - sa) / (float)puff_count;
    for (i = 0; i < puff_count; i++) {
        fast_vec_scale(dust_puff_position, step, convert_integer_to_float(i));
        fast_vec_add(dust_puff_position, segment_start, dust_puff_position);
        r = random_int_between(0x40, 0x70);
        g = random_int_between(0x40, 0x7F);
        color = g | (g << 16 | g << 8);
        scale = random_float_between(1.0f, 1.02f);
        spin = random_int_between(-2, 2);
        dust_puff_velocity[0] = randf_sym(0.0f, 0.0025f);
        dust_puff_velocity[1] = randf_sym(0.0f, 0.0025f);
        dust_puff_velocity[2] =
            random_float_between(sand_shark_dust_vertical_velocity_scale_gp * 0.1f,
                                 sand_shark_dust_vertical_velocity_scale_gp);
        dust_puff = spawn_puff(dust_puff_position,
                               sand_shark_dust_position_jitter_radius_gp,
                               1.0f,
                               scale,
                               spin,
                               dust_puff_velocity,
                               size_delta_per_puff + sa * 210000.0f,
                               r << 24 | color);
        if (dust_puff) {
            dust_puff_extension = dust_puff + 0x20;
            if (random_integer_below(2)) {
                dust_puff[3] = 0x44;
            }
            *(unsigned short *)(dust_puff + 0xA) = D_L02_001619E0;
            *(int *)(dust_puff_extension + 4) = 2;
            dust_puff_extension[0xA] = r;
            dust_puff_extension[0xB] = D_L02_001619E0;
        }
    }
}

#include "qcopy.h"

/* Transforms two table vectors by the moby's matrix and passes them on. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D5098), where it is exact; names translated to the US level program. */

extern float D_L02_001D3230[][4] NOT_SDA;
float        D_L02_001619C0[4] __attribute__((section(".sdata"))) = {-3.25f, 0.0f, 0.2f, 0.0f};

extern short sand_shark_dust_endpoint_size_scale_gp __asm__("D_L02_001619D0") __attribute__((sda));
extern void  emit_sand_shark_dust_puffs_along_segment(
     void *, void *, void *, int, float, float) __asm__("FUN_L02_002d3a50");

void emit_sand_shark_dust_segment(struct Moby *moby,
                                  int          segment_start_index,
                                  int          segment_end_index,
                                  int          puff_count) __asm__("FUN_L02_002d3c98");

void emit_sand_shark_dust_segment(struct Moby *moby,
                                  int          segment_start_index,
                                  int          segment_end_index,

                                  int puff_count) {
    float  va[4];
    float  vb[4];
    float *pa = D_L02_001D3230[segment_start_index];
    float *pb;
    qcopy(va, pa);
    va[3] = 1.0f;
    fast_vec_sub(va, va, D_L02_001619C0);
    transform_vector_by_basis(va, va, &moby->rot_row0);
    fast_vec_add(va, va, &moby->pos);
    pb = D_L02_001D3230[segment_end_index];
    qcopy(vb, pb);
    vb[3] = 1.0f;
    fast_vec_sub(vb, vb, D_L02_001619C0);
    transform_vector_by_basis(vb, vb, &moby->rot_row0);
    fast_vec_add(vb, vb, &moby->pos);
    emit_sand_shark_dust_puffs_along_segment(
        moby,
        va,
        vb,
        puff_count,
        pa[3] * *(float *)&sand_shark_dust_endpoint_size_scale_gp,
        pb[3] * *(float *)&sand_shark_dust_endpoint_size_scale_gp);
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D51D0), where it is exact; names translated to the US level program. */

extern void
emit_sand_shark_dust_segment(struct Moby *moby, int a, int b, int d) __asm__("FUN_L02_002d3c98");

int  D_L02_001619EC = 0xa;
int  D_L02_001619F0 = 0xa;
int  D_L02_001619F4 = 8;
int  D_L02_001619F8 = 8;
void emit_sand_shark_dust_burst(void *moby) __asm__("FUN_L02_002d3dd0");

void emit_sand_shark_dust_burst(void *moby) {
    emit_sand_shark_dust_segment(moby, 0, 1, D_L02_001619F0);
    emit_sand_shark_dust_segment(moby, 2, 3, D_L02_001619EC);
    emit_sand_shark_dust_segment(moby, 4, 5, D_L02_001619F4);
    emit_sand_shark_dust_segment(moby, 6, 7, D_L02_001619F4);
    emit_sand_shark_dust_segment(moby, 8, 9, D_L02_001619F8);
}

/* pvars of the Aridia enemy sand_shark_update updates (it leaves its burrow, walks, jumps and digs back in). */
typedef struct {
    /* 0x000 */ char          pad0[0x20];
    /* 0x020 */ float         unk20;
    /* 0x024 */ short         unk24;
    /* 0x026 */ char          pad26[3];
    /* 0x029 */ unsigned char unk29;
    /* 0x02A */ char          pad2A[4];
    /* 0x02E */ unsigned char unk2E;
    /* 0x02F */ char          pad2F[0x11];
    /* 0x040 */ OvlVec4       vel;
    /* 0x050 */ char          pad50[8];
    /* 0x058 */ unsigned char unk58;
    /* 0x059 */ char          pad59[3];
    /* 0x05C */ unsigned char unk5C;
    /* 0x05D */ char          pad5D[0x6B];
    /* 0x0C8 */ short         unkC8;
    /* 0x0CA */ char          padCA[6];
    /* 0x0D0 */ void         *unkD0;
    /* 0x0D4 */ char          padD4[0x4C];
    /* 0x120 */ char          unk120[0x60];
    /* 0x180 */ int           walk_mode;
    /* 0x184 */ float         walk_184;
    /* 0x188 */ float         walk_188;
    /* 0x18C */ float         walk_18C;
    /* 0x190 */ char          pad190[8];
    /* 0x198 */ float         fall_speed;
    /* 0x19C */ char          pad19C[8];
    /* 0x1A4 */ float         walk_step;
    /* 0x1A8 */ char          pad1A8[0x28];
    /* 0x1D0 */ OvlVec4       target;
    /* 0x1E0 */ char          pad1E0[0x34];
    /* 0x214 */ int           unk214;
    /* 0x218 */ char          pad218[8];
    /* 0x220 */ OvlVec4       home;
    /* 0x230 */ OvlVec4       unk230;
    /* 0x240 */ float         turn_vel;
    /* 0x244 */ float         turn_offset;
    /* 0x248 */ float         turn_range;
    /* 0x24C */ float         unk24C;
    /* 0x250 */ char          pad250[4];
    /* 0x254 */ float         unk254;
    /* 0x258 */ int           unk258;
    /* 0x25C */ short         unk25C;
    /* 0x25E */ short         unk25E;
    /* 0x260 */ int           turn_timer;
    /* 0x264 */ char          pad264[8];
    /* 0x26C */ int           unk26C;
    /* 0x270 */ int           unk270;
    /* 0x274 */ int           unk274;
    /* 0x278 */ int           unk278;
    /* 0x27C */ int           unk27C;
    /* 0x280 */ char          pad280[4];
    /* 0x284 */ int           unk284;
    /* 0x288 */ int           unk288;
    /* 0x28C */ int           unk28C;
    /* 0x290 */ int           path;
    /* 0x294 */ float         unk294;
    /* 0x298 */ int           unk298;
    /* 0x29C */ int           unk29C;
    /* 0x2A0 */ char          unk2A0[0x10];
} Varsd3e50;

extern float        D_0015ED6C;
extern struct Moby *D_L02_0015FFD8_macro __asm__("D_L02_0015FFD8") MACRO_ADDR;
extern int          D_L02_0015F5CC_e50 __asm__("D_L02_0015F5CC");
extern int          camera_secondary_mode __asm__("D_001413D4") NOT_SDA;
extern int         *D_L02_001B0AB0_e50[] __asm__("D_L02_001B0AB0");
extern char         D_L02_001673C0_e50[] __asm__("D_L02_001673C0");
extern char         sand_shark_missing_path_message_format[] __asm__("D_L02_001FA8A0");
extern char         sand_shark_empty_path_message_format[] __asm__("D_L02_001FA8C0");
extern void         FUN_001e93b0_e50(char *, short) __asm__("FUN_001e93b0");
extern int          rand_e50(void) __asm__("FUN_001160d8");
extern float        probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float        approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern int          is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int          is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");
extern int          allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void         FUN_L00_00257470_e50(void *, int, int) __asm__("FUN_L00_00257470");
extern void         random_vector(void *, float, float) __asm__("FUN_L00_00257d78");
extern void
FUN_L00_00258278_c(void *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern void FUN_L00_00258b50(void *, void *, void *, void *, float);
extern void play_entity_sound(void *, void *, float, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void FUN_L00_0025a120_e50(void *) __asm__("FUN_L00_0025a120");
extern void
           turn_angle_e50(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern int step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern void FUN_L00_00263ac8_e50(void *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void
FUN_L00_00269958_e50(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern int  FUN_L00_002dc9f0_e50(void *, void *) __asm__("FUN_L00_002dc9f0");
extern void FUN_L01_0026d930_e50(void *) __asm__("FUN_L01_0026d930");
extern void emit_sand_shark_dust_burst(void *) __asm__("FUN_L02_002d3dd0");
extern int  select_group_class_29c_moby(void *) __asm__("FUN_L02_002d5658");
extern void emit_sand_shark_radial_particles(void *, int) __asm__("FUN_L02_002d57f8");
extern void FUN_L02_002d5a68_e50(void *) __asm__("FUN_L02_002d5a68");
extern void FUN_L02_002d5c88_e50(void *) __asm__("FUN_L02_002d5c88");
extern void FUN_L02_002d63c0_e50(void *) __asm__("FUN_L02_002d63c0");

int           D_L02_001619A8 = 0;
unsigned char D_L02_00161900 = 0xa;

#include "rnc/overlay/l02/mobys/sand_shark_data/sand_shark_update.h"

int D_L02_001619B0 = 0xa000000;
int D_L02_001619E4 = 0x40306058;
int D_L02_001619E8 = 0x306058;
/* Aridia burrowing enemy: profiled with EE timer 0 around its state machine. */
void sand_shark_update(struct Moby *moby) __asm__("FUN_L02_002d3e50");

void sand_shark_update(struct Moby *moby) {
    enum {
        SAND_SHARK_INITIALIZE = 0,
        SAND_SHARK_BEGIN_RETURN_HOME_WITH_HEIGHT_OFFSET = 1,
        SAND_SHARK_WANDER_AROUND_HOME = 2,
        SAND_SHARK_WAIT_FOR_TARGET_IN_VOLUME = 3,
        SAND_SHARK_APPROACH_TARGET = 5,
        SAND_SHARK_CLOSE_RANGE_ATTACK = 6,
        SAND_SHARK_RETURN_HOME = 7,
        SAND_SHARK_KNOCKBACK_UNTIL_LANDING = 8,
        SAND_SHARK_SUCK_CANNON_INTERACTION = 9,
        SAND_SHARK_WAIT_FOR_HERO_LAUNCH_TRIGGER = 12,
        SAND_SHARK_BALLISTIC_FLIGHT = 13,
        SAND_SHARK_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK = 14,
        SAND_SHARK_FACE_TARGET = 15,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_HEIGHT_OFFSET = 17,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME_WITH_HEIGHT_OFFSET = 18,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET = 19,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME = 20,
        SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK = 21,
        SAND_SHARK_APPROACH_TARGET_WITH_HEIGHT_OFFSET = 22,
        SAND_SHARK_RETURN_HOME_WITH_HEIGHT_OFFSET = 23,
        SAND_SHARK_DEATH_KNOCKBACK = 24,
        SAND_SHARK_FADE_OUT = 25,
        SAND_SHARK_INACTIVE_WAIT_FOR_RELAUNCH = 26,
        SAND_SHARK_BALLISTIC_FLIGHT_WITH_PARTICLES = 27,
        SAND_SHARK_WAIT_FOR_LINKED_MOBY_BEFORE_INITIALIZE = 29
    };
    Varsd3e50 *vars;
    OvlVec4    position_or_offset_scratch;
    OvlVec4    target_or_particle_vector;
    OvlVec4    target_home_delta;
    OvlVec4    navigation_point;
    OvlVec4    particle_position_or_forward_step;
    float      f;

    if (D_L02_001619A8 != 0) {
        return;
    }
    *(volatile unsigned int *)0x10000010 = 0x81;
    *(volatile unsigned int *)0x10000000 = 0;
    vars = (Varsd3e50 *)moby->pvars;
    FUN_L00_00263ac8_e50(moby, 0, vars->unk2A0, 2.1f);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L02_001673C0_e50) < 28.0f) {
        FUN_L00_0025a120_e50(moby);
        moby->shadow_range = 0x16;
    }
    FUN_L02_002d5c88_e50(moby);
    *(volatile unsigned int *)0x10000000;
    switch (moby->state) {
    case SAND_SHARK_INITIALIZE:
        if (vars->path == -1) {
            FUN_001e93b0_e50(sand_shark_missing_path_message_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (*D_L02_001B0AB0_e50[vars->path] == 0) {
            FUN_001e93b0_e50(sand_shark_empty_path_message_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (vars->unk288 >= 0) {
            extern struct Moby *D_L02_0015FFD8_gp __asm__("D_L02_0015FFD8") __attribute__((sda));
            (*(int *)(D_L02_0015FFD8_gp[vars->unk288].pvars + 0x14C))++;
        }
        qcopy(&vars->home, &moby->pos);
        qcopy(&vars->unk230, &moby->pos);
        vars->unk24 = 1;
        vars->unk20 = 1.0f;
        vars->unk58 = D_L02_00161900;
        vars->unk5C = 1;
        vars->unk2E = 1;
        vars->unk29 = 1;
        vars->unk25E = 0;
        FUN_L01_0026d930_e50(&vars->walk_mode);
        vars->walk_188 = vars->walk_18C = 2.0f;
        vars->walk_mode = 0x200;
        vars->walk_184 = 0.5f;
        vars->walk_step = vars->unk254 * D_0015ED6C;
        if (rand_e50() & 1) {
            moby->flags |= 0x8000;
        }
        vars->unkD0 = &D_L02_001619B0;
        if (vars->unk26C >= 0) {
            if (vars->unk28C == -1) {
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = 0xC;
            moby->collision = 0;
            moby->pos.z += 10.0f;
            moby->flags = (moby->flags | 1) & ~0x1000;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(3));
            }
        } else if (vars->unk278 >= 0) {
            vars->walk_18C = 0.2f;
            moby->state = 3;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(3));
            }
        } else if (vars->unk258 >= 0) {
            moby->state = 0x1D;
            moby->collision = 0;
            moby->flags = (moby->flags | 0x41) & ~0x1000;
        } else {
            moby->state = 1;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(3));
            }
        }
        break;
    case SAND_SHARK_WAIT_FOR_LINKED_MOBY_BEFORE_INITIALIZE:
        if (((struct Moby *)((char *)D_L02_0015FFD8_macro + (vars->unk258 << 8)))->state >= 5 &&
            camera_secondary_mode != 0x72) {
            moby->flags = (moby->flags & 0xFFBE) | 0x1000;
            moby->collision = moby->pclass->collision;
            vars->unk258 = -1;
            vars->unk25C = scale_game_frames(0x1518);
            moby->state = 0;
            moby->pos.z -= sand_shark_height_offset_gp;
        }
        break;
    case SAND_SHARK_BEGIN_RETURN_HOME_WITH_HEIGHT_OFFSET:
        fast_vec_sub(&position_or_offset_scratch, &hero.motion.pos, &moby->pos);
        moby->state = 0x12;
        if (moby->prev_seq != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(3));
        }
        break;
    case SAND_SHARK_WANDER_AROUND_HOME:
        moby->pos.z += sand_shark_height_offset_gp;
        qcopy(&position_or_offset_scratch, &vars->home);
        FUN_L00_00258278_c(
            moby,
            fast_add_rotations(fast_arc_tan(position_or_offset_scratch.f[0] - moby->pos.x,
                                            position_or_offset_scratch.f[1] - moby->pos.y),
                               vars->turn_offset),
            &vars->turn_vel,
            0.05f,
            0.3f,
            0.04f);
        moby->pos.x += fast_cos(moby->rot.z) * D_0015ED6C;
        moby->pos.y += fast_sin(moby->rot.z) * D_0015ED6C;
        if (fast_dec_timer(&vars->turn_timer)) {
            vars->turn_timer = scale_game_frames(0x3C);
            f = vars->turn_range * 0.017453292f;
            vars->turn_offset = random_float_between(-f, f);
        }
        if (moby->visible != 0 && (random_integer_below(0xFF) & 1)) {
            target_or_particle_vector.q = *(OvlQuad *)&moby->pos;
            FUN_L02_002d5a68_e50(&target_or_particle_vector);
        }
        moby->pos.z -= sand_shark_height_offset_gp;
        if (vars->unk214 != 2) {
            if (vars->unk274 >= 0) {
                if ((!is_point_inside_clip_volume(&vars->target, vars->unk274) ||
                     vars->unk25E != 0) &&
                    random_integer_below(9) == 0) {
                    vars->unk274 = -1;
                }
            } else if (random_integer_below(4) == 0) {
                vars->turn_offset = vars->turn_range * 0.017453292f;
                if (vars->unk284 != 0) {
                    vars->turn_offset = vars->turn_offset * (float)vars->unk284;
                } else if (random_integer_below(0x100) & 1) {
                    vars->turn_offset = -vars->turn_offset;
                }
                vars->turn_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
                vars->walk_step = D_0015ED6C * 4.0f;
                moby->state = 0x11;
                if (moby->prev_seq != 0xC) {
                    blend_moby_animation(moby, 0xC, 0, scale_game_frames(3));
                }
            }
        } else if (vars->unk25E != 0 && random_integer_below(0x13) == 0) {
            moby->state = 0x15;
            allocate_voice_for_target_entry(4, 0, moby);
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            vars->unk274 = -1;
        }
        break;
    case SAND_SHARK_WAIT_FOR_TARGET_IN_VOLUME:
        if (vars->unk214 != 2 && is_point_inside_clip_volume(&vars->target, vars->unk278)) {
            moby->state = 5;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(3));
            }
        }
        break;
    case SAND_SHARK_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK: {
        int hit14;
        qcopy(&target_or_particle_vector, &vars->target);
        hit14 = 0;
        FUN_L00_00258278_c(moby,
                           fast_arc_tan(target_or_particle_vector.f[0] - moby->pos.x,
                                        target_or_particle_vector.f[1] - moby->pos.y) +
                               vars->turn_offset,
                           &vars->turn_vel,
                           0.05f,
                           0.3f,
                           0.2f);
        position_or_offset_scratch.f[0] = 2.0f * fast_cos(moby->rot.z);
        position_or_offset_scratch.f[1] = 2.0f * fast_sin(moby->rot.z);
        position_or_offset_scratch.f[2] = 0.0f;
        fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
        FUN_L00_00258b50(
            moby, &vars->walk_mode, &position_or_offset_scratch, &target_home_delta, 1.0f);
        FUN_L02_002d63c0_e50(moby);
        if (FUN_L00_00261b48(
                vars->path, &moby->pos, &target_or_particle_vector, &navigation_point) &&
            fast_vec_dist(&moby->pos, &navigation_point) < vars->unk294) {
            hit14 = 1;
        }
        if (vars->unk25E != 0) {
            if (hit14) {
                moby->state = 0xF;
                if (moby->prev_seq != 5) {
                    blend_moby_animation(moby, 5, 0, scale_game_frames(0xA));
                }
            } else if (vector_distance_xy(&moby->pos, &target_or_particle_vector) < 1.5f) {
                moby->state = 6;
                if (moby->prev_seq != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(0xA));
                }
            } else if (vars->unk214 != 2) {
                moby->state = 5;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(0xF));
                }
            }
        } else {
            moby->state = 0x12;
        }
        break;
    }
    case SAND_SHARK_FACE_TARGET:
        turn_angle_e50(
            &moby->rot.z,
            fast_arc_tan(vars->target.f[0] - moby->pos.x, vars->target.f[1] - moby->pos.y),
            &vars->turn_vel,
            frame_time_sq * 6.2831855f,
            frame_time_sq * 6.2831855f,
            D_0015ED6C * 6.2831855f);
        if (vars->unk25E != 0) {
            if (vector_distance_xy(&moby->pos, &vars->target) < 1.5f) {
                moby->state = 6;
                if (moby->prev_seq != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(0xA));
                }
            } else if (vars->unk214 != 2) {
                moby->state = 5;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(0xF));
                }
            }
        } else {
            moby->state = 0x12;
        }
        break;
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_HEIGHT_OFFSET:
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME_WITH_HEIGHT_OFFSET: {
        float g;
        moby->pos.z += sand_shark_height_offset_gp;
        g = probe_ground_height(&moby->pos, 0, 0.5f);
        if (g < vars->home.f[2]) {
            g = vars->home.f[2];
        }
        qcopy(&position_or_offset_scratch, &moby->pos);
        position_or_offset_scratch.f[2] = g;
        approach_value(&moby->pos.z, g, D_0015ED6C * 4.0f);
        if (g == moby->pos.z) {
            if (moby->prev_seq != 0xC) {
                blend_moby_animation(moby, 0xC, 0, scale_game_frames(5));
            }
            if (moby->state == 0x11) {
                moby->state = 0x16;
            } else {
                moby->state = 0x17;
            }
        }
        moby->pos.z -= sand_shark_height_offset_gp;
        if (scale_game_frames(0xA) < D_L02_0015F5CC_e50) {
            emit_sand_shark_radial_particles(&position_or_offset_scratch, 6);
        }
        break;
    }
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET:
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_RETURN_HOME:
    case SAND_SHARK_ALIGN_TO_GROUND_BEFORE_APPROACH_TARGET_WITH_PATH_PROXIMITY_CHECK: {
        float g;
        moby->pos.z += sand_shark_height_offset_gp;
        g = probe_ground_height(&moby->pos, 0, 0.5f);
        qcopy(&position_or_offset_scratch, &moby->pos);
        position_or_offset_scratch.f[2] = g;
        if (approach_value(&moby->pos.z, g + sand_shark_height_offset_gp, D_0015ED6C * 4.0f) ==
            0.0f) {
            if (moby->state == 0x13) {
                moby->state = 5;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(3));
                }
            } else if (moby->state == 0x14) {
                moby->state = 7;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(3));
                }
            } else {
                vars->unk294 = random_float_between(2.0f, 3.5f);
                vars->walk_step = D_0015ED6C * 4.0f;
                moby->state = 0xE;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(3));
                }
            }
        }
        moby->pos.z -= sand_shark_height_offset_gp;
        emit_sand_shark_radial_particles(&position_or_offset_scratch, 6);
        break;
    }
    case SAND_SHARK_APPROACH_TARGET:
    case SAND_SHARK_APPROACH_TARGET_WITH_HEIGHT_OFFSET:
        if (moby->state == 0x16) {
            moby->pos.z += sand_shark_height_offset_gp;
            if (random_integer_below(0xFF) & 1) {
                particle_position_or_forward_step.q = *(OvlQuad *)&moby->pos;
                FUN_L02_002d5a68_e50(&particle_position_or_forward_step);
            }
        }
        qcopy(&navigation_point, &vars->target);
        FUN_L00_00258278_c(
            moby,
            fast_arc_tan(navigation_point.f[0] - moby->pos.x, navigation_point.f[1] - moby->pos.y) +
                vars->turn_offset,
            &vars->turn_vel,
            0.05f,
            0.3f,
            0.2f);
        position_or_offset_scratch.f[0] = 2.0f * fast_cos(moby->rot.z);
        position_or_offset_scratch.f[1] = 2.0f * fast_sin(moby->rot.z);
        position_or_offset_scratch.f[2] = 0.0f;
        fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
        FUN_L00_00258b50(
            moby, &vars->walk_mode, &position_or_offset_scratch, &target_or_particle_vector, 1.0f);
        FUN_L02_002d63c0_e50(moby);
        if (moby->state == 0x16) {
            moby->pos.z -= sand_shark_height_offset_gp;
        }
        if (vars->unk214 != 2) {
            if (vars->unk278 >= 0 && !is_point_inside_clip_volume(&vars->target, vars->unk278)) {
                moby->state = 7;
            } else {
                fast_vec_sub(&target_home_delta, &vars->target, &vars->home);
                if (vector_distance_xy(&moby->pos, &navigation_point) < 12.0f &&
                    moby->state == 0x16) {
                    moby->state = 0x13;
                    allocate_voice_for_target_entry(4, 0, moby);
                    if (moby->prev_seq != 0) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(3));
                    }
                } else if (vector_distance_xy(&moby->pos, &navigation_point) < 1.5f &&
                           moby->state == 5) {
                    moby->state = 6;
                    if (moby->prev_seq != 1) {
                        blend_moby_animation(moby, 1, 0, scale_game_frames(3));
                    }
                }
            }
        } else if (vars->unk278 >= 0) {
            moby->state = 7;
        } else if (vars->unk25E != 0) {
            moby->state = 0xE;
        } else {
            moby->state = 0x12;
        }
        break;
    case SAND_SHARK_CLOSE_RANGE_ATTACK:
        FUN_L00_00258278_c(
            moby,
            fast_arc_tan(vars->target.f[0] - moby->pos.x, vars->target.f[1] - moby->pos.y),
            &vars->turn_vel,
            0.05f,
            0.3f,
            0.2f);
        if (moby->seq == moby->prev_seq && is_value_within_interpolated_window(moby, 13.0f)) {
            fast_vec_normalize(&position_or_offset_scratch, &moby->rot_row0, 0.75f);
            fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
            position_or_offset_scratch.f[2] += 0.5f;
            play_entity_sound(moby, &position_or_offset_scratch, 0.5f, 1, 1.0f, 1.0f, 0, 1, 0);
        }
        if ((moby->anim_flags & 2) && vector_distance_xy(&moby->pos, &vars->target) > 1.5f) {
            moby->state = 5;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(3));
            }
        }
        {
            float g = probe_ground_height(&moby->pos, 0, 0.5f);
            vars->fall_speed -= frame_time_sq * 9.8f;
            moby->pos.z += vars->fall_speed;
            if (moby->pos.z < g) {
                vars->fall_speed = 0.0f;
                moby->pos.z = g;
            }
        }
        break;
    case SAND_SHARK_RETURN_HOME:
    case SAND_SHARK_RETURN_HOME_WITH_HEIGHT_OFFSET:
        if (moby->state == 0x17) {
            moby->pos.z += sand_shark_height_offset_gp;
            if (random_integer_below(0xFF) & 1) {
                particle_position_or_forward_step.q = *(OvlQuad *)&moby->pos;
                FUN_L02_002d5a68_e50(&particle_position_or_forward_step);
            }
        }
        qcopy(&navigation_point, &vars->home);
        FUN_L00_00258278_c(
            moby,
            fast_arc_tan(navigation_point.f[0] - moby->pos.x, navigation_point.f[1] - moby->pos.y),
            &vars->turn_vel,
            0.02f,
            0.3f,
            0.1f);
        position_or_offset_scratch.f[0] = 2.0f * fast_cos(moby->rot.z);
        position_or_offset_scratch.f[1] = 2.0f * fast_sin(moby->rot.z);
        position_or_offset_scratch.f[2] = 0.0f;
        fast_vec_add(&position_or_offset_scratch, &position_or_offset_scratch, &moby->pos);
        FUN_L00_00258b50(
            moby, &vars->walk_mode, &position_or_offset_scratch, &target_or_particle_vector, 1.0f);
        if (moby->state == 0x17) {
            moby->pos.z -= sand_shark_height_offset_gp;
        }
        if (vector_distance_xy(&moby->pos, &vars->home) < 0.5f) {
            vars->unk298 = 0;
            if (moby->state == 7) {
                if (vars->unk278 >= 0) {
                    moby->state = 3;
                    if (moby->prev_seq != 2) {
                        blend_moby_animation(moby, 2, 0, scale_game_frames(3));
                    }
                } else {
                    moby->state = 0x12;
                    vars->walk_step = D_0015ED6C * 1.5f;
                    if (moby->prev_seq != 0) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(3));
                    }
                }
            } else {
                moby->state = 2;
                vars->walk_step = D_0015ED6C * 1.5f;
                if (moby->prev_seq != 0xC) {
                    blend_moby_animation(moby, 0xC, 0, scale_game_frames(5));
                }
            }
        }
        if (vars->unk298 == 0) {
            if (vars->unk214 != 2) {
                fast_vec_sub(&target_home_delta, &vars->target, &vars->home);
                if (vars->unk278 >= 0) {
                    if (is_point_inside_clip_volume(&vars->target, vars->unk278)) {
                        moby->state = 5;
                        vars->walk_step = vars->unk254 * D_0015ED6C;
                    }
                } else if (fast_vec_length(&target_home_delta) < vars->unk24C) {
                    if (moby->state == 7) {
                        moby->state = 5;
                    } else {
                        moby->state = 0x16;
                    }
                    vars->walk_step = vars->unk254 * D_0015ED6C;
                    if (moby->prev_seq != 0) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(3));
                    }
                }
            } else if (vars->unk25E != 0 && random_integer_below(0x13) == 0) {
                moby->state = 0x15;
                allocate_voice_for_target_entry(4, 0, moby);
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, 0);
                }
                vars->unk274 = -1;
            }
        } else {
            extern float frame_time __asm__("D_0015ED6C") __attribute__((sda));
            fast_vec_normalize(
                &particle_position_or_forward_step, &moby->rot_row0, frame_time * 0.5f);
            fast_vec_add(&moby->pos, &moby->pos, &particle_position_or_forward_step);
        }
        break;
    case SAND_SHARK_KNOCKBACK_UNTIL_LANDING:
        if (step_moby_motion(moby, vars->unk120) & 1) {
            moby->state = 0xB;
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 8, scale_game_frames(3));
            }
            return;
        }
        if (moby->pos.z < 0.0f) {
            emit_sand_shark_dust_burst(moby);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case SAND_SHARK_SUCK_CANNON_INTERACTION:
        if (FUN_L00_002dc9f0_e50(moby, vars->unk120)) {
            moby->state = 1;
            vars->unkC8 = 0;
        }
        break;
    case SAND_SHARK_WAIT_FOR_HERO_LAUNCH_TRIGGER: {
        int in;
        if (is_point_inside_clip_volume(&hero.motion.pos, vars->unk26C)) {
            in = 1;
        } else {
            in = is_point_inside_clip_volume(&hero.motion.pos, vars->unk270) != 0;
        }
        if (in) {
            struct Moby *dest = &D_L02_0015FFD8_macro[vars->unk28C];
            float        dz = dest->pos.z - vars->home.f[2];
            float        up = random_float_between(6.0f, 10.0f) * D_0015ED6C;
            float flight_duration = (square_root(2.0f * (frame_time_sq * 10.8f) * dz) + 2.0f * up) /
                                    (frame_time_sq * 10.8f);
            float speed = vector_distance_xy(&vars->home, &dest->pos) / flight_duration;

            fast_vec_sub(&vars->vel, &vars->home, &dest->pos);
            vars->vel.f[2] = 0.0f;
            fast_vec_normalize(&vars->vel, &vars->vel, speed);
            vars->vel.f[2] = up;
            qcopy(&moby->pos, &dest->pos);
            moby->collision = moby->pclass->collision;
            moby->flags = (moby->flags & 0xFFFE) | 0x1000;
            if (moby->prev_seq != 0xB) {
                blend_moby_animation(moby, 0xB, 0, 0);
            }
            moby->state = 0xD;
        }
        break;
    }
    case SAND_SHARK_BALLISTIC_FLIGHT_WITH_PARTICLES: {
        int life;

        random_vector(&target_or_particle_vector, 0.0f, 0.2f);
        fast_vec_add(&position_or_offset_scratch, &target_or_particle_vector, &moby->pos);
        fast_vec_scale(&target_or_particle_vector, &target_or_particle_vector, D_0015ED6C * 5.0f);
        life = scale_game_frames(random_int_between(0x14, 0x28));
        FUN_L00_00269958_e50(&moby->pos,
                             &target_or_particle_vector,
                             D_L02_001619E4,
                             D_L02_001619E8,
                             life,
                             0x64,
                             random_int_between(0x78, 0x96),
                             0);
    }
        /* fallthrough */
    case SAND_SHARK_BALLISTIC_FLIGHT: {
        float sink;
        moby->rot.y = fast_subtract_rotations(moby->rot.y, D_0015ED6C * 12.566371f);
        fast_vec_add(&moby->pos, &moby->pos, &vars->vel);
        sink = sand_shark_height_offset_gp;
        vars->vel.f[2] -= frame_time_sq * 10.8f;
        if (moby->pos.z < vars->home.f[2] - sink) {
            moby->collision = moby->pclass->collision;
            moby->flags |= 0x1000;
            moby->pos.z = vars->home.f[2] - sink;
            moby->state = 0x17;
            moby->rot.y = 0.0f;
            if (moby->prev_seq != 0xC) {
                blend_moby_animation(moby, 0xC, 0, scale_game_frames(1));
            }
        } else if (moby->pos.z < vars->home.f[2] + 0.6f) {
            position_or_offset_scratch.q = *(OvlQuad *)&moby->pos;
            emit_sand_shark_radial_particles(&position_or_offset_scratch, 6);
        }
        break;
    }
    case SAND_SHARK_DEATH_KNOCKBACK: {
        int knockback_status_flags = step_moby_motion(moby, vars->unk120);
        if ((knockback_status_flags & 1) && vars->unk29C == 0) {
            vars->unk29C = 1;
            position_or_offset_scratch.q = *(OvlQuad *)&moby->pos;
            emit_sand_shark_radial_particles(&position_or_offset_scratch, 0x1B);
        }
        if (knockback_status_flags & 0x140) {
            FUN_L00_00257470_e50(moby, 0, -1);
            vars->unk27C = select_group_class_29c_moby(moby);
            if (vars->unk27C != 0) {
                emit_sand_shark_dust_burst(moby);
                moby->state = 0x1A;
                moby->anim_speed = 0.0f;
                qcopy(&moby->pos, &vars->home);
                moby->collision = 0;
                moby->flags |= 0x41;
                moby->pos.z -= 10.0f;
            } else {
                emit_sand_shark_dust_burst(moby);
                mark_moby_for_removal(moby);
                return;
            }
        } else if (moby->pos.z < 0.0f) {
            emit_sand_shark_dust_burst(moby);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case SAND_SHARK_FADE_OUT:
        if (moby->alpha >= 8) {
            moby->alpha -= 8;
        } else {
            vars->unk27C = select_group_class_29c_moby(moby);
            if (vars->unk27C != 0) {
                emit_sand_shark_dust_burst(moby);
                moby->alpha = 0x80;
                moby->state = 0x1A;
                qcopy(&moby->pos, &vars->home);
                moby->collision = 0;
                moby->pos.z -= 10.0f;
            } else {
                emit_sand_shark_dust_burst(moby);
                mark_moby_for_removal(moby);
                return;
            }
        }
        break;
    case SAND_SHARK_INACTIVE_WAIT_FOR_RELAUNCH:
        vars->unk27C = select_group_class_29c_moby(moby);
        if (vars->unk27C == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    *(volatile unsigned int *)0x10000000;
}

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D6A90), where it is exact; names translated to the US level program. */

extern int D_L02_0015FFD8_m __asm__("D_L02_0015FFD8");
extern int D_L02_001ABE40[];

int select_group_class_29c_moby(void *argp) {
    char  *moby = argp;
    int    result = 0;
    short *group_ids;
    if (((unsigned char *)moby)[0x21] == 0xFF) {
        return 0;
    }
    group_ids = (short *)D_L02_001ABE40[((unsigned char *)moby)[0x21]];
    do {
        int   group_moby_byte_offset = (*(unsigned short *)group_ids & 0x7FFF) << 8;
        char *group_moby = (char *)(group_moby_byte_offset + D_L02_0015FFD8_m);
        if (MOBY(group_moby)->oclass == 0x29C && ((unsigned char *)group_moby)[0x20] < 0x7F) {
            if (result == 0 || (random_integer_below(0xFF) & 1)) {
                result = D_L02_0015FFD8_m + group_moby_byte_offset;
            }
        }
    } while (*group_ids++ >= 0);
    return result;
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D6B60), where it is exact; names translated to the US level program. */

extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   D_L02_0015FFD8; /* no foreign declaration */

void extend_group_sand_shark_timers(char *moby) __asm__("FUN_L02_002d5728");

void extend_group_sand_shark_timers(char *moby) {
    short *group_ids;
    if (((unsigned char *)moby)[0x21] != 0xFF) {
        group_ids = (short *)D_L02_001ABE40[((unsigned char *)moby)[0x21]];
        do {
            char *group_moby =
                (char *)D_L02_0015FFD8 + ((*(unsigned short *)group_ids & 0x7FFF) << 8);
            if (MOBY(group_moby)->oclass == 0x244) {
                char *group_moby_vars = ((char *)MOBY(group_moby)->pvars);
                float proposed_timer_duration = scale_time(random_float_between(240.0f, 300.0f));
                if ((float)*(short *)(group_moby_vars + 0x25E) < proposed_timer_duration) {
                    *(short *)(group_moby_vars + 0x25E) =
                        truncate_float_to_s32(proposed_timer_duration);
                }
            }
        } while (*group_ids++ >= 0);
    }
}

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

#include "rnc/overlay/l02/mobys/sand_shark_data/emit_sand_shark_radial_particles.h"

extern float sand_shark_vertical_motion_z_range_start_gp __asm__("D_L02_00161920")
    __attribute__((sda));
extern float sand_shark_vertical_motion_z_range_end_gp __asm__("D_L02_00161924")
    __attribute__((sda));
extern float sand_shark_radial_motion_z_range_start_gp __asm__("D_L02_00161928")
    __attribute__((sda));
extern float sand_shark_radial_motion_z_range_end_gp __asm__("D_L02_0016192C") __attribute__((sda));
extern float sand_shark_radial_motion_xy_range_start_gp __asm__("D_L02_00161930")
    __attribute__((sda));
extern float sand_shark_radial_motion_xy_range_end_gp __asm__("D_L02_00161934")
    __attribute__((sda));
extern float sand_shark_vertical_motion_w_gp __asm__("D_L02_00161938") __attribute__((sda));
extern float sand_shark_radial_motion_w_gp __asm__("D_L02_0016193C") __attribute__((sda));
extern int   sand_shark_particle_frame_count_range_start_gp __asm__("D_L02_00161940")
    __attribute__((sda));
extern int sand_shark_particle_frame_count_range_end_gp __asm__("D_L02_00161944")
    __attribute__((sda));
extern int sand_shark_particle_second_frame_count_range_start_gp __asm__("D_L02_00161948")
    __attribute__((sda));
extern int sand_shark_particle_second_frame_count_range_end_gp __asm__("D_L02_0016194C")
    __attribute__((sda));
extern int sand_shark_particle_third_frame_count_range_start_gp __asm__("D_L02_00161950")
    __attribute__((sda));
extern int sand_shark_particle_third_frame_count_range_end_gp __asm__("D_L02_00161954")
    __attribute__((sda));
int D_L02_00161958 = 0x60306058;
int D_L02_0016195C = 0x20306058;
int D_L02_00161960 = -1;

/* Same routine as FUN_L12_002e19a8: spawns n particles around pos, fewer when D_L02_00160178 is high. */
void emit_sand_shark_radial_particles(void *particle_origin, int particle_count) {
    float  saved_source_position[4];
    float  particle_position[4];
    float  vertical_motion[4];
    float  radial_motion[4];
    float  radial_angle;
    float *source_position;
    int    i;
    int    frame_count, second_frame_count, third_frame_count;
    source_position = saved_source_position;
    *(OvlQuad *)source_position = *(OvlQuad *)particle_origin;
    if (sand_shark_particle_emission_throttle_input > 0x5DC) {
        particle_count = particle_count / 4;
    } else if (sand_shark_particle_emission_throttle_input > 0x400) {
        particle_count = particle_count / 2;
    }
    for (i = 0; i < particle_count; i++) {
        radial_angle = random_angle_radians();
        qcopy(particle_position, source_position);
        particle_position[2] = particle_position[2] + random_float_between(-0.25f, -0.75f);
        particle_position[0] = particle_position[0] + random_float_between(-0.25f, 0.25f);
        particle_position[1] = particle_position[1] + random_float_between(-0.25f, 0.25f);
        clear_u64_value(vertical_motion);
        radial_motion[0] = fast_cos(radial_angle) *
                           (random_float_between(sand_shark_radial_motion_xy_range_start_gp,
                                                 sand_shark_radial_motion_xy_range_end_gp) *
                            D_0015ED6C);
        radial_motion[1] = fast_sin(radial_angle) *
                           (random_float_between(sand_shark_radial_motion_xy_range_start_gp,
                                                 sand_shark_radial_motion_xy_range_end_gp) *
                            D_0015ED6C);
        *(int *)&radial_motion[2] = 0;
        vertical_motion[2] = random_float_between(sand_shark_vertical_motion_z_range_start_gp,
                                                  sand_shark_vertical_motion_z_range_end_gp) *
                             D_0015ED6C;
        radial_motion[2] = random_float_between(sand_shark_radial_motion_z_range_start_gp,
                                                sand_shark_radial_motion_z_range_end_gp) *
                           D_0015ED6C;
        vertical_motion[3] = sand_shark_vertical_motion_w_gp;
        radial_motion[3] = sand_shark_radial_motion_w_gp;
        frame_count = truncate_float_to_s32(
            random_float_between((float)sand_shark_particle_frame_count_range_start_gp,
                                 (float)sand_shark_particle_frame_count_range_end_gp));
        second_frame_count = truncate_float_to_s32(
            random_float_between((float)sand_shark_particle_second_frame_count_range_start_gp,
                                 (float)sand_shark_particle_second_frame_count_range_end_gp));
        third_frame_count = truncate_float_to_s32(
            random_float_between((float)sand_shark_particle_third_frame_count_range_start_gp,
                                 (float)sand_shark_particle_third_frame_count_range_end_gp));
        spawn_spark(particle_position,
                    vertical_motion,
                    radial_motion,
                    D_L02_00161958,
                    D_L02_0016195C,
                    frame_count,
                    second_frame_count,
                    third_frame_count,
                    D_L02_00161960);
    }
}

/* Spawns one particle above pos unless D_L02_00160178 is already high. */
extern float sand_shark_single_particle_vertical_motion_z_range_start_gp __asm__("D_L02_00161964")
    __attribute__((sda));
extern float sand_shark_single_particle_vertical_motion_z_range_end_gp __asm__("D_L02_00161968")
    __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_z_range_start_gp __asm__("D_L02_0016196C")
    __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_z_range_end_gp __asm__("D_L02_00161970")
    __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_xy_range_start_gp __asm__("D_L02_00161974")
    __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_xy_range_end_gp __asm__("D_L02_00161978")
    __attribute__((sda));
extern float sand_shark_single_particle_vertical_motion_w_gp __asm__("D_L02_0016197C")
    __attribute__((sda));
extern float sand_shark_single_particle_radial_motion_w_gp __asm__("D_L02_00161980")
    __attribute__((sda));
extern s32 sand_shark_single_particle_frame_count_range_start_gp __asm__("D_L02_00161984")
    __attribute__((sda));
extern s32 sand_shark_single_particle_frame_count_range_end_gp __asm__("D_L02_00161988")
    __attribute__((sda));
extern s32 sand_shark_single_particle_second_frame_count_range_start_gp __asm__("D_L02_0016198C")
    __attribute__((sda));
extern s32 sand_shark_single_particle_second_frame_count_range_end_gp __asm__("D_L02_00161990")
    __attribute__((sda));
extern s32 sand_shark_single_particle_third_frame_count_range_start_gp __asm__("D_L02_00161994")
    __attribute__((sda));
extern s32 sand_shark_single_particle_third_frame_count_range_end_gp __asm__("D_L02_00161998")
    __attribute__((sda));
s32 D_L02_0016199C = 0x40306058;
s32 D_L02_001619A0 = 0x10306058;
s32 D_L02_001619A4 = -1;

void emit_sand_shark_single_particle(Vec4 *from) __asm__("FUN_L02_002d5a68");

void emit_sand_shark_single_particle(Vec4 *from) {
    Vec4  saved_source_position;
    Vec4  at;
    Vec4  vertical_motion;
    Vec4  radial_motion;
    Vec4 *source_position = &saved_source_position;
    Vec4 *particle_position;
    float radial_angle;
    int   frame_count, second_frame_count, third_frame_count;

    saved_source_position.q = from->q;
    if (sand_shark_particle_emission_throttle_input >= 0x6A5)
        return;
    radial_angle = random_angle_radians();
    particle_position = &at;
    qcopy(particle_position, source_position);
    at.f[2] = at.f[2] + random_float_between(0.125f, 0.0f);
    at.f[0] = at.f[0] + random_float_between(-0.125f, 0.125f);
    at.f[1] = at.f[1] + random_float_between(-0.125f, 0.125f);
    clear_u64_value(&vertical_motion);
    radial_motion.f[0] =
        fast_cos(radial_angle) *
        (random_float_between(sand_shark_single_particle_radial_motion_xy_range_start_gp,
                              sand_shark_single_particle_radial_motion_xy_range_end_gp) *
         D_0015ED6C);
    radial_motion.f[1] =
        fast_sin(radial_angle) *
        (random_float_between(sand_shark_single_particle_radial_motion_xy_range_start_gp,
                              sand_shark_single_particle_radial_motion_xy_range_end_gp) *
         D_0015ED6C);
    radial_motion.f[2] = 0.0f;
    vertical_motion.f[2] =
        random_float_between(sand_shark_single_particle_vertical_motion_z_range_start_gp,
                             sand_shark_single_particle_vertical_motion_z_range_end_gp) *
        D_0015ED6C;
    radial_motion.f[2] =
        random_float_between(sand_shark_single_particle_radial_motion_z_range_start_gp,
                             sand_shark_single_particle_radial_motion_z_range_end_gp) *
        D_0015ED6C;
    vertical_motion.f[3] = sand_shark_single_particle_vertical_motion_w_gp;
    radial_motion.f[3] = sand_shark_single_particle_radial_motion_w_gp;
    frame_count = truncate_float_to_s32(
        random_float_between((float)sand_shark_single_particle_frame_count_range_start_gp,
                             (float)sand_shark_single_particle_frame_count_range_end_gp));
    second_frame_count = truncate_float_to_s32(
        random_float_between((float)sand_shark_single_particle_second_frame_count_range_start_gp,
                             (float)sand_shark_single_particle_second_frame_count_range_end_gp));
    third_frame_count = truncate_float_to_s32(
        random_float_between((float)sand_shark_single_particle_third_frame_count_range_start_gp,
                             (float)sand_shark_single_particle_third_frame_count_range_end_gp));
    spawn_spark(&at,
                &vertical_motion,
                &radial_motion,
                D_L02_0016199C,
                D_L02_001619A0,
                frame_count,
                second_frame_count,
                third_frame_count,
                D_L02_001619A4);
}

/* pvars of the class FUN_L02_002d5c88 updates (only the fields it touches). */
typedef struct {
    char          pad0[0x20];
    float         hp; /* 0x20 */
    char          pad24[0xA];
    unsigned char b2E; /* 0x2E: 2 = respawn requested */
    char          pad2F[9];
    int           i38; /* 0x38 */
    char          pad3C[0xD4];
    short         anim[3]; /* 0x110: passed to trooper_anim_update / trooper_anim_finish */
    char          pad116;
    unsigned char b117; /* 0x117 */
    char          pad118[8];
    char          knock[0x10]; /* 0x120: passed to trooper_start_knockback */
    float         f130;        /* 0x130 */
    float         f134;
    float         f138;
    float         f13C;
    int           i140; /* 0x140 */
    int           i144;
    float         f148;
    int           i14C; /* 0x14C: count kept by the spawner moby */
    char          pad150[0xD];
    unsigned char b15D; /* 0x15D */
    char          pad15E[0xE];
    float         f16C; /* 0x16C */
    float         f170;
    float         f174;
    char          pad178[0x58];
    float         tgt[4]; /* 0x1D0: point followed along the path */
    char          pad1E0[0x30];
    int           i210; /* 0x210 */
    int           i214;
    char          pad218[8];
    float         home[4]; /* 0x220 */
    char          pad230[0x1C];
    float         f24C; /* 0x24C */
    float         f250;
    char          pad254[8];
    short         t25C; /* 0x25C */
    short         t25E;
    char          pad260[0x1C];
    int           i27C; /* 0x27C */
    int           i280;
    char          pad284[4];
    int           i288; /* 0x288: spawner moby index, -1 if none */
    char          pad28C[4];
    int           i290; /* 0x290: path index */
    char          pad294[8];
    int           i29C; /* 0x29C */
} Vars5c88;

#include "rnc/overlay/l02/mobys/sand_shark_data/handle_sand_shark_damage_and_targeting.h"

extern int  *D_L02_001B0AB0_5c88[] __asm__("D_L02_001B0AB0");
extern float FUN_00213508(void *, int, float);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern int   FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);
extern int   select_group_class_29c_moby(void *argp) __asm__("FUN_L02_002d5658");
extern void  extend_group_sand_shark_timers(char *arg) __asm__("FUN_L02_002d5728");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

/* Path-following enemy: respawns at its home point on request, takes hits (dies and drops to the
   ground), then follows its path and retargets the hero when it strays. */
void handle_sand_shark_damage_and_targeting(unsigned char *moby) __asm__("FUN_L02_002d5c88");

void handle_sand_shark_damage_and_targeting(unsigned char *moby) {
    Vars5c88 *moby_vars = ((Vars5c88 *)MOBY(moby)->pvars);
    char     *hit_record;
    int      *path;
    int       hit;
    float     dmg;
    float     k;
    float     ground_height;
    OvlVec4   ground_probe_position;
    OvlVec4   hit_knockback_vector;

    moby_vars->i280 = moby[0x20];
    if (moby[0x20] == 0) {
        return;
    }
    if (moby_vars->b2E == 2) {
        moby_vars->b2E = 1;
        if (moby_vars->i288 >= 0) {
            (*(Vars5c88 **)((moby_vars->i288 << 8) + D_L02_0015FFD8 + 0x78))->i14C -= 1;
        }
        if ((moby_vars->i27C = select_group_class_29c_moby(moby)) == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby[0x20] = 0x1A;
        qcopy(moby + 0x10, moby_vars->home);
        MOBY(moby)->collision = 0;
        MOBY(moby)->pos.z -= 10.0f;
    }
    if (moby_vars->i38 != 0 || (!fast_dec_timer_s16(&moby_vars->t25C) &&
                                vector_distance_xy(moby + 0x10, &hero.motion.pos) < 10.0f)) {
        moby_vars->t25E = truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
    }
    moby_vars->i38 = 0;
    if (fast_dec_timer_s16(&moby_vars->t25E)) {
        moby_vars->f250 = moby_vars->f24C;
    } else {
        moby_vars->f250 = moby_vars->f24C + 5.0f;
    }
    dmg = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    resolve_moby_hit_response(moby, hit_record, &moby_vars->hp, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && moby[0x20] != 0x18) {
        extend_group_sand_shark_timers((char *)moby);
        moby_vars->i29C = 0;
        moby_vars->hp -= dmg;
        if (moby_vars->hp <= 0.0f) {
            moby_vars->hp -= dmg;
            moby_vars->i140 = 0x200;
            moby_vars->f130 = sand_shark_death_knockback_gravity_gp * frame_time_sq;
            moby_vars->f134 =
                sand_shark_death_knockback_horizontal_speed_approach_rate_gp * frame_time_sq;
            moby_vars->f138 = sand_shark_death_knockback_horizontal_speed_gp * D_0015ED6C;
            moby_vars->f13C = sand_shark_death_knockback_vertical_speed_gp * D_0015ED6C;
            moby_vars->b15D = 0;
            moby_vars->i144 = 9;
            moby_vars->f16C = 2.0f * D_0015ED6C;
            moby_vars->f148 = 0.5f;
            qcopy(&ground_probe_position, moby + 0x10);
            ground_probe_position.f[2] += 2.0f;
            ground_height = FUN_00213508(&ground_probe_position, 0, 0.5f);
            if (MOBY(moby)->pos.z < ground_height) {
                MOBY(moby)->pos.z = ground_height;
            }
            if (moby_vars->i288 >= 0) {
                extern int D_L02_0015FFD8_s __asm__("D_L02_0015FFD8") __attribute__((sda));
                (*(Vars5c88 **)((moby_vars->i288 << 8) + D_L02_0015FFD8_s + 0x78))->i14C -= 1;
                moby_vars->b15D = 0;
            } else {
                ground_probe_position.f[2] += 0.01f;
            }
            moby_vars->i144 |= 0x20;
            MOBY(moby)->flags &= 0xEFFF;
            hit_knockback_vector.q = *(OvlQuad *)(hit_record + 0x10);
            trooper_knockback_arc(&hit_knockback_vector, &k, &moby_vars->f138, &moby_vars->f13C);
            trooper_start_knockback(k, moby, moby_vars->knock, 6, 1, 0);
            moby_vars->f148 = 0.85f;
            moby_vars->f170 = 7.0f;
            moby_vars->f174 = 14.0f;
            moby[0x20] = 0x18;
            MOBY(moby)->collision = 0;
            moby_vars->b117 = 0x78;
            trooper_anim_update(moby, moby_vars->anim);
        }
    }
    moby[0xA4] = 0xFF;
    trooper_anim_finish(moby, moby_vars->anim);
    path = D_L02_001B0AB0_5c88[moby_vars->i290];
    if (FUN_L00_0025ff38(moby_vars->f250, moby, moby_vars->tgt, 0, 0, path + 4, *path) != 2 &&
        (moby_vars->f250 < vector_distance_xy(moby_vars->home, moby_vars->tgt) ||
         absolute_float(MOBY(moby)->pos.z - moby_vars->tgt[2]) > 3.0f)) {
        moby_vars->i214 = 2;
    }
    if (moby_vars->i210 == 0) {
        moby_vars->i210 = (int)hero.moby;
        qcopy(moby_vars->tgt, &hero.motion.pos);
    }
}

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D7648), where it is exact; names translated to the US level program. */

extern char  D_L02_0015FFD8_c __asm__("D_L02_0015FFD8");
extern void *store_moby_in_suck_cannon(void *) __asm__("FUN_L00_002dbb20");

void *handle_sand_shark_suck_cannon_storage(struct Moby *moby) __asm__("FUN_L02_002d6210");

void *handle_sand_shark_suck_cannon_storage(struct Moby *moby) {
    void *helper_result = store_moby_in_suck_cannon(moby);
    if (helper_result) {
        int spawner_moby_index;
        moby->state = 9;
        spawner_moby_index = *(int *)((char *)moby->pvars + 0x288);
        if (spawner_moby_index >= 0) {
            char *spawner_vars =
                *(char **)(*(int *)&D_L02_0015FFD8_c + spawner_moby_index * 256 + 0x78);
            (*(int *)(spawner_vars + 0x14C))--;
        }
    } else if (moby->state == 9) {
        moby->state = 1;
    }
    return helper_result;
}

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D7778), where it is exact; names translated to the US level program. */

void prepare_sand_shark_for_relaunch_or_remove(struct Moby *moby) __asm__("FUN_L02_002d6340");

void prepare_sand_shark_for_relaunch_or_remove(struct Moby *moby) {
    char *sand_shark_private_vars = (char *)moby->pvars;
    int   selected_class_29c_moby = select_group_class_29c_moby(moby);
    *(int *)(sand_shark_private_vars + 0x27C) = selected_class_29c_moby;
    if (selected_class_29c_moby != 0) {
        moby->state = 0x1A;
        moby->anim_speed = 0;
        qcopy(&moby->pos, sand_shark_private_vars + 0x220);
        moby->collision = 0;
        moby->pos.z -= 10.0f;
    } else {
        emit_sand_shark_dust_burst(moby);
        mark_moby_for_removal(moby);
    }
}

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D77F8), where it is exact; names translated to the US level program. */

extern char *D_L02_001B0AB0[];
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");

void begin_sand_shark_return_home_if_outside_path(struct Moby *moby) __asm__("FUN_L02_002d63c0");

void begin_sand_shark_return_home_if_outside_path(struct Moby *moby) {
    char *sand_shark_instance_vars = (char *)moby->pvars;
    char *entry = D_L02_001B0AB0[*(int *)(sand_shark_instance_vars + 0x290)];
    if (point_in_path_xy(&moby->pos, entry + 0x10, *(int *)entry) == 0) {
        moby->state = 0x12;
        if (moby->prev_seq) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(20));
        }
        *(int *)(sand_shark_instance_vars + 0x298) = 1;
    }
}

extern float fast_cos(float);
extern float fast_sin(float);
