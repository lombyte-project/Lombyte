#ifndef LOMBYTE_RNC_AUDIO_SOUND_CALCULATE_VOICE_DISTANCE_VOLUME_H
#define LOMBYTE_RNC_AUDIO_SOUND_CALCULATE_VOICE_DISTANCE_VOLUME_H

#include "types.h"

struct VoiceVolumeDefinition;

s32 calculate_voice_distance_volume(struct VoiceVolumeDefinition *,
                                    f32,
                                    f32,
                                    f32) __asm__("FUN_0022c6f8");

#endif /* LOMBYTE_RNC_AUDIO_SOUND_CALCULATE_VOICE_DISTANCE_VOLUME_H */
