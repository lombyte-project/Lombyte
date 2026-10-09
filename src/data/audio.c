#include "types.h"
#include "sda.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/audio/sound_read_work.h"
#include "rnc/audio/voice_pool.h"

struct StartSoundWork sound_read_work DATA_AT(00137B00) = {0};

VoicePool voice_pool DATA_AT(0013E550) = {0};

struct MusicStreamState music_stream_state DATA_AT(001516D0) = {0};
