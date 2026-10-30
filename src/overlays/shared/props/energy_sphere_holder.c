/* Shared moby class energy_sphere_holder: holds an energy sphere and draws its beam effects. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/props/energy_sphere_holder_data/update_energy_sphere_holder_beam_effects.h"

typedef struct {
    float ph[4];
    float spd[4];
    int   t[4];
    float sz[4];
    char *tgt;
} Beam;

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int   scale_game_frames(int);
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void initialize_energy_sphere_holder_vars(char *) __asm__("FUN_L06_002f7548");
void        update_energy_sphere_holder_beam_effects(char *moby) __asm__("FUN_L06_002f7628");
extern unsigned char *func_L00_00273E08_g(
    void *, int, unsigned char, int, int, int, int, float) __asm__("FUN_L00_00272f68");
extern void
func_L00_00273F80_g(void *, void *, int, int, int, int, float) __asm__("FUN_L00_002730e0");

extern int  level_frame_count __asm__("D_L06_0015F5CC");
extern char D_L06_00167540[];
extern s32  level_frame_count __asm__("D_L06_0015F5CC");
extern int  energy_sphere_holder_completion_bits[] __asm__("D_L06_001BAC50");
extern int  energy_sphere_holder_beam_color_start_gp __asm__("D_L06_00161E58") __attribute__((sda));
extern int  energy_sphere_holder_beam_color_end_gp __asm__("D_L06_00161E5C") __attribute__((sda));
extern int  energy_sphere_holder_beam_texture_index_gp __asm__("D_L06_00161E64")
    __attribute__((sda));
extern int   energy_sphere_holder_spark_color_gp __asm__("D_L06_00161E68") __attribute__((sda));
extern float energy_sphere_holder_spark_size_scale_gp __asm__("D_L06_00161E6C")
    __attribute__((sda));
extern int energy_sphere_holder_spark_lifetime_frames_gp __asm__("D_L06_00161E70")
    __attribute__((sda));

int   D_L10_00161C14 = 0x40ff4040;
int   D_L10_00161C18 = 0x10208080;
int   D_L10_00161C20 = 0x35;
int   D_L10_00161C24 = 0x30ff3040;
float D_L10_00161C28 = 0.5f;
int   D_L10_00161C2C = 0x14;
int   D_L17_00161DE4 = 0x40ff4040;
int   D_L17_00161DE8 = 0x10208080;
int   D_L17_00161DF0 = 0x35;
int   D_L17_00161DF4 = 0x30ff3040;
float D_L17_00161DF8 = 0.5f;
int   D_L17_00161DFC = 0x14;

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F86B8), where it is exact; names translated to the US level program. */
void energy_sphere_holder_update(char *m) __asm__("FUN_L06_002f7288");

void energy_sphere_holder_update(char *holder_moby) {
    enum {
        ENERGY_SPHERE_HOLDER_INITIALIZE = 0,
        ENERGY_SPHERE_HOLDER_HANDLE_HITS_AND_UPDATE_BEAM = 1
    };
    char          *holder_vars = ((char *)MOBY(holder_moby)->pvars);
    char          *spawned_moby;
    unsigned char *visited_table;
    switch (((unsigned char *)holder_moby)[0x20]) {
    case ENERGY_SPHERE_HOLDER_INITIALIZE:
        if ((*(int *)(current_level_index * 256 + ((short)MOBY(holder_moby)->save_id >> 5) * 4 +
                      (char *)D_0014C190_257218) >>
                 (MOBY(holder_moby)->save_id & 0x1F) &
             1) &&
            (current_level_index != 10 ||
             (visited_table = (unsigned char *)((char *)visited52),
              (visited_table + ((unsigned char *)holder_moby)[0xB0]))[0xA0] == 0xFF)) {
            holder_moby[0x20] = 2;
            holder_moby[0xBC] = 1;
            break;
        }
        spawned_moby = create_moby(0x410);
        spawned_moby[0x30] = 0x40;
        spawned_moby[0x31] = 1;
        MOBY(spawned_moby)->draw_dist = 0x40;
        MOBY(spawned_moby)->spawn_frame = ((long)MOBY(holder_moby)->spawn_frame);
        MOBY(spawned_moby)->flags = MOBY(holder_moby)->flags | 0x5000;
        qcopy(spawned_moby + 0x10, holder_moby + 0x10);
        MOBY(spawned_moby)->rot.x = random_angle_radians();
        MOBY(spawned_moby)->rot.y = random_angle_radians();
        MOBY(spawned_moby)->rot.z = random_angle_radians();
        MOBY(spawned_moby)->pos.z += 0.85f;
        *(char **)(holder_vars + 0x40) = spawned_moby;
        holder_moby[0x20] = 1;
        initialize_energy_sphere_holder_vars(holder_moby);
        *(int *)(current_level_index * 256 + ((short)MOBY(holder_moby)->save_id >> 5) * 4 +
                 (char *)D_0014C190_257218) &= ~(1 << (MOBY(holder_moby)->save_id & 0x1F));
        energy_sphere_holder_completion_bits[(short)MOBY(holder_moby)->save_id >> 5] &=
            ~(1 << (MOBY(holder_moby)->save_id & 0x1F));
        break;
    case ENERGY_SPHERE_HOLDER_HANDLE_HITS_AND_UPDATE_BEAM: {
        char *hit = find_moby_hit(holder_moby, 0x330000, 0);
        if (hit != 0) {
            spawn_nearby_effect(*(void **)(holder_vars + 0x40),
                                *(int *)(hit + 0x20),
                                0x10000,
                                hit,
                                hit + 0x10,
                                1.0f);
            ((unsigned char *)holder_moby)[0xA4] = 0xFF;
        }
        if ((*(char **)(holder_vars + 0x40))[0x20] < 0) {
            holder_moby[0xBC] = 1;
            holder_moby[0x20] = 2;
            *(int *)(current_level_index * 256 + ((short)MOBY(holder_moby)->save_id >> 5) * 4 +
                     (char *)D_0014C190_257218) |= 1 << (MOBY(holder_moby)->save_id & 0x1F);
            energy_sphere_holder_completion_bits[(short)MOBY(holder_moby)->save_id >> 5] |=
                1 << (MOBY(holder_moby)->save_id & 0x1F);
        } else {
            update_energy_sphere_holder_beam_effects(holder_moby);
        }
        break;
    }
    }
}

