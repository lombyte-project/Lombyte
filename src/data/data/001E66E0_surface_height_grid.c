#include "types.h"
#include "rnc/gameplay/surface_height_grid.h"

SurfaceHeightGrid surface_height_grid __attribute__((section(".data"))) = {{0, 0, 0, 0x42, 0, 0, 0, 0x42}, -16.0f, -16.0f, 2.0f, 2.0f};
