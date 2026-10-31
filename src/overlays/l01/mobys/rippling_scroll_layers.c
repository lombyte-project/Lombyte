/* Level 01 moby class rippling_scroll_layers (0x2f9): lowers and ripples its four scroll layers once, then pulses their glow and queues their draw callback. */
#include "types.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/cuboid.h"
#include "sda.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
#include "rnc/overlay/scroll_layer.h"

extern s32 level_frame_count __asm__("D_L01_0015F5CC") MACRO_ADDR;

s32        rippling_scroll_layers_vertices_rippled __asm__("D_L01_00161CE0") = 0;
extern s32 level_frame_count __asm__("D_L01_0015F5CC");
void       update_and_draw_scrolling_overlay_layers(s32, void *) __asm__("FUN_L01_002b96e0");

void draw_rippling_scroll_layers(void) __asm__("FUN_L01_002feb28");

/* Draw callback: draws the class's four scroll layers. */
void draw_rippling_scroll_layers(void) {
    hud_counter_update();
    update_and_draw_scrolling_overlay_layers(0x4, rippling_scroll_layers);
}

/* On first use, lowers and ripples the layers' vertices; every frame, pulses their glow and queues draw_rippling_scroll_layers. */
void rippling_scroll_layers_update(void *callback_argument) __asm__("FUN_L01_002feb58");

/* Moby class 761 */
void rippling_scroll_layers_update(void *callback_argument) {
    s32    i;
    s32    glow_layer_index;
    s32    j;
    s32    k;
    s32    ripple_layer_index;
    s32    vertex_pair_start_index;
    Vec4f *lowered_vertex;
    Vec4f *ripple_vertex_pair;
    f32    glow_factor;

    if (rippling_scroll_layers_vertices_rippled == 0) {
        rippling_scroll_layers_vertices_rippled = 1;
        for (i = 0; i < 4; i++) {
            lowered_vertex = (rippling_scroll_layers + i)->verts;
            for (j = 0; j < (rippling_scroll_layers + i)->count; j++) {
                lowered_vertex->w = lowered_vertex->z = lowered_vertex->z - 0.45f;
                lowered_vertex++;
            }
        }
        k = 1;
        for (ripple_layer_index = 0; ripple_layer_index < 4; ripple_layer_index++) {
            k--;
            ripple_vertex_pair = (rippling_scroll_layers + ripple_layer_index)->verts;
            for (vertex_pair_start_index = 0;
                 vertex_pair_start_index < (rippling_scroll_layers + ripple_layer_index)->count;
                 vertex_pair_start_index += 2) {
                if (k & 1) {
                    ripple_vertex_pair[0].z += 0.05f;
                    ripple_vertex_pair[0].w -= 0.05f;
                    ripple_vertex_pair[1].z -= 0.05f;
                    ripple_vertex_pair[1].w += 0.05f;
                } else {
                    ripple_vertex_pair[0].z -= 0.05f;
                    ripple_vertex_pair[0].w += 0.05f;
                    ripple_vertex_pair[1].z += 0.05f;
                    ripple_vertex_pair[1].w -= 0.05f;
                }
                ripple_vertex_pair += 2;
                k++;
            }
        }
    }
    glow_factor =
        fast_sin((convert_integer_to_float(level_frame_count & 0x3F) - 32.0f) * 0.09817477f);
    glow_factor = glow_factor * 0.5f + 0.5f;
    for (glow_layer_index = 3; glow_layer_index >= 0; glow_layer_index--) {
        rippling_scroll_layers[glow_layer_index].glow = glow_factor;
    }
    add_draw_callback(draw_rippling_scroll_layers, callback_argument);
}

Vec4f rippling_scroll_layer_0_bounds __asm__("D_L01_00161CF0") = {
    204.202f, 131.949f, 64.022f, 33.691f};

Vec4f rippling_scroll_layer_1_bounds __asm__("D_L01_00161D00") = {
    262.251f, 129.584f, 70.353f, 28.613f};

Vec4f rippling_scroll_layer_2_bounds __asm__("D_L01_00161D10") = {
    300.714f, 148.496f, 76.373f, 25.605f};

Vec4f rippling_scroll_layer_3_bounds __asm__("D_L01_00161D20") = {
    289.864f, 187.932f, 86.91f, 27.57f};

