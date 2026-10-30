/* Shared moby classes: hoverboard_ring, lamp. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "sda.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/math/rotations/extract_matrix_rotation_quaternion.h"
#include "rnc/overlay/shared/camera/init_camera_system.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031C0F0), where it is exact; names translated to the US level program. */

extern unsigned char *
spawn_mode_controlled_orbiting_trail_particle(float  f0,
                                              float  f1,
                                              int    a0,
                                              void  *pos,
                                              int    timer,
                                              int    flag,
                                              int    col,
                                              float *vel) __asm__("FUN_L00_002743a8");

void hoverboard_ring_update(char *moby) __asm__("FUN_L05_0031abe0");

void hoverboard_ring_update(char *moby) {
    enum {
        HOVERBOARD_RING_STATE_INIT = 0,
        HOVERBOARD_RING_STATE_ACTIVE = 1
    };
    switch (((unsigned char *)moby)[0x20]) {
    case HOVERBOARD_RING_STATE_INIT:
        ((unsigned char *)moby)[0x30] = 0x80;
        MOBY(moby)->flags |= 0x41;
        moby[0x20] = 1;
        break;
    case HOVERBOARD_RING_STATE_ACTIVE: {
        float bounding_sphere[4];
        int   i;
        qcopy(bounding_sphere, moby + 0x10);
        bounding_sphere[3] = 8.0f;
        if (fast_bsphere_check((char *)bounding_sphere, 128.0f) != -1) {
            if (hero.state.control_mode == 0x16) {
                for (i = 0; i < 3; i++) {
                    float first_random_angle = random_angle_radians();
                    float second_random_angle = random_angle_radians();
                    spawn_mode_controlled_orbiting_trail_particle(first_random_angle,
                                                                  second_random_angle,
                                                                  (int)moby,
                                                                  D_L05_0015F580,
                                                                  scale_game_frames(0x28),
                                                                  1,
                                                                  0x7F,
                                                                  0);
                }
            }
        }
        break;
    }
    }
}

#include "rnc/mobys/moby.h"
#include "rnc/math/vector.h"

/* Vars of the level 5 mine FUN_L05_0031c8e0 drives. */
struct MineVars_c8e0 {
    u8  pad0[0x60];
    s32 alpha;                  /* 0x60: glow alpha, faded out with camera distance */
    u8  glow_r, glow_g, glow_b; /* 0x64: moby glow colour */
    u8  pad67;
    s32 color;                     /* 0x68: spark colour */
    u8  spark_r, spark_g, spark_b; /* 0x6C */
    u8  max_alpha;                 /* 0x6F */
};

/* A spark effect FUN_L00_0026d000 returns. */
struct Spark_c8e0 {
    u8  pad0[3];
    u8  flags; /* 0x03 */
    u8  pad4[6];
    s16 life; /* 0x0A */
    u8  padC[0x14];
    struct SparkDraw_c8e0 {
        u8  pad0[4];
        s32 mode; /* 0x24 */
        u8  pad8[2];
        u8  fade; /* 0x2A */
        u8  life; /* 0x2B */
    } draw;
};

extern f32 D_L05_00161FC8_gp __asm__("D_L05_00161FC8") __attribute__((sda));

#include "rnc/overlay/shared/props/hoverboard_ring_globals.h"

extern f32  debris_axis_launch_speed_gp __asm__("D_L05_00161FD0") __attribute__((sda));
extern f32  debris_upward_launch_speed_gp __asm__("D_L05_00161FD4") __attribute__((sda));
extern void allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern struct Moby *spawn_debris_c8e0(f32,
                                      struct Moby *,
                                      s32,
                                      void *,
                                      void *,
                                      s32,
                                      s32,
                                      void *,
                                      void *,
                                      void *) __asm__("FUN_L00_00263fd8");
