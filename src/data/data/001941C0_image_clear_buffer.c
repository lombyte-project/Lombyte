#include "types.h"
#include "rnc/rendering/image_clear_buffer.h"

u8 image_clear_buffer[4096] __attribute__((section(".data"))) = {0};
