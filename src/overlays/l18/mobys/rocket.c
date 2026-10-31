/* Level 18 rocket (class 0x234): flight, trail, explosion and target pulse. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

#include "rnc/overlay/l18/mobys/rocket_globals.h"

extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
void        mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

#include "qcopy.h"

extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D6440), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x24];
    float f24;
    float f28;
    int   c;
    int   lim;
    int   end;
    int   flags;
} L18D;

extern char *D_L18_00174858;
extern float FUN_L04_001f1978(float);
extern int   D_L18_0015F5C4;
void         spawn_rocket_trail_effects(void *mobyp, void *vec) __asm__("FUN_L18_002d5488");
extern void  spawn_rocket_explosion(void *, void *) __asm__("FUN_L18_002d5610");
void         draw_rocket_target_pulse_quad(struct Moby *m) __asm__("FUN_L18_002d5768");

void rocket_moby_update(struct Moby *moby) __asm__("FUN_L18_002d5050");

/* Moby class 564 */
void rocket_moby_update(struct Moby *moby) {
    L18D *moby_vars = (L18D *)moby->pvars;
    float target_movement_step[4];
    float movement_step[4];
    float rotation_angles[4];
    float previous_position[4];
    float target_heading_basis[12];
    float configured_rotation_basis[12];
    float aA0[16];
    float movement_blend_weight, pitch_angle_or_remaining_frames, k;
    int   movement_collision_result;
    if (hero.state.current == 0x72 || D_L18_0015F5C4 == 2) {
        mark_moby_for_removal(moby);
    } else if (moby->state == 0) {
        qcopy(previous_position, &moby->pos);
        movement_blend_weight = convert_integer_to_float(moby_vars->c);
        movement_blend_weight = movement_blend_weight / convert_integer_to_float(moby_vars->lim);
        if (1.0f < movement_blend_weight) {
            movement_blend_weight = 1.0f;
        }
        fast_vec_sub(target_movement_step, (char *)moby_vars + 0x10, &moby->pos);
        fast_vec_scale(target_movement_step,
                       target_movement_step,
                       1.0f / convert_integer_to_float(moby_vars->end - moby_vars->c));
        clear_u64_value(rotation_angles);
        rotation_angles[2] = fast_arc_tan(target_movement_step[0], target_movement_step[1]);
        euler_rotation_basis(target_heading_basis, rotation_angles);
        transpose_rotation_matrix(aA0, target_heading_basis);
        clear_u64_value(rotation_angles);
        rotation_angles[2] = moby_vars->f24;
        rotation_angles[1] = moby_vars->f28;
        euler_rotation_basis(configured_rotation_basis, rotation_angles);
        transform_vector_by_basis(target_movement_step, target_movement_step, aA0);
        transform_vector_by_basis(
            target_movement_step, target_movement_step, configured_rotation_basis);
        transform_vector_by_basis(target_movement_step, target_movement_step, target_heading_basis);
        lerp_vector(movement_step, moby_vars, target_movement_step, movement_blend_weight);
        spawn_rocket_trail_effects(moby, movement_step);
        fast_vec_add(&moby->pos, &moby->pos, movement_step);
        moby->rot.z = fast_arc_tan(movement_step[0], movement_step[1]);
        pitch_angle_or_remaining_frames =
            fast_arc_tan(vector_length_xy(movement_step), movement_step[2]);
        k = frame_time;
        moby->rot.y = -pitch_angle_or_remaining_frames;
        moby->rot.x = fast_add_rotations(moby->rot.x, k * 18.849556f);
        moby_vars->c = moby_vars->c + 1;
        movement_collision_result = collision_line(previous_position, &moby->pos, 0, moby, 0);
        if (movement_collision_result) {
            char *collision_filter_moby = D_L18_00174858;
            if (collision_filter_moby != 0) {
                short h = MOBY(collision_filter_moby)->oclass;
                if (h == 0x1DF) {
                    moby_vars->flags = moby_vars->flags & 4;
                } else if (h != 0x1A3) {
                    movement_collision_result = 0;
                }
            }
        }
        if (moby_vars->c == moby_vars->end || movement_collision_result) {
            spawn_rocket_explosion(moby, movement_step);
            mark_moby_for_removal(moby);
        } else if (moby_vars->flags & 2) {
            pitch_angle_or_remaining_frames = (float)(moby_vars->end - moby_vars->c);
            if (pitch_angle_or_remaining_frames <=
                FUN_L04_001f1978(rocket_target_pulse_lead_time)) {
                add_draw_callback(draw_rocket_target_pulse_quad, moby);
            }
        }
    }
}

