#ifndef RNC_BROWSER_QZERO_H
#define RNC_BROWSER_QZERO_H

/* Portable form of include/qzero.h's `sq $0`: clear one 16-byte quadword. */
#define qzero(p) __builtin_memset((p), 0, 16)

#endif
