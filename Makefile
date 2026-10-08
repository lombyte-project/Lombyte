# Lombyte — Ratchet & Clank (PS2, USA) matching decompilation
#
# The boot ELF is recompiled and must match retail byte-for-byte (full-ELF
# SHA gate); `make overlays` proves the level overlay C function by function.
# `make iso` patches the ELF into a copy of your legally owned disc image,
# leaving the raw game data untouched.
#
# Setup: put a legally owned disc dump in dumps/ (e.g. dumps/game.iso) and
# install the toolchain (docs/building.md). Then run `make elf`.

.PHONY: elf iso clean-iso check progress overlays

check: ## Run the public CI checks locally (tests, script parse, progress report)
	python3 scripts/test_public_tools.py -v
	python3 -m py_compile scripts/*.py
	python3 scripts/gen_progress_report.py
	python3 scripts/generate_treemap.py
	@# Struct layout checks (src/check/*_layout_check.c), compiled with the EE compiler when present.
	@cc=tools/ee-gcc2.9-991111-01-patched; if [ -x $$cc/cc1 ]; then \
	  for f in $$(find src/check -name '*_layout_check.c' | sort); do \
	    $$cc/cpp -Iinclude $$f | $$cc/cc1 -quiet -o /dev/null || exit 1; done; \
	  echo "layout checks: OK"; else echo "layout checks: skipped (no $$cc)"; fi

progress: ## After `make elf`: measure C_FUZZY and preview the report and map in build/progress/
	$${VENV:-.venv}/bin/python scripts/gen_progress_report.py --workspace "$${BASELINE_ROOT:-build/baseline}"
	$${VENV:-.venv}/bin/python scripts/generate_treemap.py

elf: ## Rebuild the boot ELF byte-for-byte (full baseline + SHA gate)
	./verify-baseline.sh
	@ls -l "$${BASELINE_ROOT:-build/baseline}/config/us/build/SCUS_971.99"

overlays: ## Build src/overlays and prove every overlay function in C against retail (docs/overlays.md)
	$${VENV:-.venv}/bin/python configure.py --overlays
	$${VENV:-.venv}/bin/python scripts/overlay_unit.py

iso: elf ## Patch the rebuilt boot ELF into a copy of the disc image
	@iso="$$(ls dumps/*.iso 2>/dev/null | head -1)"; \
	test -n "$$iso" || { echo "no dumps/*.iso found; place a legally owned dump in dumps/" >&2; exit 1; }; \
	python3 scripts/rebuild-iso.py --iso "$$iso"

clean-iso:
	@rm -f "build/Ratchet & Clank (USA) - rebuilt.iso"