Vec4f rippling_scroll_layer_0_verts[48] __asm__("D_L01_001FAB40") = {
    {171.372f, 138.258f,      60.2f, 0.0f},
    { 170.82f,  130.06f,      60.2f, 0.0f},
    {175.603f, 137.459f,    60.212f, 0.0f},
    {174.987f, 128.788f,    60.212f, 0.0f},
    {178.553f, 137.585f,    60.243f, 0.0f},
    {177.551f, 127.618f,    60.243f, 0.0f},
    {181.018f, 136.168f,    60.255f, 0.0f},
    {180.441f, 125.129f,    60.255f, 0.0f},
    {184.037f, 135.496f,    60.363f, 0.0f},
    {183.352f, 122.468f,    60.563f, 0.0f},
    {187.934f, 134.774f,     60.65f, 0.0f},
    {186.011f, 118.462f,     61.05f, 0.0f},
    {190.267f, 133.853f,     60.65f, 0.0f},
    {188.695f, 115.906f,     61.75f, 0.0f},
    {192.908f, 132.089f,     61.65f, 0.0f},
    {191.534f, 114.077f,     62.45f, 0.0f},
    { 196.11f, 131.417f,     62.45f, 0.0f},
    { 194.88f, 113.614f,     63.05f, 0.0f},
    { 199.16f, 130.744f,     63.05f, 0.0f},
    {197.997f, 113.168f,     63.65f, 0.0f},
    {202.036f, 130.341f,     63.75f, 0.0f},
    { 200.85f, 113.596f,     64.15f, 0.0f},
    {205.045f, 129.668f,     64.25f, 0.0f},
    { 204.21f, 114.087f,     64.75f, 0.0f},
    {207.732f, 130.608f,     64.75f, 0.0f},
    { 208.22f, 116.046f,     65.35f, 0.0f},
    {210.748f, 131.477f,     65.25f, 0.0f},
    {211.867f, 114.408f, 66.049995f, 0.0f},
    {213.647f, 131.796f,     65.85f, 0.0f},
    {215.921f, 113.232f,     66.85f, 0.0f},
    {216.885f,  131.71f, 66.549995f, 0.0f},
    {220.536f, 111.442f,     67.45f, 0.0f},
    {219.239f, 131.091f,     66.85f, 0.0f},
    {223.554f, 113.213f, 67.649994f, 0.0f},
    {221.957f, 130.144f,    67.417f, 0.0f},
    {225.803f, 116.275f,    67.917f, 0.0f},
    {  224.8f, 129.408f,    68.035f, 0.0f},
    {227.283f, 118.191f,    68.035f, 0.0f},
    {226.946f, 128.987f,    68.043f, 0.0f},
    {229.113f, 120.565f,    68.043f, 0.0f},
    {228.578f, 128.961f,    68.043f, 0.0f},
    {231.175f,  122.27f,    68.043f, 0.0f},
    { 230.22f, 129.364f,    68.041f, 0.0f},
    {233.022f, 123.384f,    68.041f, 0.0f},
    {231.913f, 129.506f,    68.037f, 0.0f},
    {235.162f, 124.533f,    68.037f, 0.0f},
    {233.742f, 130.659f,    68.042f, 0.0f},
    {237.069f, 125.844f,    68.042f, 0.0f},
};

f32 rippling_scroll_layer_0_uvs[48][2] __asm__("D_L01_001FAE40") = {
    {0.0f,  -8.0f},
    {1.0f,  -8.0f},
    {0.0f, -7.75f},
    {1.0f, -7.75f},
    {0.0f,  -7.5f},
    {1.0f,  -7.5f},
    {0.0f, -7.25f},
    {1.0f, -7.25f},
    {0.0f,  -7.0f},
    {1.0f,  -7.0f},
    {0.0f, -6.75f},
    {1.0f, -6.75f},
    {0.0f,  -6.5f},
    {1.0f,  -6.5f},
    {0.0f, -6.25f},
    {1.0f, -6.25f},
    {0.0f,  -6.0f},
    {1.0f,  -6.0f},
    {0.0f, -5.75f},
    {1.0f, -5.75f},
    {0.0f,  -5.5f},
    {1.0f,  -5.5f},
    {0.0f, -5.25f},
    {1.0f, -5.25f},
    {0.0f,  -5.0f},
    {1.0f,  -5.0f},
    {0.0f, -4.75f},
    {1.0f, -4.75f},
    {0.0f,  -4.5f},
    {1.0f,  -4.5f},
    {0.0f, -4.25f},
    {1.0f, -4.25f},
    {0.0f,  -4.0f},
    {1.0f,  -4.0f},
    {0.0f, -3.75f},
    {1.0f, -3.75f},
    {0.0f,  -3.5f},
    {1.0f,  -3.5f},
    {0.0f, -3.25f},
    {1.0f, -3.25f},
    {0.0f,  -3.0f},
    {1.0f,  -3.0f},
    {0.0f, -2.75f},
    {1.0f, -2.75f},
    {0.0f,  -2.5f},
    {1.0f,  -2.5f},
    {0.0f, -2.25f},
    {1.0f, -2.25f},
};

