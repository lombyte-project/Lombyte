/* Shared moby class nanotech_crate_fragment (also the other crate fragments): the pieces a broken crate flies apart into, and the function that spawns them. */

#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/collision.h"

typedef struct {
    void         *o;
    float         f4, f8;
    short         hc, he;
    unsigned char p10[0x10];
    int           i20;
    float         f24, f28;
} V_2af8b0;

typedef struct {
    u8  pad0[4];
    f32 f4;
    f32 f8;
    s16 hC;
    s16 hE;
    f32 vel[4] __attribute__((aligned(16)));
    f32 f20, f24, f28;
} PV;

typedef struct {
    u8  pad0[0x10];
    f32 pos[4] __attribute__((aligned(16)));
    u8  b20, b21, b22, b23;
    u8  pad24[8];
    f32 f2c;
    u8  pad30[0x10];
    f32 f40, f44, f48;
    u8  pad4c[0x2C];
    PV *pv;
} M_2afa48;

extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern void  reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
int          FUN_001f96f8_2af8b0(int) __asm__("FUN_001f96f8");
float        randf_sym(float, float) __asm__("FUN_L00_00257c48");
void         FUN_L00_00250df8_2af8b0(void *) __asm__("FUN_L00_00250df8");
extern s32   coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern void  vscl_2afa48(void *, void *, f32) __asm__("FUN_001f9a68");

extern float D_L00_00161448_gp __asm__("D_L00_00161448") __attribute__((sda));
extern float D_L00_0016144C_gp __asm__("D_L00_0016144C") __attribute__((sda));
extern f32   D_L00_00161470_gp __asm__("D_L00_00161470") __attribute__((sda));
extern s32   D_L00_00161474_gp __asm__("D_L00_00161474") __attribute__((sda));

float D_L01_00161448 = 0;
float D_L01_0016144C = 3.6e+02f;
f32   D_L01_00161470 = 0.9f;
s32   D_L01_00161474 = 4;
float D_L02_00161418 = 0;
float D_L02_0016141C = 3.6e+02f;
f32   D_L02_00161440 = 0.9f;
s32   D_L02_00161444 = 4;
float D_L03_00161428 = 0;
float D_L03_0016142C = 3.6e+02f;
f32   D_L03_00161450 = 0.9f;
s32   D_L03_00161454 = 4;
f32   D_L04_00161440 = 0.9f;
s32   D_L04_00161444 = 4;
float D_L05_00161468 = 0;
float D_L05_0016146C = 3.6e+02f;
f32   D_L05_00161490 = 0.9f;
s32   D_L05_00161494 = 4;
float D_L06_00161418 = 0;
float D_L06_0016141C = 3.6e+02f;
f32   D_L06_00161440 = 0.9f;
s32   D_L06_00161444 = 4;
float D_L07_00161418 = 0;
float D_L07_0016141C = 3.6e+02f;
f32   D_L07_00161440 = 0.9f;
s32   D_L07_00161444 = 4;
float D_L08_00161418 = 0;
float D_L08_0016141C = 3.6e+02f;
f32   D_L08_00161440 = 0.9f;
s32   D_L08_00161444 = 4;
float D_L09_001614A8 = 0;
float D_L09_001614AC = 3.6e+02f;
f32   D_L09_001614D0 = 0.9f;
s32   D_L09_001614D4 = 4;
float D_L10_00161428 = 0;
float D_L10_0016142C = 3.6e+02f;
f32   D_L10_00161450 = 0.9f;
s32   D_L10_00161454 = 4;
float D_L11_001614A8 = 0;
float D_L11_001614AC = 3.6e+02f;
f32   D_L11_001614D0 = 0.9f;
s32   D_L11_001614D4 = 4;
float D_L12_00161428 = 0;
float D_L12_0016142C = 3.6e+02f;
f32   D_L12_00161450 = 0.9f;
s32   D_L12_00161454 = 4;
float D_L13_001614F8 = 0;
float D_L13_001614FC = 3.6e+02f;
f32   D_L13_00161520 = 0.9f;
s32   D_L13_00161524 = 4;
float D_L14_00161630 = 0;
float D_L14_00161634 = 3.6e+02f;
f32   D_L14_00161658 = 0.9f;
s32   D_L14_0016165C = 4;
float D_L15_00161588 = 0;
float D_L15_0016158C = 3.6e+02f;
f32   D_L15_001615B0 = 0.9f;
s32   D_L15_001615B4 = 4;
float D_L16_001614D8 = 0;
float D_L16_001614DC = 3.6e+02f;
f32   D_L16_00161500 = 0.9f;
s32   D_L16_00161504 = 4;
float D_L17_00161500 = 0;
float D_L17_00161504 = 3.6e+02f;
f32   D_L17_00161528 = 0.9f;
s32   D_L17_0016152C = 4;
float D_L18_001614B8 = 0;
float D_L18_001614BC = 3.6e+02f;
f32   D_L18_001614E0 = 0.9f;
s32   D_L18_001614E4 = 4;

