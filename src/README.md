# Source layout

`src/` is organized by semantic subsystem: `audio`, `gameplay`, `input`,
`math`, `rendering`, `runtime`, `sdk`, `storage`, `ui`, `video`, and `world`,
plus `overlays` for the level code layer.

- `src/assembly/` marks units whose current implementation still uses an
  assembly oracle. Its subdirectories follow the same subsystem boundaries;
  `src/assembly/textbin/` retains the owner paths for those oracle-backed
  functions.
- Semantically grouped C units live directly under their logical subsystem,
  even when they were first recovered from a textbin region. Compiler routing
  for those moved units is recorded explicitly in `configure.py`.
- `src/overlays/` holds the level code overlays (`shared/` and `lNN/`), see
  [`docs/overlays.md`](../docs/overlays.md); they are compiled by
  `make overlays`, not linked into the boot executable.

The active configuration lists every build owner explicitly. Keep each owner
path aligned with its source file. Do not infer that a C file is safe to replace
merely because it compiles: consult the source-quality audit and preserve the
exact oracle until all objdiff measures are exact.
