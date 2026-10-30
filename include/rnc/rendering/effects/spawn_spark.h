#ifndef LOMBYTE_RNC_RENDERING_EFFECTS_SPAWN_SPARK_H
#define LOMBYTE_RNC_RENDERING_EFFECTS_SPAWN_SPARK_H

#include "types.h"

struct Obj;

struct Obj *spawn_spark(void *source_quadword,
                        void *first_vector,
                        void *second_vector,
                        s32   initial_spark_word,
                        s32   second_spark_word,
                        s32   frame_count,
                        s32   second_frame_count,
                        s32   third_frame_count,
                        s32   lookup_index_and_flags) __asm__("FUN_00218888");

#endif /* LOMBYTE_RNC_RENDERING_EFFECTS_SPAWN_SPARK_H */
