/* Shared moby class 0x131: a homing shot, and the function that spawns it. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/weapons/blaster_shot_data/spawn_homing_shot.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/debug/debug_menu.h"

typedef union {
    u128 q;
    f32  f[4];
} V4;

typedef struct Moby Moby;

/* Four floats aligned to 16 bytes, the layout moved by one lq/sq. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A;

typedef int w128 __attribute__((mode(TI)));

/* Vars of the homing shot FUN_L00_002caf70 spawns. */
typedef struct {
    f32   target_x, target_y, target_z;
    f32   unk0C;
    f32   yaw_rate;
    f32   pitch_rate;
    f32   rest_yaw;
    f32   rest_pitch;
    s32   ignore;
    s16   timer;
    s16   life;
    s32   bounces;
    Moby *follower;
    Moby *trail[16];
} ShotVars;

typedef struct {
    Vec4A dir;
    Moby *moby;
    s32   flags;
    u8    b18;
    u8    b19;
    s16   oclass;
    f32   radius;
    s32   i20;
} ShotQuery;

extern float D_0015ED6C MACRO_ADDR; /* frame-rate scale */
extern void             fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void             reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");

extern char *spawn_source_attached_fading_effect_particle(char *, int, int, int, float) __asm__("FUN_L00_0026daa0");
extern char  collision_hit_point[] __asm__("D_L00_00173E60");
extern float D_0015ED6C;
extern Vec4  zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;
extern float D_L00_00161890_gp __asm__("D_L00_00161890") __attribute__((sda));
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");

extern unsigned char *
spawn_random_size_rotated_particle(char *a, void *b, int c, float f) __asm__("FUN_L00_00275158");
extern void *FUN_L00_0026dd20(void *, void *, unsigned, int, float);
extern void  FUN_L00_002cbcc0_c(unsigned char *, int) __asm__("FUN_L00_002cbcc0");
extern void  add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");

extern unsigned char D_L00_00173E60[]; /* +0x20: hit normal */
extern char          D_L00_00166FF0_2cb418[] __asm__("D_L00_00166FF0");
extern char          D_0013F3D0[];
extern Moby         *D_001403E0_lui __asm__("D_001403E0") NOT_SDA;

extern float D_0015ED70_lui __asm__("D_0015ED70");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern void  FUN_L00_002cbcc0_2cb418(Moby *, s32) __asm__("FUN_L00_002cbcc0");
extern void  spawn_spark_2cb418(void *, void *, s32, s32, f32) __asm__("FUN_L00_0026dd20");
extern f32   spring_turn(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern void  scale_vector_xyz_2cb418(void *, void *, f32) __asm__("FUN_001f9a68");
extern void  fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern s32   collide_line_2cb418(void *, void *, s32, s32, void *) __asm__("FUN_001efa68");
extern s32   FUN_001f0b58_2cb418(void) __asm__("FUN_001f0b58");
extern void  add_vector_xyz_2cb418(void *, void *, void *) __asm__("FUN_001f9a10");

extern f32 homing_shot_pitch_bias[2] __asm__("D_L00_00161894");
extern f32 D_L00_00161890[2];
extern f32 jitter __asm__("D_L00_0016187C") __attribute__((sda));
extern f32 trail_speed __asm__("D_L00_00161880") __attribute__((sda));
extern f32 trail_decay __asm__("D_L00_00161884") __attribute__((sda));

extern void  free_effect_particle(void *) __asm__("FUN_L00_00267a08");
extern f32   FUN_L00_00257be8_2cbcc0(f32, f32) __asm__("FUN_L00_00257be8");
extern s32   FUN_L00_00257b50_2cbcc0(s32) __asm__("FUN_L00_00257b50");
extern s32   FUN_001fef20_2cbcc0(s32) __asm__("FUN_L00_001fef20");
extern void  FUN_001ff500_2cbcc0(void *, void *, f32) __asm__("FUN_L00_001ff500");
extern void *FUN_0026dd20_2cbcc0(void *, void *, u32, s32, f32) __asm__("FUN_L00_0026dd20");
extern u8   *spawn_puff(f32, f32, f32, f32, void *, s32, void *, u32) __asm__("FUN_L00_0026d000");
extern float D_0015ED60;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC3C0), where it is exact; names translated to the US level program. */

