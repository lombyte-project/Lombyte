/* Shared moby classes: trespasser, swingshot_hook, doom_bot, mine_glove, bomb_glove, decoy, devestator_ammo, swingshot, glove_of_doom. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/quad.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/refresh_moby_spatial_bounds_from_basis.h"
#include "rnc/overlay/shared/enemies/trooper_play_sound.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/overlay/shared/math/vectors/fast_subtract_rotations_cf.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C2A20), where it is exact; names translated to the US level program. */

extern unsigned char D_0013A4E0[];

#include "rnc/data_labels.h"

extern void update_trespasser_pickup(void *) __asm__("FUN_L00_002c1778");

/* Runs FUN_L00_002c1778 when the moby data word 4 is set, else sets byte 0xBC from a mask test. */
void trespasser_update(struct Moby *m) __asm__("FUN_L00_002c1718");

void trespasser_update(struct Moby *trespasser_moby) {
    if (*(int *)(((char *)trespasser_moby->pvars) + 4) != 0) {
        update_trespasser_pickup(trespasser_moby);
    } else if (*(int *)(D_0013A4E0 + 0x2600) & *(int *)(((unsigned char *)&D_0013E533) + 0x1EBD)) {
        trespasser_moby->command = 1;
    } else {
        trespasser_moby->command = 0;
    }
}

