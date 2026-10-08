/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "sifrpc.h"

OFFSET_CHECK(command, struct sceSifClientData, command, 0x10);
OFFSET_CHECK(buff, struct sceSifClientData, buff, 0x14);
OFFSET_CHECK(cbuff, struct sceSifClientData, cbuff, 0x18);
OFFSET_CHECK(func, struct sceSifClientData, func, 0x1C);
OFFSET_CHECK(para, struct sceSifClientData, para, 0x20);
OFFSET_CHECK(serve, struct sceSifClientData, serve, 0x24);
SIZE_CHECK(client, struct sceSifClientData, 0x28);