char *spawn_homing_shot(char *owner,
                        char *spawn_pos,
                        char *target,
                        float vx,
                        float vy,
                        float yaw,
                        float pitch) __asm__("FUN_L00_002caf70");

char *spawn_homing_shot(

    char *owner, char *spawn_pos, char *target, float vx, float vy, float yaw, float pitch) {
    float          start_position[4];
    float          probe[4];
    float          direction[4];
    float          scaled[4];
    float          spark[4];
    unsigned char *moby;
    char          *vars;
    char          *trail;
    unsigned char *variant;
    int            result;
    int            timer;
    int            lifetime;
    int            i;
    int            sparks;
    float          length;
    float          blend;
    float          index;
    float          fraction;
    float         *start;

    start = start_position;
    *(w128 *)start_position = *(w128 *)spawn_pos;
    moby = create_moby(0x131);
    if (moby != 0) {
        vars = *(char **)(moby + 0x78);
        *(char **)(vars + 0x20) = owner;
        moby[0x30] = 0x7F;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x31] = 1;
        *(long *)(moby + 0x38) = *(long *)(owner + 0x38);
        *(float *)(moby + 0x40) = D_L00_00161890_gp;
        *(float *)(moby + 0x44) = fast_add_rotations(pitch, (&D_L00_00161890_gp)[1]);
        *(float *)(moby + 0x48) = yaw;
        qcopy(moby + 0x10, start);
        if (target != 0) {
            int bounds;
            qcopy(vars, target + 0x10);
            bounds = (int)get_moby_target_info(target);
            if (bounds != 0) {
                *(float *)(vars + 0x8) = *(float *)(vars + 0x8) + *(float *)(bounds + 0x10);
            } else {
                *(float *)(vars + 0x8) = *(float *)(vars + 0x8) + 0.5f;
            }
        } else {
            clear_u64_value(vars);
        }
        *(float *)(vars + 0x18) = vx;
        *(float *)(vars + 0x1C) = vy;
        *(int *)(vars + 0x10) = 0;
        *(int *)(vars + 0x14) = 0;
        timer = scale_game_frames(0x23);
        variant = item_text_variant;
        timer = timer + variant[0xF] * 10;
        *(int *)(vars + 0x28) = 0;
        *(short *)(vars + 0x24) = timer;
        *(short *)(vars + 0x26) = timer;
        *(w128 *)probe = *(w128 *)(owner + 0x10);
        probe[2] = start[2];
        result = collision_line(probe, start, 0, (int)owner, 0);
        if (result != 0) {
            build_spherical_offset(
                direction, D_0015ED6C * 40.0f, *(float *)(moby + 0x48), -*(float *)(moby + 0x44));
            qcopy(moby + 0x10, collision_hit_point);
            start = scaled;
            sparks = 4;
            do {
                *(w128 *)spark = 0;
                spark[0] = random_float_between(-1.0f, 1.0f);
                sparks--;
                spark[1] = random_float_between(-1.0f, 1.0f);
                spark[2] = random_float_between(-1.0f, 1.0f);
                *(w128 *)scaled = *(w128 *)spark;
                reflect_vector(spark, direction, (unsigned char *)&coll_hit_normal);
                length = fast_vec_length(spark);
                fast_vec_normalize(start, start, length * 0.5f);
                add_vector_xyz(spark, spark, start);
                fast_vec_normalize(
                    spark, spark, random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                result = scale_game_frames(10);
                timer = scale_game_frames(15);
                lifetime = random_int_between(result, timer);
                FUN_L00_0026dd20(moby + 0x10, (char *)spark, 0x7F2F4F6F, lifetime, 30000.0f);
            } while (sparks >= 0);
            variant = item_text_variant;
            if (variant[0xF] == 0) {
                FUN_L00_002cbcc0_c(moby, 0);
                mark_moby_for_removal(moby);
                return 0;
            }
            *(int *)(vars + 0x28) = *(int *)(vars + 0x28) + 1;
            if (variant[0xF] + 5 < *(int *)(vars + 0x28)) {
                FUN_L00_002cbcc0_c(moby, 1);
                mark_moby_for_removal(moby);
                return 0;
            }
            reflect_vector(direction, direction, (unsigned char *)&coll_hit_normal);
            *(float *)(moby + 0x48) = fast_arc_tan(direction[0], direction[1]);
            length = vector_length_xy(direction);
            *(float *)(moby + 0x44) = -fast_arc_tan(length, direction[2]);
            return moby;
        } else {
            spawn_source_attached_fading_effect_particle(
                moby, 0x2F4F7F7F, *(short *)(vars + 0x24), -1, homing_shot_attached_particle_size);
            for (i = 0; i < 17; i++) {
                trail =
                    (char *)spawn_random_size_rotated_particle(moby + 0x10, &zero_vector, 1, 0.0f);
                if (trail != 0) {
                    *(float *)(trail + 0xC) = random_float_between(
                        homing_shot_trail_initial_size_min, homing_shot_trail_initial_size_max);
                    index = convert_integer_to_float(i);
                    fraction = index / convert_integer_to_float(16);
                    blend = cubic_hermite_scalar(-1.5f, 0.0f, 1.0f, 0.0f, fraction);
                    *(float *)(trail + 0xC) =
                        *(float *)(trail + 0xC) +
                        (homing_shot_trail_size_blend_target - *(float *)(trail + 0xC)) * blend;
                    result = truncate_float_to_s32(blend * -64.0f + 128.0f);
                    *(int *)(trail + 4) = (result << 24) | 0xFFFFFF;
                    ((int *)(vars + 0x2C))[i] = (int)trail;
                } else {
                    ((int *)(vars + 0x2C))[i] = 0;
                }
            }
        }
    }
    return moby;
}