extern void
spawn_trail_c8e0(void *, s32, f32, f32, s32, s32, s32, void *, f32) __asm__("FUN_L00_00272060");
extern struct Spark_c8e0       *
spawn_puff(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");
char *spawn_moby_class_5ea_from_source(unsigned char *source_moby) __asm__("FUN_L05_0031d160");

extern char *spawn_moby_class_5ea_from_source(unsigned char *) __asm__("FUN_L05_0031d160");
void         draw_moby_glow_sprite(char *m) __asm__("FUN_L05_0031cf58");

/* Mine update: sets its glow, fades it out with camera distance and blows up into debris, smoke and sparks when something touches it. */
void update_moby_glow_and_contact_explosion(struct Moby *moby) __asm__("FUN_L05_0031c8e0");

void update_moby_glow_and_contact_explosion(struct Moby *moby) {
    struct MineVars_c8e0  *vars = (struct MineVars_c8e0 *)moby->pvars;
    Vec4                   tip;
    Vec4                   debris_velocity;
    Vec4                   effect_motion;
    Vec4                   start;
    f32                    distance;
    f32                    fade;
    f32                    size;
    f32                    speed;
    f32                    yaw;
    f32                    pitch;
    char                  *hit;
    struct Moby           *debris;
    struct Spark_c8e0     *spark;
    struct SparkDraw_c8e0 *draw;
    s32                    sign;
    s32                    turn;
    s32                    i;
    s32                    frames;

    if (moby->state == 0) {
        moby->state = 1;
        moby->glow_color = (0x7F000000 | vars->glow_b << 16 | vars->glow_g << 8) | vars->glow_r;
        vars->color = vars->spark_b << 16 | vars->spark_g << 8 | vars->spark_r;
    }
    distance = vector_distance_xy(D_L05_001671C0, &moby->pos);
    if (48.0f <= distance) {
        return;
    }
    if (32.0f <= distance) {
        fade = (distance - 32.0f) * 0.0625f;
        vars->alpha =
            truncate_float_to_s32(convert_integer_to_float(vars->max_alpha) * (1.0f - fade));
    } else {
        vars->alpha = vars->max_alpha;
    }
    add_draw_callback(draw_moby_glow_sprite, moby);
    hit = find_moby_hit(moby, 0x810000, 0);
    moby->hit_slot = 0xFF;
    if (hit == 0) {
        return;
    }
    fast_vec_scale(&tip, &moby->rot_row2, D_L05_00161FC8_gp);
    fast_vec_add(&tip, &moby->pos, &tip);
    if (!(*(s32 *)(hit + 0x24) & 0x800000) && !(fast_vec_dist(&tip, hit) <= 0.8f)) {
        if ((*(struct Moby **)(hit + 0x20))->oclass != 0x47) {
            return;
        }
        if (!(fast_vec_dist(&tip, &(*(struct Moby **)(hit + 0x20))->pos) <= 0.8f)) {
            return;
        }
    }
    allocate_voice_for_target_entry(0, 0, moby);
    spawn_moby_class_5ea_from_source((unsigned char *)moby);
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f,
                               moby,
                               0x5ED,
                               &moby->pos,
                               &moby->rot,
                               scale_game_frames(0x5A),
                               0,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    spawn_proximity_gated_bolts(moby);
    fast_vec_scale(&debris_velocity, &moby->rot_row0, debris_axis_launch_speed_gp * frame_scale);
    debris_velocity.f[2] += debris_upward_launch_speed_gp * frame_scale;
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f,
                               moby,
                               0x5EE,
                               &moby->pos,
                               &moby->rot,
                               scale_game_frames(0x5A),
                               0,
                               &debris_velocity,
                               D_L05_0015F580,
                               D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    debris_velocity.f[0] = -debris_velocity.f[0];
    debris_velocity.f[1] = -debris_velocity.f[1];
    moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f,
                               moby,
                               0x5EE,
                               &moby->pos,
                               &moby->rot,
                               scale_game_frames(0x5A),
                               0,
                               &debris_velocity,
                               D_L05_0015F580,
                               D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    for (i = 0; i < 24; i++) {
        size = random_float_between(0.3f, 4.0f);
        sign = random_integer_below(2);
        if (sign == 0) {
            sign--;
        }
        yaw = random_angle_radians();
        pitch = random_float_between(0.34906584f, 1.5358897f);
        build_spherical_offset(
            &effect_motion, random_float_between(0.05f, 0.1f) * frame_scale, yaw, pitch);
        qcopy(&start, &tip);
        start.f[0] += randf_sym(0.0f, 0.15f);
        start.f[1] += randf_sym(0.0f, 0.15f);
        start.f[2] += randf_sym(0.0f, 0.15f);
        frames = scale_game_frames(0x3C);
        spawn_trail_c8e0(&start,
                         frames,
                         size * 0.1f,
                         size,
                         vars->color | 0x7F000000,
                         0,
                         sign,
                         &effect_motion,
                         0.003f);
        sign = -sign;
        frames = scale_game_frames(0x3C);
        spawn_trail_c8e0(
            &start, frames, size * 0.07f, size * 0.7f, 0x7F7F7F7F, 1, sign, &effect_motion, 0.003f);
        effect_motion.f[1] = 0.0f;
        effect_motion.f[0] = 0.0f;
        effect_motion.f[2] = random_float_between(0.01f, 0.05f) * frame_scale;
        speed = random_float_between(60000.0f, 200000.0f);
        turn = random_integer_below(2);
        spark = spawn_puff(&tip,
                           0.3f,
                           1.0f,
                           1.02f,
                           random_integer_below(2) ? -turn : turn,
                           &effect_motion,
                           speed,
                           0x407F7F7F);
        if (spark != 0) {
            spark->life = scale_game_frames(random_int_between(0x5A, 0x78));
            draw = &spark->draw;
            draw->mode = 2;
            draw->fade = random_int_between(0x40, 0x60);
            draw->life = spark->life;
            if (random_integer_below(2) != 0) {
                spark->flags = 0x44;
            }
        }
    }
    mark_moby_for_removal(moby);
}
/* Draws the moby's camera-facing glow sprite, flickering its alpha at random. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031E468), where it is exact; names translated to the US level program. */

typedef struct {
    float m[4][4];
    u32   col[4];
    float uv[8];
    u64   a, b, c, d;
} Pk;

extern float glow_sprite_local_vertices[4][4] __asm__("D_L05_00215C70");
extern void  draw_geometry_quad(void *, void *, int);

extern short glow_sprite_camera_offset_distance __asm__("D_L05_00161FCC") __attribute__((sda));
extern char  D_L05_001671C0_lui[] __asm__("D_L05_001671C0") NOT_SDA;

void draw_moby_glow_sprite(char *m) {
    char *d = *(char **)(m + 0x78);
    Pk    pk;
    float up[4];
    float M[4][4];
    float sprite_offset[4];
    float camera_facing_offset[4];
    u32   col;
    int   j;
    *(OvlQuad *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    pk.b = get_effect_texture(0xB);
    pk.d = 0x8000000048ULL;
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 5;
    pk.uv[0] = 0.0f;
    pk.uv[1] = 0.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 1.0f;
    pk.uv[7] = 1.0f;
    fast_vec_scale(sprite_offset, m + 0xE0, *(float *)&D_L05_00161FC8_gp);
    fast_vec_add(M[3], m + 0x10, sprite_offset);
    fast_vec_sub(M[0], D_L05_001671C0_lui, M[3]);
    fast_vec_normalize(M[0], M[0], 1.0f);
    fast_vec_cross(M[1], M[0], up);
    fast_vec_normalize(M[1], M[1], 1.0f);
    fast_vec_cross(M[2], M[1], M[0]);
    fast_vec_scale(camera_facing_offset, M[0], *(float *)&glow_sprite_camera_offset_distance);
    fast_vec_add(M[3], M[3], camera_facing_offset);
    if (((unsigned char *)d)[0x67] != 0 && random_integer_below(8) == 0) {
        *(int *)(d + 0x60) >>= 1;
    }
    col = (*(int *)(d + 0x60) << 24) | *(int *)(d + 0x68);
    pk.col[3] = col;
    pk.col[2] = col;
    pk.col[1] = col;
    pk.col[0] = col;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(pk.m[j], glow_sprite_local_vertices[j], *(float *)(d + 0x70));
        transform_vector(pk.m[j], pk.m[j], M);
    }
    draw_geometry_quad(&pk, 0, 0);
}

char *spawn_moby_class_5ea_from_source(unsigned char *source_moby) {
    unsigned char *destination_moby = (unsigned char *)create_moby(0x5EA);
    if (destination_moby != 0) {
        destination_moby[0x30] = source_moby[0x30];
        MOBY(destination_moby)->draw_dist = ((unsigned short)MOBY(source_moby)->draw_dist);
        destination_moby[0x31] = 1;
        MOBY(destination_moby)->rot.x = MOBY(source_moby)->rot.x;
        MOBY(destination_moby)->rot.y = MOBY(source_moby)->rot.y;
        MOBY(destination_moby)->rot.z = MOBY(source_moby)->rot.z;
        MOBY(destination_moby)->spawn_frame = ((long)MOBY(source_moby)->spawn_frame);
        qcopy(destination_moby + 0x10, source_moby + 0x10);
        update_entity(destination_moby);
    }
    return (char *)destination_moby;
}
/* Sets up a moby's vector from the level table and clamps its height for the current state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032A868), where it is exact; names translated to the US level program. */

void update_hero_follow_camera_focus(char *moby) __asm__("FUN_L05_00329358");

void update_hero_follow_camera_focus(char *moby) {
    float  hero_focus_offset[4];
    char  *orbit = *(char **)(moby + 0x70);
    float *focus_position = (float *)(orbit + 0x80);
    *(OvlQuad *)hero_focus_offset = 0;
    hero_focus_offset[2] = 0.7f;
    transform_vector(hero_focus_offset, hero_focus_offset, &hero);
    fast_vec_add(focus_position, &hero.motion.pos, hero_focus_offset);
    qcopy(orbit + 0x1D0, focus_position);
    if (current_level_index == 5) {
        float height_reference_position[4];
        *(OvlQuad *)height_reference_position = 0;
        height_reference_position[0] = 286.16f;
        height_reference_position[1] = 447.82f;
        height_reference_position[2] = 63.37f;
        height_reference_position[3] = 1.0f;
        if (vector_distance_xy(height_reference_position, focus_position) < 48.0f) {
            if (focus_position[2] < 62.4f) {
                focus_position[2] = 62.4f;
            }
        } else {
            if (focus_position[2] < 67.2f) {
                focus_position[2] = 67.2f;
            }
        }
    } else if (current_level_index == 0x10) {
        if (focus_position[2] < 77.5f) {
            focus_position[2] = 77.5f;
        }
    }
    fast_vec_normalize(focus_position + 8, &hero.gravity_dir, -1.0f);
}

extern char *D_L05_00167200 NOT_SDA;
void                        set_camera_follow_parameters(float spring_distance,
                                                         float spring_height,
                                                         float focus_distance,
                                                         float airborne_spring_distance,
                                                         float airborne_spring_height,
                                                         float airborne_focus_distance,
                                                         float position_spring_response_coefficient,
                                                         float position_spring_coefficient) __asm__("FUN_L05_003294c0");

void set_camera_follow_parameters(float spring_distance,
                                  float spring_height,
                                  float focus_distance,
                                  float airborne_spring_distance,
                                  float airborne_spring_height,
                                  float airborne_focus_distance,
                                  float position_spring_response_coefficient,

                                  float position_spring_coefficient) {
    char *camera = D_L05_00167200;
    char *orbit = *(char **)(camera + 0x70);

    *(float *)(orbit + 0x184) = spring_distance;
    *(float *)(orbit + 0x188) = spring_height;
    *(float *)(orbit + 0x180) = focus_distance;
    *(float *)(orbit + 0x190) = airborne_spring_distance;
    *(float *)(orbit + 0x194) = airborne_spring_height;
    *(float *)(orbit + 0x18C) = airborne_focus_distance;
    orbit = *(char **)(camera + 0x70);
    if (position_spring_response_coefficient < 0.0f) {
        *(float *)(orbit + 0x10) = 0.01f;
    } else {
        *(float *)(orbit + 0x10) = position_spring_response_coefficient;
    }
    if (position_spring_coefficient < 0.0f) {
        *(float *)(orbit + 0x14) = 0.2f;
    } else {
        *(float *)(orbit + 0x14) = position_spring_coefficient;
    }
}
/* Places the camera moby relative to the hero's target and builds its look-at basis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032AA50), where it is exact; names translated to the US level program. */

void build_hero_follow_camera_transform(char *camera) __asm__("FUN_L05_00329540");

void build_hero_follow_camera_transform(char *camera) {
    char        *orbit = *(char **)(camera + 0x70);
    char        *at = camera + 0x30;
    char        *up = camera + 0x10;
    char        *side = camera + 0x20;
    struct Hero *hero_state;
    char        *focus;
    char        *spring;
    float        target_height_offset[4];
    float        look_direction[4];
    float        look_target[4];
    float        camera_offset[4];
    float        gravity_up[4];
    update_hero_follow_camera_focus(camera);
    hero_state = &hero;
    focus = orbit + 0x80;
    spring = orbit + 0xE0;
    fast_vec_normalize(camera_offset, ((char *)hero_state->moby) + 0xC0, 1.0f);
    fast_vec_normalize(camera_offset, camera_offset, *(float *)(orbit + 0xE0));
    fast_vec_sub(at, focus, camera_offset);
    fast_vec_normalize(gravity_up, &hero_state->gravity_dir, -1.0f);
    fast_vec_normalize(camera_offset, gravity_up, *(float *)(spring + 0x10));
    fast_vec_add(at, at, camera_offset);
    fast_vec_normalize(target_height_offset, gravity_up, *(float *)(focus + 0x4C));
    fast_vec_add(look_target, target_height_offset, focus);
    fast_vec_sub(look_direction, look_target, at);
    fast_vec_normalize(camera, look_direction, 1.0f);
    fast_vec_cross(up, camera, gravity_up);
    fast_vec_cross(side, up, camera);
    fast_vec_normalize(up, up, 1.0f);
    fast_vec_normalize(side, side, 1.0f);
    qcopy(camera + 0x40, camera);
}

#include "qcopy.h"
#include "rnc/mobys/moby.h"

typedef struct {
    Vec4 pos; /* 0x00 */
    u8   pad10[0x20];
    Vec4 side;      /* 0x30 */
    s32  state;     /* 0x40: 5 after a preset reset */
    s32  unk44;     /* 0x44 */
    f32  max_angle; /* 0x48 */
    f32  dist;      /* 0x4C */
    u8   pad50[8];
    f32  unk58; /* 0x58 */
    f32  unk5C; /* 0x5C */
} CameraFocus;

/* View block at orbit + 0x30. */
typedef struct {
    Vec4 vel; /* 0x00 */
    u8   pad10[0x10];
    f32  unk20; /* 0x20 */
    f32  unk24; /* 0x24 */
    s32  unk28; /* 0x28 */
    s16  unk2C; /* 0x2C */
    u8   pad2E[0x12];
    f32  aspect; /* 0x40 */
    u8   pad44[0xC];
} CameraView;

#include "rnc/camera/camera_spring.h"

typedef struct {
    Vec4 vel;   /* 0x00 */
    f32  unk10; /* 0x10 */
    u8   pad14[0xC];
    Vec4 accel; /* 0x20 */
    u8   pad30[4];
    f32  unk34; /* 0x34 */
    f32  unk38; /* 0x38 */
    s32  unk3C; /* 0x3C */
    s32  unk40; /* 0x40 */
    u8   pad44[0xC];
} CameraTrack;

/* Orbit state the camera keeps while it circles the hero. */
typedef struct {
    Vec4         axis;  /* 0x00 */
    f32          unk10; /* 0x10: 0.01 unless the preset gives one (FUN_L05_003294c0) */
    f32          unk14; /* 0x14: 0.2 unless the preset gives one */
    s32          unk18; /* 0x18 */
    s16          mode;  /* 0x1C: 0 waiting, 1 orbiting */
    u8           pad1E[0x12];
    CameraView   view;          /* 0x30 */
    CameraFocus  focus;         /* 0x80 */
    CameraSpring spring;        /* 0xE0 */
    CameraTrack  track;         /* 0x100 */
    f32          goal[3];       /* 0x150: yaw, pitch, distance */
    f32          vel[3];        /* 0x15C */
    f32          accel[3];      /* 0x168 */
    f32          damp[3];       /* 0x174 */
    f32          dist;          /* 0x180 */
    f32          spring_dist;   /* 0x184 */
    f32          spring_height; /* 0x188 */
    f32          unk18C;        /* 0x18C */
    f32          unk190;        /* 0x190 */
    f32          unk194;        /* 0x194 */
    u8           pad198[0x18];
    Vec4         unk1B0; /* 0x1B0 */
    Vec4         unk1C0; /* 0x1C0 */
} CameraOrbit;

typedef struct {
    u8           pad0[0x30];
    Vec4         pos; /* 0x30 */
    u8           pad40[0x10];
    f32          yaw;   /* 0x50 */
    f32          pitch; /* 0x54 */
    f32          dist;  /* 0x58 */
    u8           pad5C[0x14];
    CameraOrbit *orbit; /* 0x70 */
    u8           pad74[0xA];
    s16          unk7E; /* 0x7E: cleared after a preset reset */
    u8           pad80[4];
    s16          preset; /* 0x84: row of D_L05_0015EF50 */
} Camera;

/* Camera preset: distances, angle limits and damping. */
typedef struct {
    f32 unk0; /* 0x00 */
    u8  pad4[0x1C];
    f32 unk20;         /* 0x20 */
    f32 unk24;         /* 0x24 */
    f32 spring_dist;   /* 0x28 */
    f32 spring_height; /* 0x2C */
    f32 dist;          /* 0x30 */
    f32 unk34;         /* 0x34 */
    f32 unk38;         /* 0x38 */
    f32 unk3C;         /* 0x3C */
    f32 unk40;         /* 0x40 */
    f32 unk44;         /* 0x44 */
    f32 height_vel;    /* 0x48: spring velocity of the camera height */
} CameraPreset;

typedef struct {
    u8            pad0[0x1C];
    CameraPreset *preset; /* 0x1C */
} CameraPresetRow;

extern CameraPresetRow *D_L05_0015EF50;

/* Resets the camera from its preset (row cam->preset of D_L05_0015EF50): distances,
   angle limits and damping, cleared velocities, then places it. */
void reset_hero_follow_camera_from_preset(Camera *camera_to_reset) __asm__("FUN_L05_003296e0");

void reset_hero_follow_camera_from_preset(Camera *camera_to_reset) {
    CameraPreset *preset = D_L05_0015EF50[camera_to_reset->preset].preset;
    CameraOrbit  *orbit_state;
    CameraFocus  *focus;
    CameraSpring *spring;
    CameraOrbit  *orbit;
    CameraTrack  *track;
    CameraView   *view;

    preset->height_vel = 0;
    orbit_state = camera_to_reset->orbit;
    orbit_state->spring_dist = preset->spring_dist;
    orbit_state->spring_height = preset->spring_height;
    orbit_state->dist = preset->dist;
    orbit_state->unk190 = preset->unk34;
    orbit_state->unk194 = preset->unk38;
    orbit_state->unk18C = preset->unk3C;
    clear_u64_value(&orbit_state->unk1B0);
    clear_u64_value(&orbit_state->unk1C0);
    focus = &camera_to_reset->orbit->focus;
    focus->state = 5;
    focus->unk44 = D_001413D0[0];
    focus->dist = orbit_state->dist;
    focus->max_angle = 0.209439516f;
    focus->unk58 = 0.00174532924f;
    focus->unk5C = -0.0043633231f;
    spring = &camera_to_reset->orbit->spring;
    spring->dist = orbit_state->spring_dist;
    spring->unk4 = 0.5f;
    spring->height = orbit_state->spring_height;
    orbit = camera_to_reset->orbit;
    orbit->unk10 = 0.01f;
    orbit->unk14 = 0.2f;
    orbit->unk18 = 0;
    orbit->mode = 0;
    clear_u64_value(orbit);
    track = &camera_to_reset->orbit->track;
    clear_u64_value(track);
    clear_u64_value(&track->accel);
    track->unk40 = 0;
    track->unk10 = preset->unk0;
    track->unk34 = preset->unk20;
    track->unk38 = preset->unk24;
    track->unk3C = 0;
    view = &camera_to_reset->orbit->view;
    view->unk20 = 0.8f;
    view->unk24 = 0.3f;
    view->aspect = 1.3333334f;
    view->unk28 = 0;
    view->unk2C = 0;
    clear_u64_value(view);
    set_camera_follow_parameters(preset->spring_dist,
                                 preset->spring_height,
                                 preset->dist,
                                 preset->unk34,
                                 preset->unk38,
                                 preset->unk3C,
                                 preset->unk40,
                                 preset->unk44);
    build_hero_follow_camera_transform((char *)camera_to_reset);
    camera_to_reset->unk7E = 0;
}

extern float FUN_001ebd78(void *, float, float, float, float, float);

/* Camera that orbits the hero while hero.vert_ramp_launch is set, easing yaw, pitch and distance toward their goals. */
void update_hero_follow_camera_orbit(Camera *cam) __asm__("FUN_L05_003298d8");

void update_hero_follow_camera_orbit(Camera *hero_follow_camera) {
    Vec4         up;
    Vec4         side;
    Vec4         fwd;
    Vec4         rel;
    Vec4         along;
    Vec4         flat;
    Vec4         unit;
    Vec4         yaw_rotated_forward;
    CameraOrbit *orbit = hero_follow_camera->orbit;
    CameraFocus *focus = &orbit->focus;
    f32         *spring = orbit->goal;
    f32          dot;
    f32          angle_normalization_length;
    f32          initial_yaw;
    f32          pitch;
    f32          tilt;

    fast_vec_normalize(&up, &hero.gravity_dir, -1.0f);
    if (orbit->mode == 0 && hero.vert_ramp_launch != 0) {
        orbit->mode = 1;
        fast_vec_cross(&side, &hero.moby->rot_row2, &up);
        fast_vec_normalize(&side, &side, 1.0f);
        fast_vec_cross(&fwd, &up, &side);
        fast_vec_normalize(&fwd, &fwd, 1.0f);
        fast_vec_sub(&rel, &hero_follow_camera->pos, focus);
        dot = fast_vec_dot(&rel, &up);
        fast_vec_normalize(&along, &up, dot);
        fast_vec_sub(&flat, &rel, &along);
        dot = fast_vec_dot(&fwd, &flat);
        angle_normalization_length = fast_vec_length(&flat);
        if (angle_normalization_length == 0.0f) {
            angle_normalization_length = 0.0001f;
        }
        initial_yaw = 1.5707964f - fast_arc_sin(dot / angle_normalization_length);
        fast_vec_normalize(&unit, &flat, 1.0f);
        if (fast_vec_dot(&side, &unit) < 0.0f) {
            initial_yaw = -initial_yaw;
        }
        hero_follow_camera->yaw = initial_yaw;
        build_look_at_matrix(&yaw_rotated_forward, &fwd, &up, initial_yaw);
        dot = fast_vec_dot(&yaw_rotated_forward, &rel);
        angle_normalization_length = fast_vec_length(&rel);
        if (angle_normalization_length == 0.0f) {
            angle_normalization_length = 0.0001f;
        }
        tilt = 1.5707964f - fast_arc_sin(dot / angle_normalization_length);
        fast_vec_normalize(&unit, &rel, 1.0f);
        pitch = -tilt;
        if (fast_vec_dot(&up, &unit) < 0.0f) {
            pitch = tilt;
        }
        hero_follow_camera->pitch = pitch;
        hero_follow_camera->dist = fast_vec_length(&rel);
        qcopy(&orbit->focus.side, &fwd);
        spring[0] = 3.1415927f;
        spring[1] = -1.5358897f;
        spring[2] = 5.0f;
        spring[3] = 0.0f;
        spring[4] = 0.0f;
        spring[5] = 0.0f;
        spring[6] = 0.005f;
        spring[9] = 0.2f;
        spring[7] = 0.005f;
        spring[10] = 0.2f;
        spring[8] = 0.005f;
        spring[11] = 0.2f;
    }
    if (orbit->mode == 1) {
        fast_vec_normalize(&side, &focus->side, hero_follow_camera->dist);
        build_look_at_matrix(&side, &side, &up, hero_follow_camera->yaw);
        fast_vec_cross(&fwd, &side, &up);
        fast_vec_normalize(&fwd, &fwd, 1.0f);
        build_look_at_matrix(&side, &side, &fwd, hero_follow_camera->pitch);
        fast_vec_add(&hero_follow_camera->pos, focus, &side);
        hero_follow_camera->yaw = rope_spring_turn(
            &spring[3], hero_follow_camera->yaw, spring[0], spring[6], spring[9], 0.0f);
        hero_follow_camera->pitch = rope_spring_turn(
            &spring[4], hero_follow_camera->pitch, spring[1], spring[7], spring[10], 0.0f);
        hero_follow_camera->dist = FUN_001ebd78(
            &spring[5], hero_follow_camera->dist, spring[2], spring[8], spring[11], 0.0f);
        if (hero.vert_ramp_launch == 0) {
            orbit->mode = 0;
            fast_vec_scale(orbit, orbit, -1.0f);
        }
    }
}
/* Camera follow: springs the position toward the focus, keeps clear of walls, eases distance and height, then rebuilds the basis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032B1F8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_32B1F8;

typedef struct {
    V_32B1F8 pos;
    char     pad10[0x10];
    V_32B1F8 axis;
    char     pad30[0x1C];
    float    dist;
} Focus;

typedef struct {
    float dist;
    char  pad4[0xC];
    float height;
} Spring;

typedef struct {
    V_32B1F8 vel;
    char     pad10[0x10];
    float    state[4];
    char     pad30[4];
    float    k34, k38, k3C;
    float    blend;
} Track;

typedef struct {
    float    vel[4];
    float    k10, k14, k18;
    short    mode;
    char     pad1E[0x62];
    Focus    focus;
    char     padD0[0x10];
    Spring   spring;
    char     padF4[0xC];
    Track    track;
    char     pad150[0x30];
    float    dist;
    float    spring_dist;
    float    spring_height;
    float    focus_dist;
    float    spring_dist2;
    float    spring_height2;
    char     pad198[8];
    V_32B1F8 aim;
    V_32B1F8 bias;
    float    bias_vel[4];
    V_32B1F8 next;
} Orbit_32B1F8;

typedef struct {
    V_32B1F8      fwd;
    V_32B1F8      up;
    V_32B1F8      side;
    V_32B1F8      pos;
    char          pad40[0x24];
    float         vel[3];
    Orbit_32B1F8 *orbit;
    char          pad74[0x10];
    short         preset;
} Camera_32B1F8;

extern int              coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
f32                     cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
extern char             D_0013E633_32B1F8[] __asm__("D_0013F5E0");
extern CameraPresetRow *D_L05_0015EF50_macro __asm__("D_L05_0015EF50") MACRO_ADDR;
extern float            D_L05_001621A8_gp __asm__("D_L05_001621A8") __attribute__((sda));
extern float spring_32B1F8(float, float, float *, float, float, float) __asm__("FUN_001ebd78");

extern float D_L05_00174278 NOT_SDA;
void update_orbit_camera_position(Camera_32B1F8 *camera) __asm__("FUN_L05_00329ce8");

void update_orbit_camera_position(Camera_32B1F8 *camera) {
    V_32B1F8      down;
    V_32B1F8      focus;
    V_32B1F8      rel;
    V_32B1F8      lateral;
    V_32B1F8      radial;
    V_32B1F8      base;
    V_32B1F8      goal;
    V_32B1F8      offset;
    V_32B1F8      probe;
    V_32B1F8      step;
    Orbit_32B1F8 *orbit = camera->orbit;
    char         *up_axis = D_0013E633_32B1F8;
    CameraPreset *preset;
    Focus        *focus_state;
    Track        *track;
    float         focus_distance;
    Spring       *spring;
    float        *vel;
    float         along;
    V_32B1F8     *pos;
    int           probe_step;
    int           orbiting = 0;

    preset = D_L05_0015EF50_macro[camera->preset].preset;
    track = &orbit->track;
    focus_distance = orbit->dist;
    focus_state = &orbit->focus;
    spring = &orbit->spring;
    fast_vec_normalize(&down, up_axis, -1.0f);
    qcopy(&focus, &focus_state->pos);
    if (orbit->mode == 0) {
        fast_vec_sub(&rel, pos = &camera->pos, &focus);
        probe_step = 0;
        along = fast_vec_dot(&rel, &focus_state->axis);
        fast_vec_normalize(&offset, &focus_state->axis, along);
        fast_vec_sub(&lateral, &rel, &offset);
        fast_vec_normalize(&radial, &lateral, spring->dist);
        fast_vec_add(&base, &focus, &radial);
        fast_vec_normalize(&offset, &focus_state->axis, spring->height);
        fast_vec_add(&goal, &base, &offset);
        fast_vec_add(&goal, &goal, &orbit->bias);
        camera->pos.x = spring_32B1F8(
            camera->pos.x, goal.x, &orbit->vel[0], orbit->k10, orbit->k14, orbit->k18);
        camera->pos.y = spring_32B1F8(
            camera->pos.y, goal.y, &orbit->vel[1], orbit->k10, orbit->k14, orbit->k18);
        camera->pos.z = spring_32B1F8(
            camera->pos.z, goal.z, &orbit->vel[2], orbit->k10, orbit->k14, orbit->k18);
        vel = camera->vel;
        probe.x = vel[0];
        probe.y = vel[1];
        probe.z = vel[2];
        *(int *)&probe.w = 0;
        fast_vec_sub(&step, pos, &probe);
        fast_vec_scale(&step, &step, 0.16666667f);
        while (probe_step < 6) {
            if (coll_sphere(0.5f, &probe, 18, 0) != 0) {
                spring->height = spring_32B1F8(spring->height,
                                               D_L05_00174278 - focus.z,
                                               &preset->height_vel,
                                               0.05f,
                                               0.2f,
                                               0.0f);
                break;
            }
            probe_step++;
            fast_vec_add(&probe, &probe, &step);
        }
        if (probe_step == 6) {
            orbit->bias.x =
                spring_32B1F8(orbit->bias.x, 0.0f, &orbit->bias_vel[0], 0.02f, 0.2f, 0.05f);
            orbit->bias.y =
                spring_32B1F8(orbit->bias.y, 0.0f, &orbit->bias_vel[1], 0.02f, 0.2f, 0.05f);
            orbit->bias.z =
                spring_32B1F8(orbit->bias.z, 0.0f, &orbit->bias_vel[2], 0.02f, 0.2f, 0.05f);
            spring->height = spring_32B1F8(
                spring->height, orbit->spring_height, &preset->height_vel, 0.005f, 0.2f, 0.0f);
        }
        if (((Hero *)(D_0013E633_32B1F8 - 0x290))->air_frames.s != 0 &&
            2.0f < ((Hero *)(D_0013E633_32B1F8 - 0x290))->ground_distance) {
            if (absolute_float(spring->dist - orbit->spring_dist2) < 0.015f)
                spring->dist = orbit->spring_dist2;
            else if (spring->dist < orbit->spring_dist2)
                spring->dist = spring->dist + 0.015f;
            else
                spring->dist = spring->dist - 0.015f;
            if (probe_step == 6) {
                if (absolute_float(spring->height - orbit->spring_height2) < 0.05f)
                    spring->height = orbit->spring_height2;
                else if (spring->height < orbit->spring_height2)
                    spring->height = spring->height + 0.05f;
                else
                    spring->height = spring->height - 0.05f;
            }
            if (absolute_float(focus_state->dist - orbit->focus_dist) < 0.02f)
                focus_state->dist = orbit->focus_dist;
            else if (focus_state->dist < orbit->focus_dist)
                focus_state->dist = focus_state->dist + 0.02f;
            else
                focus_state->dist = focus_state->dist - 0.02f;
        } else {
            if (absolute_float(spring->dist - orbit->spring_dist) < 0.03f)
                spring->dist = orbit->spring_dist;
            else if (spring->dist < orbit->spring_dist)
                spring->dist = spring->dist + 0.03f;
            else
                spring->dist = spring->dist - 0.03f;
            if (absolute_float(spring->height - orbit->spring_height) < 0.1f)
                spring->height = orbit->spring_height;
            else if (spring->height < orbit->spring_height)
                spring->height = spring->height + 0.1f;
            else
                spring->height = spring->height - 0.1f;
            if (absolute_float(focus_state->dist - focus_distance) < 0.04f)
                focus_state->dist = focus_distance;
            else if (focus_state->dist < focus_distance)
                focus_state->dist = focus_state->dist + 0.04f;
            else
                focus_state->dist = focus_state->dist - 0.04f;
        }
        orbiting = 1;
    } else {
        qcopy(&orbit->aim, *(char **)(up_axis + 0x1DF0) + 0xE0);
        if (absolute_float(spring->dist - orbit->spring_dist) < 0.015f)
            spring->dist = orbit->spring_dist;
        else if (spring->dist < orbit->spring_dist)
            spring->dist = spring->dist + 0.015f;
        else
            spring->dist = spring->dist - 0.015f;
        if (absolute_float(spring->height - orbit->spring_height) < 0.05f)
            spring->height = orbit->spring_height;
        else if (spring->height < orbit->spring_height)
            spring->height = spring->height + 0.05f;
        else
            spring->height = spring->height - 0.05f;
        if (absolute_float(focus_state->dist - focus_distance) < 0.02f)
            focus_state->dist = focus_distance;
        else if (focus_state->dist < focus_distance)
            focus_state->dist = focus_state->dist + 0.02f;
        else
            focus_state->dist = focus_state->dist - 0.02f;
    }
    update_hero_follow_camera_orbit(camera);
    qcopy(&focus_state->pos, &orbit->next);
    qcopy(&focus, &focus_state->pos);
    fast_vec_normalize(&offset, &focus_state->axis, focus_state->dist);
    fast_vec_add(&goal, &focus, &offset);
    fast_vec_sub(&rel, &goal, &camera->pos);
    fast_vec_normalize(&rel, &rel, 1.0f);
    along = fast_vec_dot(&camera->fwd, &rel);
    fast_vec_cross(&camera->up, &rel, &down);
    fast_vec_normalize(&camera->up, &camera->up, 1.0f);
    fast_vec_cross(&camera->side, &camera->up, &rel);
    if (orbiting == 0 || absolute_float(along) < 0.5f ||
        (vel = (float *)(D_0013E633_32B1F8 - 0x290), ((Hero *)vel)->trick_input != 0)) {
        track->blend =
            spring_32B1F8(track->blend, 0.0f, track->state, track->k34, track->k38, track->k3C);
    } else {
        float tilt;
        fast_vec_normalize(&probe, ((char *)((Hero *)vel)->moby) + 0xC0, 1.0f);
        tilt = fast_vec_dot(&probe, &camera->side);
        fast_vec_normalize(&step, &camera->side, tilt);
        fast_vec_sub(&probe, &probe, &step);
        tilt = fast_vec_dot(&probe, &camera->up);
        track->blend = spring_32B1F8(track->blend,
                                     tilt * D_L05_001621A8_gp,
                                     track->state,
                                     track->k34,
                                     track->k38,
                                     track->k3C);
    }
    if (0.00001f < absolute_float(track->blend)) {
        fast_vec_normalize(&probe, &camera->up, track->blend);
        fast_vec_add(&goal, &goal, &probe);
    }
    fast_vec_sub(&rel, &goal, &camera->pos);
    fast_vec_normalize(&camera->fwd, &rel, 1.0f);
    fast_vec_cross(&camera->up, &camera->fwd, &down);
    fast_vec_normalize(&camera->up, &camera->up, 1.0f);
    fast_vec_cross(&camera->side, &camera->up, &camera->fwd);
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032BBF0), where it is exact; names translated to the US level program. */

extern void reset_hero_follow_camera_from_preset(Camera *) __asm__("FUN_L05_003296e0");

extern char *D_L05_00167204 NOT_SDA;
void reset_camera_from_source_transform(char *out) __asm__("FUN_L05_0032a6e0");

void reset_camera_from_source_transform(char *out) {
    char *source_camera_transform = D_L05_00167204;
    qcopy(out + 0x30, source_camera_transform + 0x30);
    qcopy(out, source_camera_transform);
    qcopy(out + 0x10, source_camera_transform + 0x10);
    qcopy(out + 0x20, source_camera_transform + 0x20);
    extract_matrix_rotation_quaternion(out + 0x40, out);
    reset_hero_follow_camera_from_preset((Camera *)out);
}

f32 D_L14_0016229C = 4.0f;
f32 D_L14_001622A4 = 0.075f;
f32 D_L14_001622A8 = 0.08f;

float D_L14_001F6200[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

short                D_L14_001622A0 = -13107;
float                D_L16_00162198 = 2.0f;
float D_L16_00174278 NOT_SDA = 0;

float glow_sprite_local_vertices[4][4] __asm__("D_L05_00215C70") = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};
