# Configuration

Everything here is for the USA release (`SCUS_971.99`).

| Path                           | What                                                        |
| :----------------------------- | :---------------------------------------------------------- |
| `us/rnc1.us.yaml`              | the splat configuration of the boot executable               |
| `us/symbol_addrs.txt`          | boot symbol names and addresses                              |
| `us/undefined_syms.txt`, `us/undefined_funcs_auto.txt` | symbols the boot link takes from outside |
| `us/unit_categories.json`      | the hand-written assembly units, left out of the C goal      |
| `us/checksum.sha1`             | the retail executable's SHA-1                                |
| `us/levels/`                   | the 19 level programs: function list, data, names ([README](us/levels/README.md)) |
| `ghidra/ghidra_functions.csv`  | the function list the boot build reads for raw-code units    |

Not in Git: `us/SCUS_971.99` (your extracted executable), `us/overlays/`
(level records and assembly from `scripts/overlay-extract.py`), `us/asm/`,
`us/expected/` and the files `configure.py` writes (`build.ninja`, the `.ld`
scripts, `objdiff.json`, `permuter_settings.toml`, `padless-asm.py`).