/* Ported probe rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC868), where it is exact; names translated to the US level program. */

/* D_L00_00161894 again, through the symbol below it: retail's second load stays lui */

void update_homing_shot(Moby *shot_moby) __asm__("FUN_L00_002cb418");

void update_homing_shot(Moby *shot_moby) {
    Vec4A     start;
    Vec4A     step;
    ShotQuery query;
    Vec4A     vector_scratch;
    Vec4A     spark_and_pushout_scratch;
    Vec4A     spark_velocity;
    ShotVars *vars;
    float     yaw;
    float     pitch;
    float     speed;
    float     spark_direction_length;
    int       i;
    int       j;
    int       left;

    vars = *(ShotVars **)&shot_moby->pvars;
    shot_moby->rot.y = fast_add_rotations(shot_moby->rot.y, -homing_shot_pitch_bias[0]);
    if (fast_vec_length(vars) < 0.1f) {
        if (vars->timer < scale_game_frames(0x23) - scale_game_frames(1) && vars->bounces == 0) {
            yaw = vars->rest_yaw;
            pitch = vars->rest_pitch;
        } else {
            yaw = shot_moby->rot.z;
            pitch = shot_moby->rot.y;
        }
    } else {
        yaw = fast_arc_tan(vars->target_x - shot_moby->pos.x, vars->target_y - shot_moby->pos.y);
        pitch = -fast_arc_tan(vector_distance_xy((Vec4A *)&shot_moby->pos, vars),
                              vars->target_z - shot_moby->pos.z);
    }
    if (fast_vec_length(vars) < 0.1f) {
        shot_moby->rot.z = spring_turn(&vars->yaw_rate,
                                       shot_moby->rot.z,
                                       yaw,
                                       D_0015ED70_lui * 6.2831855f,
                                       D_0015ED70_lui * 3.1415927f,
                                       D_0015ED6C_lui * 6.2831855f);
        shot_moby->rot.y = spring_turn(&vars->pitch_rate,
                                       shot_moby->rot.y,
                                       pitch,
                                       D_0015ED70_lui * 6.2831855f,
                                       D_0015ED70_lui * 3.1415927f,
                                       D_0015ED6C_lui * 6.2831855f);
    } else {
        shot_moby->rot.z = spring_turn(&vars->yaw_rate,
                                       shot_moby->rot.z,
                                       yaw,
                                       D_0015ED70_lui * 6.2831855f,
                                       D_0015ED70_lui * 3.1415927f,
                                       D_0015ED6C_lui * 4.712389f);
        shot_moby->rot.y = spring_turn(&vars->pitch_rate,
                                       shot_moby->rot.y,
                                       pitch,
                                       D_0015ED70_lui * 6.2831855f,
                                       D_0015ED70_lui * 3.1415927f,
                                       D_0015ED6C_lui * 4.712389f);
    }
    build_spherical_offset(&step, D_0015ED6C_lui * 40.0f, shot_moby->rot.z, -shot_moby->rot.y);
    qcopy(&start, (Vec4A *)&shot_moby->pos);
    add_vector_xyz_2cb418((Vec4A *)&shot_moby->pos, (Vec4A *)&shot_moby->pos, &step);
    if (shot_moby->pos.x < 0.0f || shot_moby->pos.y < 0.0f || shot_moby->pos.z < 0.0f ||
        vector_distance_xy((Vec4A *)&shot_moby->pos, &g_debug_camera) > 64.0f) {
        FUN_L00_002cbcc0_2cb418(shot_moby, 0);
        mark_moby_for_removal(shot_moby);
        return;
    }
    query.flags = 0x10001;
    query.radius = 0.25f;
    query.moby = shot_moby;
    query.i20 = 1;
    fast_vec_sub(&query, (Vec4A *)&shot_moby->pos, D_0013F3D0);
    set_vector_length_xy(&query, &query, 1.0f);
    query.dir.z = 1.0f;
    query.b18 = 1;
    query.b19 = 1;
    query.dir.w = 5627.9248f;
    query.oclass = shot_moby->oclass;
    if (fast_dec_timer_s16(&vars->timer) != 0) {
        FUN_L00_002cbcc0_2cb418(shot_moby, 0);
        mark_moby_for_removal(shot_moby);
        return;
    }
    if (collide_line_2cb418(&start, (Vec4A *)&shot_moby->pos, 0, vars->ignore, &query) != 0) {
        if (FUN_001f0b58_2cb418() == 0) {
            if (vars->timer > scale_game_frames(3)) {
                vars->timer = scale_game_frames(3);
            }
        } else {
            qcopy((Vec4A *)&shot_moby->pos, ((char *)D_L00_00173E60));
            for (i = 0; i < 5; i++) {
                *(u128 *)&spark_and_pushout_scratch = 0;
                spark_and_pushout_scratch.x = random_float_between(-1.0f, 1.0f);
                spark_and_pushout_scratch.y = random_float_between(-1.0f, 1.0f);
                spark_and_pushout_scratch.z = random_float_between(-1.0f, 1.0f);
                *(u128 *)&vector_scratch = *(u128 *)&spark_and_pushout_scratch;
                reflect_vector(&spark_and_pushout_scratch, &step, ((char *)D_L00_00173E60) + 0x20);
                fast_vec_normalize(&vector_scratch,
                                   &vector_scratch,
                                   fast_vec_length(&spark_and_pushout_scratch) * 0.5f);
                add_vector_xyz_2cb418(
                    &spark_and_pushout_scratch, &spark_and_pushout_scratch, &vector_scratch);
                fast_vec_normalize(
                    &spark_and_pushout_scratch,
                    &spark_and_pushout_scratch,
                    random_float_between(D_0015ED6C_lui * 3.0f, D_0015ED6C_lui * 6.0f));
                spawn_spark_2cb418((Vec4A *)&shot_moby->pos,
                                   &spark_and_pushout_scratch,
                                   0x7F2F4F6F,
                                   random_int_between(scale_game_frames(10), scale_game_frames(15)),
                                   30000.0f);
            }
            if (item_text_variant[15] == 0) {
                FUN_L00_002cbcc0_2cb418(shot_moby, 0);
                mark_moby_for_removal(shot_moby);
                return;
            }
            vars->bounces++;
            switch (FUN_001f0b58_2cb418()) {
            case 0:
            case 1:
            case 3:
            case 4:
            case 7:
            case 11:
            case 13:
                vars->bounces = 1000;
                break;
            }
            if (item_text_variant[15] + 5 < vars->bounces) {
                FUN_L00_002cbcc0_2cb418(shot_moby, 1);
                mark_moby_for_removal(shot_moby);
                return;
            }
            reflect_vector(&vector_scratch, &step, ((char *)D_L00_00173E60) + 0x20);
            shot_moby->rot.z = fast_arc_tan(vector_scratch.x, vector_scratch.y);
            shot_moby->rot.y = -fast_arc_tan(vector_length_xy(&vector_scratch), vector_scratch.z);
            vars->yaw_rate = 0.0f;
            vars->pitch_rate = 0.0f;
            fast_vec_normalize(&spark_and_pushout_scratch, ((char *)D_L00_00173E60) + 0x20, 0.05f);
            add_vector_xyz_2cb418(
                (Vec4A *)&shot_moby->pos, ((char *)D_L00_00173E60), &spark_and_pushout_scratch);
            if (D_001403E0_lui != 0 && D_001403E0_lui->oclass == 0xA8 &&
                D_001403E0_lui->state != 0xFE && D_001403E0_lui->state != 0xFD) {
                allocate_voice_for_target_entry(4, 0, D_001403E0_lui);
            }
        }
    }
    shot_moby->rot.y = fast_add_rotations(shot_moby->rot.y, D_L00_00161890[1]);
    left = vars->life - vars->timer;
    if (left < scale_game_frames(13)) {
        speed = convert_integer_to_float(left);
        speed = speed / convert_integer_to_float(scale_game_frames(13));
        speed = speed * trail_speed;
    } else {
        speed = trail_speed;
    }
    fast_vec_sub(&step, &start, (Vec4A *)&shot_moby->pos);
    fast_vec_normalize(&step, &step, speed);
    qcopy(&vector_scratch, (Vec4A *)&shot_moby->pos);
    if (vars->follower != 0) {
        qcopy((Vec4A *)&vars->follower->pos, &vector_scratch);
    }
    for (j = 0; j < 16; j++) {
        Moby *trail_moby = vars->trail[j];
        if (trail_moby != 0) {
            add_vector_xyz_2cb418((Vec4A *)&trail_moby->pos, &vector_scratch, &step);
            qcopy(&vector_scratch, (Vec4A *)&trail_moby->pos);
            trail_moby->pos.x = trail_moby->pos.x + randf_sym(0.0f, jitter);
            trail_moby->pos.y = trail_moby->pos.y + randf_sym(0.0f, jitter);
            trail_moby->pos.z = trail_moby->pos.z + randf_sym(0.0f, jitter);
            scale_vector_xyz_2cb418(&step, &step, trail_decay);
        }
    }
    if (random_integer_below(5) != 0) {
        return;
    }
    fast_vec_cross(&spark_and_pushout_scratch, &step, D_L00_00166FF0_2cb418);
    build_look_at_matrix(
        &spark_and_pushout_scratch, &spark_and_pushout_scratch, &step, randf_sym(0.0f, 3.1415927f));
    spark_direction_length = fast_vec_length(&spark_and_pushout_scratch);
    if (spark_direction_length == 0.0f) {
        return;
    }
    scale_vector_xyz_2cb418(
        &spark_velocity,
        &spark_and_pushout_scratch,
        random_float_between(D_0015ED6C_lui * 0.5f, D_0015ED6C_lui + D_0015ED6C_lui) /
            spark_direction_length);
    spawn_spark_2cb418((Vec4A *)&shot_moby->pos,
                       &spark_velocity,
                       0x7F2F4F6F,
                       random_int_between(scale_game_frames(10), scale_game_frames(15)),
                       30000.0f);
}

