#include "types.h"
#include "rnc/audio/voice_pool.h"

VoicePool voice_pool __attribute__((section(".data"))) = {0};
