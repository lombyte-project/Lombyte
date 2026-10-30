# Configuration

- `us/rnc1.us.yaml` is the active Splat configuration for `SCUS_971.99`.
- `overlays/us/` is the catalogue of the level code overlays (streamed level
  code); see `overlays/README.md`. Their sources live under `src/overlays/`
  (`shared/`, `lNN/`, functions named `FUN_LNN_xxxxxxxx`) and are not part of
  the boot link.
- `us/unit_categories.json` lists the intentional-asm units excluded from the
  C goal; `us/checksum.sha1` is the retail ELF's SHA-1.
- `us/symbol_addrs.txt`, `us/undefined_syms.txt`, and
  `us/undefined_funcs_auto.txt` are active symbol inputs.
- `ghidra/ghidra_functions.csv` is the Ghidra function list the oracle tooling
  reads.

Ignored, never committed: `us/SCUS_971.99` (your extracted ELF), `us/asm/`,
`us/overlays/` (level records from `scripts/overlay-extract.py`) and the files
`configure.py` writes (`build.ninja`, the `.ld` scripts, `objdiff.json`,
`permuter_settings.toml`, `padless-asm.py`).
