#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/type2id/_type2id.s", _type2id);
#else
/* type2id exact recovery: named c1 + empty-asm barrier on shift after its initial 0 keeps the
   cg `shift = 0` assignment alive (branch-likely delay fill), reproducing retail scheduling. */

#include "types.h"

extern u8 D_00132ED8[];

s64 _type2id(u32 id, s64 value)
{
  volatile u64 pad;
  u64 result = 0;
  s32 new_var;
  s32 shift = 0;
  u64 field;
  u64 *entry;
  u64 c1;
  register s32 offset asm("$6");
  register s64 v asm("$7");
  v = value;
  if (0xA > id)
  {
    offset = 0x10;
    offset = id * offset;
    new_var = offset;
    entry = (u64 *) ((void *) (((u8 *) D_00132ED8) - (-offset)));
    field = entry[1];
    c1 = ((u64) 0xFFFF000000);
    __asm__ __volatile__("" : "+r"(shift));
    if (field == c1)
    {
      goto ca;
    }
    if (c1 < field)
    {
      goto cg;
    }
    if (field == ((u64) 0xFF00000000))
    {
      goto cb;
    }
    goto done;
    ca:
    shift = 0x18;

    goto done;
    cg:
    shift = 0;

    goto done;
    cb:
    shift = 0x20;

    done:
    result = (*((u64 *) (((u8 *) D_00132ED8) + new_var))) | (((u64) v) << shift);

    asm __volatile__("" : "+r"(v));
  }
  return (s64) result;
}

#endif /* NON_MATCHING */
