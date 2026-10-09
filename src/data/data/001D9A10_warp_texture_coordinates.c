#include "types.h"
#include "rnc/rendering/warp_effect.h"

f32 warp_texture_coordinates[4][2] __attribute__((section(".data"))) = {{0, 0.5f}, {1.0f, 0.5f}, {0, 0.5f}, {1.0f, 0.5f}};
