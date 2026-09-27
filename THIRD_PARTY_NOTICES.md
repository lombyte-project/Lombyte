# Third-party notices

Most of this repository is covered by the MIT license. Parts of `src/`
reconstruct runtime code that was statically linked into the retail
executable; code reconstructed from third-party binaries remains the sole
intellectual property of the respective copyright holders.

The set of such files changes as the decompilation progresses, so it is not
enumerated here. Affected files carry the upstream attribution in their source
comments; this page groups them by upstream project and points to the full
license texts in `licenses/`.

## GCC runtime — libgcc2 and soft-float (fp-bit)

GPL-2.0-or-later with the standard GCC runtime exception: linking this library
with other files does not by itself place the resulting executable under the
GPL. Full text: [`licenses/GPL-2.0.txt`](licenses/GPL-2.0.txt).

## rac1-decomp (PAL decompilation)

Some functions in `src/` are C written by the
[rac1-decomp](https://github.com/Lynder063/rac1-decomp) project for the PAL
release of the game, ported to this build by an agreed exchange between the
two projects. Each such file names its origin in its first line
(`Ported from rac1-decomp, the PAL decompilation (<file>, <function>)`).
rac1-decomp has not chosen a license for its own code yet; the ported code
remains its authors' work and is not covered by this repository's MIT license
beyond what they grant.

## newlib

BSD-style Regents of the University of California notice. The newlib allocator
derives from dlmalloc 2.6.5 by Doug Lea, released to the public domain.
Collection license: [`licenses/COPYING.NEWLIB.txt`](licenses/COPYING.NEWLIB.txt).

## David Gay dtoa

The AT&T permissive notice, reproduced in every affected file:

```
/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/
```

## Patched EE-GCC toolchain source patch

`patches/ee-gcc-2.9-991111-01/` modifies the GPLv2 EE-GCC 2.9 snapshot from
[SSXModding/ps2-ee-toolchain](https://github.com/SSXModding/ps2-ee-toolchain)
at `b595ded`. Patch and binaries built from it are GPLv2-or-later; see
[`licenses/GPL-2.0.txt`](licenses/GPL-2.0.txt). Binaries are not distributed
with this repository.