extern void *create_moby() __asm__("FUN_0020c4f8");

/* Spawns a moby that travels along a path, with its heading set from the direction vector. */
float       rocket_spawn_scale_multiplier_gp __asm__("D_L18_00161960") = 5.0f;
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

char *create_rocket_moby(float *pos,
                         float *initial_movement_step,
                         float *target,
                         float  movement_rotation_z,
                         float  movement_rotation_y,
                         int    frames,
                         int    kind) __asm__("FUN_L18_002d5348");

char *create_rocket_moby(float *rocket_spawn_position,
                         float *initial_movement_step,
                         float *target,
                         float  movement_rotation_z,
                         float  movement_rotation_y,
                         int    frames,

                         int kind) {
    char *moby = create_moby(0x234);
    if (moby != 0) {
        char *rocket_private_vars;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        set_moby_color(moby, 0xC0, 0xC0, 0xC0);
        MOBY(moby)->scale = MOBY(moby)->scale * rocket_spawn_scale_multiplier_gp;
        qcopy(moby + 0x10, rocket_spawn_position);
        rocket_private_vars = ((char *)MOBY(moby)->pvars);
        qcopy(rocket_private_vars, initial_movement_step);
        qcopy(rocket_private_vars + 0x10, target);
        *(float *)(rocket_private_vars + 0x24) = movement_rotation_z;
        *(float *)(rocket_private_vars + 0x28) = movement_rotation_y;
        *(int *)(rocket_private_vars + 0x2C) = 0;
        *(int *)(rocket_private_vars + 0x34) = frames;
        *(int *)(rocket_private_vars + 0x30) = frames >> 1;
        *(int *)(rocket_private_vars + 0x38) = kind;
        MOBY(moby)->rot.z = fast_arc_tan(initial_movement_step[0], initial_movement_step[1]);
        MOBY(moby)->rot.y =
            -fast_arc_tan(vector_length_xy(initial_movement_step), initial_movement_step[2]);
        allocate_voice_for_target_entry(0, 0, (int)moby);
    }
    return moby;
}

#include "sda.h"

extern char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");
extern float          random_float_between(float, float) __asm__("FUN_002132a8");
extern unsigned char *FUN_L00_002712b8(void  *pos,
                                       float *vec,
                                       int    s,
                                       int    a,
                                       int    col,
                                       int    n,
                                       float  x,
                                       float  y,
                                       float  z,
                                       float  w,
                                       float  pw);
extern void           FUN_L00_00257d78(float *, float, float);
extern char          *D_L18_001B2ADC_lui __asm__("D_L18_001B2ADC") NOT_SDA;

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D6878), where it is exact; names translated to the US level program. */

extern int scale_game_frames(int);

typedef union {
    u128  q;
    float v[4];
} QU;

extern void transform_vector_by_basis(void *, void *, void *);

void spawn_rocket_trail_effects(void *mobyp, void *movement_step) __asm__("FUN_L18_002d5488");

void spawn_rocket_trail_effects(void *mobyp, void *movement_step) {
    char          *moby = mobyp;
    float          effect_vector[4];
    float          effect_spawn_position[4];
    QU             world_effect_direction;
    unsigned char *particle;
    FUN_L00_00257d78(effect_vector, frame_time * 0.1f, frame_time * 0.2f);
    fast_vec_scale(effect_spawn_position, movement_step, random_float_between(0.0f, 1.0f));
    fast_vec_add(effect_spawn_position, effect_spawn_position, moby + 0x10);
    particle = FUN_L00_002712b8(effect_spawn_position,
                                effect_vector,
                                scale_game_frames(0x14),
                                0x7F,
                                0x606060,
                                3,
                                40000.0f,
                                1000.0f,
                                1.0f,
                                -0.0002f,
                                0.0f);
    if (particle != 0) {
        particle[2] = *(unsigned char *)D_L18_001B2ADC_lui;
        particle[3] = 0x44;
    }
    world_effect_direction.q = 0;
    world_effect_direction.v[2] = 0.02f;
    transform_vector_by_basis(&world_effect_direction, &world_effect_direction, moby + 0xC0);
    spawn_burst(effect_spawn_position,
                &world_effect_direction,
                0x4F007FFF,
                0x1FFFFFFF,
                scale_game_frames(0x14),
                1,
                20000.0f);
}

extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");

#include "rnc/overlay/quad.h"

