/* Shared moby class bot_door_lock: its update, glow and the targets it activates. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"

#include "rnc/rendering/geometry_quad.h"
#include "rnc/overlay/quad.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/mobys/moby.h"

extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

#include "rnc/overlay/level_camera.h"

extern OvlVec4 D_L06_001fe8d0[];
extern float   D_L06_001fe960[][2];
extern u32     D_L06_001fe9a8[];
extern int     D_L06_001fe9d0[][4];
void           draw_bot_door_lock_glow(struct Moby *moby) __asm__("FUN_L06_00307578");

void draw_bot_door_lock_glow(struct Moby *moby) {
    struct GeometryQuad quad;
    OvlVec4             result[4];
    float               rotation[4];
    float               matrix[12];
    OvlVec4             pos[4];
    OvlVec4             corners[4];
    int                 i;
    OvlVec4            *pos0, *pos1, *pos2, *pos3;
    OvlVec4            *result0, *result1, *result2, *result3;
    float              *data = ((float *)moby->pvars);
    rotation[0] = 0.0f;
    rotation[1] = 0.0f;
    rotation[2] = l06_camera.yaw;
    euler_rotation_basis(matrix, rotation);
    data[7] += 0.01f;
    if (1.0f < data[7])
        data[7] -= 1.0f;
    quad.texture_state = 1;
    quad.primitive = 0x4000000044ULL;
    quad.reserved.value = 0;
    quad.texture = get_effect_texture(0x18);
    result0 = &result[0];
    result1 = &result[1];
    result2 = &result[2];
    result3 = &result[3];
    pos0 = &pos[0];
    pos1 = &pos[1];
    pos2 = &pos[2];
    pos3 = &pos[3];
    for (i = 0; i < 4; i++) {
        transform_vector_by_basis(result0, &D_L06_001fe8d0[D_L06_001fe9d0[i][0]], matrix);
        transform_vector_by_basis(result1, &D_L06_001fe8d0[D_L06_001fe9d0[i][1]], matrix);
        transform_vector_by_basis(result2, &D_L06_001fe8d0[D_L06_001fe9d0[i][2]], matrix);
        transform_vector_by_basis(result3, &D_L06_001fe8d0[D_L06_001fe9d0[i][3]], matrix);
        quad.positions[0].f[0] = moby->pos.x + result[0].f[0] * data[9];
        quad.positions[0].f[1] = moby->pos.y + result[0].f[1] * data[9];
        quad.positions[0].f[2] = moby->pos.z + result[0].f[2];
        quad.positions[1].f[0] = moby->pos.x + result[1].f[0] * data[9];
        quad.positions[1].f[1] = moby->pos.y + result[1].f[1] * data[9];
        quad.positions[1].f[2] = moby->pos.z + result[1].f[2];
        quad.positions[2].f[0] = moby->pos.x + result[2].f[0] * data[9];
        quad.positions[2].f[1] = moby->pos.y + result[2].f[1] * data[9];
        quad.positions[2].f[2] = moby->pos.z + result[2].f[2];
        quad.positions[3].f[0] = moby->pos.x + result[3].f[0] * data[9];
        quad.positions[3].f[1] = moby->pos.y + result[3].f[1] * data[9];
        quad.positions[3].f[2] = moby->pos.z + result[3].f[2];
        quad.colors[0] = D_L06_001fe9a8[D_L06_001fe9d0[i][0]];
        quad.colors[1] = D_L06_001fe9a8[D_L06_001fe9d0[i][1]];
        quad.colors[2] = D_L06_001fe9a8[D_L06_001fe9d0[i][2]];
        quad.colors[3] = D_L06_001fe9a8[D_L06_001fe9d0[i][3]];
        quad.texture_coordinates[0][0] = D_L06_001fe960[D_L06_001fe9d0[i][0]][0];
        quad.texture_coordinates[0][1] = D_L06_001fe960[D_L06_001fe9d0[i][0]][1] + data[7];
        quad.texture_coordinates[1][0] = D_L06_001fe960[D_L06_001fe9d0[i][1]][0];
        quad.texture_coordinates[1][1] = D_L06_001fe960[D_L06_001fe9d0[i][1]][1] + data[7];
        quad.texture_coordinates[2][0] = D_L06_001fe960[D_L06_001fe9d0[i][2]][0];
        quad.texture_coordinates[2][1] = D_L06_001fe960[D_L06_001fe9d0[i][2]][1] + data[7];
        quad.texture_coordinates[3][0] = D_L06_001fe960[D_L06_001fe9d0[i][3]][0];
        quad.texture_coordinates[3][1] = D_L06_001fe960[D_L06_001fe9d0[i][3]][1] + data[7];
        draw_geometry_quad(&quad, 0, 1);
    }
    quad.texture = get_effect_texture(0x1b);
    data[8] += 0.01f;
    if (1.0f < data[8])
        data[8] = 0.0f;
    {
        /* Fakematch: local-alloc swaps the registers of 1.4f and 1.1f; pin them. */
        register float scale __asm__("$f5") = 1.4f;
        register float offset __asm__("$f4") = 1.1f;
        corners[0].q = 0;
        corners[0].f[0] = -data[8] * 1.2f * data[9];
        corners[0].f[1] = -data[8] * 1.2f * data[9];
        corners[0].f[2] = data[8] * scale + offset;
        corners[1].q = 0;
        corners[1].f[0] = data[8] * 1.2f * data[9];
        corners[1].f[1] = -data[8] * 1.2f * data[9];
        corners[1].f[2] = data[8] * scale + offset - 0.2f;
        corners[2].q = 0;
        corners[2].f[0] = -data[8] * 1.2f * data[9];
        corners[2].f[1] = data[8] * 1.2f * data[9];
        corners[2].f[2] = data[8] * scale + offset;
        corners[3].q = 0;
        corners[3].f[0] = data[8] * 1.2f * data[9];
        corners[3].f[1] = data[8] * 1.2f * data[9];
        corners[3].f[2] = data[8] * scale + offset - 0.2f;
    }
    pos[0].q = corners[0].q;
    pos[3].q = corners[3].q;
    pos[1].q = corners[1].q;
    pos[2].q = corners[2].q;
    rotation[2] = l06_camera.yaw;
    rotation[0] = 0.0f;
    rotation[1] = 0.0f;
    euler_rotation_basis(matrix, rotation);
    transform_vector_by_basis(result0, pos0, matrix);
    transform_vector_by_basis(result1, pos1, matrix);
    transform_vector_by_basis(result2, pos2, matrix);
    transform_vector_by_basis(result3, pos3, matrix);
    quad.positions[0].f[0] = moby->pos.x + result[0].f[0];
    quad.positions[0].f[1] = moby->pos.y + result[0].f[1];
    quad.positions[0].f[2] = moby->pos.z + result[0].f[2];
    quad.positions[1].f[0] = moby->pos.x + result[1].f[0];
    quad.positions[1].f[1] = moby->pos.y + result[1].f[1];
    quad.positions[1].f[2] = moby->pos.z + result[1].f[2];
    quad.positions[2].f[0] = moby->pos.x + result[2].f[0];
    quad.positions[2].f[1] = moby->pos.y + result[2].f[1];
    quad.positions[2].f[2] = moby->pos.z + result[2].f[2];
    quad.positions[3].f[0] = moby->pos.x + result[3].f[0];
    quad.positions[3].f[1] = moby->pos.y + result[3].f[1];
    quad.positions[3].f[2] = moby->pos.z + result[3].f[2];
    quad.colors[0] = ((u32)(int)((1.0f - data[8]) * 128.0f) << 24) | 0x808080;
    quad.colors[3] = quad.colors[2] = quad.colors[1] = quad.colors[0];
    quad.texture_coordinates[0][0] = 0.0f;
    quad.texture_coordinates[0][1] = 0.0f;
    quad.texture_coordinates[1][0] = 1.0f;
    quad.texture_coordinates[1][1] = 0.0f;
    quad.texture_coordinates[2][0] = 0.0f;
    quad.texture_coordinates[2][1] = 1.0f;
    quad.texture_coordinates[3][0] = 1.0f;
    quad.texture_coordinates[3][1] = 1.0f;
    draw_geometry_quad(&quad, 0, 1);
}

