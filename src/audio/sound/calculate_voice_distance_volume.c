#include "types.h"

struct VoiceVolumeDefinition
{
  u8 pad_0[0x8];
  s32 far_volume;
  s32 near_volume;
  u8 pad_10[0x9];
  u8 attenuation_flags;
};
extern f32 func_001FA6C0();
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
s32 calculate_voice_distance_volume(struct VoiceVolumeDefinition *definition, f32 distance, f32 near_distance, f32 far_distance) __asm__("FUN_0022c6f8");

s32 calculate_voice_distance_volume(struct VoiceVolumeDefinition *definition, f32 distance, f32 near_distance, f32 far_distance) {
  f32 volume_range;
  if (!(definition->attenuation_flags & 1))
  {
    goto linear_falloff;
  }
  if (distance <= near_distance)
  {
    return definition->near_volume;
  }
  if (far_distance <= distance)
  {
    return definition->far_volume;
  }
  volume_range = func_001FA6C0(definition->near_volume - definition->far_volume);
  return definition->far_volume + truncate_float_to_s32(((far_distance - distance) * (far_distance - distance) * volume_range) / ((far_distance - near_distance) * (far_distance - near_distance)));
  linear_falloff:
  if (distance <= near_distance)
  {
    return definition->near_volume;
  }
  if (far_distance <= distance)
  {
    return definition->far_volume;
  }
  volume_range = func_001FA6C0(definition->near_volume - definition->far_volume);
  return definition->far_volume + truncate_float_to_s32(((far_distance - distance) * volume_range) / (far_distance - near_distance));
}
/* Recovered original symbol name. */
extern __typeof__(calculate_voice_distance_volume) sound_GetFade__FP8SoundDeffff __attribute__((alias("FUN_0022c6f8")));