void clear_homing_shot_trail_and_spawn_end_effects(u8 *shot_moby,
                                                   s32 enabled) __asm__("FUN_L00_002cbcc0");

void clear_homing_shot_trail_and_spawn_end_effects(u8 *shot_moby, s32 enabled) {
    s32    i;
    s32    j;
    void **handle;
    u8    *shot_position;
    V4     vectors[2];
    s32    lifetime;
    s32    color;
    s32    sign;
    u8    *effect;
    s32    alpha;

    handle = (void **)(*(u8 **)(shot_moby + 0x78) + 0x2c);
    for (i = 16; i >= 0; i--) {
        if (*handle != 0)
            free_effect_particle(*handle);
        *handle++ = 0;
    }
    shot_position = shot_moby + 0x10;

    if (enabled != 0) {
        V4 *vector = &vectors[1];
        for (j = 4; j >= 0; j--) {
            vectors[1].q = 0;
            vectors[1].f[0] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[1].f[1] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[1].f[2] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[0].q = vectors[1].q;
            FUN_001ff500_2cbcc0(
                vector,
                &vectors[0],
                FUN_L00_00257be8_2cbcc0(D_0015ED6C_lui * 3.0f, D_0015ED6C_lui * 6.0f));
            lifetime = random_int_between(FUN_001fef20_2cbcc0(10), FUN_001fef20_2cbcc0(15));
            FUN_0026dd20_2cbcc0(shot_position, vector, 0x7f2f4f6f, lifetime, 35000.0f);
        }
    }

    {
        s32 j;
        for (j = 2; j >= 0; j--) {
            color = ((u32)random_int_between(0x20, 0x80) << 24) | 0x606060;
            sign = FUN_L00_00257b50_2cbcc0(2) ? -1 : 1;
            vectors[0].q = 0;
            vectors[0].f[2] = 1.0f;
            vectors[0].f[3] = 1.0f;
            vectors[0].f[2] = FUN_L00_00257be8_2cbcc0(0.01f, 0.025f) * D_0015ED60;
            effect =
                spawn_puff(0.2f, 1.0f, 1.02f, 140000.0f, shot_position, sign, &vectors[0], color);
            if (effect != 0) {
                *(s16 *)(effect + 0xa) = FUN_001fef20_2cbcc0(random_int_between(50, 80));
                /* The direction is no longer needed; retain the effect field address. */
                i = (s32)(effect + 0x20);
                *(s32 *)(i + 4) = 2;
                ((u8 *)i)[0xa] = color >> 24;
                ((u8 *)i)[0xb] = effect[0xa];
                alpha = FUN_L00_00257b50_2cbcc0(2);
                if (alpha) {
                    /* The fixed low color byte is 0x60, so the alpha byte is 0x44. */
                    alpha = 0x44;
                    effect[3] = alpha;
                }
            }
        }
    }
}