unsigned char *spawn_crate_debris(unsigned char *owner,
                                  void          *vel,
                                  void          *pos,
                                  void          *rot,
                                  int            type,
                                  int            n,
                                  float          lo,
                                  float          hi) __asm__("FUN_L00_002af8b0");

unsigned char *spawn_crate_debris(unsigned char *owner,
                                  void          *initial_velocity,
                                  void          *spawn_position,
                                  void          *initial_rotation,
                                  int            type,
                                  int            initial_bounce_count,
                                  float          lo,

                                  float hi) {
    unsigned char *debris_moby = create_moby(type);
    if (debris_moby) {
        V_2af8b0 *debris_data = ((V_2af8b0 *)MOBY(debris_moby)->pvars);
        float     scale_multiplier;
        debris_data->o = owner;
        debris_moby[0x30] = 0xFF;
        MOBY(debris_moby)->draw_dist = 0x7E;
        debris_moby[0x31] = 1;
        MOBY(debris_moby)->spawn_frame = *(long *)(owner + 0x38);
        debris_moby[0x20] = 0;
        MOBY(debris_moby)->rot.x = random_angle_radians();
        MOBY(debris_moby)->rot.y = random_angle_radians();
        MOBY(debris_moby)->rot.z = random_angle_radians();
        qcopy(debris_moby + 0x10, spawn_position);
        qcopy(debris_moby + 0x40, initial_rotation);
        qcopy(debris_data->p10, initial_velocity);
        debris_data->f4 = MOBY(debris_moby)->pos.z;
        {
            short t = FUN_001f96f8_2af8b0(0x14);
            debris_data->i20 = 0;
            debris_data->he = t;
        }
        debris_data->f24 =
            randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * frame_time;
        debris_data->f28 =
            randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * frame_time;
        scale_multiplier = random_float_between(lo, hi);
        MOBY(debris_moby)->scale *= scale_multiplier;
        debris_data->hc = initial_bounce_count;
        debris_data->f8 = scale_multiplier * 0.25f;
        FUN_L00_00250df8_2af8b0(debris_moby);
    }
    return debris_moby;
}

void nanotech_crate_fragment_update(M_2afa48 *m) __asm__("FUN_L00_002afa48");

void nanotech_crate_fragment_update(M_2afa48 *fragment_moby) {
    enum {
        NANOTECH_CRATE_FRAGMENT_BOUNCING = 0,
        NANOTECH_CRATE_FRAGMENT_FADING = 1
    };
    PV   *pv = fragment_moby->pv;
    u128  old;
    u128 *hit_normal;
    qcopy(&old, fragment_moby->pos);
    pv->vel[2] -= frame_time_sq * 20.0f;
    fast_vec_add(fragment_moby->pos, fragment_moby->pos, pv->vel);
    fragment_moby->f40 = fast_add_rotations(fragment_moby->f40, pv->f20);
    fragment_moby->f44 = fast_add_rotations(fragment_moby->f44, pv->f24);
    fragment_moby->f48 = fast_add_rotations(fragment_moby->f48, pv->f28);
    switch (fragment_moby->b20) {
    case NANOTECH_CRATE_FRAGMENT_BOUNCING:
        if (fragment_moby->pos[2] < pv->f4 - 3.0f) {
            fragment_moby->b20 = 1;
        } else if (fast_dec_timer_s16(&pv->hE) && pv->hC != 0 &&
                   coll_sphere(fragment_moby->pos, 2, 0, pv->f8)) {
            hit_normal = (u128 *)&coll_hit_normal;
            if (fast_vec_dot(pv->vel, hit_normal) < 0.0f) {
                qcopy(fragment_moby->pos, hit_normal - 1);
                pv->hC--;
                reflect_vector(pv->vel, pv->vel, hit_normal);
                vscl_2afa48(pv->vel, pv->vel, 0.75f);
                pv->f20 = 0.0f;
                pv->f24 = randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * frame_time;
                pv->f28 = randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * frame_time;
            }
        }
        break;
    case NANOTECH_CRATE_FRAGMENT_FADING:
        fragment_moby->f2c *= D_L00_00161470_gp;
        if (fragment_moby->b23 < D_L00_00161474_gp) {
            mark_moby_for_removal(fragment_moby);
        } else {
            fragment_moby->b23 -= D_L00_00161474_gp;
        }
        break;
    }
}
