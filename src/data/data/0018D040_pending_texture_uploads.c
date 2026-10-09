#include "sda.h"
#include "types.h"
#include "rnc/rendering/texture_upload.h"

struct TextureUpload pending_texture_uploads[64] DATA_AT(0018D040) = {0};