/* state transitions of a moby depending on its id at 0xA6 */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00309088), where it is exact; names translated to the US level program. */

extern int  trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

void activate_bot_door_lock_target(unsigned char *lock_moby,
                                   unsigned char *target_moby) __asm__("FUN_L06_00307c48");

void activate_bot_door_lock_target(unsigned char *lock_moby, unsigned char *target_moby) {
    switch (MOBY(target_moby)->oclass) {
    case 0x3F7:
    case 0x502:
        if (target_moby[0x20] == 2) {
            target_moby[0x20] = 3;
            allocate_voice_for_target_entry(0, 0, (int)target_moby);
        }
        break;
    case 0x404:
        if (target_moby[0x20] == 1)
            target_moby[0x20] = 2;
        break;
    case 0x542:
        target_moby[0x20] = 2;
        trooper_play_sound(0, 0, (int)target_moby, 0x434);
        target_moby[0x30] = 0xFF;
        jet_clear_persistent(lock_moby[0xB0]);
        break;
    }
}

extern void draw_bot_door_lock_glow(struct Moby *moby) __asm__("FUN_L06_00307578");

extern float bot_door_lock_number_bob_amplitude_gp __asm__("D_L06_001622DC") __attribute__((sda));
extern float bot_door_lock_number_bob_period_gp __asm__("D_L06_001622E0") __attribute__((sda));
extern float bot_door_lock_color_pulse_period_gp __asm__("D_L06_001622E4") __attribute__((sda));
extern float bot_door_lock_number_color_pulse_amplitude_gp __asm__("D_L06_001622E8")
    __attribute__((sda));