extern void func_L00_0025F4A8_alt(void *,
                                  void *,
                                  void *,
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
                                  float,
                                  int,
                                  int,
                                  int,
                                  int) __asm__("FUN_L00_0025e450");

float rocket_explosion_hit_priority_gp __asm__("D_L18_0016196C") = 3.0f;

void spawn_rocket_explosion(void *moby_object, void *movement_step) {
    char  *moby = moby_object;
    float  movement_step_copy[4];
    float *qp = movement_step_copy;
    int    flags;
    float  flag_selected_effect_scalar, flag_selected_effect_argument, effect_scale, hit_radius, e;
    *(OvlQuad *)qp = *(OvlQuad *)movement_step;
    flags = *(int *)(((char *)MOBY(moby)->pvars) + 0x38);
    effect_scale = (flags & 1) ? 2.0f : 0.75f;
    flag_selected_effect_scalar = (flags & 1) ? 4.0f : 2.0f;
    flag_selected_effect_argument = (flags & 1) ? 2.0f : 1.0f;
    hit_radius = (flags & 4) ? 0.0f : 1.5f;
    func_L00_0025F4A8_alt(moby,
                          qp,
                          moby + 0x10,
                          hit_radius,
                          1.0f,
                          0x14,
                          6,
                          0x20,
                          flag_selected_effect_scalar,
                          flag_selected_effect_argument,
                          9.0f,
                          effect_scale,
                          1,
                          5.0f,
                          0,
                          0,
                          -1,
                          0);
    if (hit_radius != 0.0f) {
        if (hero.vehicle_mode == 2) {
            play_entity_sound(
                (int)moby, 2.0f, moby + 0x10, 2, rocket_explosion_hit_priority_gp, 1.0f, 0, 1, 0);
        }
    }
}

/* Draws the moby's pulsing glow quad: four corners around its position with a fading colour. */
typedef struct {
    float              m[4][4];
    unsigned int       col[4];
    float              uv[4][2];
    unsigned long long a, b, c, d;
} Pk;

float sparkle_quad_corners[4][4] __asm__("D_L18_001D3630") = {
    { 1.0f,  1.0f, 0.1f, 0.0f},
    { 1.0f, -1.0f, 0.1f, 0.0f},
    {-1.0f,  1.0f, 0.1f, 0.0f},
    {-1.0f, -1.0f, 0.1f, 0.0f},
};

float sparkle_quad_uvs[8] __asm__("D_L18_001D3670") = {
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

extern unsigned int       fast_tween_color(float, int, int) __asm__("FUN_001fa6e0");
extern unsigned long long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void               draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

#include "rnc/overlay/l18/mobys/rocket_data/draw_rocket_target_pulse_quad.h"

int rocket_target_pulse_color_start_gp __asm__("D_L18_00161980") =
    0x800020f0;                                                              /* RGBA, 0xAABBGGRR */
int rocket_target_pulse_color_end_gp __asm__("D_L18_00161984") = 0x8000f0f0; /* RGBA, 0xAABBGGRR */

void draw_rocket_target_pulse_quad(struct Moby *m) {
    Pk           pk;
    char        *d = (char *)m->pvars;
    unsigned int col;
    int          i;
    col = fast_tween_color(
        (fast_sin(fast_normalize_angle((*(int *)(d + 0x2C) & 0xF) * 0.0625f * 6.28318f)) + 1.0f) *
            0.5f,
        rocket_target_pulse_color_start_gp,
        rocket_target_pulse_color_end_gp);
    pk.b = get_effect_texture(0xD);
    pk.d = (unsigned long long)rocket_target_pulse_blend_minuend_selector_gp |
           ((unsigned long long)rocket_target_pulse_blend_subtrahend_selector_gp << 2) |
           ((unsigned long long)rocket_target_pulse_blend_alpha_factor_selector_gp << 4) |
           ((unsigned long long)rocket_target_pulse_blend_addend_selector_gp << 6) |
           0x8000000000ULL;
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 0;
    for (i = 0; i < 4; i++) {
        float *quad_vertex = pk.m[i];
        fast_vec_add(quad_vertex, sparkle_quad_corners[i], d + 0x10);
        quad_vertex[3] = 1.0f;
        pk.col[i] = col;
        pk.uv[i][0] = sparkle_quad_uvs[i * 2];
        pk.uv[i][1] = sparkle_quad_uvs[i * 2 + 1];
    }
    draw_geometry_quad(&pk, 0, 0);
}
