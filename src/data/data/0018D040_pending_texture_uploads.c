#include "types.h"
#include "rnc/rendering/texture_upload.h"

struct TextureUpload pending_texture_uploads[64] __attribute__((section(".data"))) = {0};
