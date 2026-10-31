/* Level 14 morphoray pickup (class record morphoray): update, private state setup and particles. */
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/camera/update_cam.h"
#include "rnc/overlay/l14/mobys/morphoray_globals.h"

void initialize_morphoray_private_state(struct Moby *moby) __asm__("FUN_L14_00305a38");

extern u8 D_0014C050[];

extern u8 D_0013D4D5 NOT_SDA;

extern s32 D_L14_0015F5CC;

extern s32 D_L14_0015F5C4;

void update_morphoray_particles(struct Moby *morphoray_moby) __asm__("FUN_L14_00305b18");

extern void update_morphoray_particles(struct Moby *) __asm__("FUN_L14_00305b18");

f32 morphoray_ground_height_offset_gp __asm__("D_L14_001621E8") = 0.8f;

f32 morphoray_yaw_step_gp __asm__("D_L14_001621EC") = 0.02617994f;

f32 morphoray_position_offset_scale_gp __asm__("D_L14_001621F0") = 0.4f;

extern void start_scene(s32) __asm__("FUN_L00_00298840");

extern void jet_clear_persistent(s32) __asm__("FUN_L00_002502a0");

extern int memcard_save_data(s32, s32) __asm__("FUN_0020b178");

extern void queue_dialogue_message(s32, s32) __asm__("FUN_L00_00263d40");

extern void help_request(s32, s32) __asm__("FUN_L00_00203908");

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

void morphoray_pickup_update(struct Moby *moby) __asm__("FUN_L14_00305758");

/* Moby class 1354 */
void morphoray_pickup_update(struct Moby *moby) {
    enum {
        MORPHORAY_PICKUP_INITIALIZE = 0,
        MORPHORAY_PICKUP_WAIT_FOR_HERO = 1,
        MORPHORAY_PICKUP_WAIT_TO_REMOVE = 2,
        MORPHORAY_PICKUP_APPROACH_ZERO_AND_REMOVE = 3
    };
    u8 *moby_bytes = (u8 *)moby;
    u8 *morphoray_pvars = (u8 *)moby->pvars;
    f32 position_offset[4];
    u8  state = moby->state;

    switch (state) {
    case MORPHORAY_PICKUP_INITIALIZE:
        if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            return;
        }
        initialize_morphoray_private_state((void *)moby);
        probe_ground_height(moby_bytes + 0x10, 0, 0.5f);
        moby->rot.x = 0.0f;
        moby->rot.y = -1.5707964f;
        moby->pos.z = D_L14_00174568[0] + morphoray_ground_height_offset_gp;
        qcopy(morphoray_pvars, moby_bytes + 0x10);
        moby->state = 1;
        break;
    case MORPHORAY_PICKUP_WAIT_FOR_HERO:
        *(f32 *)(moby_bytes + 0x48) =
            fast_add_rotations(*(f32 *)(moby_bytes + 0x48), morphoray_yaw_step_gp);
        qcopy(moby_bytes + 0x10, morphoray_pvars);
        fast_vec_scale(position_offset, moby_bytes + 0xd0, morphoray_position_offset_scale_gp);
        fast_vec_add(moby_bytes + 0x10, moby_bytes + 0x10, position_offset);
        update_morphoray_particles(moby);
        if (D_L14_0015F5CC % 10 != 0)
            return;
        if (!(vector_distance_xy(moby_bytes + 0x10, hero.motion.pos.f) < 1.0f))
            return;
        if (!(absolute_float(*(f32 *)(moby_bytes + 0x18) - hero.motion.pos.f[2]) < 2.0f))
            return;
        if (D_0013D4D5 != 0)
            *(s32 *)(morphoray_pvars + 0x18) = state;
        else
            *(s32 *)(morphoray_pvars + 0x18) = 0;
        grant_inventory_item(0x15, 1);
        start_scene(6);
        moby->state = 2;
        break;
    case MORPHORAY_PICKUP_WAIT_TO_REMOVE:
        if (D_L14_0015F5C4 != state) {
            if (D_0014C050[moby->mission + current_level_index * 16] != 0xFF) {
                jet_clear_persistent(moby->mission);
                memcard_save_data(0, -1);
            }
            if (*(s32 *)(morphoray_pvars + 0x18) != 0)
                queue_dialogue_message(0x53e0, -1);
            else
                queue_dialogue_message(0x36b2, -1);
            help_request(0x36b0, 0x7a);
            mark_moby_for_removal(moby);
            return;
        }
        *(u16 *)(moby_bytes + 0x34) |= 0x41;
        break;
    case MORPHORAY_PICKUP_APPROACH_ZERO_AND_REMOVE:
        approach_value(
            (f32 *)(moby_bytes + 0x2c), 0.0f, *(f32 *)(*(u8 **)(moby_bytes + 0x24) + 0x24) * 0.02f);
        if (*(f32 *)(moby_bytes + 0x2c) == 0.0f)
            mark_moby_for_removal(moby);
        break;
    }
}