extern float bot_door_lock_max_effect_horizontal_scale_gp __asm__("D_L06_001622EC")
    __attribute__((sda));
extern unsigned char D_001413F4 NOT_SDA;
extern unsigned char            D_0015EDB4;
extern unsigned char            D_0014C050[];
extern unsigned int             fast_tween_color(float, int, int) __asm__("FUN_001fa6e0");
extern float                    random_angle_radians(void) __asm__("FUN_00213308");
extern void
draw_number(void *, void *, int, unsigned int, int, int, int, float) __asm__("FUN_L01_00285768");

extern int level_mobys_q __asm__("D_L06_0015FFD8") __attribute__((sda));
void       bot_door_lock_update(char *moby) __asm__("FUN_L06_00307d30");

void bot_door_lock_update(char *moby) {
    enum {
        BOT_DOOR_LOCK_STATE_INITIALIZE = 0,
        BOT_DOOR_LOCK_STATE_ACTIVE = 1,
        BOT_DOOR_LOCK_STATE_SHRINK_EFFECT = 2
    };
    float       *data = ((float *)MOBY(moby)->pvars);
    int         *idata = (int *)data;
    float        number_anchor_position[4];
    float        offset[3];
    float        result[4];
    float        angle;
    int          value;
    unsigned int color;
    unsigned int color_channels;
    int          i;

    if (*(int *)data == -1)
        return;
    data[10] = fast_add_rotations(
        data[10], (360.0f / bot_door_lock_color_pulse_period_gp) * 0.017453292f * frame_time);
    MOBY(moby)->glow_color =
        fast_tween_color((fast_cos(data[10]) + 1.0f) * 0.5f, 0x80464646, 0x80828282);
    switch ((unsigned char)moby[0x20]) {
    case BOT_DOOR_LOCK_STATE_INITIALIZE:
        idata[5] = idata[4];
        data[6] = random_angle_radians();
        data[10] = random_angle_radians();
        moby[0x20] = 1;
        data[9] = bot_door_lock_max_effect_horizontal_scale_gp;
        return;
    case BOT_DOOR_LOCK_STATE_ACTIVE:
        *(OvlQuad *)number_anchor_position = *(OvlQuad *)&MOBY(moby)->pos;
        number_anchor_position[2] += 2.75f;
        if (fast_vec_dist(D_L06_00167540, moby + 0x10) < 32.0f) {
            data[9] += frame_time + frame_time;
            if (bot_door_lock_max_effect_horizontal_scale_gp < data[9])
                data[9] = bot_door_lock_max_effect_horizontal_scale_gp;
        } else if (36.0f < fast_vec_dist(D_L06_00167540, moby + 0x10)) {
            data[9] -= frame_time + frame_time;
            if (data[9] < 0.0f)
                data[9] = 0.0f;
        }
        if (0.0f < data[9]) {
            add_draw_callback((int)draw_bot_door_lock_glow, (int)moby);
            data[6] = fast_add_rotations(
                data[6], (360.0f / bot_door_lock_number_bob_period_gp) * 0.017453292f * frame_time);
            number_anchor_position[2] += bot_door_lock_number_bob_amplitude_gp * fast_cos(data[6]);
            value = truncate_float_to_s32(bot_door_lock_number_color_pulse_amplitude_gp *
                                          fast_cos(data[10]));
            color_channels = (value / 2 + 120) * 0x100 | 0x20000000U;
            color = (value / 3 + 13) * 0x10000 | color_channels | (value + 150U);
            if (D_001413F4 == 1) {
                if (idata[5] < 10) {
                    draw_number(
                        number_anchor_position, moby, idata[5] + 2, color, 0x7f, 4, 0xff, 0.5f);
                } else {
                    if (D_0015EDB4) {
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[0] = fast_cos(angle) * 0.15f;
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[1] = fast_sin(angle) * 0.15f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, number_anchor_position);
                        draw_number(offset, moby, idata[5] / 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[0] = fast_cos(angle) * -0.4f;
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[1] = fast_sin(angle) * -0.4f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, number_anchor_position);
                        draw_number(offset, moby, idata[5] % 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                    } else {
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[0] = fast_cos(angle) * -0.4f;
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[1] = fast_sin(angle) * -0.4f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, number_anchor_position);
                        draw_number(offset, moby, idata[5] / 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[0] = fast_cos(angle) * 0.15f;
                        angle = fast_add_rotations(MOBY(moby)->rot.z, 1.5707964f);
                        offset[1] = fast_sin(angle) * 0.15f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, number_anchor_position);
                        draw_number(offset, moby, idata[5] % 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                    }
                }
            }
        }
        if (idata[5] < 1 ||
            D_0014C050[(unsigned char)moby[0xb0] + current_level_index * 16] == 0xff) {
            for (i = 0; i < 4; i++) {
                if (idata[i] != -1)
                    activate_bot_door_lock_target(
                        (unsigned char *)moby, (unsigned char *)(level_mobys_q + idata[i] * 0x100));
            }
            moby[0x20] = 2;
        }
        break;
    case BOT_DOOR_LOCK_STATE_SHRINK_EFFECT:
        data[9] -= frame_time + frame_time;
        if (data[9] < 0.0f)
            data[9] = 0.0f;
        if (0.0f < data[9])
            add_draw_callback((int)draw_bot_door_lock_glow, (int)moby);
        return;
    default:
        return;
    }
}

s32 D_L06_001FE9D0[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001DA210[1] __attribute__((section(".data"))) = {0};

float D_L10_001DA1A0 NOT_SDA = 0;
u32 D_L10_001DA1E8   NOT_SDA = 0x10808080;
float                D_L10_00161F24 = 0.025f;
float                D_L10_00161F28 = 2.5f;
float                D_L10_00161F2C = 2.0f;
float                D_L10_00161F30 = 36.0f;
float                D_L10_00161F34 = 0.75f;
