#ifndef LOMBYTE_MOBY_UPDATE_H
#define LOMBYTE_MOBY_UPDATE_H

/* The engine's moby update call: `if (update) update(moby);` written as asm
   in the original. Retail leaves both delay slots empty
   (`beqz; nop; move $4; jalr; nop`), which is what an asm block in reorder
   mode assembles to; a C call always gets the argument move in its jalr slot.
   Used only by the three moby update loops (update_visible_resident_objects,
   link_and_update_all_mobys, update_all_mobys_profiled). */
#define MOBY_CALL_UPDATE(update, moby)                               \
    __asm__ __volatile__("beqz %0, 1f\n\tmove $4, %1\n\tjalr %0\n1:" \
                         :                                           \
                         : "r"(update), "r"(moby)                    \
                         : "$4",                                     \
                           "$5",                                     \
                           "$6",                                     \
                           "$7",                                     \
                           "$8",                                     \
                           "$9",                                     \
                           "$10",                                    \
                           "$11",                                    \
                           "$12",                                    \
                           "$13",                                    \
                           "$14",                                    \
                           "$15",                                    \
                           "$24",                                    \
                           "$25",                                    \
                           "$31",                                    \
                           "memory")

#endif /* LOMBYTE_MOBY_UPDATE_H */
