#include "types.h"
#include "rnc/rendering/object_render_class.h"

ObjectRenderClass *object_render_classes[128] __attribute__((section(".data"))) = {0};