Vec4f rippling_scroll_layer_1_verts[48] __asm__("D_L01_001FAFC0") = {
    {233.742f, 130.659f, 68.042f, 0.0f},
    {237.069f, 125.844f, 68.042f, 0.0f},
    {236.025f, 131.934f, 68.217f, 0.0f},
    {239.213f, 127.115f, 68.217f, 0.0f},
    { 237.94f, 133.121f, 68.424f, 0.0f},
    {240.394f, 127.957f, 68.424f, 0.0f},
    {241.272f, 133.699f, 68.596f, 0.0f},
    {242.863f, 128.403f, 68.596f, 0.0f},
    { 243.62f, 134.003f, 68.769f, 0.0f},
    {245.121f, 128.706f, 68.769f, 0.0f},
    {245.914f,   134.3f, 68.941f, 0.0f},
    {247.414f, 129.014f, 68.941f, 0.0f},
    {248.208f,   134.4f, 69.114f, 0.0f},
    {249.708f, 129.125f, 69.114f, 0.0f},
    {250.501f, 134.697f, 69.286f, 0.0f},
    {252.002f, 129.433f, 69.286f, 0.0f},
    {252.795f, 134.994f, 69.459f, 0.0f},
    {254.295f, 129.741f, 69.459f, 0.0f},
    {255.089f, 135.291f, 69.631f, 0.0f},
    {256.589f, 130.049f, 69.631f, 0.0f},
    {257.382f, 135.588f, 69.804f, 0.0f},
    {258.883f, 130.358f, 69.804f, 0.0f},
    {259.748f, 135.896f, 69.977f, 0.0f},
    {261.129f, 130.658f, 69.977f, 0.0f},
    {  262.4f, 136.111f, 70.149f, 0.0f},
    {263.468f, 130.849f, 70.149f, 0.0f},
    {264.502f, 136.116f, 70.271f, 0.0f},
    { 265.28f, 130.843f, 70.271f, 0.0f},
    {266.681f, 136.023f,  70.42f, 0.0f},
    { 267.28f, 130.751f,  70.42f, 0.0f},
    {269.554f, 135.821f, 70.581f, 0.0f},
    {270.063f, 130.532f, 70.581f, 0.0f},
    {272.366f, 135.597f, 70.826f, 0.0f},
    { 272.73f, 130.272f, 70.826f, 0.0f},
    {275.238f, 135.169f, 71.168f, 0.0f},
    {275.271f, 129.837f, 71.168f, 0.0f},
    {278.035f, 134.509f, 71.519f, 0.0f},
    {277.735f, 129.202f, 71.519f, 0.0f},
    {280.695f, 133.706f, 71.853f, 0.0f},
    {280.198f, 128.425f, 71.853f, 0.0f},
    {283.238f, 132.867f, 72.147f, 0.0f},
    {282.699f, 127.587f, 72.147f, 0.0f},
    {285.715f, 132.075f, 72.391f, 0.0f},
    {285.254f, 126.768f, 72.391f, 0.0f},
    {288.218f, 131.371f, 72.583f, 0.0f},
    {287.839f, 126.026f, 72.583f, 0.0f},
    {290.773f, 130.649f, 72.755f, 0.0f},
    {290.433f, 125.348f, 72.755f, 0.0f},
};

