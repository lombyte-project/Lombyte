/* Level 01 moby class burning_debris (0x5e6): the smoking wreckage thrown out by destroyed jets, which bursts on impact or timeout, and its spawner. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/l01/mobys/burning_debris_globals.h"

/* pvars of the debris FUN_L01_0030ba18 moves (spawn_debris fills them) */
typedef struct {
    Vec4f vel;   /* 0x00 */
    Vec4f spin;  /* 0x10 */
    s32   timer; /* 0x20 */
    s32   owner; /* 0x24 */
} BlobVars;

typedef struct {
    u8    pad0[0x1C];
    s32   have_normal; /* 0x1C */
    u8    pad20[0x20];
    Vec4f normal; /* 0x40 */
} BlobHitInfo;

f32        burning_debris_gravity_coefficient_gp __asm__("D_L01_00162058") = 1e+01f;
f32        burning_debris_velocity_multiplier_gp __asm__("D_L01_0016205C") = 0.995f;
extern f32 burning_debris_horizontal_damping_threshold_gp __asm__("D_L01_00162060")
    __attribute__((sda));
extern BlobHitInfo collision_hit __asm__("D_L01_001742C0");
extern s32         D_L01_0015F580;
extern void        add_draw_callback(void *, void *) __asm__("FUN_001f4600");
extern void        draw_moby_offset_textured_quad(void) __asm__("FUN_L01_0030bfa8");
extern int         coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void        FUN_L00_0025e450(void *,
                                    void *,
                                    void *,
                                    f32,
                                    f32,
                                    s32,
                                    s32,
                                    s32,
                                    f32,
                                    f32,
                                    f32,
                                    s32,
                                    f32,
                                    f32,
                                    s32,
                                    s32,
                                    s32,
                                    s32);
