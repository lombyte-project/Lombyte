# Configuration

- `us/rnc1.us.yaml` is the active Splat configuration for `SCUS_971.99`.
- `overlays/us/` is the catalogue of the level code overlays (streamed level
  code); see `overlays/README.md`. Their sources live under `src/overlays/`
  (`shared/`, `lNN/`, functions named `FUN_LNN_xxxxxxxx`) and are not part of
  the boot link.
- `us/unit_categories.json` lists the intentional-asm units excluded from the
  C goal; `us/recovered_names.json` records the recovered function names
  (`docs/recovered-names.md`); `us/checksum.sha1` is the retail ELF's SHA-1.
- `us/symbol_addrs.txt`, `us/undefined_syms.txt`, and
  `us/undefined_funcs_auto.txt` are active symbol inputs.
- `ghidra/` contains function, call-graph, and data-reference exports used by
  the build tooling.

The ignored `us/SCUS_971.99` file is a local copy of the extracted ELF. It is
never committed.
