/* Ported from rac1-decomp (src/game/lights.c, func_00202260). */

#include "qcopy.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/rendering/lights/create_point_light.h"
#include "rnc/rendering/lights/point_light.h"

/* One directional light (0x40 bytes); levels load up to 12 from their
   header. */
struct DirectionalLight {
    f32 color[4];
    f32 direction[4];
    u8  pad_20[0x20];
};

/* The light update_all_point_lights points along the camera. */
#define CAMERA_DIRECTIONAL_LIGHT 13

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
    enum {
        POINT_LIGHT_LINK_INACTIVE = 0,
        POINT_LIGHT_LINK_PENDING_CREATION = 1,
        POINT_LIGHT_LINK_CREATED = 2
    };
    struct DirectionalLight *lights = (struct DirectionalLight *)dir_lights;
    float                    lighting_direction_angle;
    int                      light_index;

    lights[CAMERA_DIRECTIONAL_LIGHT].color[3] = -0.3f;
    lights[CAMERA_DIRECTIONAL_LIGHT].color[0] = 0.8f;
    lights[CAMERA_DIRECTIONAL_LIGHT].color[1] = 0.8f;
    lights[CAMERA_DIRECTIONAL_LIGHT].color[2] = 0.8f;
    lighting_direction_angle = fast_add_rotations(*(float *)camera_lighting_reference_angle, -0.8f);
    lights[CAMERA_DIRECTIONAL_LIGHT].direction[0] = fast_cos(lighting_direction_angle) * 0.866f;
    lights[CAMERA_DIRECTIONAL_LIGHT].direction[1] = fast_sin(lighting_direction_angle) * 0.866f;
    lights[CAMERA_DIRECTIONAL_LIGHT].direction[2] = -0.5f;
    *(int *)&lights[CAMERA_DIRECTIONAL_LIGHT].direction[3] = 0;
    for (light_index = 0; light_index < 8; light_index++) {
        struct PointLight     *light = &point_lights[light_index];
        struct PointLightSlot *slot = &((struct PointLightSlot *)point_light_data)[light_index];

        if (slot->link_state != POINT_LIGHT_LINK_INACTIVE && fast_vec_dist(light->sphere, slot->linked_sphere) > 8.0f) {
            qcopy(slot->linked_sphere, light->sphere);
            if (slot->link_state == POINT_LIGHT_LINK_PENDING_CREATION) {
                create_point_light(light_index);
                slot->link_state = POINT_LIGHT_LINK_CREATED;
            } else if (slot->link_state == POINT_LIGHT_LINK_CREATED) {
                refresh_point_light(light_index);
            }
        }
    }
}

extern __typeof__(update_all_point_lights) func_00201A28 __attribute__((alias("FUN_00201a28")));