f32 rippling_scroll_layer_1_uvs[48][2] __asm__("D_L01_001FB2C0") = {
    {0.0f, -7.25f},
    {1.0f, -7.25f},
    {0.0f,  -7.0f},
    {1.0f,  -7.0f},
    {0.0f, -6.75f},
    {1.0f, -6.75f},
    {0.0f,  -6.5f},
    {1.0f,  -6.5f},
    {0.0f, -6.25f},
    {1.0f, -6.25f},
    {0.0f,  -6.0f},
    {1.0f,  -6.0f},
    {0.0f, -5.75f},
    {1.0f, -5.75f},
    {0.0f,  -5.5f},
    {1.0f,  -5.5f},
    {0.0f, -5.25f},
    {1.0f, -5.25f},
    {0.0f,  -5.0f},
    {1.0f,  -5.0f},
    {0.0f, -4.75f},
    {1.0f, -4.75f},
    {0.0f,  -4.5f},
    {1.0f,  -4.5f},
    {0.0f, -4.25f},
    {1.0f, -4.25f},
    {0.0f,  -4.0f},
    {1.0f,  -4.0f},
    {0.0f, -3.75f},
    {1.0f, -3.75f},
    {0.0f,  -3.5f},
    {1.0f,  -3.5f},
    {0.0f, -3.25f},
    {1.0f, -3.25f},
    {0.0f,  -3.0f},
    {1.0f,  -3.0f},
    {0.0f, -2.75f},
    {1.0f, -2.75f},
    {0.0f,  -2.5f},
    {1.0f,  -2.5f},
    {0.0f, -2.25f},
    {1.0f, -2.25f},
    {0.0f,  -2.0f},
    {1.0f,  -2.0f},
    {0.0f, -1.75f},
    {1.0f, -1.75f},
    {0.0f,  -1.5f},
    {1.0f,  -1.5f},
};

Vec4f rippling_scroll_layer_2_verts[48] __asm__("D_L01_001FB440") = {
    {290.773f, 130.649f, 72.755f, 0.0f},
    {290.433f, 125.348f, 72.755f, 0.0f},
    {293.212f, 130.033f, 72.936f, 0.0f},
    {293.138f, 124.591f, 72.936f, 0.0f},
    {295.373f, 129.683f, 73.113f, 0.0f},
    {296.073f, 124.158f, 73.113f, 0.0f},
    {297.324f, 129.831f, 73.265f, 0.0f},
    {299.157f, 124.393f, 73.265f, 0.0f},
    {299.083f, 130.498f, 73.383f, 0.0f},
    {302.139f, 125.441f, 73.383f, 0.0f},
    {300.675f, 131.678f, 73.471f, 0.0f},
    {304.746f, 127.278f, 73.471f, 0.0f},
    {302.149f, 133.239f, 73.562f, 0.0f},
    {306.833f, 129.594f, 73.562f, 0.0f},
    { 303.42f, 135.064f, 73.873f, 0.0f},
    {308.334f, 131.811f, 73.873f, 0.0f},
    {304.829f, 137.076f, 74.184f, 0.0f},
    {309.744f, 133.898f, 74.184f, 0.0f},
    {306.238f, 139.088f, 74.496f, 0.0f},
    {311.153f, 135.985f, 74.496f, 0.0f},
    {307.647f, 141.101f, 74.808f, 0.0f},
    {312.562f, 138.073f, 74.808f, 0.0f},
    {309.056f, 143.113f, 75.119f, 0.0f},
    {313.971f,  140.16f, 75.119f, 0.0f},
    {310.465f, 145.126f, 75.431f, 0.0f},
    { 315.38f, 142.247f, 75.431f, 0.0f},
    {311.875f, 147.138f, 75.743f, 0.0f},
    {316.789f, 144.334f, 75.743f, 0.0f},
    {313.284f, 149.151f, 76.054f, 0.0f},
    {318.198f, 146.422f, 76.054f, 0.0f},
    {314.678f, 151.189f, 76.366f, 0.0f},
    {319.617f, 148.491f, 76.366f, 0.0f},
    {315.924f, 153.038f, 76.548f, 0.0f},
    {321.135f, 150.691f, 76.548f, 0.0f},
    {317.187f, 155.638f, 76.758f, 0.0f},
    {323.112f,  154.99f, 76.758f, 0.0f},
    {317.091f, 158.203f, 77.305f, 0.0f},
    {322.783f, 160.058f, 77.305f, 0.0f},
    {315.756f, 160.491f, 78.318f, 0.0f},
    {320.706f, 163.774f, 78.318f, 0.0f},
    { 314.85f, 161.583f, 78.914f, 0.0f},
    {319.135f, 165.715f, 78.914f, 0.0f},
    {312.491f,  163.31f, 79.496f, 0.0f},
    {316.154f, 167.978f, 79.496f, 0.0f},
    {309.532f, 165.205f, 79.941f, 0.0f},
    {313.233f, 169.914f, 79.941f, 0.0f},
    {306.765f, 167.302f, 80.493f, 0.0f},
    {310.996f, 171.682f, 80.493f, 0.0f},
};

