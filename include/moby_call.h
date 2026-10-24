#ifndef LOMBYTE_MOBY_CALL_H
#define LOMBYTE_MOBY_CALL_H

/*
 * Calls a moby's update function: `move $4, moby; jalr update`, written as
 * inline asm in the original. The three moby update loops (00264740, 002648e0,
 * FUN_00212e28) are the only retail jalr sites that leave both the null-test
 * beqz slot and the jalr slot empty while a fillable argument move sits right
 * before the jalr, and they keep the pointer in $v1. A C call always fills a
 * slot and puts the pointer in $v0; a compiler that left such slots empty would
 * break 16 exact units, so it is the source, not the compiler.
 * The clobbers are the call-clobbered registers; update stays an in/out operand
 * because the callee may change $v1.
 */
#define moby_call_update(update, moby)                                                          \
    __asm__ __volatile__("move $4, %1\n\tjalr %0"                                               \
                         : "+r"(update)                                                         \
                         : "r"(moby)                                                            \
                         : "$1", "$2", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", \
                           "$13", "$14", "$15", "$24", "$25", "$31", "hi", "lo", "$f0", "$f1",  \
                           "$f2", "$f3", "$f4", "$f5", "$f6", "$f7", "$f8", "$f9", "$f10",      \
                           "$f11", "$f12", "$f13", "$f14", "$f15", "$f16", "$f17", "$f18",      \
                           "$f19", "memory")

#endif /* LOMBYTE_MOBY_CALL_H */
