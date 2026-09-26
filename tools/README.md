# `tools/` — local toolchain (not tracked)

Install the matching toolchain here; the directory contents are ignored by Git
(only this README is tracked).

The expected layout and versions are documented in the top-level README's
**Building** section: the reconstructed game compiler (required — `configure.py`
refuses to generate a build without it), the frozen EE-GCC 2.9 tree, the SN
EE-GCC 2.95.2 tree, the R5900 binutils, and the objdiff CLI.

No game data, compiler binaries, or proprietary tools are distributed with
this repository.