f32 rippling_scroll_layer_2_uvs[48][2] __asm__("D_L01_001FB740") = {
    {0.0f,  -7.5f},
    {1.0f,  -7.5f},
    {0.0f, -7.25f},
    {1.0f, -7.25f},
    {0.0f,  -7.0f},
    {1.0f,  -7.0f},
    {0.0f, -6.75f},
    {1.0f, -6.75f},
    {0.0f,  -6.5f},
    {1.0f,  -6.5f},
    {0.0f, -6.25f},
    {1.0f, -6.25f},
    {0.0f,  -6.0f},
    {1.0f,  -6.0f},
    {0.0f, -5.75f},
    {1.0f, -5.75f},
    {0.0f,  -5.5f},
    {1.0f,  -5.5f},
    {0.0f, -5.25f},
    {1.0f, -5.25f},
    {0.0f,  -5.0f},
    {1.0f,  -5.0f},
    {0.0f, -4.75f},
    {1.0f, -4.75f},
    {0.0f,  -4.5f},
    {1.0f,  -4.5f},
    {0.0f, -4.25f},
    {1.0f, -4.25f},
    {0.0f,  -4.0f},
    {1.0f,  -4.0f},
    {0.0f, -3.75f},
    {1.0f, -3.75f},
    {0.0f,  -3.5f},
    {1.0f,  -3.5f},
    {0.0f, -3.25f},
    {1.0f, -3.25f},
    {0.0f,  -3.0f},
    {1.0f,  -3.0f},
    {0.0f, -2.75f},
    {1.0f, -2.75f},
    {0.0f,  -2.5f},
    {1.0f,  -2.5f},
    {0.0f, -2.25f},
    {1.0f, -2.25f},
    {0.0f,  -2.0f},
    {1.0f,  -2.0f},
    {0.0f, -1.75f},
    {1.0f, -1.75f},
};

Vec4f rippling_scroll_layer_3_verts[40] __asm__("D_L01_001FB8C0") = {
    {306.765f, 167.302f, 80.493f, 0.0f},
    {310.996f, 171.682f, 80.493f, 0.0f},
    {304.354f,  169.95f, 81.046f, 0.0f},
    {309.343f, 173.576f, 81.046f, 0.0f},
    {302.852f, 172.921f, 81.932f, 0.0f},
    {307.952f, 176.517f, 81.932f, 0.0f},
    {301.356f, 174.453f, 83.002f, 0.0f},
    {306.004f, 179.037f, 83.002f, 0.0f},
    {299.127f, 175.829f, 84.574f, 0.0f},
    {303.639f, 180.892f, 84.574f, 0.0f},
    {296.959f, 177.496f, 86.531f, 0.0f},
    {301.442f, 182.452f, 86.531f, 0.0f},
    {294.743f,  179.21f, 88.355f, 0.0f},
    {299.049f, 184.213f, 88.355f, 0.0f},
    { 292.51f, 180.888f, 89.883f, 0.0f},
    {296.593f, 185.994f, 89.883f, 0.0f},
    {290.335f, 182.454f, 91.013f, 0.0f},
    {294.184f, 187.701f, 91.013f, 0.0f},
    {287.792f, 184.176f, 91.811f, 0.0f},
    {291.403f, 189.661f, 91.811f, 0.0f},
    {285.367f, 185.574f, 92.489f, 0.0f},
    {288.742f,  191.28f, 92.489f, 0.0f},
    { 282.65f,  186.96f, 93.294f, 0.0f},
    {285.856f, 192.792f, 93.294f, 0.0f},
    {279.927f, 188.305f, 93.899f, 0.0f},
    { 283.12f, 194.193f, 93.899f, 0.0f},
    {277.075f, 189.844f, 94.163f, 0.0f},
    {280.518f, 195.732f, 94.163f, 0.0f},
    {275.012f, 191.306f, 94.163f, 0.0f},
    {278.621f, 197.138f, 94.163f, 0.0f},
    {273.061f, 192.695f, 94.163f, 0.0f},
    {276.671f, 198.579f, 94.163f, 0.0f},
    {271.072f, 194.112f, 94.163f, 0.0f},
    {274.681f, 200.049f, 94.163f, 0.0f},
    {269.126f, 195.498f, 94.163f, 0.0f},
    {272.735f, 201.486f, 94.163f, 0.0f},
    { 267.18f, 196.872f, 94.163f, 0.0f},
    {270.788f, 202.932f, 94.163f, 0.0f},
    {265.296f, 198.283f, 94.089f, 0.0f},
    {268.839f, 204.323f, 94.089f, 0.0f},
};

