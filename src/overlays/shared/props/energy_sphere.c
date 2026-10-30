/* Shared moby class energy_sphere: breaks into particles when destroyed. */

#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/entities.h"

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          unk2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          unk48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            unk90;
} Obj;

extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void  random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern float fast_add_rotations(float, float);
extern void  spawn_moving_hero_relative_alpha_fade_particle(
     void *, float, void *, int, int, int, int) __asm__("FUN_L00_002730e0");
extern int  voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void spawn_energy_sphere_destruction_particles(char *m) __asm__("FUN_L06_002f7c68");

extern float D_0015ED6C;
extern f32   D_0015ED6C;
extern float energy_sphere_rotation_rate_x_degrees_gp __asm__("D_L06_00161E74")
    __attribute__((sda));
extern float energy_sphere_rotation_rate_y_degrees_gp __asm__("D_L06_00161E78")
    __attribute__((sda));
extern float energy_sphere_rotation_rate_z_degrees_gp __asm__("D_L06_00161E7C")
    __attribute__((sda));
extern float D_L06_00161E80 __attribute__((sda));
extern float energy_sphere_destruction_speed_range_start_gp __asm__("D_L06_00161E84")
    __attribute__((sda));
extern float energy_sphere_destruction_speed_range_end_gp __asm__("D_L06_00161E88")
    __attribute__((sda));
extern int energy_sphere_destruction_nonzero_branch_color_start_gp __asm__("D_L06_00161E98")
    __attribute__((sda));
extern int energy_sphere_destruction_nonzero_branch_color_end_gp __asm__("D_L06_00161E9C")
    __attribute__((sda));
extern int energy_sphere_destruction_zero_branch_color_start_gp __asm__("D_L06_00161EA0")
    __attribute__((sda));
extern int energy_sphere_destruction_zero_branch_color_end_gp __asm__("D_L06_00161EA4")
    __attribute__((sda));
extern float energy_sphere_destruction_scale_range_start_gp __asm__("D_L06_00161EA8")
    __attribute__((sda));
extern float energy_sphere_destruction_scale_range_end_gp __asm__("D_L06_00161EAC")
    __attribute__((sda));
extern unsigned char l06_energy_sphere_destruction_particle_lifetime __asm__("D_L06_00161E94")
    __attribute__((sda));

float         D_L10_00161C30 = 5.8e+02f;
float         D_L10_00161C34 = 1.8e+02f;
float         D_L10_00161C38 = 1.4e+02f;
float         D_L10_00161C3C = 3.0f;
float         D_L10_00161C40 = 1.0f;
float         D_L10_00161C44 = 2.0f;
int           D_L10_00161C54 = 0x5040c0c0;
int           D_L10_00161C58 = 0x40008080;
int           D_L10_00161C5C = 0x50804040;
int           D_L10_00161C60 = 0x40c02020;
float         D_L10_00161C64 = 0.5f;
float         D_L10_00161C68 = 0.3f;
unsigned char D_L10_00161C50 = 0x14;
float         D_L17_00161E00 = 5.8e+02f;
float         D_L17_00161E04 = 1.8e+02f;
float         D_L17_00161E08 = 1.4e+02f;
float         D_L17_00161E0C = 3.0f;
float         D_L17_00161E10 = 1.0f;
float         D_L17_00161E14 = 2.0f;
int           D_L17_00161E24 = 0x5040c0c0;
int           D_L17_00161E28 = 0x40008080;
int           D_L17_00161E2C = 0x50804040;
int           D_L17_00161E30 = 0x40c02020;
float         D_L17_00161E34 = 0.5f;
float         D_L17_00161E38 = 0.3f;
unsigned char D_L17_00161E20 = 0x14;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F8EE8), where it is exact; names translated to the US level program. */
void energy_sphere_update(struct Moby *m) __asm__("FUN_L06_002f7ab8");

