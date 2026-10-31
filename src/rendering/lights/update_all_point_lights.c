/* Ported from rac1-decomp (src/game/lights.c, func_00202260). */

#include "qcopy.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/rendering/lights/create_point_light.h"
#include "rnc/rendering/lights/point_light.h"

extern char              camera_lighting_reference_angle[] __asm__("D_00187098");
extern char              dir_lights[] __asm__("D_0019BDC0");
extern struct PointLight point_lights[] __asm__("D_0019C1C0");
extern float             fast_add_rotations(float, float) __asm__("func_001FA580");
extern float             fast_cos(float) __asm__("func_001F9DC8");
extern float             fast_sin(float) __asm__("func_001F9DE0");
extern float             fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern void              refresh_point_light(int) __asm__("FUN_00201f58");
void                     update_all_point_lights(void) __asm__("FUN_00201a28");

void update_all_point_lights(void) {
    char *lighting_presets_base = dir_lights;
    float lighting_direction_angle;
    int   light_index;

    *(float *)(lighting_presets_base + 0x34C) = -0.3f;
    *(float *)(lighting_presets_base + 0x340) = 0.8f;
    *(float *)(lighting_presets_base + 0x344) = 0.8f;
    *(float *)(lighting_presets_base + 0x348) = 0.8f;
    lighting_direction_angle = fast_add_rotations(*(float *)camera_lighting_reference_angle, -0.8f);
    *(float *)(lighting_presets_base + 0x350) = fast_cos(lighting_direction_angle) * 0.866f;
    *(float *)(lighting_presets_base + 0x354) = fast_sin(lighting_direction_angle) * 0.866f;
    *(float *)(lighting_presets_base + 0x358) = -0.5f;
    *(int *)(lighting_presets_base + 0x35C) = 0;
    for (light_index = 0; light_index < 8; light_index++) {
        struct PointLight     *light = &point_lights[light_index];
        struct PointLightSlot *slot = &((struct PointLightSlot *)point_light_data)[light_index];

        if (slot->link_state != 0 && fast_vec_dist(light->sphere, slot->linked_sphere) > 8.0f) {
            qcopy(slot->linked_sphere, light->sphere);
            if (slot->link_state == 1) {
                create_point_light(light_index);
                slot->link_state = 2;
            } else if (slot->link_state == 2) {
                refresh_point_light(light_index);
            }
        }
    }
}

extern __typeof__(update_all_point_lights) func_00201A28 __attribute__((alias("FUN_00201a28")));
