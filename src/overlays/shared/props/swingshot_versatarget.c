/* Shared moby class swingshot_versatarget (and swingshot_swinging_versatarget). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "qcopy.h"

typedef struct Hero Hero;

extern s32 D_L02_0015F5CC;

extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   D_L02_0015F5CC; /* no foreign declaration */
extern int   D_L02_0015FFD8; /* no foreign declaration */
extern float versatarget_particle_base_lifetime_gp __asm__("D_L02_00161CB0") __attribute__((sda));
extern int   versatarget_particle_color_gp __asm__("D_L02_00161CB4") __attribute__((sda));
extern float versatarget_particle_scale_gp __asm__("D_L02_00161CB8") __attribute__((sda));
extern float D_L02_00161CBC;
extern void  spawn_moving_hero_relative_alpha_fade_particle(
     void *, void *, int, int, int, int, float) __asm__("FUN_L00_002730e0");
void        try_select_camera_offset_effect_moby(char *m) __asm__("FUN_L02_002fa7c8");
extern void func_L02_002FBB10_c(char *) __asm__("FUN_L02_002fa6d8");
extern void try_select_camera_offset_effect_moby(char *) __asm__("FUN_L02_002fa7c8");
extern float
func_L00_0025CE58_f(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

/* Hideable flower: appears when its slot is free, faces and sparkles at the hero (class 0x323), hides again
 * when its slot is used. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002E1888), where it is exact; names translated to the US level program. */

void swingshot_versatarget_update(char *m) __asm__("FUN_L02_002e0450");

void swingshot_versatarget_update(char *moby) {
    enum {
        VERSATARGET_STATE_INITIALIZE = 0,
        VERSATARGET_STATE_ACTIVE = 1,
        VERSATARGET_STATE_INACTIVE = 2
    };
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float particle_position[4];
    float particle_velocity[4];
    char *linked_moby;
    switch (((unsigned char *)moby)[0x20]) {
    case VERSATARGET_STATE_INITIALIZE:
        if (*(int *)(moby_data + 0x30) != -1 ||
            (*(int *)(moby_data + 0x3C) != -1 &&
             (linked_moby = (char *)(D_L02_0015FFD8 + (*(int *)(moby_data + 0x3C) << 8))) != 0 &&
             ((unsigned char *)linked_moby)[0x20] != 0xFE &&
             ((unsigned char *)linked_moby)[0x20] != 0xFD)) {
            ((unsigned char *)moby)[0x30] = 0xFF;
            moby[0x20] = 2;
            MOBY(moby)->flags |= 0x41;
        } else {
            set_moby_color(moby, 0xC0, 0xC0, 0xC0);
            func_L02_002FBB10_c(moby);
            moby[0x20] = 1;
        }
        break;
    case VERSATARGET_STATE_ACTIVE:
        if (MOBY(moby)->oclass == 0x323) {
            struct Hero *hero_ptr = &hero;
            float       *rz = (float *)&MOBY(moby)->rot.z;
            float        yaw = fast_arc_tan(hero_ptr->motion.pos.f[0] - MOBY(moby)->pos.x,
                                     hero_ptr->motion.pos.f[1] - MOBY(moby)->pos.y);
            func_L00_0025CE58_f(rz,
                                yaw,
                                (float *)(moby_data + 0x38),
                                frame_time_sq * 6.2831855f,
                                frame_time_sq * 6.2831855f,
                                frame_time * 12.566371f);
            if (D_L02_0015F5CC & 1) {
                if (((unsigned char *)moby)[0x31] != 0) {
                    float direction_sign = 1.0f;
                    int   particle_lifetime;
                    if (D_L02_0015F5CC & 4)
                        direction_sign = -1.0f;
                    fast_vec_normalize(
                        particle_position, moby + 0xD0, direction_sign * D_L02_00161CBC);
                    fast_vec_normalize(particle_velocity,
                                       particle_position,
                                       -2.0f / scale_time(versatarget_particle_base_lifetime_gp));
                    fast_vec_add(particle_position, particle_position, moby + 0x10);
                    particle_lifetime =
                        truncate_float_to_s32(scale_time(versatarget_particle_base_lifetime_gp)) &
                        0xFF;
                    spawn_moving_hero_relative_alpha_fade_particle(particle_position,
                                                                   particle_velocity,
                                                                   versatarget_particle_color_gp,
                                                                   particle_lifetime,
                                                                   random_integer_below(0xFF) &
                                                                       0xFF,
                                                                   0,
                                                                   versatarget_particle_scale_gp);
                }
            }
        }
        {
            struct Hero *hero_reference = &hero;
            if (hero_reference->state.current != 0x2C)
                try_select_camera_offset_effect_moby(moby);
        }
        break;
    case VERSATARGET_STATE_INACTIVE:
        if (visited52[*(int *)(moby_data + 0x30) + (current_level_index << 4)] != 0 ||
            (*(int *)(moby_data + 0x3C) != -1 &&
             ((linked_moby = (char *)(D_L02_0015FFD8 + (*(int *)(moby_data + 0x3C) << 8))) == 0 ||
              ((unsigned char *)linked_moby)[0x20] == 0xFE ||
              ((unsigned char *)linked_moby)[0x20] == 0xFD))) {
            MOBY(moby)->flags &= 0xFFBE;
            *(int *)(moby_data + 0x30) = -1;
            moby[0x20] = 0;
        }
        break;
    }
}

float D_L03_001619A4 = 3e+01f;
int   D_L03_001619A8 = 0x60408080;
float D_L03_001619AC = 1.25f;
float D_L03_001619B0 = 1.0f;
float D_L04_00161B24 = 3e+01f;
int   D_L04_00161B28 = 0x60408080;
float D_L04_00161B2C = 1.25f;
float D_L04_00161B30 = 1.0f;
float D_L05_00161B84 = 3e+01f;
int   D_L05_00161B88 = 0x60408080;
float D_L05_00161B8C = 1.25f;
float D_L05_00161B90 = 1.0f;
float D_L06_00161AA4 = 3e+01f;
int   D_L06_00161AA8 = 0x60408080;
float D_L06_00161AAC = 1.25f;
float D_L06_00161AB0 = 1.0f;
float D_L07_00161944 = 3e+01f;
int   D_L07_00161948 = 0x60408080;
float D_L07_0016194C = 1.25f;
float D_L07_00161950 = 1.0f;
float D_L09_00161A1C = 3e+01f;
int   D_L09_00161A20 = 0x60408080;
float D_L09_00161A24 = 1.25f;
float D_L09_00161A28 = 1.0f;
float D_L10_00161990 = 3e+01f;
int   D_L10_00161994 = 0x60408080;
float D_L10_00161998 = 1.25f;
float D_L10_0016199C = 1.0f;
float D_L11_00161C00 = 3e+01f;
int   D_L11_00161C04 = 0x60408080;
float D_L11_00161C08 = 1.25f;
float D_L11_00161C0C = 1.0f;
float D_L12_00161B28 = 3e+01f;
int   D_L12_00161B2C = 0x60408080;
float D_L12_00161B30 = 1.25f;
float D_L12_00161B34 = 1.0f;
float D_L13_00161B64 = 3e+01f;
int   D_L13_00161B68 = 0x60408080;
float D_L13_00161B6C = 1.25f;
float D_L13_00161B70 = 1.0f;
float D_L14_00161D1C = 3e+01f;
int   D_L14_00161D20 = 0x60408080;
float D_L14_00161D24 = 1.25f;
float D_L14_00161D28 = 1.0f;
float D_L15_00161CEC = 3e+01f;
int   D_L15_00161CF0 = 0x60408080;
float D_L15_00161CF4 = 1.25f;
float D_L15_00161CF8 = 1.0f;
float D_L16_00161A90 = 3e+01f;
int   D_L16_00161A94 = 0x60408080;
float D_L16_00161A98 = 1.25f;
float D_L16_00161A9C = 1.0f;
float D_L17_00161BD4 = 3e+01f;
int   D_L17_00161BD8 = 0x60408080;
float D_L17_00161BDC = 1.25f;
float D_L17_00161BE0 = 1.0f;
float D_L18_00161D18 = 3e+01f;
int   D_L18_00161D1C = 0x60408080;
float D_L18_00161D20 = 1.25f;
float D_L18_00161D24 = 1.0f;
