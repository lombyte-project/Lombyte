/* Compile-time EE layout checks; never linked into the game. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/ui/help/help_record.h"

SIZE_CHECK(record, HelpRecord, 8);
OFFSET_CHECK(count, HelpRecord, count, 0);
OFFSET_CHECK(best, HelpRecord, best, 2);
OFFSET_CHECK(flags, HelpRecord, flags, 4);
