/* Level 08 moby class 0x147: water surface; queues its draw and splashes the hero entering the water. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/overlay/shared/rendering/build_interpolated_dma_packet.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "rnc/math/random.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D6C58), where it is exact; names translated to the US level program. */
extern float scale_time(float) __asm__("FUN_001f96b0");

/* Splashes the hero into the water: once inits the water plane, queues the
 * surface draw, and when the hero breaks the surface spawns spray and ripples. */
int          water_entry_plane_initialized_gp __asm__("D_L08_00161970") = 0;
extern float water_entry_plane_data[4] __asm__("D_L08_001D7060");
extern f32   D_L08_0015F580[] MACRO_ADDR;
extern void  FUN_L08_002da0d0(void *);
extern int   FUN_L00_00257b90(int, int);
extern void  spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");
extern unsigned char      *
spawn_splash(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");
void water_surface_update(struct Moby *moby) __asm__("FUN_L08_002da0f0");

float water_entry_plane_data[4] __asm__("D_L08_001D7060") = {-2.6e+02f, -2.2e+02f, 15.0f, 1.6e+05f};

/* Moby class 327 */
void water_surface_update(struct Moby *moby) {
    Vec4         pos;
    Vec4         spray_velocity_or_splash_position;
    struct Moby *water_entry_moby;
    int          i;
    int          spray_random_value;
    float        sc, sp;

    if (water_entry_plane_initialized_gp == 0) {
        water_entry_plane_initialized_gp = 1;
        prepare_scaled_table_with_byte_128(water_entry_plane_data, 0.6666667f);
    }
    add_draw_callback(FUN_L08_002da0d0, moby);
    if (current_level_index != 8)
        return;
    if (!(hero.motion.pos.f[2] < water_entry_plane_data[2]))
        return;
    if (!(water_entry_plane_data[2] <= hero.motion.pos.f[2] - hero.motion.displacement.f[2]))
        return;
    if (voice_is_playing(hero.moby, hero.health.unk22B2)) {
        release_voice_slot(hero.health.unk22B2);
        hero.health.unk22B2 = -1;
    }
    qcopy(&pos, &hero.motion.pos);
    pos.f[2] = water_entry_plane_data[2];
    water_entry_moby = spawn_burst_moby(&pos, 3.0f);
    if (water_entry_moby)
        water_entry_moby->alpha = 0x70;
    for (i = 0; i < 16; i++) {
        float spray_heading_radians = random_angle_radians();
        float k = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
        spray_velocity_or_splash_position.f[0] = fast_cos(spray_heading_radians) * k;
        spray_velocity_or_splash_position.f[1] = fast_sin(spray_heading_radians) * k;
        spray_velocity_or_splash_position.f[2] =
            random_float_between(frame_time * 3.0f, frame_time * 6.5f);
        spray_random_value = FUN_L00_00257b90(0x5A, 0x78);
        spawn_debris_chunk(
            &pos, &spray_velocity_or_splash_position, random_integer_below(2), spray_random_value);
    }
    for (i = 0; i < 16; i++) {
        unsigned char *splash;
        random_vector(spray_velocity_or_splash_position.f, 1.0f, 1.0f);
        fast_vec_add(&spray_velocity_or_splash_position, &spray_velocity_or_splash_position, &pos);
        spray_velocity_or_splash_position.f[2] = water_entry_plane_data[2] + 0.05f;
        sc = random_float_between(0.7f, 1.0f);
        sp = i == 0 ? 2.0f : -2.0f;
        splash = spawn_splash(
            &spray_velocity_or_splash_position, D_L08_0015F580, &water_entry_plane_data[2], sc, sp);
        if (splash)
            *(short *)(splash + 0xA) =
                truncate_float_to_s32(scale_time(random_float_between(30.0f, 60.0f)));
    }
}