f32 rippling_scroll_layer_3_uvs[40][2] __asm__("D_L01_001FBB40") = {
    {0.0f, -7.75f},
    {1.0f, -7.75f},
    {0.0f,  -7.5f},
    {1.0f,  -7.5f},
    {0.0f, -7.25f},
    {1.0f, -7.25f},
    {0.0f,  -7.0f},
    {1.0f,  -7.0f},
    {0.0f, -6.75f},
    {1.0f, -6.75f},
    {0.0f,  -6.5f},
    {1.0f,  -6.5f},
    {0.0f, -6.25f},
    {1.0f, -6.25f},
    {0.0f,  -6.0f},
    {1.0f,  -6.0f},
    {0.0f, -5.75f},
    {1.0f, -5.75f},
    {0.0f,  -5.5f},
    {1.0f,  -5.5f},
    {0.0f, -5.25f},
    {1.0f, -5.25f},
    {0.0f,  -5.0f},
    {1.0f,  -5.0f},
    {0.0f, -4.75f},
    {1.0f, -4.75f},
    {0.0f,  -4.5f},
    {1.0f,  -4.5f},
    {0.0f, -4.25f},
    {1.0f, -4.25f},
    {0.0f,  -4.0f},
    {1.0f,  -4.0f},
    {0.0f, -3.75f},
    {1.0f, -3.75f},
    {0.0f,  -3.5f},
    {1.0f,  -3.5f},
    {0.0f, -3.25f},
    {1.0f, -3.25f},
    {0.0f,  -3.0f},
    {1.0f,  -3.0f},
};

ScrollLayer rippling_scroll_layers[4] = {
    {.verts = rippling_scroll_layer_0_verts,
     .uvs = rippling_scroll_layer_0_uvs,
     .bounds = &rippling_scroll_layer_0_bounds,
     .count = 48,
     .rate0 = 0.007f,
     .rate1 = 0.004f,
     .unk18 = 0.025f,
     .frame_step = 1,
     .du0 = -0.1f,
     .dv0 = 0.9f,
     .du1 = 0.1f,
     .dv1 = 0.9f,
     .color = 0x808080,
     .tex0 = 0x2c,
     .tex1 = 0x2c,
     .alpha0 = 0x28,
     .alpha1 = 0x28},
    {.verts = rippling_scroll_layer_1_verts,
     .uvs = rippling_scroll_layer_1_uvs,
     .bounds = &rippling_scroll_layer_1_bounds,
     .count = 48,
     .rate0 = 0.007f,
     .rate1 = 0.004f,
     .unk18 = 0.025f,
     .frame_step = 1,
     .du0 = -0.1f,
     .dv0 = 0.9f,
     .du1 = 0.1f,
     .dv1 = 0.9f,
     .color = 0x808080,
     .tex0 = 0x2c,
     .tex1 = 0x2c,
     .alpha0 = 0x28,
     .alpha1 = 0x28},
    {.verts = rippling_scroll_layer_2_verts,
     .uvs = rippling_scroll_layer_2_uvs,
     .bounds = &rippling_scroll_layer_2_bounds,
     .count = 48,
     .rate0 = 0.007f,
     .rate1 = 0.004f,
     .unk18 = 0.025f,
     .frame_step = 1,
     .du0 = -0.1f,
     .dv0 = 0.9f,
     .du1 = 0.1f,
     .dv1 = 0.9f,
     .color = 0x808080,
     .tex0 = 0x2c,
     .tex1 = 0x2c,
     .alpha0 = 0x28,
     .alpha1 = 0x28},
    {.verts = rippling_scroll_layer_3_verts,
     .uvs = rippling_scroll_layer_3_uvs,
     .bounds = &rippling_scroll_layer_3_bounds,
     .count = 40,
     .rate0 = 0.007f,
     .rate1 = 0.004f,
     .unk18 = 0.025f,
     .frame_step = 1,
     .du0 = -0.1f,
     .dv0 = 0.9f,
     .du1 = 0.1f,
     .dv1 = 0.9f,
     .color = 0x808080,
     .tex0 = 0x2c,
     .tex1 = 0x2c,
     .alpha0 = 0x28,
     .alpha1 = 0x28},
};
