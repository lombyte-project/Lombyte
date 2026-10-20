/* Shared moby classes: blarg_commander. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/quad.h"


extern char *FUN_0020c4f8_8670(int) __asm__("FUN_0020c4f8");
extern void FUN_001f9810_8670(void *, int) __asm__("FUN_001f9810");
extern void FUN_L00_00250df8_8670(void *) __asm__("FUN_L00_00250df8");

char *FUN_L12_00308670(char *source_object) {
    unsigned char *allocated_object = (unsigned char *)FUN_0020c4f8_8670(0x55B);
    if (allocated_object != 0) {
        char *object_data = *(char **)(allocated_object + 0x78);
        allocated_object[0x30] = 0xFF;
        *(short *)(allocated_object + 0x32) = 0x7F;
        allocated_object[0x31] = 1;
        allocated_object[0x20] = 0;
        *(OvlQuad *)(allocated_object + 0x10) = *(OvlQuad *)(source_object + 0x10);
        *(OvlQuad *)(allocated_object + 0x40) = *(OvlQuad *)(source_object + 0x40);
        *(char **)(allocated_object + 0xB8) = source_object;
        FUN_001f9810_8670(object_data, 0x20);
        *(int *)(object_data + 0x10) = (int)(object_data + 0x20);
        FUN_L00_00250df8_8670(allocated_object);
    }
    return (char *)allocated_object;
}
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_sqrt(float) __asm__("FUN_001f9988");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

#include "rnc/gameplay/entities/moby.h"

/* Pvars of a lobbed projectile, as set up at launch. */
typedef struct {
    u8 pad0[0x20];
    Vec4f arc;          /* 0x20: filled by trooper_start_knockback */
    f32 rise;           /* 0x30: D_0015ED70 * 40 */
    f32 fall;           /* 0x34: D_0015ED70 * 100 */
    f32 launch_speed;   /* 0x38 */
    f32 unk3C;          /* 0x3C */
    s32 timer;          /* 0x40 */
    s32 unk44;          /* 0x44: set to 1 on launch */
    f32 unk48;          /* 0x48: 0.7 on launch */
    u8 pad4C[0x11];
    u8 unk5D;
    u8 pad5E[0xE];
    f32 unk6C;          /* 0x6C */
    f32 unk70;          /* 0x70: -1 on launch */
    f32 unk74;          /* 0x74: -1 on launch */
} LobbedVars;

void FUN_L12_00308708(void *unused, struct Moby *moby) {
    LobbedVars *lobbed_vars = (LobbedVars *)moby->pvars;
    float dir[4];
    float ang;
    float rise_sqrt_term, frame_rise_sqrt_term;
    float frame_scaled_rise;
    float g;

    if (random_integer_below(3) == 0) {
        allocate_voice_for_target_entry(1, 0x20, moby);
    }
    lobbed_vars->unk48 = 0.7f;
    lobbed_vars->timer = truncate_float_to_s32(716.8f);
    lobbed_vars->unk6C = 0.0f;
    moby->update_dist = 0xFF;
    moby->flags &= ~6;
    ang = fast_add_rotations(moby->rot.z, 3.1415927f);
    dir[0] = fast_cos(ang);
    dir[1] = fast_sin(ang);
    dir[2] = 1.0f;
    dir[3] = 5627.925f;
    g = frame_time_sq;
    lobbed_vars->rise = g * 40.0f;
    lobbed_vars->fall = g * 100.0f;
    rise_sqrt_term = fast_sqrt(lobbed_vars->rise * 4.0f);
    frame_rise_sqrt_term = fast_sqrt(frame_time_sq * 40.0f * 4.0f);
    frame_scaled_rise = frame_time_sq * 40.0f;
    lobbed_vars->launch_speed = 3.0f / ((rise_sqrt_term + rise_sqrt_term) / frame_scaled_rise) + frame_time_sq * 100.0f * 0.5f * ((frame_rise_sqrt_term + frame_rise_sqrt_term) / frame_scaled_rise);
    lobbed_vars->unk3C = fast_sqrt(frame_scaled_rise * 4.0f);
    lobbed_vars->unk5D = 0;
    lobbed_vars->unk44 = 1;
    trooper_knockback_arc(dir, &ang, &lobbed_vars->launch_speed, &lobbed_vars->unk3C);
    trooper_start_knockback(ang, moby, &lobbed_vars->arc, 1, 5, 2);
    lobbed_vars->unk6C = frame_time + frame_time;
    lobbed_vars->unk70 = -1.0f;
    lobbed_vars->unk74 = -1.0f;
    moby->flags &= ~0x1000;
    blend_moby_animation(moby, 1, 0, 0);
}
#include "sda.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002BD3D0.c: func_L12_00309CF8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x40];
    float vx, vy;
} MO;

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void blarg_commander_update(char *m) __asm__("FUN_L12_00308918");
void blarg_commander_update(char *moby) {
    int st;
    char *moby_pvars;
    char *motion_state;
    int r;
    MO *mo = (MO *)moby;
    char v[16];
    st = *(unsigned char *)(moby + 0x53);
    moby_pvars = *(char **)(moby + 0x78);
    if (st == 1) {
        motion_state = moby_pvars + 0x20;
        r = mine_fall(moby, motion_state);
        mo->vx = fast_add_rotations(mo->vx, frame_time * 4.3633232f);
        mo->vy = fast_add_rotations(mo->vy, frame_time * 4.3633232f);
        *(unsigned short *)(moby + 0x34) &= 0xEFFF;
        if (r & 0x61) {
            qcopy(v, moby + 0x10);
            ((float *)v)[2] += 1.0f;
            func_L00_0025F4A8_alt(moby, motion_state, v, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 0, 15.0f, 1,
                                  st, -1, 0);
            mark_moby_for_removal(moby);
        } else if (*(float *)(moby + 0x18) < 5.0f) {
            mark_moby_for_removal(moby);
        }
    }
}
