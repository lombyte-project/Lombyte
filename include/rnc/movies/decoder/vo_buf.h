#ifndef LOMBYTE_RNC_MOVIES_DECODER_VO_BUF_H
#define LOMBYTE_RNC_MOVIES_DECODER_VO_BUF_H

#include "types.h"

/* Decoded picture ring (type name from voBufReset__FP5VoBuf): dec_bs0 fills
   data[write] and marks tag[write]; the end-of-image handler drops count. */
struct VoData {
    u8 v[0xD0000];
};

#define VO_TAG_FILLED 2 /* set by vo_buf_inc_count */

struct VoTag {
    s32 status;
    u8  pad_4[0x3C];
    u8  body[0x138C0 - 0x40]; /* GS image transfer built by set_image_tag */
};

struct VoBuf {
    struct VoData *data;
    struct VoTag  *tag;
    volatile s32   write;
    volatile s32   count;
    s32            size;
};

void           vo_buf_create(struct VoBuf  *buf,
                             struct VoData *data,
                             struct VoTag  *tag,
                             s32            size) __asm__("FUN_0023d190");
void           vo_buf_delete(struct VoBuf *buf) __asm__("func_0023D1E0");
void           vo_buf_reset(struct VoBuf *buf) __asm__("func_0023D1E8");
s32            vo_buf_is_full(struct VoBuf *buf) __asm__("func_0023D1F8");
void           vo_buf_inc_count(struct VoBuf *buf) __asm__("FUN_0023d210");
struct VoData *vo_buf_get_data(struct VoBuf *buf) __asm__("FUN_0023d288");
s32            vo_buf_is_empty(struct VoBuf *buf) __asm__("func_0023D2C8");
struct VoTag  *vo_buf_get_tag(struct VoBuf *buf) __asm__("FUN_0023d2d8");
void           vo_buf_dec_count(struct VoBuf *buf) __asm__("func_0023D340");

#endif /* LOMBYTE_RNC_MOVIES_DECODER_VO_BUF_H */