typedef struct {
    u8  p0[0x20];
    f32 r[4];
    f32 a[4];
    s32 c[4];
    f32 x40, x44, x48, x4c;
} MorphorayPrivateState;

void initialize_morphoray_private_state(struct Moby *moby) {
    MorphorayPrivateState *m = (MorphorayPrivateState *)moby->pvars;
    s32                    i;
    for (i = 0; i < 4; i++)
        m->r[i] = random_integer_below(0xFF);
    m->a[0] = -1.0f;
    m->a[1] = -2.25f;
    m->a[2] = 1.25f;
    m->a[3] = 2.5f;
    for (i = 0; i < 4; i++)
        m->c[i] = scale_game_frames(i * 0x40 + 0x3F);
    m->x40 = 2.0f;
    m->x44 = 2.5f;
    m->x4c = 2.0f;
    m->x48 = 2.5f;
}

typedef struct {
    float pad[2];
    float z;
    float pad2[5];
    float a[4];
    float b[4];
    int   t[4];
    float c[4];
} FxData14;

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

extern unsigned char *
FUN_L00_00272f68(float f, void *pos, int a1, unsigned char a2, int idx, int flag, int s, int mode);

void update_morphoray_particles(struct Moby *morphoray_moby) {
    float     particle_position_and_step[8];
    FxData14 *effect_data = (FxData14 *)morphoray_moby->pvars;
    float    *a;
    int      *t;
    int       i, j, c, k;
    float     unwrapped_particle_value, elapsed_timer_fraction;
    j = 0;
    effect_data->z += 0.1f;
    fast_vec_sub(particle_position_and_step, D_L14_001674C0, effect_data);
    t = effect_data->t;
    a = effect_data->a;
    fast_vec_normalize(particle_position_and_step, particle_position_and_step, -0.3f);
    fast_vec_normalize(&particle_position_and_step[4], particle_position_and_step, 0.1f);
    fast_vec_add(particle_position_and_step, particle_position_and_step, effect_data);
    effect_data->z -= 0.1f;
    for (i = 3; i >= 0; i--) {
        unwrapped_particle_value = *a + effect_data->b[j];
        *a = unwrapped_particle_value;
        if (unwrapped_particle_value >= 255.0f) {
            *a = unwrapped_particle_value - 255.0f;
        } else if (unwrapped_particle_value <= 0.0f) {
            *a = unwrapped_particle_value + 255.0f;
        }
        if (fast_dec_timer(t)) {
            effect_data->t[j] = scale_game_frames(255);
        }
        t++;
        elapsed_timer_fraction =
            convert_integer_to_float(scale_game_frames(255) - effect_data->t[j]);
        elapsed_timer_fraction = elapsed_timer_fraction / (float)scale_game_frames(255);
        c = fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - elapsed_timer_fraction));
        k = truncate_float_to_s32(*a++);
        FUN_L00_00272f68(effect_data->c[j], particle_position_and_step, c, k, 0x35, 1, 2, 0);
        j++;
        fast_vec_add(
            particle_position_and_step, particle_position_and_step, &particle_position_and_step[4]);
    }
}

extern int D_L14_0015F5C4;

extern void queue_dialogue_message(int message_id,
                                   int message_duration_frames) __asm__("FUN_L00_00263d40");
