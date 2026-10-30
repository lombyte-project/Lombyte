#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"
#include "rnc/math/random.h"

struct Sub {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s16 h10;
    s16 h12;
    s16 h14;
    s16 h16;
    s16 h18;
};

struct Obj {
    u8         pad_0;
    u8         b1;
    u8         b2;
    u8         b3;
    s32        w4;
    u8         b8;
    u8         b9;
    s16        hA;
    f32        fC;
    u128       q10;
    struct Sub sub;
};

extern u8         *D_001CDF80[];
extern u8         *default_spark_lookup[] __asm__("D_001CDFDC");
extern struct Obj *create_part(s32) __asm__("FUN_00217a30");
extern s32         truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void        pack_vector_xyz_with_shared_scale(void *, void *) __asm__("func_00214A98");
extern void        load_display_text_resource_entry(void *, void *) __asm__("func_00214BC0");

struct Obj *spawn_spark(u128 *source_quadword,
                        f32  *first_vector,
                        f32  *second_vector,
                        s32   initial_spark_word,
                        s32   second_spark_word,
                        s32   frame_count,
                        s32   second_frame_count,
                        s32   third_frame_count,
                        s32   lookup_index_and_flags) __asm__("FUN_00218888");

struct Obj *spawn_spark(u128 *source_quadword,
                        f32  *first_vector,
                        f32  *second_vector,
                        s32   initial_spark_word,
                        s32   second_spark_word,
                        s32   frame_count,
                        s32   second_frame_count,
                        s32   third_frame_count,

                        s32 lookup_index_and_flags) {
    struct Obj *spark_part;
    struct Sub *spark_substructure;
    f32         first_vector_scratch[4];
    f32         second_vector_scratch[4];

    spark_part = create_part(2);
    if (spark_part != 0) {
        qcopy(&spark_part->q10, source_quadword);
        spark_part->w4 = initial_spark_word;
        spark_part->b9 = truncate_float_to_s32(4.0f) + 0x40;
        spark_part->fC = 0.0f;
        spark_part->b1 = 0;
        spark_part->b3 = 0x44;
        spark_part->b8 = truncate_float_to_s32(random_float_between(spark_part->fC, 255.0f));
        if (lookup_index_and_flags == -1) {
            spark_part->b2 = *default_spark_lookup[0];
        } else {
            spark_part->b2 = *D_001CDF80[(u16)lookup_index_and_flags];
            if (lookup_index_and_flags & 0x10000) {
                spark_part->b3 = 0x48;
            }
        }
        spark_part->hA = frame_count;
        spark_substructure = &spark_part->sub;
        spark_substructure->h16 = truncate_float_to_s32(first_vector[3] * 210000.0f / 1000.0f);
        spark_substructure->h18 = truncate_float_to_s32(second_vector[3] * 210000.0f / 1000.0f);
        pack_vector_xyz_with_shared_scale(first_vector, &spark_substructure->a);
        pack_vector_xyz_with_shared_scale(second_vector, &spark_substructure->b);
        load_display_text_resource_entry(first_vector_scratch, &spark_substructure->a);
        load_display_text_resource_entry(second_vector_scratch, &spark_substructure->b);
        spark_substructure->c = initial_spark_word;
        spark_substructure->d = second_spark_word;
        spark_substructure->h10 = frame_count;
        spark_substructure->h12 = second_frame_count;
        spark_substructure->h14 = third_frame_count;
    }
    return spark_part;
}

extern __typeof__(spawn_spark) func_00218888 __attribute__((alias("FUN_00218888")));