extern float FUN_001f9b48_2c1928(void *, void *) __asm__("FUN_001f9b48");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001f9bf8_2c1928(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001f9b80_2c1928(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_2c1928(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f9988_2c1928(float) __asm__("FUN_001f9988");
extern float FUN_001f9dc8_2c1928(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2c1928(float) __asm__("FUN_001f9de0");

void compute_mine_launch_velocity(float   *origin_position,
                                  OvlQuad *target_position,
                                  float   *out,
                                  float    gravity,
                                  float    min_horizontal_distance,
                                  float    max_range) __asm__("FUN_L00_002c1928");

void compute_mine_launch_velocity(float   *origin_position,
                                  OvlQuad *target_position,
                                  float   *out,
                                  float    gravity,
                                  float    min_horizontal_distance,

                                  float max_range) {
    OvlQuad effective_target_position[1];
    float   half = gravity * max_range * 0.5f;
    float   effective_horizontal_distance, dz, launch_heading, launch_component_speed_squared,
        launch_component_speed;

    if (max_range < FUN_001f9b48_2c1928(origin_position, target_position)) {
        fast_vec_sub(effective_target_position, target_position, origin_position);
        FUN_001f9bf8_2c1928(
            effective_target_position, effective_target_position, max_range - 0.01f);
        fast_vec_add(effective_target_position, effective_target_position, origin_position);
    } else {
        effective_target_position[0] = *target_position;
    }
    effective_horizontal_distance = FUN_001f9b80_2c1928(origin_position, effective_target_position);
    dz = ((float *)effective_target_position)[2] - origin_position[2];
    launch_heading =
        FUN_001f9e90_2c1928(((float *)effective_target_position)[0] - origin_position[0],
                            ((float *)effective_target_position)[1] - origin_position[1]);
    if (effective_horizontal_distance < min_horizontal_distance)
        effective_horizontal_distance = min_horizontal_distance;
    if (effective_horizontal_distance <= dz)
        dz = effective_horizontal_distance - 0.01f;
    launch_component_speed_squared =
        gravity * effective_horizontal_distance * effective_horizontal_distance /
        ((effective_horizontal_distance - dz) + (effective_horizontal_distance - dz));
    if (half < launch_component_speed_squared)
        launch_component_speed_squared = half;
    launch_component_speed = FUN_001f9988_2c1928(launch_component_speed_squared);
    out[0] = FUN_001f9dc8_2c1928(launch_heading) * launch_component_speed;
    out[1] = FUN_001f9de0_2c1928(launch_heading) * launch_component_speed;
    out[2] = launch_component_speed;
}

#include "qcopy.h"

int        rand(void) __asm__("FUN_001160d8");
int        FUN_001f96f8_2c3440(int) __asm__("FUN_001f96f8");
void       spawn_particle(float *,
                          unsigned char,
                          unsigned char,
                          unsigned char,
                          float,
                          float,
                          int) __asm__("FUN_L00_00269b70");
void       mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern s16 D_L00_001B0830_lui[] __asm__("D_L00_001B0830") NOT_SDA;
void       destroy_decoy_with_particles(unsigned char *moby) __asm__("FUN_L00_002c3440");

void destroy_decoy_with_particles(unsigned char *moby) {
    float         decoy_particle_position[4] __attribute__((aligned(16)));
    int           i;
    int           r, g;
    unsigned char b;
    float         randomized_scalar;
    for (i = 0; i < 10; i++) {
        qcopy(decoy_particle_position, moby + 0x10);
        decoy_particle_position[0] += random_float_between(-0.2f, 0.2f);
        decoy_particle_position[1] += random_float_between(-0.2f, 0.2f);
        decoy_particle_position[2] += random_float_between(-0.2f, 0.2f);
        r = (rand() + 0x30) & 0x3F;
        g = (rand() + 0x20) & 0x3F;
        b = rand() & 0x2F;
        randomized_scalar = random_float_between(100000.0f, 600000.0f);
        spawn_particle(decoy_particle_position,
                       r,
                       g,
                       b,
                       randomized_scalar,
                       0.0f,
                       FUN_001f96f8_2c3440(rand() % 40 + 10));
    }
    mine_spawn_drops(moby, D_L00_001B0830_lui);
    mark_moby_for_removal(moby);
}

typedef struct {
    u8  pad[0x60];
    f32 f60;
    f32 f64;
    s32 i68;
} O_2c40a0;

typedef struct {
    u8        pad[0x10];
    u8        v10[8];
    f32       f18;
    u8        pad2[0x78 - 0x1C];
    O_2c40a0 *p78;
} S_2c40a0;

typedef struct {
    u8  pad[0x80];
    u8  v80[8];
    f32 f88;
    u8  pad2[0x1E2 - 0x8C];
    s16 h1E2;
} P_2c40a0;

extern P_2c40a0 D_0013F350_2c40a0 __asm__("D_0013F350");
f32             FUN_001f9b80_2c40a0(void *, void *) __asm__("FUN_001f9b80");
f32             FUN_001f99c0_2c40a0(f32) __asm__("FUN_001f99c0");
void            FUN_001f9a28_2c40a0(void *, void *, void *) __asm__("FUN_001f9a28");
f32             FUN_001f9b20_2c40a0(void *) __asm__("FUN_001f9b20");
f32             FUN_001f9af0_2c40a0(void *) __asm__("FUN_001f9af0");
f32             vendor_spring(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
f32             FUN_00213508_2c40a0(void *, f32, s32) __asm__("FUN_00213508");
void            FUN_001f9bf8_2c40a0(void *, void *, f32) __asm__("FUN_001f9bf8");
void FUN_L00_00258830_2c40a0(void *, void *, f32, f32, f32, int) __asm__("FUN_L00_00258830");
void update_decoy_proximity_motion(S_2c40a0 *self) __asm__("FUN_L00_002c40a0");

void update_decoy_proximity_motion(S_2c40a0 *self) {
    O_2c40a0 *moby_vars = self->p78;
    P_2c40a0 *P = &D_0013F350_2c40a0;
    s16       proximity_response_gate = P->h1E2;
    f32       horizontal_displacement[4];
    f32       horizontal_separation_error, horizontal_separation_distance, ground_height;
    if (moby_vars->i68 == 0) {
        if (proximity_response_gate == 0)
            return;
        if (!(FUN_001f9b80_2c40a0(P->v80, self->v10) < 1.0f))
            return;
        if (!(FUN_001f99c0_2c40a0(P->f88 - self->f18) < 1.3f))
            return;
        moby_vars->i68 = 1;
    }
    FUN_001f9a28_2c40a0(horizontal_displacement, self->v10, P->v80);
    horizontal_displacement[2] = 0.0f;
    horizontal_separation_error = FUN_001f9b20_2c40a0(horizontal_displacement) - 1.4f;
    if (horizontal_separation_error > 0.0f)
        horizontal_separation_error = 0.0f;
    if (proximity_response_gate == 0)
        horizontal_separation_error = 0.0f;
    vendor_spring(&horizontal_separation_error,
                  0.0f,
                  &moby_vars->f60,
                  frame_scale_sq * 0.02f,
                  frame_scale_sq * 0.3f,
                  frame_time * 4.0f);
    if (horizontal_separation_error == 0.0f && moby_vars->f60 == 0.0f) {
        if (FUN_001f99c0_2c40a0(self->f18 - FUN_00213508_2c40a0(self->v10, 0.5f, 0)) < 0.02f)
            moby_vars->i68 = 0;
    }
    horizontal_separation_distance = FUN_001f9af0_2c40a0(horizontal_displacement);
    if (horizontal_separation_distance < moby_vars->f60)
        moby_vars->f60 = horizontal_separation_distance;
    if (moby_vars->f60 < horizontal_separation_distance)
        FUN_001f9bf8_2c40a0(horizontal_displacement, horizontal_displacement, moby_vars->f60);
    FUN_L00_00258830_2c40a0(self, horizontal_displacement, 0.5f, 0.4f, 0.0f, 0);
    ground_height = FUN_00213508_2c40a0(self->v10, 0.5f, 0);
    if (self->f18 < ground_height) {
        self->f18 = ground_height;
        moby_vars->f64 = 0.0f;
        return;
    }
    moby_vars->f64 -= frame_time_sq * 24.0f;
    self->f18 += moby_vars->f64;
    if (self->f18 < ground_height)
        self->f18 = ground_height;
}

s32  collision_hit_material() __asm__("FUN_001f0b58");
void destroy_decoy_with_particles();
/* Calls FUN_L00_002c3440 on m for collision_hit_material results 0, 1, 3, 8, 11, 12 and 13. */
void destroy_decoy_on_selected_materials(void *moby) __asm__("FUN_L00_002c4328");

void destroy_decoy_on_selected_materials(void *moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    switch (collision_hit_material()) {
    case COLLISION_MATERIAL_WATER:
    case 1:
    case 3:
    case 8:
    case 11:
    case 12:
    case 13:
        destroy_decoy_with_particles(moby);
        break;
    }
}

#include "eetypes.h"
#include "qcopy.h"
#include "rnc/overlay/item_defs.h"

extern ItemDef item_defs_lui[] __asm__("D_L00_00179AC0") NOT_SDA;
int            FUN_001f96f8_002c52a8(int) __asm__("FUN_001f96f8");
unsigned char *spawn_on_path(u128 *a, int b, int c, int d) __asm__("FUN_L00_002c52a8");

unsigned char *spawn_on_path(u128 *spawn_position, int weapon_index, int ammo_amount, int d) {
    unsigned char *spawned_moby = 0;
    {
        ItemDef *spawn_config_entry = (ItemDef *)(weapon_index * 0x4C + (u8 *)item_defs_lui);
        if (spawn_config_entry->ammo_pickup_cls > 0)
            spawned_moby = create_moby(spawn_config_entry->ammo_pickup_cls);
    }
    if (spawned_moby) {
        int *moby_vars;
        spawned_moby[0x30] = 0xff;
        *(short *)(spawned_moby + 0x32) = 0xff;
        moby_vars = *(int **)(spawned_moby + 0x78);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        spawned_moby[0xbc] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        if (ammo_amount == -1)
            moby_vars[0] = ((VendorItemInfo *)(weapon_index * 0x18 + (u8 *)D_L00_001C40B0))->hC;
        else
            moby_vars[0] = ammo_amount;
        moby_vars[2] = weapon_index;
        *(float *)(moby_vars + 3) = (float)d;
        moby_vars[8] = FUN_001f96f8_002c52a8(0x10);
        *(float *)(moby_vars + 10) = *(float *)(spawned_moby + 0x18);
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
    }
    return spawned_moby;
}

extern float D_0015ED60_2c5be8 __asm__("D_0015ED60");
extern float D_0015ED6C_2c5be8 __asm__("D_0015ED6C");
extern int   D_15F5CC_2c5be8 __asm__("D_L00_0015F5CC");
extern int   add_ammo(int, int) __asm__("FUN_L00_00233e40");
extern void  queue_formatted_dialogue_message(int, int, int) __asm__("FUN_L00_00263db0");
extern void  clear_u64_value(void *) __asm__("FUN_001f99f8");
extern int   f99a8_2c5be8(int) __asm__("FUN_001f99a8");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");

extern int weapon_ammo_pickup_totals[] __asm__("D_0013DE08");
extern int D_L00_00161838;
int        try_collect_ammo_pickup(struct Moby *pickup_moby) __asm__("FUN_L00_002c5be8");

int try_collect_ammo_pickup(struct Moby *pickup_moby) {
    int  *pickup_vars = ((int *)pickup_moby->pvars);
    int   before;
    float excess_ammo_amount;
    int   ammo_added;

    if (pickup_moby->state == 2) {
    fail:
        return 0;
    }
    before = weapon_ammo_counts[pickup_vars[2]];
    excess_ammo_amount = add_ammo(pickup_vars[2], pickup_vars[0]);
    if (before >= weapon_ammo_counts[pickup_vars[2]]) {
        goto fail;
    }
    ammo_added = weapon_ammo_counts[pickup_vars[2]] - before;
    queue_formatted_dialogue_message(
        ammo_added == 1
            ? ((ItemDef *)(pickup_vars[2] * 0x4C + (char *)item_defs_lui))->pickup_message_single
            : ((ItemDef *)(pickup_vars[2] * 0x4C + (char *)item_defs_lui))->pickup_message_multiple,
        ammo_added,
        -1);
    if (excess_ammo_amount < (float)pickup_vars[0]) {
        weapon_ammo_pickup_totals[pickup_vars[2]] += weapon_ammo_counts[pickup_vars[2]] - before;
        pickup_moby->state = 2;
        pickup_moby->scale *= D_0015ED60_2c5be8 * 0.200000048f + 1.0f;
        clear_u64_value(pickup_vars + 4);
        {
            int event_counter_delta = D_15F5CC_2c5be8 - D_L00_00161838;
            int event_gate_threshold;
            *(float *)(pickup_vars + 9) = D_0015ED6C_2c5be8 * 8.0f;
            event_gate_threshold = f99a8_2c5be8(event_counter_delta);
            if (scale_game_frames(10) < event_gate_threshold) {
                trooper_play_sound(0, 0, pickup_moby, 0xD5);
                D_L00_00161838 = D_15F5CC_2c5be8;
            }
        }
    }
    return 1;
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2c5dc8;

typedef struct {
    float u, v;
} UV;

typedef struct {
    V_2c5dc8     corner[4];
    unsigned int color[4];
    UV           uv[4];
    long         x70, tex, x80, x88;
} Quad;

typedef struct {
    char  pad0[0x14];
    char *x14;
    float x18, x1c, x20;
} Vars_2c5dc8;

extern V_2c5dc8 g_debug_camera_lui __asm__("D_L00_00166DC0") NOT_SDA;
long            get_effect_texture(int) __asm__("FUN_001f44b8");
void            sub_2c5dc8(void *, void *, void *) __asm__("FUN_001f9a28");
void            fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
float           fast_vec_length(void *) __asm__("FUN_001f9af0");
void            fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
float           fast_normalize_angle(float) __asm__("FUN_001fa610");
void            fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void            draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
void            draw_swingshot_cable(struct Moby *moby) __asm__("FUN_L00_002c5dc8");

void draw_swingshot_cable(struct Moby *moby) {
    Quad     segment_quad;
    V_2c5dc8 previous_segment_center, segment_center, attachment_position, segment_direction, f,
        unused_span_perpendicular, wobble_vector_scratch, segment_to_reference, k;
    Vars_2c5dc8 *v;
    unsigned int col;
    float        half = 0.05f;
    float        span_length, distance_along_span, step, x, amplitude_envelope, amp, wob;
    if (moby == 0)
        return;
    v = ((Vars_2c5dc8 *)moby->pvars);
    if (v == 0)
        return;
    segment_quad.tex = get_effect_texture(0xF);
    col = 0x80808080;
    segment_quad.color[3] = col;
    segment_quad.color[2] = col;
    segment_quad.color[1] = col;
    segment_quad.color[0] = col;
    segment_quad.x80 = 0xFF9000000260L;
    segment_quad.x70 = 5;
    segment_quad.x88 = 0xFF00000044L;
    segment_quad.uv[0].u = 0.0f;
    segment_quad.uv[0].v = 0.0f;
    segment_quad.uv[1].u = 1.0f;
    segment_quad.uv[1].v = 0.0f;
    segment_quad.uv[2].u = 0.0f;
    segment_quad.uv[2].v = 1.0f;
    segment_quad.uv[3].u = 1.0f;
    segment_quad.uv[3].v = 1.0f;
    moby_point_position(moby, 0, &attachment_position);
    qcopy(&previous_segment_center, v->x14 + 0x10);
    qcopy(&segment_center, &previous_segment_center);
    sub_2c5dc8(&segment_direction, &attachment_position, &previous_segment_center);
    distance_along_span = 0.0f;
    span_length = fast_vec_length(&segment_direction);
    qcopy(&wobble_vector_scratch, &segment_direction);
    wobble_vector_scratch.f[2] -= 0.1f;
    fast_vec_cross(&f, &segment_direction, &wobble_vector_scratch);
    fast_vec_cross(&unused_span_perpendicular, &segment_direction, &f);
    qcopy(&segment_quad.corner[0], &previous_segment_center);
    qcopy(&segment_quad.corner[2], &previous_segment_center);
    segment_quad.corner[0].f[2] -= half;
    segment_quad.corner[2].f[2] += half;
    wob = fast_cos(fast_normalize_angle(v->x18));
    while (distance_along_span < span_length) {
        step = 0.2f;
        if (span_length - distance_along_span < step)
            step = span_length - distance_along_span;
        distance_along_span += step;
        x = distance_along_span * 3.1415927f;
        amplitude_envelope = fast_sin(x / span_length);
        amp = fast_sin(fast_normalize_angle(x / v->x1c)) * amplitude_envelope * wob * v->x20;
        fast_vec_normalize(&segment_direction, &segment_direction, step);
        fast_vec_add(&segment_center, &segment_center, &segment_direction);
        qcopy(&segment_quad.corner[1], &segment_center);
        qcopy(&segment_quad.corner[3], &segment_center);
        sub_2c5dc8(&segment_to_reference, &g_debug_camera_lui, &segment_center);
        fast_vec_cross(&k, &segment_to_reference, &segment_direction);
        fast_vec_normalize(&k, &k, 0.05f);
        fast_vec_add(&segment_quad.corner[1], &segment_quad.corner[1], &k);
        sub_2c5dc8(&segment_quad.corner[3], &segment_quad.corner[3], &k);
        fast_vec_normalize(&wobble_vector_scratch, &f, amp);
        fast_vec_add(&segment_quad.corner[1], &segment_quad.corner[1], &wobble_vector_scratch);
        fast_vec_add(&segment_quad.corner[3], &segment_quad.corner[3], &wobble_vector_scratch);
        draw_geometry_quad(&segment_quad, 0, 1);
        qcopy(&previous_segment_center, &segment_center);
        qcopy(&segment_quad.corner[0], &segment_quad.corner[1]);
        qcopy(&segment_quad.corner[2], &segment_quad.corner[3]);
    }
}

#include "eetypes.h"

int            scale_game_frames(int) __asm__("FUN_001f96f8");
unsigned char *spawn_swingshot_hook(unsigned char *owner_moby,
                                    float          initial_hook_speed) __asm__("FUN_L00_002c6f88");

unsigned char *spawn_swingshot_hook(unsigned char *owner_moby, float initial_hook_speed) {
    unsigned char *spawned_moby = create_moby(0xd1);
    if (spawned_moby) {
        char *moby_vars = *(char **)(spawned_moby + 0x78);
        *(float *)(moby_vars + 4) = initial_hook_speed;
        *(unsigned char **)moby_vars = owner_moby;
        spawned_moby[0x30] = 0xff;
        *(short *)(spawned_moby + 0x32) = 0xff;
        spawned_moby[0x31] = 1;
        set_moby_color(spawned_moby, 0x80, 0x80, 0x80);
        moby_point_position(owner_moby, 0, spawned_moby + 0x10);
        *(u128 *)(spawned_moby + 0xc0) = *(u128 *)(owner_moby + 0xc0);
        *(u128 *)(spawned_moby + 0xd0) = *(u128 *)(owner_moby + 0xd0);
        *(u128 *)(spawned_moby + 0xe0) = *(u128 *)(owner_moby + 0xe0);
        if (spawned_moby[0x53])
            blend_moby_animation(spawned_moby, 0, 0, scale_game_frames(10));
        update_entity(spawned_moby);
        spawned_moby[0x20] = 0;
        *(unsigned short *)(spawned_moby + 0x34) |= 4;
    }
    return spawned_moby;
}

int                             FUN_001f96f8_2c7070(int) __asm__("FUN_001f96f8");
extern unsigned char D_001413FF NOT_SDA;
void                            swingshot_hook_update(unsigned char *m) __asm__("FUN_L00_002c7070");

void swingshot_hook_update(unsigned char *hook_moby) {
    enum {
        HOOK_INITIAL_ANIMATION = 0,
        HOOK_IDLE_ANIMATION = 1,
        HOOK_REMOVAL_ANIMATION = 2,
        HOOK_LAUNCH_ANIMATION = 3,
        HOOK_FLIGHT_ANIMATION = 4,
        HOOK_ATTACHED_ANIMATION = 5,
        HOOK_SKIP_ANIMATION_TRANSITIONS = 6,
        HOOK_RETURN_ANIMATION = 7
    };
    unsigned char **hook_vars;
    struct Moby    *owner_moby;
    if (hook_moby == 0)
        return;
    hook_vars = *(unsigned char ***)(hook_moby + 0x78);
    if (hook_vars == 0)
        return;
    owner_moby = hook_vars[0];
    if (owner_moby == 0 || owner_moby->oclass != 0xD0 || owner_moby->state == 0xFE ||
        owner_moby->state == 0xFD ||
        *(unsigned char **)(((unsigned char *)owner_moby->pvars) + 0x14) != hook_moby) {
        goto kill;
    }
    if ((owner_moby->flags & 1) || D_001413FF) {
        *(unsigned short *)(hook_moby + 0x34) |= 0x41;
    } else {
        *(unsigned short *)(hook_moby + 0x34) &= ~0x41;
    }
    switch (hook_moby[0x20]) {
    case HOOK_INITIAL_ANIMATION:
        if (hook_moby[0x70] & 2) {
            if (hook_moby[0x53] != 1)
                blend_moby_animation(hook_moby, 1, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 1;
            *(float *)(hook_moby + 0x58) = 1.0f;
        }
        break;
    case HOOK_IDLE_ANIMATION:
        if (hook_vars[0][0x20] == 2) {
            if (hook_moby[0x53] != 3)
                blend_moby_animation(hook_moby, 3, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 3;
            *(float *)(hook_moby + 0x58) = 1.0f;
        } else if (hook_vars[0][0x20] == 8) {
            if (hook_moby[0x53] != 2)
                blend_moby_animation(hook_moby, 2, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 2;
            *(float *)(hook_moby + 0x58) = 1.0f;
        }
        break;
    case HOOK_REMOVAL_ANIMATION:
        if (hook_moby[0x70] & 2) {
        kill:
            mark_moby_for_removal(hook_moby);
            return;
        }
        break;
    case HOOK_LAUNCH_ANIMATION:
        if (hook_moby[0x70] & 2) {
            if (hook_moby[0x53] != 4)
                blend_moby_animation(hook_moby, 4, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 4;
        }
        break;
    case HOOK_FLIGHT_ANIMATION:
        if ((unsigned char)(hook_vars[0][0x20] - 3) < 3) {
            if (hook_moby[0x53] != 5)
                blend_moby_animation(hook_moby, 5, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 5;
        }
        break;
    case HOOK_ATTACHED_ANIMATION:
        if (hook_moby[0x70] & 2)
            *(int *)(hook_moby + 0x58) = 0;
        if ((unsigned char)(hook_vars[0][0x20] - 6) < 2) {
            *(float *)(hook_moby + 0x58) = 1.0f;
            if (hook_moby[0x53] != 7)
                blend_moby_animation(hook_moby, 7, 0, FUN_001f96f8_2c7070(10));
            hook_moby[0x20] = 7;
        }
        break;
    case HOOK_SKIP_ANIMATION_TRANSITIONS:
        break;
    case HOOK_RETURN_ANIMATION:
        if (!(hook_moby[0x70] & 2))
            break;
    default:
        if (hook_moby[0x53] != 1)
            blend_moby_animation(hook_moby, 1, 0, FUN_001f96f8_2c7070(10));
        hook_moby[0x20] = 1;
        break;
    }
    refresh_moby_spatial_bounds_from_basis(hook_moby);
}

typedef union {
    u128 q;
    f32  f[4];
    s32  i[4];
} V2c7320;

extern s32  pad_held[] __asm__("D_0013CAE0") NOT_SDA;
extern void sub_2c7320(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32  fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32  fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32  vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32  fn_2c7320(void *, void *, f32, f32, s32) __asm__("FUN_L00_0025abf0");
void        compute_glove_of_doom_launch_velocity(V2c7320 *origin_position,
                                                  V2c7320 *target_position,
                                                  f32     *out,
                                                  f32      speed,
                                                  f32      unused,
                                                  f32      range) __asm__("FUN_L00_002c7320");

void compute_glove_of_doom_launch_velocity(V2c7320 *origin_position,
                                           V2c7320 *target_position,
                                           f32     *out,
                                           f32      speed,
                                           f32      unused,

                                           f32 range) {
    V2c7320      effective_target_position;
    V2c7320      u;
    f32          launch_heading;
    f32          horizontal_speed;
    f32          k;
    f32          v;
    struct Hero *P;
    if (range < fast_vec_dist(origin_position, target_position)) {
        sub_2c7320(&effective_target_position, target_position, origin_position);
        fast_vec_normalize(&effective_target_position, &effective_target_position, range - 0.01f);
        fast_vec_add(&effective_target_position, &effective_target_position, origin_position);
    } else {
        effective_target_position.q = target_position->q;
    }
    launch_heading = fast_arc_tan(effective_target_position.f[0] - origin_position->f[0],
                                  effective_target_position.f[1] - origin_position->f[1]);
    P = &hero;
    if (fast_difference_between_rotations(P->motion.rot.f[2], launch_heading) > 0.87266463f ||
        vector_distance(origin_position, &effective_target_position) < 1.5f) {
        launch_heading = P->motion.rot.f[2];
        u.f[0] = fast_cos(launch_heading) * 1.5f;
        u.f[1] = fast_sin(P->motion.rot.f[2]) * 1.5f;
        u.i[2] = 0;
        fast_vec_add(&u, &u, origin_position);
        effective_target_position.f[0] = u.f[0];
        effective_target_position.f[1] = u.f[1];
    }
    horizontal_speed = frame_time * 8.5f;
    out[0] = fast_cos(launch_heading) * horizontal_speed;
    out[1] = fast_sin(launch_heading) * horizontal_speed;
    *(s32 *)&out[2] = 0;
    out[2] = fn_2c7320(origin_position, &effective_target_position, horizontal_speed, -speed, 0);
    if (pad_held[0] & 5) {
        k = 3.5f / horizontal_speed;
    } else {
        k = 2.5f / horizontal_speed;
    }
    v = speed * k * 0.5f;
    if (v < out[2]) {
        out[2] = v;
    }
}

#include "qcopy.h"

void spawn_particle(float *,
                    unsigned char,
                    unsigned char,
                    unsigned char,
                    float,
                    float,
                    int) __asm__("FUN_L00_00269b70");
void spawn_moby_collision_particles(unsigned char *moby) __asm__("FUN_L00_002c8078");

void spawn_moby_collision_particles(unsigned char *moby) {
    enum {
        COLLISION_MATERIAL_WATER = 0
    };
    float         particle_spawn_position[4] __attribute__((aligned(16)));
    int           i;
    int           r, g;
    unsigned char b;
    float         randomized_particle_scalar;
    switch (collision_hit_material()) {
    case COLLISION_MATERIAL_WATER:
    case 1:
    case 3:
    case 8:
    case 11:
    case 12:
    case 13:
        for (i = 0; i < 20; i++) {
            qcopy(particle_spawn_position, moby + 0x10);
            particle_spawn_position[0] += random_float_between(-0.3f, 0.3f);
            particle_spawn_position[1] += random_float_between(-0.3f, 0.3f);
            particle_spawn_position[2] += random_float_between(-0.3f, 0.3f);
            r = (rand() + 0x30) & 0x3F;
            g = (rand() + 0x20) & 0x3F;
            b = rand() & 0x2F;
            randomized_particle_scalar = random_float_between(100000.0f, 800000.0f);
            spawn_particle(particle_spawn_position,
                           r,
                           g,
                           b,
                           randomized_particle_scalar,
                           0.0f,
                           scale_game_frames(rand() % 40 + 10));
        }
        moby[0x20] = 4;
        break;
    }
}

#include "qcopy.h"

unsigned char *spawn_glove_of_doom_held_moby(int   spawning_moby,
                                             u128 *spawn_position,
                                             u128 *initial_velocity) __asm__("FUN_L00_002c8218");

unsigned char *

spawn_glove_of_doom_held_moby(int spawning_moby, u128 *spawn_position, u128 *initial_velocity) {
    unsigned char *spawned_moby = create_moby(0xe6);
    if (spawned_moby) {
        char *moby_vars;
        moby_vars = *(char **)(spawned_moby + 0x78);
        spawned_moby[0x30] = 0xff;
        *(short *)(spawned_moby + 0x32) = 0xff;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(moby_vars, initial_velocity);
        *(int *)(moby_vars + 0x30) = spawning_moby;
        *(int *)(moby_vars + 0x34) = 0;
        *(short *)(moby_vars + 0x38) = 0;
        *(float *)(spawned_moby + 0x2c) =
            *(float *)(*(char **)(spawned_moby + 0x24) + 0x24) * 0.01f;
        if (hero.unk20A5 || hero.unk20AF)
            *(unsigned short *)(spawned_moby + 0x34) |= 0x41;
    }
    return spawned_moby;
}

typedef unsigned int u128_2c82f0 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2c82f0 q;
    float       f[4];
} Vec4_2c82f0;

extern float D_0015ED70_002c82f0 __asm__("D_0015ED70");
extern char  collision_hit_point[] __asm__("D_L00_00173E60") NOT_SDA;
void        *FUN_L00_002603d0_002c82f0(void *) __asm__("FUN_L00_002603d0");
void         FUN_001f9a10_002c82f0(void *, void *, void *) __asm__("FUN_001f9a10");
void         FUN_001f9a68_002c82f0(void *, void *, float) __asm__("FUN_001f9a68");
int          FUN_001efa68_002c82f0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
void         FUN_001f9bf8_002c82f0(void *, void *, float) __asm__("FUN_001f9bf8");
void         release_glove_of_doom_held_moby(Vec4_2c82f0 *release_position,
                                             u8          *released_moby,
                                             void        *initial_velocity) __asm__("FUN_L00_002c82f0");

void release_glove_of_doom_held_moby(Vec4_2c82f0 *release_position,
                                     u8          *released_moby,

                                     void *initial_velocity) {
    char        *q = *(char **)(released_moby + 0x78);
    struct Hero *P;
    char        *released_moby_position, *collision_reference_moby;
    Vec4_2c82f0  collision_start, collision_position_offset;
    qcopy(q, initial_velocity);
    P = &hero;
    if (P->ground_moby && FUN_L00_002603d0_002c82f0(P->ground_moby)) {
        FUN_001f9a10_002c82f0(q, q, &P->motion.displacement);
    }
    released_moby_position = released_moby + 0x10;
    qcopy(released_moby_position, release_position);
    {
        struct Hero *P2 = &hero;
        collision_reference_moby = (char *)P2->moby;
    }
    collision_start.q = ((Vec4_2c82f0 *)(collision_reference_moby + 0x10))->q;
    collision_start.f[2] = release_position->f[2];
    if (FUN_001efa68_002c82f0(&collision_start, release_position, 0, collision_reference_moby, 0)) {
        char *collision_hit_cursor = collision_hit_point;
        float x;
        qcopy(released_moby_position, collision_hit_cursor);
        x = D_0015ED70_002c82f0;
        collision_hit_cursor += 0x20;
        FUN_001f9bf8_002c82f0(
            &collision_position_offset, collision_hit_cursor, x * 9.0f - x * 0.1f);
        FUN_001f9a10_002c82f0(
            released_moby_position, released_moby_position, &collision_position_offset);
        reflect_vector(q, q, collision_hit_cursor);
        FUN_001f9a68_002c82f0(q, q, 0.6f);
    }
    released_moby[0x20] = 2;
}

/* Ported from rac1-decomp (nonmatching/shared/func_L00_002C12B0.c: func_L00_002C12B0). */
#include "rnc/math/asm_helpers.h"

typedef struct {
    char          pad0[10];
    unsigned char radius;
    char          padB[5];
    float         height;
} AimInfo;

typedef struct {
    OvlVec4       target, velocity, previous_position;
    int           timer, age;
    struct Moby  *target_moby, *last;
    float         turn_velocity, movement_speed, unknown48;
    int           unknown4C;
    float         f50, f54, f58, turn_rate;
    struct Moby  *owner;
    float         angle;
    unsigned char mode;
} DoomBotVars;

typedef struct {
    OvlVec4 target;
    float   vertical_speed, horizontal_speed, heading, gravity, radius, launch_frame, landing_lead;
    unsigned char anim2C, anim2D, anim2E, mode2F;
    float         turn_velocity, speed, rate, max;
} DoomBotJump;

typedef struct {
    char         pad0[0x1EC0];
    int          damage;
    float        f1EC4, f1EC8, f1ECC, turn_velocity, movement_velocity;
    int          f1ED8;
    float        turn_k, turn_d, turn_max, yawlim;
    char         pad1EEC[4];
    float        f1EF0;
    char         pad1EF4[8];
    float        move_k, move_d, move_max;
    int          f1F08;
    char         pad1F0C[4];
    DoomBotJump  config;
    char         pad1F50[0x130];
    struct Moby *moby;
} DoomBotParams;

extern CollisionHit coll_hit __asm__("D_L00_00173E40") NOT_SDA;
extern char         D_L00_00177F00[], D_L00_001E9B48[], D_L00_001E9B70[];
extern s32          D_L00_0015F5C4;
extern f32          vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32          fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern s32          truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void        *memset(void *, int, unsigned int) __asm__("FUN_001153fc");
extern void         debug_print(char *) __asm__("FUN_001e93b0");
extern void         clamp_vector_length(void *, void *, f32) __asm__("FUN_001f9c90");
extern int          coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern s32          collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void         spawn_explosion(float,
                                    void *,
                                    void *,
                                    int,
                                    float,
                                    float,
                                    int,
                                    int,
                                    int,
                                    float,
                                    float,
                                    float,
                                    float,
                                    int,
                                    int,
                                    int,
                                    int,
                                    int) __asm__("FUN_L00_0025e450");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void
send_blast_hits(void *, void *, void *, int, void *, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern int  FUN_L00_001ff600(float, void *, void *);
extern void save_state_fields_to_hero(char *) __asm__("FUN_L00_002beb90");
extern void restore_state_fields_from_hero(char *) __asm__("FUN_L00_002bebc8");
extern void save_state_vector_and_fields_to_hero(char *) __asm__("FUN_L00_002bec00");
extern void restore_saved_state_fields_from_hero(char *) __asm__("FUN_L00_002bec48");
extern int  probe_moby_raised_sphere_collision(char *, int, void *) __asm__("FUN_L00_002beab0");
extern void FUN_L00_00260400(void *, int, void *, void *, void *, void *);
extern int  move_moby_with_collision_integer_radius(void *, void *, int, int, float, float) __asm__("FUN_L00_00258ad0");
extern void FUN_L00_00258278(void *, float *, float, float, float, float);
extern struct Moby *
select_class5_target_or_hero(void *, void *, void *, void *, int) __asm__("FUN_L00_002bed90");
extern float select_moby_heading_by_collision_probes(void *,
                                                     void *,
                                                     float,
                                                     float,
                                                     float,
                                                     float,
                                                     float,
                                                     void *,
                                                     void *,
                                                     float) __asm__("FUN_L00_002bf050");
extern int   try_start_moby_hop(void *, void *, void *, int) __asm__("FUN_L00_002bf9f0");
extern void  initialize_moby_hop_trajectory(void *, void *, void *) __asm__("FUN_L00_002bf8b0");
extern int   compute_moby_state_from_vertical_probe(void *) __asm__("FUN_L00_002bfec8");
extern int   update_arc_motion(void *, void *) __asm__("FUN_L00_0025b440");
extern int   FUN_L00_00259028(void *, void *, void *, void *);

/* Updates an Agent of Doom's movement, target selection and detonation. */
void doom_bot_update(struct Moby *moby) __asm__("FUN_L00_002bffa8");

void doom_bot_update(struct Moby *moby) {
    enum {
        DOOM_BOT_INITIALIZE = 0,
        DOOM_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVEMENT = 1,
        DOOM_BOT_SELECT_MOVEMENT_MODE = 2,
        DOOM_BOT_UPDATE_TARGET_AND_MOVEMENT = 3,
        DOOM_BOT_MOVE_HIGH_TURN_RATE = 4,
        DOOM_BOT_MOVE_LOW_TURN_RATE = 5,
        DOOM_BOT_ALIGN_TO_TARGET = 6,
        DOOM_BOT_EXECUTE_JUMP = 7,
        DOOM_BOT_COLLISION_RECOVERY_FALL = 8,
        DOOM_BOT_WAIT_FOR_ANIMATION_TO_SELECT_MOVEMENT = 9,
        DOOM_BOT_WAIT_FOR_ANIMATION_COMPLETION = 10,
        DOOM_BOT_DETONATE = 11,
        DOOM_BOT_WAIT_FOR_TRANSITION_ANIMATION = 12,
        DOOM_BOT_SQUIRM = 13,
        DOOM_BOT_INITIAL_FALL = 14,
        DOOM_BOT_RESET_TO_INITIALIZE = 15
    };
    OvlVec4       scratch[4];
    float         clearance;
    DoomBotVars  *data;
    struct Moby  *target;
    AimInfo      *aim;
    float         scale, speed, z, height, reach, angle, distance;
    int           blocked, move_flags, count, result;
    unsigned char old_state;
    if (!moby || !(data = (DoomBotVars *)moby->pvars))
        return;
    if (D_L00_0015F5C4 == 2) {
        moby->flags |= 0x41;
        return;
    }
    moby->flags &= 0xFFBE;
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    if (find_moby_hit(moby, 0x800001, 0))
        moby->state = 11;
    scale = convert_integer_to_float(item_text_variant[0x14]) + 1.0f;
    moby->scale += (moby->pclass->scale * scale - moby->scale) * 0.05f;
    speed = convert_integer_to_float(item_text_variant[0x14]) * 0.5f + 1.0f;
    if (moby->seq == 0 || moby->seq == 6 || moby->seq == 7)
        moby->anim_speed = speed / scale;
    else
        moby->anim_speed = 1.0f;
    if (moby->state != 11) {
        int            damage = truncate_float_to_s32(scale * 183.296f) + 1;
        DoomBotParams *hp = (DoomBotParams *)&hero;
        hp->damage = damage;
        hp->f1EF0 = 0;
        hp->f1EC4 = scale * 0.198f + 0.02f;
        if (moby->state == 0) {
            float frame, turn;
            memset(&hp->config, 0, 64);
            frame = frame_scale;
            hp->f1ECC = hp->f1EC8 = scale * 0.25f;
            data->movement_speed = frame * 0.2f;
            turn = scale * 0.01f * frame;
            data->turn_velocity = turn;
            hp->turn_d = 0.3f;
            hp->turn_max = 2.7f;
            hp->turn_k = turn;
            hp->f1ED8 = 0;
            data->turn_rate = frame * 0.066f * speed;
            hp->config.anim2C = 2;
            hp->config.anim2D = 3;
            hp->config.anim2E = 4;
            hp->yawlim = 3.1415927f;
            hp->move_d = frame * 0.01f;
            hp->move_k = frame * 0.02f;
            hp->move_max = frame * 0.045f;
            hp->config.gravity = frame_time_sq * 29.7f;
            hp->config.radius = scale * 0.1f;
            hp->config.launch_frame = 7.5f;
            hp->config.landing_lead = 11.5f;
            hp->config.speed = frame * 0.02f;
            hp->config.rate = frame * 0.01f;
            hp->config.max = frame * 0.045f;
            data->target_moby = 0;
            moby->state = 14;
            data->timer = 0;
            data->turn_rate = frame_scale * 0.1f * speed;
            save_state_fields_to_hero((char *)data);
            save_state_vector_and_fields_to_hero((char *)data);
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
        }
        if (moby->state == 13) {
            if (!data->owner || data->owner->state == 0xFE || data->owner->state == 0xFD ||
                moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
                moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            return;
        }
        data->age++;
        if (scale_game_frames(3600) < data->age)
            moby->state = 11;
        fast_dec_timer(&data->timer);
        qcopy(&data->previous_position, &moby->pos);
        if (probe_moby_raised_sphere_collision((char *)moby, 5, 0)) {
            if (coll_hit.moby && coll_hit.unk1C < 0 &&
                (((struct Moby *)coll_hit.moby)->flags & 0x1000) &&
                ((struct Moby *)coll_hit.moby)->pclass &&
                ((struct Moby *)coll_hit.moby)->pclass->type == 5) {
                moby->state = 11;
            } else if (!coll_hit.moby || coll_hit.unk1C < 0) {
                z = moby->pos.z;
                qcopy_nc(&moby->pos, coll_hit.pad30);
                if (absolute_float(moby->pos.z - z) > 0.6f)
                    moby->pos.z = z + 0.01f;
                qcopy(&data->velocity, &coll_hit.normal_x);
                clamp_vector_length(&data->velocity, &data->velocity, frame_scale * 0.05f);
                FUN_L00_001ff600(frame_scale * 0.05f, &data->velocity, &data->velocity);
                moby->state = 8;
                if (data->target_moby) {
                    data->last = data->target_moby;
                    data->timer += 500;
                }
                if (moby->prev_seq != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            }
        }
        if (moby->pos.z < 0.1f) {
            mark_moby_for_removal(moby);
            return;
        }
        if (data->timer >= 4001)
            moby->state = 11;
    }
    if (probe_ground_height(&moby->pos, 0, 0.5f) != 0 && coll_hit.moby)
        FUN_L00_00260400(moby, coll_hit.moby, &moby->pos, &moby->rot, &moby->pos, &moby->rot);
    switch (moby->state) {
    case DOOM_BOT_INITIALIZE:
        debug_print(D_L00_001E9B48);
        moby->state = 1;
        break;
    case DOOM_BOT_COLLISION_RECOVERY_FALL: {
        DoomBotParams *hp;
        fast_vec_add(&scratch[0], &moby->pos, &data->velocity);
        data->velocity.f[2] -= frame_time_sq * 9.0f;
        save_state_fields_to_hero((char *)data);
        hp = (DoomBotParams *)&hero;
        blocked = move_moby_with_collision_integer_radius(
                      moby, &data->velocity, hp->damage, 0, scale * 0.7f, hp->f1EF0) &
                  2;
        if (blocked) {
            data->velocity.f[2] = 0;
            fast_vec_scale(&data->velocity, &data->velocity, 0.5f);
        }
        FUN_L00_001ff600(frame_scale * 0.05f, &data->velocity, &data->velocity);
        if (moby->anim_flags & 2) {
            if (!blocked) {
                if (moby->prev_seq != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            } else
                moby->state = 2;
        }
        goto Falling;
    }
    case DOOM_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVEMENT:
    case DOOM_BOT_WAIT_FOR_ANIMATION_TO_SELECT_MOVEMENT:
    case DOOM_BOT_WAIT_FOR_ANIMATION_COMPLETION:
        if (moby->anim_flags & 2)
            moby->state = 2;
        goto Contact;
    case DOOM_BOT_SELECT_MOVEMENT_MODE:
    case DOOM_BOT_UPDATE_TARGET_AND_MOVEMENT:
    case DOOM_BOT_MOVE_HIGH_TURN_RATE:
    case DOOM_BOT_MOVE_LOW_TURN_RATE: {
        target = data->target_moby;
        if (target && target->state != 0xFE && target->state != 0xFD) {
            height = target->pos.z;
            aim = (AimInfo *)suck_get_aim_height((char *)data->target_moby);
            reach = speed;
            if (aim) {
                reach = speed + convert_integer_to_float(aim->radius) * 0.125f;
                height += aim->height;
            }
            if (fast_vec_dist(&moby->pos, &data->target_moby->pos) < reach &&
                coll_sphere(&moby->pos, speed, 1, moby)) {
                moby->state = 11;
                break;
            }
            if (vector_distance(&moby->pos, &data->target_moby->pos) < reach * 0.5f &&
                absolute_float(height - (moby->pos.z + speed * 0.1f)) < reach * 0.5f) {
                save_state_vector_and_fields_to_hero((char *)data);
                initialize_moby_hop_trajectory(
                    moby, &((DoomBotParams *)&hero)->config, &data->target_moby->pos);
                restore_saved_state_fields_from_hero((char *)data);
                qcopy(data, &data->target_moby->pos);
                data->turn_rate = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(2));
                moby->state = 6;
            }
        }
        qcopy(&scratch[0], &moby->pos);
        scratch[0].f[2] += scale * 0.3f;
        if (random_integer_below(4) == 0)
            data->target_moby =
                select_class5_target_or_hero(moby, &scratch[0], &moby->rot, data->last, 1);
        data->turn_rate = frame_scale * 0.1f * speed;
        moby->state = 4;
        if (!data->target_moby) {
            angle = random_float_between(-3.1415927f, 3.1415927f);
            if (random_integer_below(4) == 0) {
                float step_height = scale * 0.35f;
                float range = 4.0f;
                float radius =
                    convert_integer_to_float(((DoomBotParams *)&hero)->damage) * 0.0009765625f;
                data->angle = select_moby_heading_by_collision_probes(moby,
                                                                      &moby->pos,
                                                                      angle,
                                                                      1.84799564f,
                                                                      range,
                                                                      step_height,
                                                                      4.0f,
                                                                      0,
                                                                      &clearance,
                                                                      radius);
            } else
                clearance = 5.0f;
        }
        if (data->target_moby && data->target_moby != ((DoomBotParams *)&hero)->moby &&
            (!data->target_moby || data->target_moby->state == 0xFE ||
             data->target_moby->state == 0xFD || !data->target_moby->pclass ||
             data->target_moby->pclass->type != 5 || !(data->target_moby->flags & 0x1000))) {
            data->target_moby = 0;
            moby->state = 2;
            data->turn_rate = frame_scale * 0.0273f * speed;
            break;
        }
        old_state = moby->state;
        if (!data->target_moby) {
            moby->state = 5;
            if (moby->prev_seq != 6)
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            data->turn_rate = frame_scale * 0.0273f * speed;
        } else {
            if (random_integer_below(4) == 0 &&
                try_start_moby_hop(moby, data, &data->target_moby->pos, 1)) {
                qcopy(data, &data->target_moby->pos);
                data->turn_rate = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(2));
                moby->state = 6;
                break;
            }
            distance = vector_distance(&moby->pos, &data->target_moby->pos);
            {
                DoomBotParams *hp = (DoomBotParams *)&hero;
                if (data->target_moby == hp->moby) {
                    moby->state = 5;
                    if ((moby->anim_flags & 2) && moby->prev_seq != 6)
                        blend_moby_animation(moby, 6, 0, scale_game_frames(0));
                    data->turn_rate = frame_scale * 0.0273f * speed;
                }
            }
            if (!(distance < 8.0f) && distance > 20.0f) {
                moby->state = 5;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                data->turn_rate = frame_scale * 0.0273f * speed;
            } else {
                moby->state = 4;
                if (moby->prev_seq != 7)
                    blend_moby_animation(moby, 7, 0, scale_game_frames(10));
                data->turn_rate = frame_scale * 0.1f * speed;
            }
            if (random_integer_below(4) == 0) {
                DoomBotParams *hp = (DoomBotParams *)&hero;
                Vec4f         *target_pos = &data->target_moby->pos;
                float          range = 4.0f;
                float          radius = convert_integer_to_float(hp->damage) * 0.0009765625f;
                data->angle = select_moby_heading_by_collision_probes(moby,
                                                                      &moby->pos,
                                                                      moby->rot.z,
                                                                      1.84799564f,
                                                                      range,
                                                                      0.35f,
                                                                      4.0f,
                                                                      target_pos,
                                                                      &clearance,
                                                                      radius);
            } else
                clearance = 5.0f;
        }
        if (moby->state == 4 && old_state == 5) {
            moby->state = 12;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            goto Contact;
        }
        data->target.f[1] = moby->pos.y + fast_sin(data->angle) * 5.0f;
        data->target.f[0] = moby->pos.x + fast_cos(data->angle) * 5.0f;
        data->target.f[2] = moby->pos.z + 1.0f;
        save_state_fields_to_hero((char *)data);
        {
            DoomBotParams *hp = (DoomBotParams *)&hero;
            FUN_L00_00258278(
                moby, &data->movement_speed, data->angle, hp->move_k, hp->move_d, hp->move_max);
        }
        fast_vec_scale(&scratch[3], &data->velocity, 5.0f);
        scratch[3].f[2] = 0;
        qcopy(&scratch[1], &moby->pos);
        scratch[1].f[2] += scale * 0.15f;
        fast_vec_add(&scratch[2], &scratch[1], &scratch[3]);
        if (!collision_line(&scratch[1], &scratch[2], 6, moby, 0) ||
            coll_hit.moby == (s32)data->target_moby ||
            (coll_hit.moby && ((struct Moby *)coll_hit.moby)->pclass &&
             ((struct Moby *)coll_hit.moby)->pclass->type == 5)) {
            z = moby->pos.z;
            move_flags =
                FUN_L00_00259028(moby, &((DoomBotParams *)&hero)->damage, data, &data->velocity);
            if (absolute_float(moby->pos.z - z) > 1.0f) {
                moby->pos.z = z;
                if (probe_moby_raised_sphere_collision((char *)moby, 5, 0)) {
                    qcopy(&moby->pos, coll_hit.pad30);
                    if (absolute_float(moby->pos.z - z) > 1.0f)
                        moby->pos.z = z;
                }
            }
        } else {
            clearance = 0;
            move_flags = 2;
            data->velocity.f[0] = moby->pos.x - data->target.f[0];
            data->velocity.f[1] = moby->pos.y - data->target.f[1];
            data->velocity.f[2] = 0.01f;
            fast_vec_normalize(&data->velocity, &data->velocity, 0.1f);
        }
        restore_state_fields_from_hero((char *)data);
        if ((move_flags & 2) || clearance < 4.0f) {
            if (try_start_moby_hop(moby, data, data, 0)) {
                data->turn_rate = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(2));
                moby->state = 6;
                goto Contact;
            }
            if (data->target_moby) {
                data->last = data->target_moby;
                data->timer += 500;
            }
            data->velocity.f[0] = moby->pos.x - data->target.f[0];
            data->velocity.f[1] = moby->pos.y - data->target.f[1];
            data->velocity.f[2] = 0.01f;
            fast_vec_normalize(&data->velocity, &data->velocity, 0.1f);
            moby->state = 8;
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            break;
        }
        goto Contact;
    }
    case DOOM_BOT_ALIGN_TO_TARGET: {
        float heading =
            fast_arc_tan(data->target.f[0] - moby->pos.x, data->target.f[1] - moby->pos.y);
        float         *velocity;
        DoomBotParams *hp;
        save_state_fields_to_hero((char *)data);
        velocity = &((DoomBotParams *)&hero)->movement_velocity;
        hp = (DoomBotParams *)&hero;
        FUN_L00_00258278(
            moby, velocity, heading, 2.0f * hp->move_k, 0.5f * hp->move_d, 2.0f * hp->move_max);
        restore_state_fields_from_hero((char *)data);
        if (fast_difference_between_rotations(moby->rot.z, heading) < 0.08726646f) {
            blend_moby_animation(moby, hp->config.anim2C, 0, scale_game_frames(3));
            moby->state = 7;
        }
        goto Contact;
    }
    case DOOM_BOT_EXECUTE_JUMP:
        z = moby->pos.z;
        save_state_vector_and_fields_to_hero((char *)data);
        result = update_arc_motion(moby, &((DoomBotParams *)&hero)->config);
        if (result == 4) {
            moby->state = result;
            if (moby->prev_seq != 7)
                blend_moby_animation(moby, 7, 0, scale_game_frames(3));
            data->turn_rate = frame_scale * 0.1f * speed;
        }
        restore_saved_state_fields_from_hero((char *)data);
        absolute_float(moby->pos.z - z);
        goto Contact;
    case DOOM_BOT_DETONATE: {
        if (!(moby->pos.x < 2.0f) && !(moby->pos.x > 1021.0f) && !(moby->pos.y < 2.0f) &&
            !(moby->pos.y > 1021.0f) && !(moby->pos.z < 2.0f) && !(moby->pos.z > 1021.0f)) {
            qcopy(&scratch[1], &moby->pos);
            scratch[2].q = 0;
            scratch[2].f[2] = 1.0f;
            scratch[2].f[3] = 1.0f;
            spawn_explosion(0.0f,
                            moby,
                            &scratch[2],
                            (int)&moby->pos,
                            0.0f,
                            2.0f * speed,
                            5,
                            2,
                            4,
                            speed,
                            9.0f,
                            1.0f,
                            15.0f,
                            -1,
                            1,
                            5,
                            -1,
                            item_text_variant[0x14]);
            scratch[1].f[2] += scale * 0.5f;
            count = collect_mobies_in_sphere(&moby->pos, speed, 0x15, moby, 0);
            scratch[3].q = *(OvlQuad *)&moby->pos;
            send_blast_hits(
                moby, &scratch[3], D_L00_00177F00, count, 0, 0x10000, 2, 3, 3.0f, 1.0f, 1.0f);
            if (item_text_variant[0x14])
                allocate_voice_for_target_entry(4, 0, moby);
            else
                allocate_voice_for_target_entry(0, 0, moby);
        }
    }
        mark_moby_for_removal(moby);
        return;
    case DOOM_BOT_WAIT_FOR_TRANSITION_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = 4;
            if (moby->prev_seq != 7)
                blend_moby_animation(moby, 7, 0, scale_game_frames(10));
            data->turn_rate = frame_scale * 0.1f * speed;
        }
        break;
    case DOOM_BOT_SQUIRM:
        debug_print(D_L00_001E9B70);
        break;
    case DOOM_BOT_INITIAL_FALL: {
        DoomBotParams *hp;
        fast_vec_add(&scratch[1], &moby->pos, &data->velocity);
        if (coll_sphere(&moby->pos, scale * 0.5f, 1, moby) && coll_hit.moby &&
            ((struct Moby *)coll_hit.moby)->oclass == 186) {
            fast_vec_sub(&scratch[1], coll_hit.pad30, &moby->pos);
            distance = fast_vec_length(&scratch[1]);
            scratch[1].f[2] = 0;
            fast_vec_normalize(&scratch[1], &scratch[1], distance * 1.1f);
            fast_vec_add(&moby->pos, &moby->pos, &scratch[1]);
        }
        data->velocity.f[2] -= frame_time_sq * 9.0f;
        save_state_fields_to_hero((char *)data);
        hp = (DoomBotParams *)&hero;
        blocked = move_moby_with_collision_integer_radius(
                      moby, &data->velocity, hp->damage, 0, scale * 0.7f, hp->f1EF0) &
                  2;
        if (blocked) {
            data->velocity.f[2] = 0;
            fast_vec_scale(&data->velocity, &data->velocity, 0.5f);
        }
        clamp_vector_length(&data->velocity, &data->velocity, frame_scale * 0.05f);
        if (!blocked) {
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
        } else
            moby->state = 2;
    }
        /* Fakematch: the do-while(0) splits the scheduling region. */
    Falling:
        do {
            float          direction = fast_arc_tan(data->velocity.f[0], data->velocity.f[1]);
            DoomBotParams *hp = (DoomBotParams *)&hero;
            FUN_L00_00258278(
                moby, &data->turn_velocity, direction, hp->turn_k, hp->turn_d, hp->turn_max);
            /* Fakematch: no-op update of data; it depends on the a1 setup, so a1 is scheduled before a0. */
            data = (DoomBotVars *)((int)data | ((unsigned char)(int)data >> 8));
        } while (0);
        restore_state_fields_from_hero((char *)data);
    Contact:
        /* Fakematch: the zero term keeps data live so combine keeps the no-op update above. */
        moby->state =
            compute_moby_state_from_vertical_probe(moby) | ((unsigned char)(int)data >> 8);
        break;
    case DOOM_BOT_RESET_TO_INITIALIZE:
        moby->state = 0;
        break;
    default:
        moby->state = 0;
        break;
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f)
        mark_moby_for_removal(moby);
    return;
}

extern int D_L00_0015F5CC_2c1778[1] __asm__("D_L00_0015F5CC");
void       FUN_00213508_2c1778(void *, int, float) __asm__("FUN_00213508");
float      FUN_001fa580_2c1778(float, float) __asm__("FUN_001fa580");
float      FUN_001f9b80_2c1778(void *, void *) __asm__("FUN_001f9b80");
float      FUN_001f99c0_2c1778(float) __asm__("FUN_001f99c0");
float      approach_value(void *, float, float) __asm__("FUN_00213ed8");

extern const float l00_trespasser_pickup_initial_height_offset __asm__("D_L00_00161828")
    __attribute__((sda));

#include "rnc/overlay/shared/weapons/trespasser_globals.h"

extern float trespasser_pickup_z_rotation_step_gp __asm__("D_L00_00161830") __attribute__((sda));

void update_trespasser_pickup(void *mv) {
    enum {
        TRESPASSER_INITIALIZE = 0,
        TRESPASSER_WAIT_FOR_HERO_PROXIMITY = 1,
        TRESPASSER_SHRINK_FOR_REMOVAL = 2
    };
    unsigned char *trespasser_moby = mv;
    struct Hero   *P;
    switch (trespasser_moby[0x20]) {
    case TRESPASSER_INITIALIZE:
        FUN_00213508_2c1778(trespasser_moby + 0x10, 0, 0.5f);
        *(float *)(trespasser_moby + 0x40) = 1.5707964f;
        *(float *)(trespasser_moby + 0x44) = 3.1415927f;
        *(float *)(trespasser_moby + 0x18) =
            collision_hit_z + l00_trespasser_pickup_initial_height_offset;
        trespasser_moby[0x20] = 1;
        break;
    case TRESPASSER_WAIT_FOR_HERO_PROXIMITY: {
        float updated_z_rotation = FUN_001fa580_2c1778(*(float *)(trespasser_moby + 0x48),
                                                       trespasser_pickup_z_rotation_step_gp);
        int   proximity_check_counter = D_L00_0015F5CC_2c1778[0];
        *(float *)(trespasser_moby + 0x48) = updated_z_rotation;
        if (proximity_check_counter % 10 == 0) {
            P = &hero;
            if (P->health.hp) {
                if (FUN_001f9b80_2c1778(trespasser_moby + 0x10, &P->motion.pos) < 1.0f) {
                    if (FUN_001f99c0_2c1778(*(float *)(trespasser_moby + 0x18) -
                                            P->motion.pos.f[2]) < 2.0f) {
                        grant_inventory_item(0x1A, 1);
                        trespasser_moby[0x20] = 2;
                    }
                }
            }
        }
    } break;
    case TRESPASSER_SHRINK_FOR_REMOVAL:
        approach_value(trespasser_moby + 0x2C,
                       0.0f,
                       *(float *)(*(char **)(trespasser_moby + 0x24) + 0x24) * 0.02f);
        if (*(float *)(trespasser_moby + 0x2C) == 0.0f)
            mark_moby_for_removal(trespasser_moby);
        break;
    }
}

typedef struct {
    u8           pad0[0x40];
    OvlVec4      grip;
    struct Moby *held;
    s32          timer;
} CarrierVars_2c1ad0;

typedef struct {
    u8  pad0[0x88];
    u16 deliveries;
    u16 unkA2;
    u32 levels;
} DeliveryStats_2c1ad0;

extern DeliveryStats_2c1ad0 D_00141680_2c1ad0 __asm__("D_00141680");
extern u8                   D_0013E531;
extern f32                  D_0015ED68;
extern f32 frame_time       MACRO_ADDR;
extern f32                  frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern s32                  D_0015EEA4;
extern u32 D_0013CAE4       NOT_SDA;
extern OvlVec4              D_L00_00166DC0_2c1ad0 __asm__("D_L00_00166DC0") NOT_SDA;

#include "rnc/overlay/level_camera.h"

extern LevelCamera l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;
extern s16         D_L00_001B07B0_lui[] __asm__("D_L00_001B07B0") NOT_SDA;
extern void initialize_moby_motion_at_position(void *, void *, void *) __asm__("FUN_L00_002aa008");
extern struct Moby *spawn_mine(void *, void *, void *) __asm__("FUN_L00_002a9ed0");
extern void         append_unique_moby_index(void *, void *, int) __asm__("FUN_L00_0025f780");
extern f32          square_root(f32) __asm__("FUN_001f9988");
extern void         fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32          scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32          collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void         copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void         transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void         transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void         transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern f32          vector_length_xy(void *) __asm__("FUN_001f9b20");
extern int          use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern int          get_ammo(int) __asm__("FUN_L00_00233e98");

#include "rnc/input/pad_state.h"
#include "rnc/math/vector.h"
#include "qcopy.h"
#include "qzero.h"

typedef struct {
    u8           pad0[0x40];
    Vec4         grip;
    struct Moby *held;
    s32          timer54;
    s32          timer58;
    s32          delay;
} GloveVars;

extern CollisionHit coll_hit __asm__("D_L00_00173E40");
extern Vec4         D_L00_00166DC0;
extern short        D_L00_001B07B0[];
extern f32          frame_scale_inv_macro __asm__("D_0015ED68") MACRO_ADDR;
extern f32          frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern s32          current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern s32          D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern void         FUN_L00_0024f7c8(void *, int, void *);
/* Float arguments first: the order they are evaluated in gives retail's registers. */
extern void FUN_L00_002c1928_f(f32, f32, f32, f32 *, f32 *, f32 *) __asm__("FUN_L00_002c1928");

/* Mine glove update: aims the glove and throws a mine at the target. */
void mine_glove_update(struct Moby *moby) __asm__("FUN_L00_002c1ad0");

void mine_glove_update(struct Moby *moby) {
    enum {
        MINE_GLOVE_INITIALIZE = 0,
        MINE_GLOVE_WAIT_FOR_ANIMATION = 1,
        MINE_GLOVE_RELEASE_HELD_MOBY = 3,
        MINE_GLOVE_POST_RELEASE_WAIT = 4,
        MINE_GLOVE_BEGIN_HELD_MOBY_REMOVAL = 5,
        MINE_GLOVE_REMOVE_HELD_MOBY = 6
    };
    GloveVars *vars;
    Vec4       saved, offset, rotatedOffset;
    Vec4       local[4];
    Vec4       pos, target;
    Vec4       basis[3];
    Vec4       matrix[4];
    Vec4       launch, arc, probe;
    float     *out;
    float      angle, slope, horizontal, vertical, radius, root;
    s32        hit;

    if (moby == 0)
        return;
    vars = (GloveVars *)moby->pvars;
    if (vars->delay < 3) {
        ++vars->delay;
        return;
    }
    offset.q = 0;
    offset.f[1] = -0.09f;
    offset.f[2] = -0.02f;
    copy_matrix3x4(basis, l00_camera.unk0);
    transpose_rotation_matrix(matrix, basis);
    if (vars == 0)
        return;
    FUN_L00_0024f7c8(moby, 0, &saved);
    build_moby_bone_transform(moby, 0, local);
    transform_vector(&rotatedOffset, &offset, local);
    fast_vec_add(&vars->grip, &saved, &rotatedOffset);
    if (vars->held != 0)
        vars->held->collision_disable = 1;
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD)
        vars->held = 0;
    if (hero.state.current == 1)
        hero_set_state(0x1E, 1);
    if (fast_dec_timer(&vars->timer58) && !hero.item_use_locked) {
        if ((hero.item_anim_active && hero.item_anim_frames == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) && hero.state.current == 0x1E &&
             get_ammo(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            allocate_hero_voice_for_entry(0x1A, 0);
            use_ammo(-1, 1);
            moby->state = 3;
        }
    }
    if (vars->held != 0 && !hero.item_use_locked) {
        out = (float *)vars->held->pvars;
        arc.q = 0;
        arc.f[0] = frame_time_macro * (item_text_variant[0x11]
                                           ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                           : ((controller_state.held & 5) ? 6.0f : 4.0f));
        launch.q = arc.q;
        transform_vector_by_basis(&launch, &launch, hero.basis);
        pos.f[0] = fast_cos(fast_add_rotations(-0.361968011f,
                                               fast_arc_tan(hero.facing_x, hero.facing_y))) *
                   0.859039009f;
        pos.f[1] = fast_sin(fast_add_rotations(-0.361968011f,
                                               fast_arc_tan(hero.facing_x, hero.facing_y))) *
                   0.859039009f;
        pos.f[2] = 0.0f;
        fast_vec_add(&pos, &pos, &hero.motion.pos);
        pos.f[2] += 0.490819991f;
        angle = fast_arc_tan(launch.f[0], launch.f[1]);
        fast_vec_scale(&target, &launch, frame_scale_inv_macro * 60.0f);
        fast_vec_add(&target, &target, &pos);
        if (controller_state.held & 5) {
            probe.q = 0;
            probe.f[2] = item_text_variant[0x11] ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                                 : ((controller_state.held & 5) ? 6.0f : 4.0f);
            arc.q = probe.q;
            hit = 0;
            transform_vector(&arc, &arc, matrix);
            fast_subtract_rotations(hero.moby->rot.z, fast_arc_tan(arc.f[0], arc.f[1]));
            angle = fast_arc_tan(arc.f[0], arc.f[1]);
            slope = fast_arc_tan(vector_length_xy(&arc), arc.f[2]);
            if (0.593411922f < slope)
                slope = 0.593411922f;
            if (slope < -1.39626336f)
                slope = -1.39626336f;
            build_spherical_offset(&arc,
                                   item_text_variant[0x11]
                                       ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                       : ((controller_state.held & 5) ? 6.0f : 4.0f),
                                   angle,
                                   slope);
            {
                float distance = vector_length_xy(&arc);
                float z = arc.f[2];
                if (distance != 0.0f)
                    horizontal = z / distance;
                else
                    horizontal = z / 0.01f;
            }
            root = square_root(frame_time_sq_macro * 9.8f *
                               (item_text_variant[0x11]
                                    ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                    : ((controller_state.held & 5) ? 6.0f : 4.0f)) *
                               0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 9.8f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                arc.f[0] = fast_cos(angle) * radius;
                arc.f[1] = fast_sin(angle) * radius;
                arc.f[2] = vertical;
                fast_vec_add(&probe, &D_L00_00166DC0, &arc);
                if (collision_line(&D_L00_00166DC0, &probe, 2, moby, 0) &&
                    coll_hit.moby != (s32)hero.moby) {
                    qcopy(&target, &coll_hit.point);
                    hit = 1;
                    angle = fast_arc_tan(target.f[0] - pos.f[0], target.f[1] - pos.f[1]);
                }
                if (!hit) {
                    target.f[0] = fast_cos(angle) * radius * 5.0f;
                    target.f[1] = fast_sin(angle) * radius * 5.0f;
                    target.f[2] = vertical * 5.0f;
                    fast_vec_add(&target, &target, &D_L00_00166DC0);
                }
            }
        }
        FUN_L00_002c1928_f(frame_time_sq_macro * 9.8f,
                           1.0f,
                           item_text_variant[0x11] ? ((controller_state.held & 5) ? 12.0f : 4.0f)
                                                   : ((controller_state.held & 5) ? 6.0f : 4.0f),
                           pos.f,
                           target.f,
                           out);
    }
    switch (moby->state) {
    case MINE_GLOVE_INITIALIZE:
        vars->held = 0;
        if (!(moby->anim_flags & 2))
            moby->state = 1;
        else
            moby->state = 2;
    case MINE_GLOVE_WAIT_FOR_ANIMATION:
        if (moby->anim_flags & 2)
            moby->state = 2;
        break;
    case MINE_GLOVE_RELEASE_HELD_MOBY:
        if (vars->held != 0) {
            initialize_moby_motion_at_position(&pos, vars->held, vars->held->pvars);
            if (D_00141680_2c1ad0.deliveries <= 0xfffe)
                ++D_00141680_2c1ad0.deliveries;
            {
                s32 frames = scale_game_frames(D_0015EEA4_macro);
                frames /= 600;
                if (D_00141680_2c1ad0.unkA2 < frames)
                    D_00141680_2c1ad0.unkA2 = scale_game_frames(D_0015EEA4_macro) / 600;
            }
            D_00141680_2c1ad0.levels =
                (D_00141680_2c1ad0.levels | (1u << current_level_index_macro)) | 0x80000000;
            append_unique_moby_index(vars->held, D_L00_001B07B0, 0x3F);
            vars->held = 0;
            vars->timer54 = scale_game_frames(10);
        }
        moby->state = 4;
        vars->timer58 = scale_game_frames(0x14);
        break;
    case MINE_GLOVE_POST_RELEASE_WAIT:
        if (hero.state.current != 0x23 && !hero.item_anim_active)
            moby->state = 2;
        break;
    case MINE_GLOVE_BEGIN_HELD_MOBY_REMOVAL:
        moby->state = 6;
        break;
    case MINE_GLOVE_REMOVE_HELD_MOBY:
        if (vars->held != 0) {
            mark_moby_for_removal(vars->held);
            vars->held = 0;
        }
        break;
    }
    fast_dec_timer(&vars->timer54);
    if (vars->held != 0) {
        qcopy(&vars->held->pos, &vars->grip);
    } else if (vars->timer54 == 0 && get_ammo(-1)) {
        qzero(&launch);
        vars->held = spawn_mine(moby, &vars->grip, &launch);
    }
}
/*
 * TODO(route): this body is DONE - it is byte-identical to retail under the
 * bundled GNU as (572/572 bytes; the only 23 differing words are relocation
 * sites: jal/lui/%lo with 0 addends). The current game route (cc1 -> Ps2EeAs)
 * reports SIZE 580/572 because Ps2EeAs 1.9.25.758 auto-inserts 2 NOPs before a
 * div.s that sits "too near a branch instruction / possible branch
 * destination" (its divbug workaround), and the else-block div.s here is right
 * after the $L7 label. Retail never has that padding: a byte scan of the whole
 * retail corpus found 0 `nop; nop; div` in 346 div words of SCUS_971.99 and
 * 26,539 div words across all 19 level text.bin - so the retail-era assembler
 * did not apply this workaround, and our vendored Ps2EeAs is from a different
 * (later) build.
 *
 * To make this function exact, whoever owns the route must first fix the
 * toolchain mismatch; this source needs no further change. Options, both
 * fail-closed (strip only NOPs that Ps2EeAs inserted relative to the GNU-as
 * reference, and only when the trimmed stream still explains every retail
 * word; validate with the whole-ELF gate):
 *   (a) in the game repo's padless-asm.py `finish` step (and the tools'
 *       overlay checker), drop the assembler-inserted div padding using the
 *       GNU-as reference object assembled from the same .s; or
 *   (b) assemble overlays with the bundled GNU as instead of Ps2EeAs, if a
 *       corpus check shows the overlays contain no Ps2EeAs-only output
 *       (short-loop / FPU hazard NOPs) - those ARE in retail and must stay.
 * Evidence and numbers: analysis/overlays/ps2eeas-div-padding.md (tools repo).
 * After the route is fixed: re-score this file, expect EXACT 572/572, then
 * promote normally. Do NOT add inline asm or a per-unit exception for this.
 */

typedef union {
    OvlQuad q;
    f32     f[4];
    s32     i[4];
} V2c2488;

extern s32  pad_held_lui[] __asm__("D_0013CAE0") NOT_SDA;
extern u8   bomb_glove_aimed_range_modifier[] __asm__("D_0013E52A");
extern void sub_2c(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32  scale_ticks(s32) __asm__("FUN_001fa6c0");
extern f32  FUN_L00_0025abf0(void *, void *, f32, f32, s32);

void aim_throw_arc(V2c2488 *pos, V2c2488 *tgt, f32 *out, f32 speed, f32 unused, f32 range) __asm__("FUN_L00_002c2488");
void aim_throw_arc(V2c2488 *origin_position,
                   V2c2488 *target_position,
                   f32     *out,
                   f32      speed,
                   f32      unused,

                   f32 range) {
    V2c2488      effective_target_position;
    V2c2488      u;
    f32          launch_heading;
    f32          horizontal_speed;
    f32          k;
    f32          v;
    struct Hero *P;
    if (range < fast_vec_dist(origin_position, target_position)) {
        sub_2c(&effective_target_position, target_position, origin_position);
        fast_vec_normalize(&effective_target_position, &effective_target_position, range - 0.01f);
        fast_vec_add(&effective_target_position, &effective_target_position, origin_position);
    } else {
        effective_target_position.q = target_position->q;
    }
    launch_heading = fast_arc_tan(effective_target_position.f[0] - origin_position->f[0],
                                  effective_target_position.f[1] - origin_position->f[1]);
    P = &hero;
    if (fast_difference_between_rotations(P->motion.rot.f[2], launch_heading) > 0.87266463f ||
        vector_distance(origin_position, &effective_target_position) < 1.5f) {
        launch_heading = P->motion.rot.f[2];
        u.f[0] = fast_cos(launch_heading) * 1.5f;
        u.f[1] = fast_sin(P->motion.rot.f[2]) * 1.5f;
        u.i[2] = 0;
        fast_vec_add(&u, &u, origin_position);
        effective_target_position.f[0] = u.f[0];
        effective_target_position.f[1] = u.f[1];
    }
    horizontal_speed = frame_time * 8.5f;
    out[0] = fast_cos(launch_heading) * horizontal_speed;
    out[1] = fast_sin(launch_heading) * horizontal_speed;
    *(s32 *)&out[2] = 0;
    out[2] =
        FUN_L00_0025abf0(origin_position, &effective_target_position, horizontal_speed, -speed, 0);
    if (pad_held_lui[0] & 5) {
        k = scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f;
        k = k / horizontal_speed;
    } else {
        k = 8.5f / horizontal_speed;
    }
    v = speed * k * 0.5f;
    if (v < out[2]) {
        out[2] = v;
    }
}

typedef struct {
    u8           pad0[0x40];
    OvlVec4      grip;
    struct Moby *held;
    s32          timer;
    s32          delay;
} CarrierVars_2c26c8;

typedef struct {
    u8  pad0[0x50];
    u16 deliveries; /* D_001416D0: saturates at 0xFFFF */
    u16 unkA2;
    u32 levels;
} DeliveryStats_2c26c8;

extern DeliveryStats_2c26c8 D_00141680_2c26c8 __asm__("D_00141680");
extern struct Moby         *D_L00_001ABA00_lui[] __asm__("D_L00_001ABA00") NOT_SDA;

#include "rnc/input/pad_state.h"

extern struct Moby *spawn_bomb(struct Moby *, void *) __asm__("FUN_L00_002ace70");
extern void         launch_bomb(void *, struct Moby *, void *) __asm__("FUN_L00_002acfd8");
/* Updates the grip, chooses a throw target, and handles the held moby states. */
void bomb_glove_update(struct Moby *moby) __asm__("FUN_L00_002c26c8");

void bomb_glove_update(struct Moby *moby) {
    enum {
        BOMB_GLOVE_INITIALIZE = 0,
        BOMB_GLOVE_WAIT_FOR_ANIMATION = 1,
        BOMB_GLOVE_RELEASE_HELD_MOBY = 3,
        BOMB_GLOVE_POST_RELEASE_WAIT = 4,
        BOMB_GLOVE_BEGIN_HELD_MOBY_REMOVAL = 5,
        BOMB_GLOVE_REMOVE_HELD_MOBY = 6
    };
    CarrierVars_2c26c8 *vars;
    OvlVec4             local[4], saved, offset, rotatedOffset, pos, target, basis[3], matrix[4];
    OvlVec4             launch;
    union {
        OvlVec4 arc;
    } temporary;
    OvlVec4 direction;
    OvlVec4 probe;
    f32     angle, slope, distance, horizontal, vertical, radius, speed;
    f32     gravity, root;
    s32     hit = 0;

    if (moby == 0)
        return;
    vars = (CarrierVars_2c26c8 *)moby->pvars;
    offset.q = 0;
    offset.f[1] = -0.09f;
    offset.f[2] = -0.02f;
    copy_matrix3x4(basis, l00_camera_lui.unk0);
    transpose_rotation_matrix(matrix, basis);
    if (vars == 0)
        return;
    if (vars->delay < 3) {
        ++vars->delay;
        return;
    }
    FUN_L00_0024f7c8((u8 *)moby, 0, &saved);
    build_moby_bone_transform(moby, 0, local);
    transform_vector(&rotatedOffset, &offset, local);
    fast_vec_add(&vars->grip, &saved, &rotatedOffset);
    if (vars->held != 0)
        vars->held->collision_disable = 1;
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD)
        vars->held = 0;
    if (hero.state.current == 1)
        hero_set_state(0x1E, 1);
    if (fast_dec_timer(&vars->timer) && !hero.item_use_locked) {
        if ((hero.item_anim_active && hero.item_anim_frames == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) && hero.state.current == 0x1E &&
             get_ammo(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            allocate_hero_voice_for_entry(0x1A, 0);
            use_ammo(-1, 1);
            moby->state = 3;
        }
    }
    if (vars->held != 0) {
        f32  best = 15.0f;
        f32 *out = (f32 *)vars->held->pvars;
        launch.q = 0;
        launch.f[0] = 1.0f;
        transform_vector_by_basis(&launch, &launch, hero.basis);
        pos.f[0] = fast_cos(fast_add_rotations(-0.361968011f,
                                               fast_arc_tan(hero.facing_x, hero.facing_y))) *
                   0.859039009f;
        pos.f[1] = fast_sin(fast_add_rotations(-0.361968011f,
                                               fast_arc_tan(hero.facing_x, hero.facing_y))) *
                   0.859039009f;
        pos.f[2] = 0.0f;
        fast_vec_add(&pos, &pos, &hero.motion.pos);
        pos.f[2] += 0.490819991f;
        angle = fast_arc_tan(launch.f[0], launch.f[1]);
        {
            f32 speed = (controller_state.held & 5)
                            ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                            : 8.5f;
            fast_vec_scale(&target, &launch, speed);
        }
        fast_vec_add(&target, &target, &pos);
        if ((controller_state.held & 5) && !hero.item_use_locked) {
            temporary.arc.q = 0;
            temporary.arc.f[2] = 1.0f;
            transform_vector(&temporary.arc, &temporary.arc, matrix);
            hit = 0;
            fast_subtract_rotations(hero.moby->rot.z,
                                    fast_arc_tan(temporary.arc.f[0], temporary.arc.f[1]));
            angle = fast_arc_tan(temporary.arc.f[0], temporary.arc.f[1]);
            slope = fast_arc_tan(vector_length_xy(&temporary.arc), temporary.arc.f[2]);
            if (0.593411922f < slope)
                slope = 0.593411922f;
            if (slope < -1.39626336f)
                slope = -1.39626336f;
            {
                f32 speed = (controller_state.held & 5)
                                ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                                : 8.5f;
                build_spherical_offset(&temporary.arc, speed, angle, slope);
            }
            qcopy(&direction, &D_L00_00166DC0_2c1ad0);
            {
                f32 distance = vector_length_xy(&temporary.arc);
                f32 z = temporary.arc.f[2];
                if (distance != 0.0f)
                    horizontal = z / distance;
                else
                    horizontal = z / 0.01f;
            }
            gravity = frame_time_sq_macro * 11.0f;
            root = square_root(gravity *
                               ((controller_state.held & 5)
                                    ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                                    : 8.5f) *
                               0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 11.0f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                temporary.arc.f[0] = fast_cos(angle) * radius;
                temporary.arc.f[1] = fast_sin(angle) * radius;
                temporary.arc.f[2] = vertical;
                fast_vec_add(&probe, &D_L00_00166DC0_2c1ad0, &temporary.arc);
                if (collision_line(&D_L00_00166DC0_2c1ad0, &probe, 2, moby, 0) &&
                    coll_hit.moby != (s32)hero.moby) {
                    qcopy(&target, &coll_hit.point);
                    hit = 1;
                    angle = fast_arc_tan(target.f[0] - pos.f[0], target.f[1] - pos.f[1]);
                }
                if (!hit) {
                    target.f[0] = fast_cos(angle) * radius * 5.0f;
                    target.f[1] = fast_sin(angle) * radius * 5.0f;
                    target.f[2] = vertical * 5.0f;
                    fast_vec_add(&target, &target, &D_L00_00166DC0_2c1ad0);
                }
            }
        } else {
            s32          entry = 0;
            struct Moby *candidate;
            hero.throw_target = 0;
            while ((candidate = D_L00_001ABA00_lui[entry]) != 0) {
                u8 *data = suck_get_aim_height(candidate);
                if ((s8)candidate->state >= 0 && data && (candidate->flags & 0x1000)) {
                    f32 candidateAngle, elevation, difference;
                    qcopy(&temporary.arc, &candidate->pos);
                    temporary.arc.f[2] += *(f32 *)(data + 0x10);
                    distance = vector_distance(&pos, &temporary.arc);
                    candidateAngle =
                        fast_arc_tan(temporary.arc.f[0] - pos.f[0], temporary.arc.f[1] - pos.f[1]);
                    elevation = fast_arc_tan(vector_distance(&pos, &temporary.arc),
                                             temporary.arc.f[2] - pos.f[2]);
                    difference = fast_difference_between_rotations(angle, candidateAngle);
                    if ((difference < 0.785398185f ||
                         (distance < 4.0f && difference < 1.57079637f)) &&
                        distance < best && elevation < 0.959931076f &&
                        temporary.arc.f[2] - pos.f[2] < 2.0f &&
                        !collision_line(&D_L00_00166DC0_2c1ad0, &temporary.arc, 6, moby, 0)) {
                        best = distance;
                        hero.throw_target = (u8 *)candidate;
                        angle = candidateAngle;
                        qcopy(&target, &temporary.arc);
                    }
                }
                ++entry;
            }
        }
        gravity = frame_time_sq_macro * 11.0f;
        speed = (controller_state.held & 5)
                    ? scale_ticks(bomb_glove_aimed_range_modifier[0]) * 2.5f + 8.5f
                    : 8.5f;
        aim_throw_arc((V2c2488 *)&pos, (V2c2488 *)&target, out, gravity, 0.00000999999975f, speed);
    }
    switch (moby->state) {
    case BOMB_GLOVE_INITIALIZE:
        vars->held = 0;
        if (!(moby->anim_flags & 2))
            moby->state = 1;
        else
            moby->state = 2;
    case BOMB_GLOVE_WAIT_FOR_ANIMATION:
        if (moby->anim_flags & 2)
            moby->state = 2;
        break;
    case BOMB_GLOVE_RELEASE_HELD_MOBY:
        if (vars->held == 0) {
            vars->held = spawn_bomb(moby, &vars->grip);
        } else {
            launch_bomb(&pos, vars->held, vars->held->pvars);
            if (D_00141680_2c26c8.deliveries <= 0xfffe)
                ++D_00141680_2c26c8.deliveries;
            {
                s32 frames = scale_game_frames(D_0015EEA4);
                frames /= 600;
                if (D_00141680_2c26c8.unkA2 < frames)
                    D_00141680_2c26c8.unkA2 = scale_game_frames(D_0015EEA4) / 600;
            }
            D_00141680_2c26c8.levels =
                (D_00141680_2c26c8.levels | (1u << current_level_index)) | 0x80000000;
            vars->held = 0;
            moby->state = 4;
            vars->timer = scale_game_frames(0x14);
        }
        break;
    case BOMB_GLOVE_POST_RELEASE_WAIT:
        if (hero.state.current != 0x23 && !hero.item_anim_active)
            moby->state = 2;
        break;
    case BOMB_GLOVE_BEGIN_HELD_MOBY_REMOVAL:
        moby->state = 6;
        break;
    case BOMB_GLOVE_REMOVE_HELD_MOBY:
        /* Retail reloads held after destruction before the common tail. */
        if (vars->held != 0)
            mark_moby_for_removal(vars->held);
        break;
    }
    if (vars->held != 0)
        qcopy(&vars->held->pos, &vars->grip);
    else if (get_ammo(-1) || hero.item_anim_active)
        vars->held = spawn_bomb(moby, &vars->grip);
}

extern u8   D_0013E539;
extern s32  moby_table __asm__("D_L00_0015FFD8");
extern s16  D_L00_001B0830_lui[] __asm__("D_L00_001B0830") NOT_SDA;
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void append_unique_moby_index(void *, void *, s32) __asm__("FUN_L00_0025f780");

u8 *spawn_decoy(s32 value, u128 *position, u128 *other_position) __asm__("FUN_L00_002c35c0");

u8 *spawn_decoy(s32 value, u128 *position, u128 *other_position) {
    u8 *flags = item_text_variant;
    u8 *spawned_moby;
    u8 *data;
    u8 *candidate;
    s32 i;
    s32 best_value;
    s32 current_value;
    f32 speed;

    spawned_moby = create_moby(flags[0x19] ? 0x76c : 0xcb);
    if (spawned_moby != 0) {
        spawned_moby[0x30] = 0xff;
        *(u16 *)(spawned_moby + 0x32) = 0xff;
        spawned_moby[0x31] = 1;
        data = *(u8 **)(spawned_moby + 0x78);
        spawned_moby[0x20] = 0;
        qcopy(spawned_moby + 0x10, position);
        qcopy(data + 0x10, other_position);
        speed = D_0015ED60_2c5be8 * -0.5f;
        *(s32 *)(data + 0x40) = value;
        *(u16 *)(data + 0x44) = 0;
        *(s32 *)(data + 0x54) = 0;
        *(f32 *)(spawned_moby + 0x2c) =
            *(f32 *)(*(u8 **)(spawned_moby + 0x24) + 0x24) * (speed + 1.0f);
        *(f32 *)(data + 0x48) = (scale_ticks(flags[0x19]) + 1.0f) * 4.0f;
        if (spawned_moby[0x53] != 0)
            blend_moby_animation(spawned_moby, 0, 0, scale_game_frames(0));
        *(s32 *)(spawned_moby + 0x94) = 0;
        *(s32 *)(spawned_moby + 0x98) = -1;
        *(f32 *)(spawned_moby + 0x48) = random_float_between(-3.14159274f, 3.14159274f);
        *(s32 *)(data + 0x58) = 0;
        *(s32 *)(data + 0x5c) = 0;
        while (D_L00_001B0830_lui[0] >= 6) {
            candidate = 0;
            best_value = 0;
            for (i = 1; i <= D_L00_001B0830_lui[0]; i++) {
                u8 *entry = (u8 *)(moby_table + (D_L00_001B0830_lui[i] << 8));
                if (entry != 0 &&
                    (*(s16 *)(entry + 0xa6) == 0xcb || *(s16 *)(entry + 0xa6) == 0x76c) &&
                    entry[0x20] < 5) {
                    current_value = *(s32 *)(*(u8 **)(entry + 0x78) + 0x54);
                    if (best_value <= current_value) {
                        candidate = entry;
                        best_value = current_value;
                    }
                }
            }
            if (candidate != 0) {
                mine_spawn_drops(candidate, D_L00_001B0830_lui);
                candidate[0x20] = 6;
            }
        }
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
        *(u16 *)data = 0;
        data[8] = 0;
        data[9] = 0;
        data[7] = 0x80;
        *(u16 *)(data + 0x0c) = 4;
        *(u16 *)(data + 0x0e) = 0xf;
        append_unique_moby_index(spawned_moby, D_L00_001B0830_lui, 0x1f);
        if (hero.unk20A5 != 0 || hero.unk20AF != 0)
            *(u16 *)(spawned_moby + 0x34) |= 0x41;
    }
    return spawned_moby;
}

#include "qzero.h"

/* Tests a movement vector against the player and turns a colliding moby. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C4B90), where it is exact; names translated to the US level program. */

extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float square_root(float) __asm__("FUN_001f9988");
extern f32   fast_vec_length(void *a) __asm__("FUN_001f9af0");

typedef struct {
    char  pad[0x28];
    float height;
    char  pad2[0x14];
    float vector[4];
} S173E40;

extern S173E40        D_L00_00173E40;
extern s32            scale_game_frames_c(s32) __asm__("func_001F96F8");
extern unsigned char *FUN_L00_002603d0(int);
extern void           fast_vec_normalize(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern char           D_00173F60_alias[] __asm__("D_L00_00173E60");

void release_moby_with_collision_check(float *release_position,
                                       char  *moby,
                                       void  *initial_velocity) __asm__("FUN_L00_002c3888");

void release_moby_with_collision_check(float *release_position,
                                       char  *moby,

                                       void *initial_velocity) {
    float        collision_start[4], collision_normal[4], rotated_normal[4];
    char        *state = *(char **)(moby + 0x78);
    struct Hero *camera;
    char        *player;
    qcopy(state + 0x10, initial_velocity);
    {
        struct Hero *view = &hero;
        if (((int)view->ground_moby) && FUN_L00_002603d0(((int)view->ground_moby)))
            fast_vec_add(state + 0x10, state + 0x10, &view->motion.displacement);
    }
    qcopy(moby + 0x10, release_position);
    camera = &hero;
    player = ((char *)camera->moby);
    *(OvlQuad *)collision_start = *(OvlQuad *)(player + 0x10);
    collision_start[2] = release_position[2];
    fast_vec_sub(release_position, release_position, collision_start);
    fast_vec_normalize(
        release_position, release_position, fast_vec_length(release_position) + 0.2f);
    fast_vec_add(release_position, release_position, collision_start);
    if (collision_line(collision_start, release_position, 0, ((int)camera->moby), 0)) {
        float smoothing;
        qcopy(moby + 0x10, D_00173F60_alias);
        qzero(state + 0x10);
        moby[0x20] = 4;
        *(short *)(state + 0x44) = scale_game_frames_c(30);
        *(float *)(moby + 0x18) = D_L00_00173E40.height;
        qcopy(collision_normal, D_L00_00173E40.vector);
        {
            float co, si;
            co = fast_cos(*(float *)(moby + 0x48));
            smoothing = 0.1f;
            si = fast_sin(*(float *)(moby + 0x48));
            rotated_normal[0] = collision_normal[0] * co + collision_normal[1] * si;
        }
        {
            float co, si;
            co = fast_cos(*(float *)(moby + 0x48));
            si = fast_sin(*(float *)(moby + 0x48));
            rotated_normal[1] = collision_normal[1] * co - collision_normal[0] * si;
            rotated_normal[2] = collision_normal[2];
        }
        *(float *)(state + 0x4C) = -fast_arc_tan(square_root(rotated_normal[0] * rotated_normal[0] +
                                                             rotated_normal[2] * rotated_normal[2]),
                                                 rotated_normal[1]);
        *(float *)(state + 0x50) = fast_arc_tan(rotated_normal[2], rotated_normal[0]);
        *(float *)(moby + 0x40) = fast_add_rotations(
            *(float *)(moby + 0x40),
            fast_subtract_rotations(*(float *)(state + 0x4C), *(float *)(moby + 0x40)) *
                (frame_scale * smoothing));
        *(float *)(moby + 0x44) = fast_add_rotations(
            *(float *)(moby + 0x44),
            fast_subtract_rotations(*(float *)(state + 0x50), *(float *)(moby + 0x44)) *
                (frame_scale * smoothing));
    } else
        moby[0x20] = 2;
}

/* Vars of a thrown moby (FUN_L00_002c3888 uses the same block). */
typedef struct {
    u8           pad_00[0x10];
    Vec4         vel;     /* 0x10: velocity per step */
    Vec4         landing; /* 0x20: predicted landing point */
    Vec4         normal;  /* 0x30: surface normal there */
    struct Moby *holder;  /* 0x40: moby it was thrown from */
    s16          timer;   /* 0x44 */
    s16          unk46;
} ThrowVars;

extern s32            D_L00_0015F5C4; /* nonzero: no throws */
extern f32 frame_time MACRO_ADDR;     /* frame-rate scale */
/* coll_hit's normal as a whole quadword (CollisionHit names only x/y/z) */
extern Vec4 D_L00_00166DC0 NOT_SDA;
extern void                FUN_001f4600(void *, void *);
extern void                draw_moby_oscillating_textured_quads(void) __asm__("FUN_L00_002c31c0");

/*
 * Predicts where a thrown moby lands: steps the velocity (with gravity) from
 * its start until collision_line hits ground (or a moby other than the hero
 * and the holder), for up to `timer` steps. Stores the landing point and
 * normal in vars and pulls the point 5% toward D_L00_00166DC0. mode 0 needs
 * a live hit moby; mode 1 starts from the hero's hand. Returns 1 on a hit.
 */
s32 predict_thrown_moby_landing(ThrowVars   *throw_vars,
                                struct Moby *thrown_moby,
                                s32          mode) __asm__("FUN_L00_002c3af8");

s32 predict_thrown_moby_landing(ThrowVars *throw_vars, struct Moby *thrown_moby, s32 mode) {
    Vec4 predicted_position, predicted_velocity, position_scratch, q, ground_probe_position;
    s32  prediction_countdown;
    s32  found = 0;
    s32  i;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4 != 0)
        return 0;
    qcopy(&position_scratch, &thrown_moby->pos);
    if (thrown_moby->state == 1) {
        predicted_position.f[0] =
            fast_cos(fast_add_rotations(-0.36196801066398620605f,
                                        fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.85903900861740112305f;
        predicted_position.f[1] =
            fast_sin(fast_add_rotations(-0.36196801066398620605f,
                                        fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.85903900861740112305f;
        predicted_position.f[2] = 0.0f;
        fast_vec_add(&predicted_position, &predicted_position, &hero.motion.pos);
        predicted_position.f[2] += 0.39081999659538269043f;
        qcopy(&predicted_velocity, &throw_vars->vel);
        if (hero.state.control_mode == 15)
            fast_vec_add(&predicted_velocity, &predicted_velocity, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0((int)hero.ground_moby) && mode)
            fast_vec_add(&predicted_velocity, &predicted_velocity, &hero.motion.displacement);
        prediction_countdown = scale_game_frames(300);
    } else {
        u8 *holder_vars = throw_vars->holder->pvars;

        qcopy(&predicted_position, &thrown_moby->pos);
        qcopy(&predicted_velocity, &throw_vars->vel);
        {
            s32 timer = throw_vars->timer;
            s32 flag = *(s32 *)(holder_vars + 0x50);

            prediction_countdown = timer;
            if (flag)
                throw_vars->unk46 = 1;
        }
    }
    predicted_velocity.f[2] -= frame_time * 0.5f;
    q.q = ((Vec4 *)&hero.moby->pos)->q;
    q.f[2] = predicted_position.f[2];
    if (collision_line(&q, &predicted_position, 0x10, hero.moby, 0) &&
        (collision_hit_material() || 0.0f < predicted_velocity.f[2])) {
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby != hero.moby &&
                 (struct Moby *)coll_hit.moby != throw_vars->holder) ||
                prediction_countdown < scale_game_frames(300) - scale_game_frames(10)) {
                ground_probe_position.q = 0;
                ground_probe_position.f[2] = 0.5f;
                qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
                found = 1;
                fast_vec_add(&ground_probe_position, &ground_probe_position, &throw_vars->landing);
                probe_ground_height(&ground_probe_position, 0, 0.5f);
            }
        } else if (coll_hit.unk1C > 0) {
            qcopy(&throw_vars->landing, &coll_hit.point);
            found = 1;
        }
    }
    while (!fast_dec_timer(&prediction_countdown) && !found) {
        qcopy(&position_scratch, &predicted_position);
        for (i = 9; i >= 0; i--) {
            fast_vec_add(&predicted_position, &predicted_position, &predicted_velocity);
            predicted_velocity.f[2] -= frame_time_sq_macro * 9.0f;
        }
        if (!collision_line(&position_scratch, &predicted_position, 0x10, thrown_moby, 0))
            continue;
        if (!collision_hit_material() && 0.0f < predicted_velocity.f[2])
            continue;
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby == hero.moby ||
                 (struct Moby *)coll_hit.moby == throw_vars->holder) &&
                !(prediction_countdown < scale_game_frames(300) - scale_game_frames(10)))
                continue;
            ground_probe_position.q = 0;
            ground_probe_position.f[2] = 0.5f;
            qcopy(&throw_vars->landing, &((struct Moby *)coll_hit.moby)->pos);
            fast_vec_add(&ground_probe_position, &ground_probe_position, &throw_vars->landing);
            probe_ground_height(&ground_probe_position, 0, 0.5f);
            break;
        } else if (coll_hit.unk1C > 0) {
            qcopy(&throw_vars->landing, &coll_hit.point);
            break;
        }
    }
    if (prediction_countdown == 0)
        return 0;
    if (mode == 0) {
        if (coll_hit.moby == 0)
            return 0;
        if (FUN_L00_002603d0(coll_hit.moby) == 0)
            goto ret0;
    }
    if (mode != 2 || coll_hit.moby == 0 || !FUN_L00_002603d0(coll_hit.moby))
        FUN_001f4600(draw_moby_oscillating_textured_quads, thrown_moby);
    throw_vars->normal.q = coll_hit_normal.q;
    fast_vec_sub(&position_scratch, &throw_vars->landing, &D_L00_00166DC0);
    fast_vec_scale(&position_scratch, &position_scratch, 0.94999998807907104492f);
    fast_vec_add(&throw_vars->landing, &position_scratch, &D_L00_00166DC0);
    return 1;
ret0:
    /* a second return 0 block, as in retail */
    return 0;
}

extern char *D_0015FFD8_2c4380 __asm__("D_L00_0015FFD8");
extern float D_0015ED60_2c4380 __asm__("D_0015ED60");
extern float D_0015ED70_2c4380 __asm__("D_0015ED70");
extern float D_0015ED6C_2c4380 __asm__("D_0015ED6C");
extern float FUN_00200240_2c4380(int) __asm__("FUN_001fa6c0");
extern int   FUN_L00_0025a420(void *, int, int);
extern int   FUN_L00_00258830(void *, void *, float, float, float, int);
extern int   coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern void  FUN_L00_00260400(void *, int, void *, void *, void *, void *);
extern void  update_decoy_proximity_motion(S_2c40a0 *) __asm__("FUN_L00_002c40a0");
extern void  destroy_decoy_on_selected_materials(void *) __asm__("FUN_L00_002c4328");
extern void  trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void  add_random_offset_to_vector_xyz(float, void *) __asm__("FUN_L00_0025f8e0");
extern void  spawn_explosion(float,
                             void *,
                             void *,
                             int,
                             float,
                             float,
                             int,
                             int,
                             int,
                             float,
                             float,
                             float,
                             float,
                             int,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void  initialize_hit_descriptor(float, void *, void *, int) __asm__("FUN_L00_00259858");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void  FUN_0028dbe8_2c4380(int, int, void *) __asm__("FUN_0022da68");
extern int   FUN_001fef20_2c4380(int) __asm__("FUN_001f96f8");
extern int   FUN_001fef68_2c4380(void *) __asm__("FUN_001f9740");
extern int   fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float FUN_001ff3d8_2c4380(void *) __asm__("FUN_001f9af0");
extern float FUN_001ff408_2c4380(void *) __asm__("FUN_001f9b20");
extern float FUN_001ff1e0_2c4380(float) __asm__("FUN_001f9988");
extern void  FUN_001ff2a8_2c4380(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001ff278_2c4380(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  FUN_001ff300_2c4380(float, void *, void *) __asm__("FUN_001f9a68");
extern float FUN_001ff8b0_2c4380(float, float) __asm__("FUN_001f9e90");
extern float FUN_001ff7e8_2c4380(float) __asm__("FUN_001f9dc8");
extern float FUN_001ff800_2c4380(float) __asm__("FUN_001f9de0");
extern float FUN_00200130_2c4380(float, float) __asm__("FUN_001fa5c8");
extern float FUN_002000e8_2c4380(float, float) __asm__("FUN_001fa580");

void decoy_update(char *moby) __asm__("FUN_L00_002c4380");

void decoy_update(char *moby) {
    enum {
        DECOY_INITIALIZE = 0,
        DECOY_HELD = 1,
        DECOY_THROWN_MOVEMENT = 2,
        DECOY_SETTLED_MOVEMENT = 3,
        DECOY_PRE_REMOVAL_DELAY = 4,
        DECOY_WAIT_TO_SHRINK = 5,
        DECOY_SHRINK_FOR_REMOVAL = 6,
        DECOY_RESET_TO_INITIALIZE = 7
    };
    int   active = ((unsigned char *)&D_0013E533)[6];
    char *vars = *(char **)(moby + 0x78);
    char *target;
    float position_effect_and_normal_scratch[4] __attribute__((aligned(16)));
    struct {
        float w[4];
        float x[4];
    } scratch __attribute__((aligned(16)));
    float  rotated_normal[4] __attribute__((aligned(16)));
    float  len, angle, cos_angle, sin_angle, rate;
    int    i, hit, state;
    short *ids;
    float  effect;
    if (*(short *)(moby + 0xa6) == 0xcb)
        active = 0;
    effect = FUN_00200240_2c4380(active);
    if (*(float *)(moby + 0x10) < 2.0f || 1021.0f < *(float *)(moby + 0x10) ||
        *(float *)(moby + 0x14) < 2.0f || 1021.0f < *(float *)(moby + 0x14) ||
        *(float *)(moby + 0x18) < 2.0f || 1021.0f < *(float *)(moby + 0x18)) {
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    if ((hero.items[0].moby == 0 || *(short *)((char *)hero.items[0].moby + 0xa6) != 0x232) &&
        (u8)moby[0x20] == 1) {
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    if (hero.vehicle_mode != 0 && (u8)moby[0x20] == 1) {
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    ++*(int *)(vars + 0x54);
    if (hero.unk20A5 != 0 && (u8)moby[0x20] < 2)
        *(u16 *)(moby + 0x34) |= 0x41;
    else
        *(u16 *)(moby + 0x34) &= 0xffbe;
    if (*(short *)(*(char **)(vars + 0x40) + 0xa6) == 0x232 && *(short *)(vars + 0x46) == 0 &&
        FUN_001ff3d8_2c4380(vars + 0x10) > 0.0f) {
        if (hero.ground_moby == 0 || !FUN_L00_002603d0((int)hero.ground_moby))
            predict_thrown_moby_landing((ThrowVars *)vars, (struct Moby *)moby, 1);
        else if (!predict_thrown_moby_landing((ThrowVars *)vars, (struct Moby *)moby, 0))
            predict_thrown_moby_landing((ThrowVars *)vars, (struct Moby *)moby, 2);
    }
    switch ((u8)moby[0x20]) {
    case DECOY_INITIALIZE:
        moby[0x20] = 1;
        *(short *)(vars + 0x44) = 0;
        *(int *)(moby + 0x98) = -1;
        *(short *)(vars + 0x6c) = 0;
        vars[0x6e] = 0;
        break;
    case DECOY_HELD: {
        char *target;
        *(short *)(vars + 0x44) = 0;
        target = *(char **)(vars + 0x40);
        if (target && (((u8)target[0x20] & 0xf0) != 0xf0) && hero.items[0].moby &&
            *(short *)((char *)hero.items[0].moby + 0xa6) == 0x232)
            return;
        mine_spawn_drops(moby, D_L00_001B0830_lui);
        mark_moby_for_removal(moby);
        return;
    }
    case DECOY_THROWN_MOVEMENT:
        if (*(float *)(moby + 0x2c) < *(float *)(*(char **)(moby + 0x24) + 0x24))
            *(float *)(moby + 0x2c) += D_0015ED60_2c4380 * 0.02f;
        qcopy(position_effect_and_normal_scratch, moby + 0x10);
        *(float *)(vars + 0x18) -= D_0015ED70_2c4380 * 9.0f;
        for (i = 1; i <= D_L00_001B0830_lui[0]; ++i) {
            char *target;
            ids = D_L00_001B0830_lui + i;
            target = (char *)D_0015FFD8_2c4380 + *ids * 0x100;
            if (target != moby && (u8)target[0x20] > 1 && target && (u8)target[0x20] != 0xfe &&
                (u8)target[0x20] != 0xfd) {
                float len;
                FUN_001ff2a8_2c4380(scratch.w, moby + 0x10, target + 0x10);
                scratch.w[2] = 0.0f;
                len = FUN_001ff3d8_2c4380(scratch.w);
                if (len < 1.8f) {
                    FUN_001ff300_2c4380((1.8f - len) * 0.015f, scratch.w, scratch.w);
                    FUN_001ff278_2c4380(vars + 0x10, vars + 0x10, scratch.w);
                }
            }
        }
        if (hero.state.current == 0x1e)
            *(int *)((char *)hero.moby + 0x94) = 0;
        hit = FUN_L00_00258830(moby, vars + 0x10, 0.2f, 0.2f, 0.0f, 0);
        if (hit & 1) {
            qcopy(scratch.w, moby + 0x10);
            scratch.w[2] += 0.2f;
            if (coll_sphere(scratch.w, 0.23f, 0, moby)) {
                if (*(float *)(vars + 0x18) < 0.0f) {
                    float len;
                    FUN_001ff2a8_2c4380(scratch.x, moby + 0x10, &hero.motion.pos);
                    scratch.x[2] = 0.0f;
                    len = FUN_001ff3d8_2c4380(scratch.x);
                    if (len < 1.8f) {
                        FUN_001ff300_2c4380((1.8f - len) * 0.015f, scratch.x, scratch.x);
                        FUN_001ff278_2c4380(vars + 0x10, vars + 0x10, scratch.x);
                    }
                }
                reflect_vector(vars + 0x10, vars + 0x10, &coll_hit.normal_x);
                if (coll_hit.moby != 0 && coll_hit.unk1C <= 0)
                    FUN_001ff300_2c4380(1.05f, vars + 0x10, vars + 0x10);
                else
                    FUN_001ff300_2c4380(0.2f, vars + 0x10, vars + 0x10);
                rate = 0.01f;
                len = FUN_001ff3d8_2c4380(vars + 0x10);
                if (len < D_0015ED60_2c4380 * rate ||
                    (hero.ground_moby && FUN_L00_002603d0((int)hero.ground_moby) &&
                     FUN_001ff3d8_2c4380(vars + 0x10) <
                         D_0015ED60_2c4380 * rate +
                             FUN_001ff3d8_2c4380(&hero.motion.displacement))) {
                    angle = FUN_001ff8b0_2c4380(coll_hit.normal_z,
                                                FUN_001ff408_2c4380(&coll_hit.normal_x));
                    if (angle < 0.61086524f) {
                        ++*(short *)(vars + 0x44);
                        if (coll_hit.moby == 0 || coll_hit.unk1C > 0 ||
                            *(short *)(vars + 0x44) > FUN_001fef20_2c4380(240)) {
                            if (((u8 *)moby)[0x53] != 1)
                                blend_moby_animation(moby, 1, 0, FUN_001fef20_2c4380(10));
                            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
                            *(int *)(moby + 0x98) = 0;
                            qzero(vars + 0x10);
                            moby[0x20] = 3;
                            *(short *)(vars + 0x44) = FUN_001fef20_2c4380(600);
                            *(float *)(moby + 0x18) = coll_hit.point.f[2];
                            qcopy(scratch.x, &coll_hit.normal_x);
                            cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                            sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                            rotated_normal[0] = scratch.x[0] * cos_angle + scratch.x[1] * sin_angle;
                            cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                            sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                            rotated_normal[1] = scratch.x[1] * cos_angle - scratch.x[0] * sin_angle;
                            rotated_normal[2] = scratch.x[2];
                            *(float *)(vars + 0x4c) = -FUN_001ff8b0_2c4380(
                                FUN_001ff1e0_2c4380(rotated_normal[0] * rotated_normal[0] +
                                                    rotated_normal[2] * rotated_normal[2]),
                                rotated_normal[1]);
                            *(float *)(vars + 0x50) =
                                FUN_001ff8b0_2c4380(rotated_normal[2], rotated_normal[0]);
                            rate = 0.1f;
                            *(float *)(moby + 0x40) =
                                FUN_002000e8_2c4380(*(float *)(moby + 0x40),
                                                    FUN_00200130_2c4380(*(float *)(vars + 0x4c),
                                                                        *(float *)(moby + 0x40)) *
                                                        (D_0015ED60_2c4380 * rate));
                            *(float *)(moby + 0x44) =
                                FUN_002000e8_2c4380(*(float *)(moby + 0x44),
                                                    FUN_00200130_2c4380(*(float *)(vars + 0x50),
                                                                        *(float *)(moby + 0x44)) *
                                                        (D_0015ED60_2c4380 * rate));
                        }
                    }
                }
                destroy_decoy_on_selected_materials(moby);
                if (fast_subtract_rotations_cf(vars + 0x6f))
                    FUN_0028dbe8_2c4380(0, 0, moby), vars[0x6f] = FUN_001fef20_2c4380(20);
                else
                    add_random_offset_to_vector_xyz(D_0015ED6C_2c4380, vars + 0x10);
            }
        }
        if (hero.state.current == 0x1e) {
            target = hero.moby;
            *(int *)(target + 0x94) = *(int *)(*(char **)(target + 0x24) + 0x10);
        }
        break;
    case DECOY_SETTLED_MOVEMENT:
    case DECOY_PRE_REMOVAL_DELAY:
        hit = FUN_L00_0025a420(moby, 0x30001, 0);
        *(float *)(moby + 0x40) = FUN_002000e8_2c4380(
            *(float *)(moby + 0x40),
            FUN_00200130_2c4380(*(float *)(vars + 0x4c), *(float *)(moby + 0x40)) *
                (D_0015ED60_2c4380 * 0.1f));
        *(float *)(moby + 0x44) = FUN_002000e8_2c4380(
            *(float *)(moby + 0x44),
            FUN_00200130_2c4380(*(float *)(vars + 0x50), *(float *)(moby + 0x44)) *
                (D_0015ED60_2c4380 * 0.1f));
        update_decoy_proximity_motion((S_2c40a0 *)moby);
        FUN_001fef68_2c4380(vars + 0x5c);
        fast_dec_timer_s16(vars + 0x6c);
        if (hit) {
            target = (char *)hit;
            if (*(int *)(target + 0x20) != *(int *)(vars + 0x58) || *(int *)(vars + 0x5c) <= 0) {
                *(int *)(vars + 0x58) = *(int *)(target + 0x20);
                *(int *)(vars + 0x5c) = FUN_001fef20_2c4380(30);
                /* Fakematch: the empty loop keeps the timer read below the cooldown store. */
                do {
                } while (0);
                if (*(short *)(vars + 0x6c) == 0) {
                    if (*(u16 *)(target + 0x28) == 0x101 &&
                        ((*(u64 *)(vars + 0x68) & 0xffffff00000000ULL) == 0)) {
                        *(short *)(vars + 0x6c) = FUN_001fef20_2c4380(100);
                        vars[0x6e] = 1;
                    } else
                        vars[0x6e] = 0;
                    FUN_0028dbe8_2c4380(0, 0, moby);
                    {
                        float len = *(float *)(target + 0x2c);
                        ((u8 *)vars)[7] = 0xfa;
                        *(float *)(vars + 0x48) -= len;
                    }
                    trooper_anim_update(moby, vars);
                    if (*(float *)(vars + 0x48) < 0.0f) {
                        *(OvlQuad *)scratch.w = 0;
                        scratch.w[0] = D_0015ED60_2c4380 * 0.5f;
                        scratch.w[2] = D_0015ED60_2c4380 * 0.7f;
                        scratch.w[1] = scratch.w[0];
                        *(OvlQuad *)position_effect_and_normal_scratch = *(OvlQuad *)scratch.w;
                        add_random_offset_to_vector_xyz(scratch.w[0],
                                                        position_effect_and_normal_scratch);
                        spawn_explosion(0,
                                        moby,
                                        position_effect_and_normal_scratch,
                                        0,
                                        0,
                                        4.0f,
                                        0,
                                        0,
                                        0,
                                        3.0f,
                                        9.0f,
                                        1.0f,
                                        12.0f,
                                        2,
                                        1,
                                        1,
                                        -1,
                                        active);
                        if (active) {
                            initialize_hit_descriptor(effect + effect, scratch.w, moby, 0x810000);
                            ((u8 *)scratch.x)[8] = 2;
                            ((u8 *)scratch.x)[9] = 1;
                            *(u16 *)((u8 *)scratch.x + 10) = *(u16 *)(moby + 0xa6);
                            collect_mobies_in_sphere(
                                moby + 0x10, effect * 3.0f, 0x10, moby, scratch.w);
                        }
                        mine_spawn_drops(moby, D_L00_001B0830_lui);
                        mark_moby_for_removal(moby);
                        return;
                    }
                }
            }
        }
        ((u8 *)moby)[0xa4] = 0xff;
        trooper_anim_finish(moby, vars);
        if (moby[0x70] & 2) {
            if (((u8 *)moby)[0x53] != 2)
                blend_moby_animation(moby, 2, 0, FUN_001fef20_2c4380(10));
            *(float *)(moby + 0x58) = random_float_between(0.7f, 1.05f);
        }
        if ((u8)moby[0x20] != 4) {
            len = probe_ground_height(moby + 0x10, 0, 0.5f);
            if (len != 0.0f) {
                destroy_decoy_on_selected_materials(moby);
                if (len + 0.02f < *(float *)(moby + 0x18)) {
                    *(float *)(vars + 0x18) -= D_0015ED70_2c4380 * 9.8f;
                    FUN_001ff278_2c4380(moby + 0x10, moby + 0x10, vars + 0x10);
                    len = probe_ground_height(moby + 0x10, 0, 0.5f);
                    if (*(float *)(moby + 0x18) < len) {
                        *(float *)(moby + 0x18) = len;
                        qzero(vars + 0x10);
                        qcopy(position_effect_and_normal_scratch, &coll_hit.normal_x);
                        cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                        sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                        scratch.w[0] = position_effect_and_normal_scratch[0] * cos_angle +
                                       position_effect_and_normal_scratch[1] * sin_angle;
                        cos_angle = FUN_001ff7e8_2c4380(*(float *)(moby + 0x48));
                        sin_angle = FUN_001ff800_2c4380(*(float *)(moby + 0x48));
                        scratch.w[1] = position_effect_and_normal_scratch[1] * cos_angle -
                                       position_effect_and_normal_scratch[0] * sin_angle;
                        scratch.w[2] = position_effect_and_normal_scratch[2];
                        *(float *)(vars + 0x4c) =
                            -FUN_001ff8b0_2c4380(FUN_001ff1e0_2c4380(scratch.w[0] * scratch.w[0] +
                                                                     scratch.w[2] * scratch.w[2]),
                                                 scratch.w[1]);
                        *(float *)(vars + 0x50) = FUN_001ff8b0_2c4380(scratch.w[2], scratch.w[0]);
                    }
                } else
                    qzero(vars + 0x10);
                if (coll_hit.moby)
                    FUN_L00_00260400(
                        moby, coll_hit.moby, moby + 0x10, moby + 0x40, moby + 0x10, moby + 0x40);
            }
        } else {
            if (*(float *)(moby + 0x2c) < *(float *)(*(char **)(moby + 0x24) + 0x24))
                *(float *)(moby + 0x2c) += D_0015ED60_2c4380 * 0.02f;
            if (fast_dec_timer_s16(vars + 0x44)) {
                if (((u8 *)moby)[0x53])
                    blend_moby_animation(moby, 0, 0, FUN_001fef20_2c4380(60));
                mine_spawn_drops(moby, D_L00_001B0830_lui);
                moby[0x20] = 5;
            }
        }
        break;
    case DECOY_WAIT_TO_SHRINK:
        len = probe_ground_height(moby + 0x10, 0, 0.5f);
        if (len != 0.0f && coll_hit.moby)
            FUN_L00_00260400(
                moby, coll_hit.moby, moby + 0x10, moby + 0x40, moby + 0x10, moby + 0x40);
        if (moby[0x70] & 2)
            *(int *)(moby + 0x58) = 0;
        if (fast_dec_timer_s16(vars + 0x44) || (moby[0x70] & 2))
            moby[0x20] = 6;
        break;
    case DECOY_SHRINK_FOR_REMOVAL: {
        float len;
        len = probe_ground_height(moby + 0x10, 0, 0.5f);
        if (len != 0.0f && coll_hit.moby) {
            FUN_L00_00260400(
                moby, coll_hit.moby, moby + 0x10, moby + 0x40, moby + 0x10, moby + 0x40);
        }
        len = D_0015ED60_2c4380;
        *(float *)(moby + 0x2c) -= *(float *)(moby + 0x2c) * (len * 0.02f);
        if (!(*(float *)(moby + 0x2c) < *(float *)(*(char **)(moby + 0x24) + 0x24) * 0.1f))
            return;
        mark_moby_for_removal(moby);
        break;
    }
    case DECOY_RESET_TO_INITIALIZE:
    default:
        moby[0x20] = 0;
    }
}

extern int   D_L00_0015F5CC_macro __asm__("D_L00_0015F5CC") MACRO_ADDR;
extern float frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern float D_0015ED6C_2c53c0 __asm__("D_0015ED6C") MACRO_ADDR;
extern int   FUN_001f96f8_2c53c0(int) __asm__("FUN_001f96f8");
extern float FUN_001f99c0_2c53c0(float) __asm__("FUN_001f99c0");
extern void  FUN_001f9a10_2c53c0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  FUN_001f9a28_2c53c0(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_2c53c0(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9b48_2c53c0(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_001f9b80_2c53c0(void *, void *) __asm__("FUN_001f9b80");
extern void  FUN_001f9bf8_2c53c0(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_001f9dc8_2c53c0(float) __asm__("FUN_001f9dc8");

typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) FxVec;

extern FxVec D_L00_00173E60_2c53c0 __asm__("D_L00_00173E60");
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern int   FUN_001f9740_2c53c0(int *) __asm__("FUN_001f9740");
extern int   FUN_L00_001f0d60_2c53c0(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int   FUN_001efa68_2c53c0(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern float approach_value(void *, float, float) __asm__("FUN_00213ed8");
extern int   FUN_001fa6d0_2c53c0(float) __asm__("FUN_001fa6d0");
extern int   FUN_001f99a8_2c53c0(int) __asm__("FUN_001f99a8");
extern int   FUN_L00_0028dc90_2c53c0(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern float jet_distance(void *, void *) __asm__("FUN_L00_00250430");

typedef struct {
    int   amount;
    char  pad4[2];
    short recovery_timer;
    int   weapon;
    char  padC[4];
    Vec4f velocity;
    int   timer;
    float rise_speed;
    float resting_height;
} AmmoPickup;

extern int D_L00_00161838_macro __asm__("D_L00_00161838") MACRO_ADDR;
/* Update ammunition pickup physics, collection, animation and scale.
   Animation states share one stack value; index reloads follow calls. */
void devestator_ammo_update(struct Moby *m) __asm__("FUN_L00_002c53c0");

void devestator_ammo_update(struct Moby *pickup_moby) {
    enum {
        AMMO_PICKUP_INITIALIZE = 0,
        AMMO_PICKUP_WAIT_FOR_COLLECTION = 1,
        AMMO_PICKUP_COLLECTED = 2,
        AMMO_PICKUP_PULSE_ALPHA = 3,
        AMMO_PICKUP_RESTORE_SCALE = 4
    };
    AmmoPickup  *pickup_vars;
    unsigned int state;
    FxVec        position;
    FxVec        predicted;
    float        value;
    float        magnitude;
    float        current;
    float        target;
    float        step;
    float        remaining;
    pickup_vars = (AmmoPickup *)pickup_moby->pvars;
    FUN_001f9740_2c53c0(&pickup_vars->timer);
    state = pickup_moby->state;
    if (state == 1 || state == 4 || state == 3) {
        magnitude = FUN_001f9af0_2c53c0(&pickup_vars->velocity);
        if (magnitude != 0.0f) {
            qcopy(&position, &pickup_moby->pos);
            position.z += 0.5f;
            if (FUN_L00_001f0d60_2c53c0(0.4f, &position, 2, 0)) {
                qcopy(&pickup_moby->pos, &coll_hit_position);
                pickup_moby->pos.z -= 0.5f;
            }
        }
        pickup_vars->velocity.z -= frame_time_sq_macro * 4.9f;
        FUN_001f9a10_2c53c0(&predicted, &pickup_moby->pos, &pickup_vars->velocity);
        pickup_vars->velocity.z -= frame_time_sq_macro * 4.9f;
        qcopy(&position, &pickup_moby->pos);
        position.z += 0.1f;
        if (FUN_001efa68_2c53c0(&position, &predicted, 2, 0, 0)) {
            clear_u64_value(&pickup_vars->velocity);
            qcopy(&pickup_moby->pos, &D_L00_00173E60_2c53c0);
        } else {
            qcopy(&pickup_moby->pos, &predicted);
        }
    }
    switch (pickup_moby->state) {
    case AMMO_PICKUP_INITIALIZE:
        pickup_moby->state = 1;
        break;
    case AMMO_PICKUP_WAIT_FOR_COLLECTION: {
        struct Hero *player;
        float       *value_ptr;
        float        height_delta;
        float        distance;
        int          duration;
        int          eligible;
        int          idx;
        int          sound_idx;
        int          account_idx;
        int          before;
        int          after;
        int          difference;
        int          sound;
        int          elapsed;
        int          random;
        int          max_ammo;
        value_ptr = &value;
        *value_ptr = (float)pickup_moby->alpha;
        duration = FUN_001f96f8_2c53c0(30);
        approach_value(value_ptr, 128.0f, (float)(128 / duration));
        pickup_moby->alpha = FUN_001fa6d0_2c53c0(*value_ptr);
        if (pickup_vars->timer > 0)
            return;
        if (FUN_001f9af0_2c53c0(&pickup_vars->velocity) != 0.0f)
            return;
        player = &hero;
        eligible = 1;
        height_delta = FUN_001f99c0_2c53c0(pickup_moby->pos.z - player->motion.pos.f[2]);
        if (player->reach_height < height_delta) {
            eligible = 0;
        } else {
            distance = FUN_001f9b80_2c53c0(&pickup_moby->pos, &player->motion.pos);
            if (player->reach_distance < distance)
                eligible = 0;
        }
        idx = pickup_vars->weapon;
        before = weapon_ammo_counts[idx];
        max_ammo = D_L00_001C40B0[idx].hE;
        if (before >= max_ammo)
            eligible = 0;
        if (pickup_moby->pos.z < pickup_vars->resting_height - 2.0f)
            eligible = 1;
        if (hero.health.hp == 0)
            eligible = 0;
        if (!eligible)
            goto pickup_failed;

        {
            add_ammo(idx, pickup_vars->amount);
            sound_idx = pickup_vars->weapon;
            after = weapon_ammo_counts[sound_idx];
            if (before < after) {
                difference = after - before;
                if (difference == 1) {
                    sound = ((ItemDef *)((char *)item_defs + sound_idx * sizeof(ItemDef)))
                                ->pickup_message_single;
                } else {
                    sound = ((ItemDef *)((char *)item_defs + sound_idx * sizeof(ItemDef)))
                                ->pickup_message_multiple;
                }
                queue_formatted_dialogue_message(sound, difference, -1);
            }
            account_idx = pickup_vars->weapon;
            weapon_ammo_pickup_totals[account_idx] += weapon_ammo_counts[account_idx] - before;
            pickup_moby->state = 2;
            pickup_moby->scale *= frame_scale_macro * 0.20000005f + 1.0f;
            clear_u64_value(&pickup_vars->velocity);
            elapsed = D_L00_0015F5CC_macro - D_L00_00161838_macro;
            pickup_vars->rise_speed = D_0015ED6C_2c53c0 * 8.0f;
            random = FUN_001f99a8_2c53c0(elapsed);
            if (FUN_001f96f8_2c53c0(10) < random) {
                FUN_L00_0028dc90_2c53c0(0, 0, (int)pickup_moby, 0xD5);
                D_L00_00161838_macro = D_L00_0015F5CC_macro;
            }
            return;
        }
    pickup_failed:
        if (pickup_moby->alpha < 0xFF)
            ++pickup_moby->alpha;
        if (jet_distance(pickup_moby, hero.moby) <= -0.2f) {
            pickup_moby->state = 3;
            pickup_vars->recovery_timer = FUN_001f96f8_2c53c0(60);
        }
        return;
    }
    case AMMO_PICKUP_COLLECTED: {
        float limit;
        float distance;
        float factor;
        float class_scale;
        approach_value(&pickup_vars->rise_speed, 0.0f, frame_time_sq_macro * 24.0f);
        pickup_moby->pos.z += pickup_vars->rise_speed;
        limit = FUN_001f9af0_2c53c0(&pickup_vars->velocity);
        approach_value(&limit, D_0015ED6C_2c53c0 * 64.0f, frame_time_sq_macro * 24.0f);
        FUN_001f9a28_2c53c0(&pickup_vars->velocity, &hero.motion.body_center, &pickup_moby->pos);
        if (pickup_moby->scale == 0.0f || FUN_001f9af0_2c53c0(&pickup_vars->velocity) < limit) {
            mark_moby_for_removal(pickup_moby);
            return;
        }
        FUN_001f9bf8_2c53c0(&pickup_vars->velocity, &pickup_vars->velocity, limit);
        FUN_001f9a10_2c53c0(&pickup_moby->pos, &pickup_moby->pos, &pickup_vars->velocity);
        distance = FUN_001f9b48_2c53c0(&pickup_moby->pos, &hero.motion.body_center);
        factor = (float)FUN_001f96f8_2c53c0(30);
        factor = factor * limit;
        factor = factor + 0.01f;
        distance = distance / factor;
        if (1.0f < distance) {
            distance = 1.0f;
        } else if (distance < 0.3f) {
            distance = 0.3f;
        }
        class_scale = pickup_moby->pclass->scale;
        approach_value(&pickup_moby->scale, class_scale * distance, class_scale * 0.1f);
        return;
    }
    case AMMO_PICKUP_PULSE_ALPHA: {
        float *value_ptr;
        float  angle;
        float  oscillation;
        float  target;
        int    phase;
        int    random;
        int    idx;
        phase = D_L00_0015F5CC_macro % 60;
        value = (float)pickup_moby->alpha;
        angle = (float)phase / 60.0f;
        angle = angle * 6.28318f;
        angle = angle - 3.14159f;
        oscillation = FUN_001f9dc8_2c53c0(angle);
        oscillation = oscillation * 5.0f;
        oscillation = oscillation + 5.0f;
        target = (float)(FUN_001fa6d0_2c53c0(oscillation) + 10);
        value_ptr = &value;
        random = FUN_001f96f8_2c53c0(30);
        approach_value(value_ptr, target, (float)(128 / random));
        pickup_moby->alpha = FUN_001fa6d0_2c53c0(*value_ptr);
        if (jet_distance(pickup_moby, hero.moby) >= 0.0f) {
            pickup_moby->state = 1;
            return;
        }
        idx = pickup_vars->weapon;
        if (weapon_ammo_counts[idx] < D_L00_001C40B0[idx].hE) {
            pickup_moby->state = 1;
        }
        return;
    }
    case AMMO_PICKUP_RESTORE_SCALE:
        current = pickup_moby->scale;
        target = pickup_moby->pclass->scale;
        step = frame_scale_macro * 0.03f;
        current += (target - current) * step;
        pickup_moby->scale = current;
        target = pickup_moby->pclass->scale;
        remaining = (target - current) / target;
        if (remaining < 0.01f) {
            pickup_moby->scale = target;
            pickup_moby->state = 1;
            pickup_moby->collision = pickup_moby->pclass->collision;
        }
        break;
    }
}

#ifndef NON_MATCHING
#include "rnc/input/pad_state.h"
#include "rnc/math/vector.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"

#include "qcopy.h"

/* Swingshot (class 208) update: keeps the hook at the glove, shows the targets, fires the hook to the swing target
   or the grind target, swings the hero, and reels the hook back. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C74B8), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int OvlQuad_6158 __attribute__((mode(TI)));

typedef unsigned int Q_2C74B8 __attribute__((mode(TI)));

typedef union {
    struct {
        float x, y, z, w;
    } f;
    Q_2C74B8 q;
} V_2C74B8 __attribute__((aligned(16)));

typedef struct {
    V_2C74B8 row[3];
} M3;

typedef struct HookData {
    char  pad0[4];
    float speed;
} HookData;

typedef struct Data {
    char  pad0[0x14];
    Moby *hook;
    float spin;
    float scale;
    float size;
} Data;

extern f32     fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float   frame_time;
extern int     FUN_L00_00205000();
extern int     hero_set_state() __asm__("FUN_L00_002223f8");
extern int     FUN_L00_002323b8();
unsigned char *spawn_swingshot_hook(unsigned char *src, float f) __asm__("FUN_L00_002c6f88");
void           suck_draw_reticle(int   a,
                                 int   b,
                                 void *pos,
                                 int   kind,
                                 int   mode,
                                 float x,
                                 float y,
                                 float z) __asm__("FUN_L00_001edf60");
extern Moby   *FUN_L00_002c6f88_2bffa8(void *, float) __asm__("FUN_L00_002c6f88");
extern void FUN_L00_001edf60_2bffa8(int, int, float, void *, int, float, int, float, int) __asm__("FUN_L00_001edf60");
extern int   FUN_001f96f8_2bffa8(int) __asm__("FUN_001f96f8");
extern float FUN_001f9b48_2bffa8(void *, void *) __asm__("FUN_001f9b48");
extern void  FUN_001f9a28_2bffa8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001f9bf8_2bffa8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  FUN_001f9a10_2bffa8(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001f9af0_2bffa8(void *) __asm__("FUN_001f9af0");
extern int   FUN_001efa68_2bffa8(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int   hero_idle_sequence(int) __asm__("FUN_L00_00205000");
extern void  draw_swingshot_cable(struct Moby *) __asm__("FUN_L00_002c5dc8");

void swingshot_update(Moby *m) __asm__("FUN_L00_002c6158");

void swingshot_update(Moby *m) {
    V_2C74B8  v0;
    V_2C74B8  v10;
    Data     *d;
    HookData *hd;
    float     speed;
    int       n;

    if (m == 0)
        return;
    if (hero.state.current == 1)
        hero_set_state(30, 1);
    d = (void *)m->pvars;
    if (d == 0)
        return;
    hd = 0;
    if (!d->hook)
        d->hook = FUN_L00_002c6f88_2bffa8(m, frame_time * 24.0f);
    if (d->hook)
        hd = (void *)d->hook->pvars;
    if ((hero.state.current == 1 || hero.state.current == 30) && hero.state_timer >= 2) {
        FUN_L00_001edf60_2bffa8((int)m, 0xFF0000FF, 1.0f, 0, 0x24, 0.0f, -1, 90.0f, 4);
        if (hero.grapple_target_in_sight && hero.grapple_target)
            FUN_L00_001edf60_2bffa8(
                (int)m, 0xFF00FF00, 0.5f, &hero.grapple_target->pos, 0x24, 0.0f, -1, 90.0f, 4);
        else if (hero.swing_target)
            FUN_L00_001edf60_2bffa8(
                (int)m, 0xFF00FF00, 0.5f, &hero.swing_target->pos, 0x24, 0.0f, -1, 90.0f, 4);
    }
    switch (m->state) {
    case 0:
    case 1:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 1)
                blend_moby_animation(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        if (d->hook) {
            moby_point_position(m, 0, &d->hook->pos);
            {
                Moby *h = d->hook;
                *(u128 *)&h->rot_row0 = *(u128 *)&m->rot_row0;
                *(u128 *)&h->rot_row1 = *(u128 *)&m->rot_row1;
                *(u128 *)&h->rot_row2 = *(u128 *)&m->rot_row2;
            }
        }
        if (hero.state.current == 0x24 || (hero.state.control_mode == 0xF && hero.unk5B4)) {
            if (hd) {
                d->size = 0.280000001f;
                d->scale = 1.0f;
                d->spin = 0.0f;
                hd->speed = frame_time * 24.0f;
                if (m->prev_seq != 3)
                    blend_moby_animation(m, 3, 0, FUN_001f96f8_2bffa8(10));
                allocate_voice_for_target_entry(0, 0, m);
                m->state = 2;
            }
        }
        if (hero.state.current == 0x2C && hd) {
            speed = FUN_001f9b48_2bffa8(&hero.swing_target_moby->pos, &d->hook->pos);
            n = FUN_001f96f8_2bffa8(7);
            if (hero.items[0].ready_frames < FUN_001f96f8_2bffa8(30))
                n = FUN_001f96f8_2bffa8(5);
            hd->speed = speed / (float)n;
            d->size = 0.280000001f;
            d->scale = 1.0f;
            d->spin = 0.0f;
            if (m->prev_seq != 3)
                blend_moby_animation(m, 3, 0, FUN_001f96f8_2bffa8(10));
            allocate_voice_for_target_entry(0, 0, m);
            m->state = 9;
        }
        break;
    case 2:
        add_draw_callback(draw_swingshot_cable, m);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f9a28_2bffa8(&v0, &hero.grapple_target->pos, &d->hook->pos);
        speed = hd->speed;
        if (hero.state.control_mode == 0xF) {
            speed = speed + hero.unk574;
        } else if (hero.state.prev_control_mode == 4 || hero.state.prev_control_mode == 2) {
            if (2.0f < hero.ground_distance)
                speed = frame_time * 48.0f;
        }
        qcopy(&v10, &hero.motion.pos);
        v10.f.z += 0.5f;
        if (FUN_001efa68_2bffa8(&v10, &hero.grapple_target->pos, 2, 0, 0)) {
            if (0.5f < hero.ground_distance) {
                hero_set_state(6, 1);
            } else {
                hero_set_state(0, 0);
                set_hero_animation(hero_idle_sequence(0), 0, (float)FUN_001f96f8_2bffa8(14));
            }
            m->state = 6;
        } else if (speed < FUN_001f9b48_2bffa8(&d->hook->pos, &hero.grapple_target->pos)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
        } else {
            qcopy(&d->hook->pos, &hero.grapple_target->pos);
            m->state = 3;
        }
        break;
    case 3:
        allocate_voice_for_target_entry(1, 0, m);
        add_draw_callback(draw_swingshot_cable, m);
        qcopy(&d->hook->pos, &hero.grapple_target->pos);
        m->state = 4;
        break;
    case 4:
        moby_point_position(m, 0, &v0);
        qcopy(&v0, &hero.motion.pos);
        v0.f.z += 0.5f;
        if (FUN_001efa68_2bffa8(&v0, &hero.grapple_target->pos, 2, 0, 0)) {
            if (hero.air_frames.s)
                hero_set_state(6, 1);
            else
                hero_set_state(0, 1);
            m->state = 6;
            break;
        }
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        add_draw_callback(draw_swingshot_cable, m);
        d->spin = d->spin + 0.69813168f;
        d->size = d->size * (frame_scale * -0.100000024f + 1.0f);
        if (d->size < 0.168000013f) {
            hero.unk5B4 = 0;
            if (d->hook && 3.5f < FUN_001f9b48_2bffa8(&d->hook->pos, &hero.motion.pos)) {
                if (!(controller_state.held & hero.items[0].button_mask)) {
                    hero.unk988 = 1;
                    hero_set_state(0, 0);
                    set_hero_animation(hero_idle_sequence(0), 0, (float)FUN_001f96f8_2bffa8(14));
                    m->state = 6;
                } else {
                    allocate_voice_for_target_entry(2, 0, m);
                    if (hero.unk978)
                        hero_set_state(0x26, 1);
                    else
                        hero_set_state(0x25, 1);
                    m->state = 5;
                }
            } else {
                hero_set_state(0, 1);
                m->state = 6;
            }
        }
        break;
    case 5:
        qcopy(&d->hook->pos, &hero.grapple_target->pos);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        d->size = d->size * (frame_scale * -0.100000024f + 1.0f);
        d->spin = d->spin + 0.69813168f;
        add_draw_callback(draw_swingshot_cable, m);
        if ((unsigned int)(hero.state.current - 0x25) >= 2)
            m->state = 6;
        break;
    case 6:
        add_draw_callback(draw_swingshot_cable, m);
        hd->speed = frame_time * 80.0f;
        m->state = 7;
        break;
    case 7:
        moby_point_position(m, 0, &v10);
        FUN_001f9a28_2bffa8(&v0, &v10, &d->hook->pos);
        if (hd->speed < FUN_001f9b48_2bffa8(&d->hook->pos, &v10)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
            add_draw_callback(draw_swingshot_cable, m);
        } else {
            qcopy(&d->hook->pos, &v10);
            if (m->prev_seq != 1)
                blend_moby_animation(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        break;
    case 8:
        break;
    case 9:
        add_draw_callback(draw_swingshot_cable, m);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f9a28_2bffa8(&v0, &hero.swing_target_moby->pos, &d->hook->pos);
        if (hd->speed < FUN_001f9af0_2bffa8(&v0)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
        } else {
            qcopy(&d->hook->pos, &hero.swing_target_moby->pos);
            m->state = 11;
            allocate_voice_for_target_entry(1, 0, m);
        }
        break;
    case 10:
        add_draw_callback(draw_swingshot_cable, m);
        m->state = 11;
        break;
    case 11:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        add_draw_callback(draw_swingshot_cable, m);
        d->spin = d->spin + 0.69813168f;
        d->size = d->size * (frame_scale * -0.600000024f + 1.0f);
        if (d->size < 0.168000013f) {
            hero.unk99C = 1;
            m->state = 12;
        }
        break;
    case 12:
        if (hero.swing_target_moby)
            qcopy(&d->hook->pos, &hero.swing_target_moby->pos);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        d->size = d->size * (frame_scale * -0.600000024f + 1.0f);
        d->spin = d->spin + 0.69813168f;
        add_draw_callback(draw_swingshot_cable, m);
        if (hero.state.current != 0x2C)
            m->state = 13;
        break;
    case 13:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        add_draw_callback(draw_swingshot_cable, m);
        hd->speed = frame_time * 80.0f;
        m->state = 14;
        break;
    case 14:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        moby_point_position(m, 0, &v10);
        FUN_001f9a28_2bffa8(&v0, &v10, &d->hook->pos);
        if (hd->speed < FUN_001f9b48_2bffa8(&d->hook->pos, &v10)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
            add_draw_callback(draw_swingshot_cable, m);
        } else {
            qcopy(&d->hook->pos, &v10);
            if (m->prev_seq != 1)
                blend_moby_animation(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        if (hero.state.current == 0x2C && hd) {
            hd->speed = FUN_001f9b48_2bffa8(&hero.swing_target_moby->pos, &d->hook->pos) / 10.0f;
            d->scale = 1.0f;
            d->size = 0.0f;
            d->spin = 0.0f;
            if (m->prev_seq != 3)
                blend_moby_animation(m, 3, 0, FUN_001f96f8_2bffa8(10));
            m->state = 9;
        }
        break;
    case 15:
        break;
    }
}
#else
extern s32   D_001413E4_2c6158 __asm__("D_001413E4");
extern s32   D_0013F4E8_2c6158 __asm__("D_0013F4E8");
extern s32   D_0013FCB8_2c6158 __asm__("D_0013FCB8");
extern u8   *D_0013FCB4_2c6158 __asm__("D_0013FCB4");
extern u8   *D_0013FCE0_2c6158 __asm__("D_0013FCE0");
extern u8   *D_0013FCE4_2c6158 __asm__("D_0013FCE4");
extern s32   D_0013F904_2c6158 __asm__("D_0013F904");
extern s32   D_00140400_2c6158 __asm__("D_00140400");
extern s32   D_0013FCC8_2c6158 __asm__("D_0013FCC8");
extern s32   D_0013FCD8_2c6158 __asm__("D_0013FCD8");
extern s32   D_0013FCEC_2c6158 __asm__("D_0013FCEC");
extern u32   D_0013CAE0_2c6158 __asm__("D_0013CAE0") NOT_SDA;
extern float D_0015ED6C_2c6158 __asm__("D_0015ED6C");
extern float D_0015ED60_2c6158 __asm__("D_0015ED60");
extern float D_0013F8C4_2c6158 __asm__("D_0013F8C4");
extern float D_0013F62C_2c6158 __asm__("D_0013F62C");
extern void  hero_set_state(s32, s32) __asm__("FUN_L00_002223f8");
extern void  FUN_001edf60_2c6158(void *, float, u32, void *, s32, float, s32, float, s32) __asm__("FUN_L00_001edf60");
extern s32   FUN_001fef20_2c6158(s32) __asm__("FUN_001f96f8");
extern void  FUN_0028dbe8_2c6158(s32, s32, void *) __asm__("FUN_0022da68");
extern void  FUN_001f93e0_2c6158(void *, void *) __asm__("FUN_L00_001f93e0");
extern float FUN_001ff450_2c6158(void *, void *) __asm__("FUN_001f9b48");
extern void  FUN_001ff2a8_2c6158(void *, void *, void *) __asm__("FUN_001f9a28");
extern s32   FUN_001efc70_2c6158(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void  FUN_001ff500_2c6158(float, void *, void *) __asm__("FUN_001f9bf8");
extern void  FUN_001ff278_2c6158(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001ff3d8_2c6158(void *) __asm__("FUN_001f9af0");
extern void *hero_idle_sequence(s32) __asm__("FUN_L00_00205000");
extern void  set_hero_animation(float, void *, s32) __asm__("FUN_L00_002323b8");

void swingshot_update(u8 *m) __asm__("FUN_L00_002c6158");

void swingshot_update(u8 *m) {
    u8     *vars;
    u8     *child;
    u8     *motion;
    OvlVec4 a, b;
    float   speed, distance;
    s32     frames;
    void   *effect;

    if (m == 0)
        return;
    if (hero.state.current == 1)
        hero_set_state(30, 1);
    vars = *(u8 **)(m + 0x78);
    if (vars == 0)
        return;
    motion = 0;
    if (*(u8 **)(vars + 0x14) == 0) {
        if ((*(u8 **)(vars + 0x14) = spawn_swingshot_hook(m, D_0015ED6C_2c6158 * 24.0f)) == 0)
            goto child_ready;
    }
    motion = *(u8 **)(*(u8 **)(vars + 0x14) + 0x78);
child_ready:;

    if (hero.state.current == 1 || hero.state.current == 30) {
        if (hero.state_timer >= 2) {
            FUN_001edf60_2c6158(m, 1.0f, 0xff0000ff, 0, 36, 0.0f, -1, 90.0f, 4);
            if (hero.grapple_target_in_sight && ((u8 *)hero.grapple_target)) {
                FUN_001edf60_2c6158(m,
                                    0.5f,
                                    0xff00ff00,
                                    ((u8 *)hero.grapple_target) + 0x10,
                                    36,
                                    0.0f,
                                    -1,
                                    90.0f,
                                    4);
            } else if (((u8 *)hero.swing_target)) {
                FUN_001edf60_2c6158(
                    m, 0.5f, 0xff00ff00, ((u8 *)hero.swing_target) + 0x10, 36, 0.0f, -1, 90.0f, 4);
            }
        }
    }
    switch (m[0x20]) {
    case 0:
    case 1:
        if (m[0x70] & 2) {
            if (m[0x53] != 1)
                blend_moby_animation(m, 1, 0, scale_game_frames(10));
            m[0x20] = 1;
        }
        if (*(u8 **)(vars + 0x14) != 0) {
            FUN_L00_0024f7c8(m, 0, *(u8 **)(vars + 0x14) + 0x10);
            child = *(u8 **)(vars + 0x14);
            *(u128 *)(child + 0xc0) = *(u128 *)(m + 0xc0);
            *(u128 *)(child + 0xd0) = *(u128 *)(m + 0xd0);
            *(u128 *)(child + 0xe0) = *(u128 *)(m + 0xe0);
        }
        if ((hero.state.current == 36 || (hero.state.control_mode == 15 && hero.unk5B4)) &&
            motion) {
            *(float *)(vars + 0x18) = 0.0f;
            *(float *)(vars + 0x1c) = 1.0f;
            *(float *)(vars + 0x20) = 0.28f;
            *(float *)(motion + 4) = D_0015ED6C_2c6158 * 24.0f;
            if (m[0x53] != 3)
                blend_moby_animation(m, 3, 0, scale_game_frames(10));
            FUN_0028dbe8_2c6158(0, 0, m);
            m[0x20] = 2;
        }
        if (hero.state.current == 44) {
            if (motion == 0)
                return;
            distance = FUN_001ff450_2c6158(((u8 *)hero.swing_target_moby) + 0x10,
                                           (*(u8 **)(vars + 0x14)) + 0x10);
            frames = scale_game_frames(7);
            if (hero.items[0].ready_frames < scale_game_frames(30))
                frames = scale_game_frames(5);
            *(float *)(motion + 4) = distance / (float)frames;
            *(float *)(vars + 0x18) = 0.0f;
            *(float *)(vars + 0x1c) = 1.0f;
            *(float *)(vars + 0x20) = 0.28f;
            if (m[0x53] != 3)
                blend_moby_animation(m, 3, 0, scale_game_frames(10));
            FUN_0028dbe8_2c6158(0, 0, m);
            m[0x20] = 9;
        }
        return;
    case 2:
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001ff2a8_2c6158(&a, ((u8 *)hero.grapple_target) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
        speed = *(float *)(motion + 4);
        if (hero.state.control_mode == 15)
            speed += hero.unk574;
        else if ((hero.state.prev_control_mode == 4 || hero.state.prev_control_mode == 2) &&
                 hero.ground_distance > 2.0f)
            speed = D_0015ED6C_2c6158 * 48.0f;
        qcopy(&b, &hero.motion.pos);
        b.f[2] += 0.5f;
        if (FUN_001efc70_2c6158(&b, ((u8 *)hero.grapple_target) + 0x10, 2, 0, 0)) {
            if (hero.ground_distance > 0.5f)
                hero_set_state(6, 1);
            else {
                hero_set_state(0, 0);
                effect = hero_idle_sequence(0);
                frames = scale_game_frames(14);
                set_hero_animation((float)frames, effect, 0);
            }
            m[0x20] = 6;
            return;
        }
        distance =
            FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        if (speed < distance) {
            FUN_001ff500_2c6158(speed, &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        m[0x20] = 3;
        return;
    case 3:
        FUN_0028dbe8_2c6158(1, 0, m);
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        m[0x20] = 4;
        return;
    case 4: {
        float factor;
        /* Retail tests ordered comparisons; an unordered fade value returns here. */
        FUN_L00_0024f7c8(m, 0, &a);
        qcopy(&a, &hero.motion.pos);
        a.f[2] += 0.5f;
        if (FUN_001efc70_2c6158(&a, ((u8 *)hero.grapple_target) + 0x10, 2, 0, 0) != 0) {
            if (hero.air_frames.s != 0)
                hero_set_state(6, 1);
            else
                hero_set_state(0, 1);
            m[0x20] = 6;
            return;
        }
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        factor = *(float *)(vars + 0x20) * (D_0015ED60_2c6158 * -0.100000024f + 1.0f);
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) = factor;
        if (!(factor < 0.16800001f))
            return;
        hero.unk5B4 = 0;
        if ((*(u8 **)(vars + 0x14)) != 0 &&
            3.5f < FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &hero.motion.pos)) {
            if ((D_0013CAE0_2c6158 & hero.items[0].button_mask) == 0) {
                hero.unk988 = 1;
                hero_set_state(0, 0);
                effect = hero_idle_sequence(0);
                frames = scale_game_frames(14);
                set_hero_animation((float)frames, effect, 0);
                m[0x20] = 6;
            } else {
                FUN_0028dbe8_2c6158(2, 0, m);
                if (hero.unk978 != 0)
                    hero_set_state(38, 1);
                else
                    hero_set_state(37, 1);
                m[0x20] = 5;
            }
        } else {
            hero_set_state(0, 1);
            m[0x20] = 6;
            return;
        }
        return;
    }
    case 5: {
        float fade_scale;
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        fade_scale = D_0015ED60_2c6158;
        if ((m[0x70] & 2) && m[0x53] != 4) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            fade_scale = D_0015ED60_2c6158;
        }
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) *= fade_scale * -0.100000024f + 1.0f;
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if ((u32)(hero.state.current - 37) < 2)
            return;
        m[0x20] = 6;
        return;
    }
    case 6:
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        *(float *)(motion + 4) = D_0015ED6C_2c6158 * 80.0f;
        m[0x20] = 7;
        return;
    case 7:
        FUN_L00_0024f7c8(m, 0, &b);
        FUN_001ff2a8_2c6158(&a, &b, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (*(float *)(motion + 4) < distance) {
            FUN_001ff500_2c6158(*(float *)(motion + 4), &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            FUN_001f93e0_2c6158(draw_swingshot_cable, m);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (m[0x53] != 1)
            blend_moby_animation(m, 1, 0, scale_game_frames(10));
        m[0x20] = 1;
        return;
    case 9: {
        float step;
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001ff2a8_2c6158(
            &a, ((u8 *)hero.swing_target_moby) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff3d8_2c6158(&a);
        step = *(float *)(motion + 4);
        if (step < distance) {
            FUN_001ff500_2c6158(step, &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.swing_target_moby) + 0x10);
        m[0x20] = 11;
        FUN_0028dbe8_2c6158(1, 0, m);
        return;
    }
    case 10:
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        m[0x20] = 11;
        return;
    case 11: {
        float factor;
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        factor = *(float *)(vars + 0x20) * (D_0015ED60_2c6158 * -0.6f + 1.0f);
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) = factor;
        if (!(factor < 0.16800001f))
            return;
        hero.unk99C = 1;
        m[0x20] = 12;
        return;
    }
    case 12: {
        float fade_scale;
        if (((u8 *)hero.swing_target_moby))
            qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.swing_target_moby) + 0x10);
        fade_scale = D_0015ED60_2c6158;
        if ((m[0x70] & 2) && m[0x53] != 4) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            fade_scale = D_0015ED60_2c6158;
        }
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) *= fade_scale * -0.6f + 1.0f;
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if (hero.state.current != 44)
            m[0x20] = 13;
        return;
    }
    case 13:
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        *(float *)(motion + 4) = D_0015ED6C_2c6158 * 80.0f;
        m[0x20] = 14;
        return;
    case 14:
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_L00_0024f7c8(m, 0, &b);
        FUN_001ff2a8_2c6158(&a, &b, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (*(float *)(motion + 4) < distance) {
            FUN_001ff500_2c6158(*(float *)(motion + 4), &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        } else {
            qcopy((*(u8 **)(vars + 0x14)) + 0x10, &b);
            if (m[0x53] != 1)
                blend_moby_animation(m, 1, 0, scale_game_frames(10));
            m[0x20] = 1;
        }
        if (hero.state.current != 44 || motion == 0)
            return;
        distance = FUN_001ff450_2c6158(((u8 *)hero.swing_target_moby) + 0x10,
                                       (*(u8 **)(vars + 0x14)) + 0x10);
        *(float *)(motion + 4) = distance / 10.0f;
        *(float *)(vars + 0x1c) = 1.0f;
        *(float *)(vars + 0x18) = 0.0f;
        *(float *)(vars + 0x20) = 0.0f;
        if (m[0x53] != 3)
            blend_moby_animation(m, 3, 0, scale_game_frames(10));
        m[0x20] = 9;
        return;
    case 15:
    default:
        return;
    }
}
#endif
/* Vars of the carrier moby that picks up and holds another moby. */
typedef struct {
    u8           pad0[0x40];
    OvlVec4      grip;  /* 0x40: where the held moby is carried */
    struct Moby *held;  /* 0x50 */
    s32          timer; /* 0x54 */
} CarrierVars;

#include "rnc/input/pad_state.h"

typedef float Vec_2c7548[4] __attribute__((aligned(16)));
typedef float Mat_2c7548[4][4] __attribute__((aligned(16)));
typedef float Mat3_2c7548[3][4] __attribute__((aligned(16)));

extern char  g_debug_camera[] __asm__("D_L00_00166DC0");
extern void  fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern int   collision_line_2c7548(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void  FUN_L00_002c7320_2c7548(float *pos,
                                     float *tgt,
                                     float *out,
                                     float  speed,
                                     float  unused,
                                     float  range) __asm__("FUN_L00_002c7320");

/* Moves the moby the carrier holds toward its throw target: ahead of the hero,
   or along the camera aim (and up to a collision hit) while pad bits 0x5 are held. */
void compute_glove_of_doom_throw_velocity(struct Moby *moby,
                                          CarrierVars *vars,
                                          OvlVec4     *grip) __asm__("FUN_L00_002c7548");

void compute_glove_of_doom_throw_velocity(struct Moby *moby, CarrierVars *vars, OvlVec4 *grip) {
    Vec_2c7548   throw_origin;
    Mat3_2c7548  basis;
    Mat_2c7548   matrix;
    Vec_2c7548   direction, target, arc, saved, probe;
    struct Moby *held;
    float       *out;
    float        horizontal, radius, angle, vertical, slope, distance, root;
    int          hit;
    float        speed;
    float       *pos = throw_origin;

    *(u128 *)pos = grip->q;
    copy_matrix3x4(basis, l00_camera.unk0);
    transpose_rotation_matrix(matrix, basis);
    held = vars->held;
    if (held == 0)
        return;
    out = (float *)held->pvars;
    *(u128 *)direction = 0;
    direction[0] = 1.0f;
    transform_vector_by_basis(direction, direction, hero.basis);
    {
        struct Hero *hero_state = &hero;
        pos[0] = fast_cos(fast_add_rotations(
                     -0.361968011f, fast_arc_tan(hero_state->facing_x, hero_state->facing_y))) *
                 0.859039009f;
        pos[1] = fast_sin(fast_add_rotations(
                     -0.361968011f, fast_arc_tan(hero_state->facing_x, hero_state->facing_y))) *
                 0.859039009f;
        pos[2] = 0.0f;
        fast_vec_add(pos, pos, &hero.motion.pos);
        pos[2] = pos[2] + 0.390819997f;
        fast_arc_tan(direction[0], direction[1]);
        speed = (controller_state.held & 5) ? 3.5f : 2.5f;
        fast_vec_scale(target, direction, speed);
        fast_vec_add(target, target, pos);
        if ((controller_state.held & 5) && hero_state->item_use_locked == 0) {
            *(u128 *)arc = 0;
            arc[2] = 1.0f;
            transform_vector(arc, arc, matrix);
            hit = 0;
            fast_subtract_rotations(hero_state->moby->rot.z, fast_arc_tan(arc[0], arc[1]));
            angle = fast_arc_tan(arc[0], arc[1]);
            slope = fast_arc_tan(vector_length_xy(arc), arc[2]);
            if (0.593411922f < slope)
                slope = 0.593411922f;
            if (slope < -1.39626336f)
                slope = -1.39626336f;
            build_spherical_offset(arc, (controller_state.held & 5) ? 3.5f : 2.5f, angle, slope);
            qcopy(saved, &l00_camera.pos);
            distance = vector_length_xy(arc);
            {
                float z = arc[2];
                if (distance != 0.0f) {
                    horizontal = z / distance;
                } else {
                    horizontal = z / 0.00999999978f;
                }
            }
            root = square_root(frame_time_sq_macro * 9.0f *
                               ((controller_state.held & 5) ? 3.5f : 2.5f) * 0.5f);
            radius = (root + root) * root * (1.0f - horizontal) / (frame_time_sq_macro * 9.0f);
            vertical = horizontal * radius;
            if (0.0f < radius) {
                arc[0] = fast_cos(angle) * radius;
                arc[1] = fast_sin(angle) * radius;
                arc[2] = vertical;
                fast_vec_add(probe, g_debug_camera, arc);
                if (collision_line_2c7548(g_debug_camera, probe, 2, (int)moby, 0) &&
                    coll_hit.moby != (s32)hero.moby) {
                    qcopy(target, coll_hit.point.f);
                    hit = 1;
                    angle = fast_arc_tan(target[0] - pos[0], target[1] - pos[1]);
                }
                if (hit == 0) {
                    horizontal = 5.0f;
                    target[0] = fast_cos(angle) * radius * horizontal;
                    target[1] = fast_sin(angle) * radius * horizontal;
                    target[2] = vertical * horizontal;
                    fast_vec_add(target, target, g_debug_camera);
                }
            }
        } else {
            hero.throw_target = 0;
        }
    }
    FUN_L00_002c7320_2c7548(pos,
                            target,
                            out,
                            frame_time_sq_macro * 9.0f,
                            0.00000999999975f,
                            (controller_state.held & 5) ? 3.5f : 2.5f);
}

/* Counters kept at D_00141680 for the carrier's deliveries. */
typedef struct {
    u8  pad0[0xA0];
    u16 deliveries; /* 0xA0: counts up to 0xFFFF */
    u16 unkA2;      /* 0xA2: raised to scale_game_frames(D_0015EEA4) / 600 */
    u32 levels;     /* 0xA4: bit per level index, plus 0x80000000 */
} DeliveryStats;

extern DeliveryStats D_00141680;
extern void          compute_glove_of_doom_throw_velocity(struct Moby *,
                                                          CarrierVars *,
                                                          OvlVec4 *) __asm__("FUN_L00_002c7548");

/* Carrier: keeps its grip point in front of it, hands the held moby over when the hero asks, and
   picks a new one up when get_ammo allows. */
void glove_of_doom_update(struct Moby *moby) __asm__("FUN_L00_002c7a58");

void glove_of_doom_update(struct Moby *moby) {
    enum {
        GLOVE_INITIALIZE = 0,
        GLOVE_WAIT_FOR_ANIMATION = 1,
        GLOVE_IDLE = 2,
        GLOVE_RELEASE_HELD_MOBY = 3,
        GLOVE_POST_RELEASE_WAIT = 4,
        GLOVE_BEGIN_SKIP_HELD_MOBY_SYNC_AND_SPAWN = 5,
        GLOVE_SKIP_HELD_MOBY_SYNC_AND_SPAWN = 6
    };
    OvlVec4      pos;
    OvlVec4      offset;
    OvlVec4      local;
    OvlVec4      grip_bone_transform[4];
    OvlVec4      grip;
    CarrierVars *vars = (CarrierVars *)moby->pvars;

    offset.q = 0;
    offset.f[0] = 0.01f;
    offset.f[1] = -0.14f;
    offset.f[2] = -0.05f;
    moby_point_position((u8 *)moby, 0, &pos);
    build_moby_bone_transform(moby, 0, grip_bone_transform);
    transform_vector(&local, &offset, grip_bone_transform);
    fast_vec_add(&vars->grip, &pos, &local);
    if (vars->held != 0) {
        vars->held->collision_disable = 1;
    }
    if (vars->held == 0 || vars->held->state == 0xFE || vars->held->state == 0xFD) {
        vars->held = 0;
    }
    if (hero.state.current == HERO_STATE_LOOK) {
        hero_set_state(0x1E, 1);
    }
    if (fast_dec_timer(&vars->timer) && !hero.item_use_locked) {
        if ((hero.item_anim_active && hero.item_anim_frames == scale_game_frames(0x11)) ||
            ((D_0013CAE4 & hero.items[0].button_mask) && hero.state.current == 0x1E &&
             get_ammo(-1)) ||
            (hero.state.current == 0x23 && hero.state_timer == scale_game_frames(0x10))) {
            allocate_hero_voice_for_entry(0x1A, 0);
            use_ammo(-1, 1);
            moby->state = 3;
        }
    }
    grip.q = vars->grip.q;
    compute_glove_of_doom_throw_velocity(moby, vars, &grip);
    switch (moby->state) {
    case GLOVE_INITIALIZE:
        vars->held = 0;
        if (moby->anim_flags & 2) {
            moby->state = 2;
        } else {
            moby->state = 1;
        }
    case GLOVE_WAIT_FOR_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = 2;
        }
        break;
    case GLOVE_IDLE:
        break;
    case GLOVE_RELEASE_HELD_MOBY:
        if (vars->held != 0) {
            if (D_00141680.deliveries < 0xFFFF) {
                D_00141680.deliveries++;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > D_00141680.unkA2) {
                D_00141680.unkA2 = scale_game_frames(D_0015EEA4) / 600;
            }
            D_00141680.levels = D_00141680.levels | (1 << current_level_index) | 0x80000000;
            release_glove_of_doom_held_moby(
                (void *)&vars->grip, (u8 *)vars->held, vars->held->pvars);
            vars->held = 0;
        } else {
            allocate_voice_for_target_entry(0, 0, moby);
        }
        moby->state = 4;
        vars->timer = scale_game_frames(0x14);
        break;
    case GLOVE_POST_RELEASE_WAIT:
        if (hero.state.current != 0x23 && !hero.item_anim_active) {
            moby->state = 2;
        }
        break;
    case GLOVE_BEGIN_SKIP_HELD_MOBY_SYNC_AND_SPAWN:
        moby->state = 6;
        break;
    case GLOVE_SKIP_HELD_MOBY_SYNC_AND_SPAWN:
        return;
    }
    if (vars->held != 0) {
        qcopy(&vars->held->pos, &vars->grip);
    } else if (get_ammo(-1)) {
        qzero(&grip);
        vars->held = (struct Moby *)spawn_glove_of_doom_held_moby(
            (int)moby, (void *)&vars->grip, (void *)&grip);
    }
}

s32 D_L03_00173F64[1] __attribute__((section(".data"))) = {0};

s32 D_L09_00173FE4[1] __attribute__((section(".data"))) = {0};

int   D_L01_00161868 = 0;
float D_L01_00161858 = 0.2f;
float D_L01_00161860 = 0;
int   D_L02_00161808 = 0;
float D_L02_001617F8 = 0.2f;
float D_L02_00161800 = 0;
int   D_L03_00161818 = 0;
float D_L03_00161808 = 0.2f;
float D_L03_00161810 = 0;
int   D_L04_00161808 = 0;
float D_L04_001617F8 = 0.2f;
float D_L04_00161800 = 0;
int   D_L05_00161858 = 0;
float D_L05_00161848 = 0.2f;
float D_L05_00161850 = 0;
int   D_L06_00161838 = 0;
float D_L06_00161828 = 0.2f;
float D_L06_00161830 = 0;
int   D_L07_00161808 = 0;
float D_L07_001617F8 = 0.2f;
float D_L07_00161800 = 0;
int   D_L08_00161858 = 0;
float D_L08_00161848 = 0.2f;
float D_L08_00161850 = 0;
int   D_L09_001618E8 = 0;
float D_L09_001618B8 = 0.2f;
float D_L09_001618C0 = 0;
int   D_L10_00161818 = 0;
float D_L10_00161808 = 0.2f;
float D_L10_00161810 = 0;
int   D_L11_00161898 = 0;
float D_L11_00161888 = 0.2f;
float D_L11_00161890 = 0;
int   D_L12_00161818 = 0;
float D_L12_00161808 = 0.2f;
float D_L12_00161810 = 0;
int   D_L13_001618E8 = 0;
float D_L13_001618D8 = 0.2f;
float D_L13_001618E0 = 0;
int   D_L14_00161A28 = 0;
float D_L14_00161A18 = 0.2f;
float D_L14_00161A20 = 0;
int   D_L15_001619E8 = 0;
float D_L15_001619B8 = 0.2f;
float D_L15_001619C0 = 0;
int   D_L16_001618C8 = 0;
float D_L16_001618B8 = 0.2f;
float D_L16_001618C0 = 0;
int   D_L17_00161918 = 0;
float D_L17_00161908 = 0.2f;
float D_L17_00161910 = 0;
int   D_L18_001618A8 = 0;
float D_L18_00161898 = 0.2f;
float D_L18_001618A0 = 0;

int D_L00_00161838_macro __asm__("D_L00_00161838") = 0;