/* Draws the moby's light beam: four flickering glow sprites spaced from its target toward the camera,
 * plus, every fourth frame while visible, a spark drifting up or down from the moby. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F8A58), where it is exact; names translated to the US level program. */
void update_energy_sphere_holder_beam_effects(char *moby) {
    float beam_sprite_position[4];
    float step[4];
    float spark_position[4];
    float spark_velocity[4];
    Beam *beam = ((Beam *)MOBY(moby)->pvars);
    int   i;
    fast_vec_sub(beam_sprite_position, D_L06_00167540, beam->tgt + 0x10);
    fast_vec_normalize(beam_sprite_position, beam_sprite_position, -0.3f);
    fast_vec_normalize(step, beam_sprite_position, 0.1f);
    fast_vec_add(beam_sprite_position, beam_sprite_position, beam->tgt + 0x10);
    for (i = 0; i < 4; i++) {
        float timer_progress;
        int   beam_sprite_color;
        beam->ph[i] += beam->spd[i];
        if (255.0f <= beam->ph[i]) {
            beam->ph[i] -= 255.0f;
        } else if (beam->ph[i] <= 0.0f) {
            beam->ph[i] += 255.0f;
        }
        if (fast_dec_timer(&beam->t[i])) {
            beam->t[i] = scale_game_frames(0xFF);
        }
        timer_progress = convert_integer_to_float(scale_game_frames(0xFF) - beam->t[i]) /
                         (float)scale_game_frames(0xFF);
        beam_sprite_color = fast_tween_color(energy_sphere_holder_beam_color_start_gp,
                                             energy_sphere_holder_beam_color_end_gp,
                                             absolute_float(0.5f - timer_progress));
        func_L00_00273E08_g(beam_sprite_position,
                            beam_sprite_color,
                            truncate_float_to_s32(beam->ph[i]),
                            energy_sphere_holder_beam_texture_index_gp,
                            energy_sphere_holder_beam_particle_variant_flag_gp,
                            2,
                            0,
                            beam->sz[i]);
        fast_vec_add(beam_sprite_position, beam_sprite_position, step);
    }
    if ((level_frame_count & 3) == 3 && ((unsigned char *)moby)[0x31] != 0) {
        float sgn = 1.0f;
        if (level_frame_count & 4)
            sgn = -1.0f;
        clear_u64_value(spark_position);
        spark_position[2] += sgn * 0.9f;
        fast_vec_normalize(
            spark_velocity,
            spark_position,
            -2.0f / (float)scale_game_frames(energy_sphere_holder_spark_lifetime_frames_gp));
        fast_vec_add(spark_position, spark_position, moby + 0x10);
        spark_position[2] += 0.85f;
        func_L00_00273F80_g(spark_position,
                            spark_velocity,
                            energy_sphere_holder_spark_color_gp,
                            (unsigned char)truncate_float_to_s32((float)scale_game_frames(
                                energy_sphere_holder_spark_lifetime_frames_gp)),
                            (unsigned char)random_integer_below(0xFF),
                            0,
                            energy_sphere_holder_spark_size_scale_gp);
    }
}

int energy_sphere_holder_beam_color_end_gp __asm__("D_L06_00161E5C") = 0x10208080;