extern void        spawn_burst(void *, void *, s32, s32, f32, s32, s32) __asm__("FUN_L00_0026cbb0");
extern int         FUN_L00_00257b90(int lo, int hi);
extern u8 *spawn_puff(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");

void burning_debris_update(struct Moby *moby) __asm__("FUN_L01_0030ba18");

/* Moby class 1510 */
void burning_debris_update(struct Moby *moby) {
    BlobVars *blob_vars;
    s32       hit;
    s32       i;
    s32       signed_puff_variation;
    s32       grey_intensity;
    s32       alpha;
    s32       grey;
    f32       s;
    u8       *puff;
    u8       *puff_data;

    blob_vars = (BlobVars *)moby->pvars;
    if (random_integer_below(2)) {
        add_draw_callback(draw_moby_offset_textured_quad, moby);
    }
    fast_vec_add(&moby->pos, &moby->pos, blob_vars);
    if (absolute_float(blob_vars->vel.x) >
        burning_debris_horizontal_damping_threshold_gp * frame_scale) {
        blob_vars->vel.x *= (burning_debris_velocity_multiplier_gp - 1.0f) * frame_scale + 1.0f;
    }
    if (absolute_float(blob_vars->vel.y) >
        burning_debris_horizontal_damping_threshold_gp * frame_scale) {
        blob_vars->vel.y *= (burning_debris_velocity_multiplier_gp - 1.0f) * frame_scale + 1.0f;
    }
    if (blob_vars->vel.z > 0.0f) {
        blob_vars->vel.z *= (burning_debris_velocity_multiplier_gp - 1.0f) * frame_scale + 1.0f;
    }
    blob_vars->vel.z -= burning_debris_gravity_coefficient_gp * frame_time_sq;
    moby->rot.x = fast_add_rotations(moby->rot.x, blob_vars->spin.x);
    moby->rot.y = fast_add_rotations(moby->rot.y, blob_vars->spin.y);
    moby->rot.z = fast_add_rotations(moby->rot.z, blob_vars->spin.z);
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    hit = coll_sphere(2.0f, &moby->pos, 0, (void *)blob_vars->owner);
    if (fast_dec_timer(&blob_vars->timer) || hit) {
        if (hit) {
            if (collision_hit.have_normal) {
                reflect_vector(blob_vars, blob_vars, &collision_hit.normal);
            } else {
                clear_u64_value(blob_vars);
            }
        }
        FUN_L00_0025e450(moby,
                         blob_vars,
                         &moby->pos,
                         0.0f,
                         0.0f,
                         0x14,
                         3,
                         4,
                         4.0f,
                         2.0f,
                         100000.0f,
                         0,
                         3.0f,
                         15.0f,
                         1,
                         1,
                         -1,
                         0);
        mark_moby_for_removal(moby);
        return;
    }
    s = random_float_between(0.5f, 1.5f) * 210000.0f;
    spawn_burst(&moby->pos,
                &D_L01_0015F580,
                0x4F007FFF,
                0x1FFFFFFF,
                s,
                FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                1);
    alpha = 0x60000000;
    for (i = 0; i < 2; i++) {
        signed_puff_variation = random_integer_below(6);
        if (random_integer_below(2)) {
            signed_puff_variation = -signed_puff_variation;
        }
        s = random_float_between(40000.0f, 100000.0f);
        grey_intensity = FUN_L00_00257b90(0x30, 0xFF);
        grey = (grey_intensity << 8) | alpha;
        puff = spawn_puff(&moby->pos,
                          0.025f,
                          1.0f,
                          1.0f,
                          signed_puff_variation,
                          &D_L01_0015F580,
                          s,
                          grey_intensity | ((grey_intensity << 16) | grey));
        if (puff != NULL) {
            puff[3] = 0x44;
            puff_data = puff + 0x20;
            *(s16 *)(puff + 0xA) = scale_game_frames(60);
            *(s32 *)(puff_data + 4) = 2;
            puff_data[0xA] = 0x60;
            puff_data[0xB] = puff[0xA];
        }
    }
}

typedef struct {
    u8    pad0[0x10];
    f32   spin_x;
    f32   spin_y;
    f32   spin_z;
    u8    pad1c[4];
    s32   timer;
    void *parent;
} DebrisVars;

typedef struct {
    u8          pad0[0x10];
    u128        pos;
    u8          state;
    u8          pad21[0xB];
    f32         scale;
    u8          b30;
    u8          b31;
    s16         h32;
    u8          pad34[0xC];
    u128        rot;
    u8          pad50[0x28];
    DebrisVars *vars;
} DebrisMoby;

extern f32 randf_sym(f32 lo, f32 hi) __asm__("FUN_L00_00257c48");

DebrisMoby *spawn_debris(DebrisMoby *parent_moby,
                         void       *debris_vector_source,
                         f32         scale) __asm__("FUN_L01_0030be70");

DebrisMoby *spawn_debris(DebrisMoby *parent_moby, void *debris_vector_source, f32 scale) {
    DebrisMoby *spawned_moby;
    DebrisVars *debris_vars;

    spawned_moby = create_moby(0x5E6);
    if (spawned_moby != NULL) {
        debris_vars = spawned_moby->vars;
        debris_vars->parent = parent_moby;
        qcopy(&spawned_moby->pos, &parent_moby->pos);
        spawned_moby->rot = parent_moby->rot;
        spawned_moby->state = 0;
        spawned_moby->b30 = 0xFF;
        spawned_moby->h32 = 0xFF;
        spawned_moby->b31 = 1;
        spawned_moby->scale = spawned_moby->scale * scale;
        fast_vec_normalize(debris_vars, debris_vector_source, random_float_between(0.8f, 1.2f));
        debris_vars->spin_x = randf_sym(0.0f, 0.008726646f);
        debris_vars->spin_y = randf_sym(0.05235988f, 0.13962634f);
        debris_vars->spin_z = randf_sym(0.0f, DEG_TO_RAD);
        debris_vars->timer = scale_game_frames(200);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

f32 burning_debris_horizontal_damping_threshold_gp __asm__("D_L01_00162060") = 0;
