# Lombyte — Ratchet & Clank (PS2, USA) matching decompilation
#
# Only the boot ELF is recompiled, and it must match retail byte-for-byte
# (full-ELF SHA gate). `make iso` patches it into a copy of your legally owned
# disc image, leaving the raw game data untouched.
#
# Setup: put a legally owned disc dump in dumps/ (e.g. dumps/game.iso) and
# install the prerequisites (see README.md). Then run `make`.

.PHONY: elf iso clean-iso check progress overlays

check: ## Run the public CI checks locally (tests, script parse)
	python3 scripts/test_public_tools.py -v
	python3 -m py_compile scripts/*.py
	python3 scripts/gen_progress_report.py --check

progress: ## Regenerate progress/report.json for decomp.dev (after `make elf`)
	$${VENV:-.venv}/bin/python scripts/gen_progress_report.py --workspace build/baseline

elf: ## Rebuild the boot ELF byte-for-byte (full baseline + SHA gate)
	./verify-baseline.sh
	@ls -l build/SCUS_971.99 2>/dev/null || echo "built ELF found in the baseline workspace (see verify-baseline.sh output)"

overlays: ## Compile the level overlay sources (src/overlays, docs/overlays.md) into build/overlays/obj
	$${VENV:-.venv}/bin/python configure.py --overlays
	ninja -C build/overlays

iso: elf ## Patch the rebuilt boot ELF into a copy of the disc image
	@iso="$$(ls dumps/*.iso 2>/dev/null | head -1)"; \
	test -n "$$iso" || (echo "no dumps/*.iso found; place a legally owned dump in dumps/"; exit 1); \
	python3 rebuild-iso.py --iso "$$iso"

clean-iso:
	@rm -f "build/Ratchet & Clank (USA) - rebuilt.iso"