void energy_sphere_update(struct Moby *sphere_moby) {
    enum {
        ENERGY_SPHERE_INITIALIZE = 0,
        ENERGY_SPHERE_UPDATE_ROTATION = 1
    };
    char *sphere_vars = (char *)sphere_moby->pvars;
    if (find_moby_hit(sphere_moby, 0x330000, 0) != 0) {
        int voice_slot_index;
        spawn_energy_sphere_destruction_particles(sphere_moby);
        allocate_voice_for_target_entry(1, 0, (int)sphere_moby);
        voice_slot_index = *(int *)(sphere_vars + 0x60);
        if (voice_slot_index != -1) {
            char *voice_slot = ((char *)&voice_pool) + voice_slot_index * 0x70;
            if (*(char **)(voice_slot + 0x88) == sphere_moby &&
                *(unsigned char *)(voice_slot + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(int *)(sphere_vars + 0x60) = -1;
        mark_moby_for_removal(sphere_moby);
        return;
    }
    switch (sphere_moby->state) {
    case ENERGY_SPHERE_INITIALIZE:
        set_moby_color(sphere_moby, 0x80, 0x80, 0x80);
        sphere_moby->scale = sphere_moby->pclass->scale * D_L06_00161E80;
        *(char **)sphere_vars = sphere_vars + 0x20;
        sphere_moby->state = 1;
        sphere_moby->flags |= 0x20;
        *(int *)(sphere_vars + 0x60) = -1;
    case ENERGY_SPHERE_UPDATE_ROTATION: {
        float rotation_delta_x = energy_sphere_rotation_rate_x_degrees_gp * DEG_TO_RAD * D_0015ED6C;
        float rotation_delta_y, rotation_delta_z;
        float x = fast_add_rotations(sphere_moby->rot.x, rotation_delta_x);
        rotation_delta_y = energy_sphere_rotation_rate_y_degrees_gp * DEG_TO_RAD * D_0015ED6C;
        sphere_moby->rot.x = x;
        x = fast_add_rotations(sphere_moby->rot.y, rotation_delta_y);
        rotation_delta_z = energy_sphere_rotation_rate_z_degrees_gp * DEG_TO_RAD * D_0015ED6C;
        sphere_moby->rot.y = x;
        sphere_moby->rot.z = fast_add_rotations(sphere_moby->rot.z, rotation_delta_z);
    }
    }
    if (voice_is_playing(sphere_moby, *(int *)(sphere_vars + 0x60)) == 0) {
        *(int *)(sphere_vars + 0x60) = allocate_voice_for_target_entry(0, 4, (int)sphere_moby);
    }
}

/* Spawn forty particles at the moby with randomised parameters. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F9098), where it is exact; names translated to the US level program. */
void spawn_energy_sphere_destruction_particles(char *moby) {
    float particle_velocity[4];
    int   i;
    for (i = 0x27; i >= 0; i--) {
        float particle_scale;
        int   particle_color;
        int   particle_lifetime;
        int   random_particle_byte;
        random_vector(particle_velocity,
                      energy_sphere_destruction_speed_range_start_gp * frame_time,
                      energy_sphere_destruction_speed_range_end_gp * frame_time);
        particle_scale = random_float_between(energy_sphere_destruction_scale_range_start_gp,
                                              energy_sphere_destruction_scale_range_end_gp);
        if (random_integer_below(2) != 0) {
            particle_color =
                fast_tween_color(energy_sphere_destruction_nonzero_branch_color_start_gp,
                                 energy_sphere_destruction_nonzero_branch_color_end_gp,
                                 random_float_between(0.0f, 1.0f));
        } else {
            particle_color = fast_tween_color(energy_sphere_destruction_zero_branch_color_start_gp,
                                              energy_sphere_destruction_zero_branch_color_end_gp,
                                              random_float_between(0.0f, 1.0f));
        }
        particle_lifetime = l06_energy_sphere_destruction_particle_lifetime;
        random_particle_byte = random_integer_below(0xFF);
        spawn_moving_hero_relative_alpha_fade_particle(moby + 0x10,
                                                       particle_scale,
                                                       particle_velocity,
                                                       particle_color,
                                                       particle_lifetime,
                                                       (unsigned char)random_particle_byte,
                                                       0);
    }
}

int energy_sphere_destruction_nonzero_branch_color_end_gp __asm__("D_L06_00161E9C") = 0x40008080;