float D_L01_001618C8 = 0;
f32   D_L01_001618CC[2] = {-1.5707964f, 0.0f};

f32   D_L01_001618B4 = 0.075f;
f32   D_L01_001618B8 = 0.15f;
f32   D_L01_001618BC = 0.85f;
float D_L02_00161860 = 0;
f32   D_L02_00161864[2] = {-1.5707964f, 0.0f};

f32   D_L02_0016184C = 0.075f;
f32   D_L02_00161850 = 0.15f;
f32   D_L02_00161854 = 0.85f;
float D_L03_00161870 = 0;
f32   D_L03_00161874[2] = {-1.5707964f, 0.0f};

f32   D_L03_0016185C = 0.075f;
f32   D_L03_00161860 = 0.15f;
f32   D_L03_00161864 = 0.85f;
float D_L04_00161868 = 0;
f32   D_L04_0016186C[2] = {-1.5707964f, 0.0f};

f32   D_L04_00161854 = 0.075f;
f32   D_L04_00161858 = 0.15f;
f32   D_L04_0016185C = 0.85f;
float D_L05_001618B0 = 0;
f32   D_L05_001618B4[2] = {-1.5707964f, 0.0f};

f32   D_L05_0016189C = 0.075f;
f32   D_L05_001618A0 = 0.15f;
f32   D_L05_001618A4 = 0.85f;
float D_L06_001618B0 = 0;
f32   D_L06_001618B4[2] = {-1.5707964f, 0.0f};

