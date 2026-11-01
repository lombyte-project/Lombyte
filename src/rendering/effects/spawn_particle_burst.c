/* Ported from rac1-decomp (src/game/effects.c, func_001EE3B0). */
#include "rnc/rendering/effects/render_queued_rotated_sprites.h"

extern int   get_effect_texture(int) __asm__("func_001F44B8");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern void  append_rotated_sprite_quad(float,
                                        float,
                                        float,
                                        float,
                                        float,
                                        int,
                                        int,
                                        int,
                                        int,
                                        int,
                                        int,
                                        int,
                                        float,
                                        float) __asm__("FUN_001f5ab0");
/*
 * The case order (1, then 0 vs negative, then 2 vs default) needs a real
 * switch so gcc's own binary-search lowering picks retail's compare order.
 *
 * The 40.0f constant is materialized once, before anything else (retail
 * loads it into $f24 before even the first call), because it is live
 * across every switch arm. The four offsets in case 1 live in a local
 * array, not scalars: retail spills them to fixed stack slots (0x0, 0x4,
 * 0x10, 0x14 with an 8-byte gap) instead of extra saved float registers.
 */
void spawn_particle_burst(QueuedRotatedSprite *sprite, float x, float y) __asm__("FUN_001ee008");

void spawn_particle_burst(QueuedRotatedSprite *sprite, float x, float y) {
    enum {
        ROTATED_SPRITE_ANGLE_FAN = 0,
        ROTATED_SPRITE_MIRRORED_QUAD = 1,
        ROTATED_SPRITE_CENTERED_SINGLE = 2
    };
    float forty = 40.0f;
    int   handle = get_effect_texture(sprite->texture_definition);
    int   mode = sprite->mode;
    float angle = sprite->angle;
    float k;
    int   i;

    switch (mode) {
    case ROTATED_SPRITE_ANGLE_FAN:
        for (i = 0; i < sprite->repetition_count; i++) {
            append_rotated_sprite_quad(x,
                                       y,
                                       forty * sprite->scale,
                                       forty * sprite->scale,
                                       angle,
                                       0x3F,
                                       0x3F,
                                       handle,
                                       0xFFFFF3,
                                       sprite->color,
                                       0,
                                       0,
                                       0.0f,
                                       0.0f);
            angle = fast_add_rotations(angle, sprite->angle_step);
        }
        break;
    case ROTATED_SPRITE_MIRRORED_QUAD: {
        float rotated_edge_offsets[6];

        rotated_edge_offsets[0] = fast_sin(angle) * forty * sprite->scale;
        rotated_edge_offsets[1] = fast_cos(angle) * forty * sprite->scale;
        rotated_edge_offsets[4] = fast_cos(angle) * forty * sprite->scale;
        rotated_edge_offsets[5] = fast_sin(angle) * -forty * sprite->scale;

        k = sprite->scale * forty;
        append_rotated_sprite_quad(
            x, y, k, k, angle, 0x3F, 0x3F, handle, 0xFFFFF3, sprite->color, 0, 0, 0.0f, 0.0f);

        k = sprite->scale;
        k *= forty;
        append_rotated_sprite_quad(x + rotated_edge_offsets[4],
                                   y + rotated_edge_offsets[5],
                                   k,
                                   k,
                                   angle,
                                   0x3F,
                                   0x3F,
                                   handle,
                                   0xFFFFF3,
                                   sprite->color,
                                   1,
                                   0,
                                   0.0f,
                                   0.0f);

        k = sprite->scale;
        k *= forty;
        append_rotated_sprite_quad(x - rotated_edge_offsets[0],
                                   y - rotated_edge_offsets[1],
                                   k,
                                   k,
                                   angle,
                                   0x3F,
                                   0x3F,
                                   handle,
                                   0xFFFFF3,
                                   sprite->color,
                                   0,
                                   1,
                                   0.0f,
                                   0.0f);

        k = sprite->scale;
        k *= forty;
        append_rotated_sprite_quad(x + rotated_edge_offsets[4] - rotated_edge_offsets[0],
                                   y + rotated_edge_offsets[5] - rotated_edge_offsets[1],
                                   k,
                                   k,
                                   angle,
                                   0x3F,
                                   0x3F,
                                   handle,
                                   0xFFFFF3,
                                   sprite->color,
                                   1,
                                   1,
                                   0.0f,
                                   0.0f);
        break;
    }
    case ROTATED_SPRITE_CENTERED_SINGLE:
        k = sprite->scale;
        k *= forty;
        append_rotated_sprite_quad(
            x, y, k, k, angle, 0x3F, 0x3F, handle, 0xFFFFF3, sprite->color, 0, 0, 0.5f, 0.5f);
        break;
    }
}

extern __typeof__(spawn_particle_burst) func_001EE008 __attribute__((alias("FUN_001ee008")));
