# Source layout

`src/` is grouped by subsystem: `audio`, `gameplay`, `input`, `math`,
`rendering`, `runtime`, `sdk`, `storage`, `ui`, `video` and `world`.

- A file is named after the function it holds (`ui/map/save_map_coverage.c`);
  `fun_<address>.c` means the function is not understood yet.
- `assembly/` holds units that still build from the retail assembly (the
  oracle), with their C draft under `#else`. Promotion moves the file out of
  it ([CONTRIBUTING.md](../CONTRIBUTING.md#6-promote-an-exact-unit)).
- `overlays/` holds the level code (`shared/` and `lNN/`), built by
  `make overlays` and not linked into the boot executable
  ([docs/build/overlays.md](../docs/build/overlays.md)).

Each file's owner path in `config/us/rnc1.us.yaml` must match its location.
Moving or renaming a file follows [docs/contributing/code-cleanup.md](../docs/contributing/code-cleanup.md).
