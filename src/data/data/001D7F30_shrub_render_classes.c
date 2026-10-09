#include "types.h"
#include "rnc/rendering/shrub_render_class.h"

ShrubRenderClass *shrub_render_classes[64] __attribute__((section(".data"))) = {0};
