/* Level 01 moby class door_left. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"

extern s32 FUN_0022da68(s32, s32, void *);

extern f32 scale_time(f32) __asm__("FUN_001f96b0");
extern u8  D_L01_00167240[];

#include "qcopy.h"

extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");

typedef struct {
    Vec4 home;
    f32  t;
    f32  radius;
} HoverVars;

typedef struct {
    u8         pad0[0x10];
    Vec4       pos;
    u8         state;
    u8         pad21[0x27];
    f32        rot_z;
    u8         pad4C[0x2C];
    HoverVars *vars;
    u8         pad7C[0x2A];
    s16        oclass;
} HoverMoby;

void door_left_update(HoverMoby *m) __asm__("FUN_L01_002fed68");

void door_left_update(HoverMoby *door_moby) {
    enum {
        DOOR_INITIALIZE = 0,
        DOOR_CLOSED = 1,
        DOOR_OPENING = 2,
        DOOR_OPEN = 3,
        DOOR_CLOSING = 4
    };
    HoverVars *door_vars;
    Vec4      *player;
    Vec4       door_translation_offset;
    f32        k;

    door_vars = door_moby->vars;
    switch (door_moby->state) {
    case DOOR_INITIALIZE:
        qcopy(&door_vars->home, &door_moby->pos);
        door_vars->t = 0.0f;
        door_moby->state = 1;
        break;
    case DOOR_CLOSED:
        player = &hero.motion.pos;
        if (vector_distance_xy(door_vars, player) < door_vars->radius &&
            absolute_float(door_moby->pos.f[2] - player->f[2]) < 2.0f) {
            door_moby->state = 2;
            if (door_moby->oclass == 0x300) {
                FUN_0022da68(0, 0, door_moby);
            }
        }
        break;
    case DOOR_OPENING:
        door_vars->t += 1.0f / scale_time(20.0f);
        if (door_vars->t >= 1.0f) {
            door_vars->t = 1.0f;
            door_moby->state = 3;
        }
        break;
    case DOOR_OPEN:
        k = 1.1f;
        if (vector_distance_xy(door_vars, &hero.motion.pos) > door_vars->radius * k &&
            vector_distance_xy(door_vars, D_L01_00167240) > door_vars->radius * k) {
            door_moby->state = 4;
            if (door_moby->oclass == 0x300) {
                FUN_0022da68(0, 0, door_moby);
            }
        }
        break;
    case DOOR_CLOSING:
        player = &hero.motion.pos;
        if (vector_distance_xy(door_vars, player) < door_vars->radius &&
            absolute_float(door_moby->pos.f[2] - player->f[2]) < 2.0f) {
            door_moby->state = 2;
        } else {
            door_vars->t -= 1.0f / scale_time(20.0f);
            if (door_vars->t <= 0.0f) {
                door_vars->t = 0.0f;
                door_moby->state = 1;
            }
        }
        break;
    }
    door_translation_offset.f[0] =
        fast_cos(door_moby->rot_z) * door_vars->t * (door_moby->oclass == 0x300 ? 2.0f : -2.0f);
    door_translation_offset.f[1] =
        fast_sin(door_moby->rot_z) * door_vars->t * (door_moby->oclass == 0x300 ? 2.0f : -2.0f);
    door_translation_offset.f[2] = 0.0f;
    door_moby->pos.f[0] = door_vars->home.f[0] + door_translation_offset.f[0];
    door_moby->pos.f[1] = door_vars->home.f[1] + door_translation_offset.f[1];
}

extern float fast_cos(float);
extern float fast_sin(float);