f32   D_L06_0016189C = 0.075f;
f32   D_L06_001618A0 = 0.15f;
f32   D_L06_001618A4 = 0.85f;
float D_L07_00161860 = 0;
f32   D_L07_00161864[2] = {-1.5707964f, 0.0f};

f32   D_L07_0016184C = 0.075f;
f32   D_L07_00161850 = 0.15f;
f32   D_L07_00161854 = 0.85f;
float D_L08_00161958 = 0;
f32   D_L08_0016195C[2] = {-1.5707964f, 0.0f};

f32   D_L08_00161944 = 0.075f;
f32   D_L08_00161948 = 0.15f;
f32   D_L08_0016194C = 0.85f;
float D_L09_00161940 = 0;
f32   D_L09_00161944[2] = {-1.5707964f, 0.0f};

f32   D_L09_0016192C = 0.075f;
f32   D_L09_00161930 = 0.15f;
f32   D_L09_00161934 = 0.85f;
float D_L10_00161890 = 0;
f32   D_L10_00161894[2] = {-1.5707964f, 0.0f};

f32   D_L10_0016187C = 0.075f;
f32   D_L10_00161880 = 0.15f;
f32   D_L10_00161884 = 0.85f;
float D_L11_001618F0 = 0;
f32   D_L11_001618F4[2] = {-1.5707964f, 0.0f};

f32   D_L11_001618DC = 0.075f;
f32   D_L11_001618E0 = 0.15f;
f32   D_L11_001618E4 = 0.85f;
float D_L12_00161990 = 0;
f32   D_L12_00161994[2] = {-1.5707964f, 0.0f};

f32   D_L12_0016197C = 0.075f;
f32   D_L12_00161980 = 0.15f;
f32   D_L12_00161984 = 0.85f;
float D_L13_00161940 = 0;
f32   D_L13_00161944[2] = {-1.5707964f, 0.0f};

f32   D_L13_0016192C = 0.075f;
f32   D_L13_00161930 = 0.15f;
f32   D_L13_00161934 = 0.85f;
float D_L14_00161AE0 = 0;
f32   D_L14_00161AE4[2] = {-1.5707964f, 0.0f};

f32   D_L14_00161ACC = 0.075f;
f32   D_L14_00161AD0 = 0.15f;
f32   D_L14_00161AD4 = 0.85f;
float D_L15_00161A60 = 0;
f32   D_L15_00161A64[2] = {-1.5707964f, 0.0f};

f32   D_L15_00161A4C = 0.075f;
f32   D_L15_00161A50 = 0.15f;
f32   D_L15_00161A54 = 0.85f;
float D_L16_00161920 = 0;
f32   D_L16_00161924[2] = {-1.5707964f, 0.0f};

f32   D_L16_0016190C = 0.075f;
f32   D_L16_00161910 = 0.15f;
f32   D_L16_00161914 = 0.85f;
float D_L17_00161970 = 0;
f32   D_L17_00161974[2] = {-1.5707964f, 0.0f};

f32   D_L17_0016195C = 0.075f;
f32   D_L17_00161960 = 0.15f;
f32   D_L17_00161964 = 0.85f;
float D_L18_00161900 = 0;
f32   D_L18_00161904[2] = {-1.5707964f, 0.0f};

f32 D_L18_001618EC = 0.075f;
f32 D_L18_001618F0 = 0.15f;
f32 D_L18_001618F4 = 0.85f;
