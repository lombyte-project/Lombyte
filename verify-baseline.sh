#!/usr/bin/env bash
set -euo pipefail

# Rebuild the boot ELF byte-for-byte from the checked-in sources, in
# build/baseline (recreated on every run). `make iso` patches the verified ELF
# into a copy of your own disc image.

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASELINE_ROOT="$PROJECT_ROOT/build/baseline"
VENV="$PROJECT_ROOT/.venv"
COMPILERS="$PROJECT_ROOT/tools/compilers"
BINUTILS="$PROJECT_ROOT/tools/binutils-mips-ps2-decompals"

die() { printf 'error: %s\n' "$1" >&2; exit 1; }
[[ "$(uname -s)" == Linux* ]] || die "run inside WSL or another Linux environment"
[[ "$PROJECT_ROOT" != /mnt/[A-Za-z]/* ]] || die "this folder is on a Windows drive, which is slow. Copy the project to the Linux disk (e.g. ~/lombyte) and run it there."

# Refuse to delete a build/baseline this script did not create.
if [[ -e "$BASELINE_ROOT" && ! -f "$BASELINE_ROOT/.rnc-baseline-root" ]]; then
  die "refusing to remove a directory that is not a baseline workspace: $BASELINE_ROOT (delete it yourself if you meant to)"
fi

[[ -x "$VENV/bin/python" ]] || die "missing virtual environment: $VENV (run ./setup.sh)"
[[ -x "$COMPILERS/game-compiler/ee-gcc" ]] || die "missing game compiler: $COMPILERS/game-compiler (run ./setup.sh)"
[[ -x "$COMPILERS/sdk-compiler/bin/ee-gcc" ]] || die "missing SDK compiler: $COMPILERS/sdk-compiler (run ./setup.sh)"
[[ -f "$COMPILERS/sn-compiler/bin/ee-gcc.exe" ]] || die "missing SN compiler: $COMPILERS/sn-compiler (run ./setup.sh)"
[[ -x "$BINUTILS/mips-ps2-decompals-ld" ]] || die "missing R5900 binutils: $BINUTILS (run ./setup.sh)"

rm -rf "$BASELINE_ROOT"
mkdir -p "$BASELINE_ROOT"
touch "$BASELINE_ROOT/.rnc-baseline-root"
cp -a "$PROJECT_ROOT/configure.py" "$BASELINE_ROOT/configure.py"
mkdir -p "$BASELINE_ROOT/scripts"
mkdir -p "$BASELINE_ROOT/tools/objdiff"
cp "$PROJECT_ROOT/tools/objdiff/objdiff-cli" "$BASELINE_ROOT/tools/objdiff/objdiff-cli"
chmod +x "$BASELINE_ROOT/tools/objdiff/objdiff-cli"
cp -a "$COMPILERS" "$BASELINE_ROOT/tools/compilers"
mkdir -p "$BASELINE_ROOT/config/us"
cp "$PROJECT_ROOT/config/us/rnc1.us.yaml" "$BASELINE_ROOT/config/us/rnc1.us.yaml"
cp "$PROJECT_ROOT/config/us/SCUS_971.99" "$BASELINE_ROOT/config/us/SCUS_971.99"
cp "$PROJECT_ROOT/config/us/checksum.sha1" "$BASELINE_ROOT/config/us/checksum.sha1"
cp "$PROJECT_ROOT/config/us/undefined_syms.txt" "$BASELINE_ROOT/config/us/undefined_syms.txt"
cp "$PROJECT_ROOT/config/us/undefined_funcs_auto.txt" "$BASELINE_ROOT/config/us/undefined_funcs_auto.txt" 2>/dev/null || true
cp "$PROJECT_ROOT/config/us/symbol_addrs.txt" "$BASELINE_ROOT/config/us/symbol_addrs.txt" 2>/dev/null || true
cp -a "$PROJECT_ROOT/include/." "$BASELINE_ROOT/include/"
cp -a "$PROJECT_ROOT/src/." "$BASELINE_ROOT/src/"
touch "$BASELINE_ROOT/config/us/undefined_syms_auto.txt"
export PATH="$VENV/bin:$BINUTILS:$PATH"
cd "$BASELINE_ROOT"
python configure.py config/us/rnc1.us.yaml -c
python configure.py config/us/rnc1.us.yaml --make-asm
# Some textbin units compile from stable raw-word oracles rather than splat's
# inline-assembler-sensitive disassembly.  Install them before compiling.
python "$PROJECT_ROOT/scripts/materialize-textbin-oracles.py" \
    --workspace "$BASELINE_ROOT" \
    --function-map "$PROJECT_ROOT/config/ghidra/ghidra_functions.csv" \
    --elf "$BASELINE_ROOT/config/us/SCUS_971.99"
set +e
ninja -C config/us
BUILD_STATUS=$?
set -e
if [[ "$BUILD_STATUS" -ne 0 ]]; then
    printf 'baseline build failed with status %s; no PASS receipt produced\n' "$BUILD_STATUS" >&2
    exit "$BUILD_STATUS"
fi
tools/objdiff/objdiff-cli report generate -p config/us/ -o config/us/report.json -f json
python3 <<'PY'
import hashlib, json, pathlib, re, subprocess
# A label names its address: data a unit defines (its bss above all, which no
# hash covers) must sit where the label says.
nm = subprocess.run(["mips-ps2-decompals-nm", "config/us/build/SCUS_971.99.elf"],
                    capture_output=True, text=True, check=True).stdout
moved = [line for line in nm.splitlines()
         if (m := re.fullmatch(r"([0-9a-f]+) \S (?:D|FUN|func|jtbl)_([0-9A-Fa-f]{8})", line))
         and int(m.group(1), 16) & 0xFFFFFFFF != int(m.group(2), 16)]
if moved:
    raise SystemExit("labels linked away from their address:\n  " + "\n  ".join(moved[:20]))
elf = pathlib.Path("config/us/build/SCUS_971.99")
retail = pathlib.Path("config/us/SCUS_971.99")
built_sha = hashlib.sha256(elf.read_bytes()).hexdigest()
expected = "e050581032e4bb3f20341307da5b69b76f1574910519155380ea771e55c3c0c9"
print("built SHA-256:", built_sha)
if hashlib.sha256(retail.read_bytes()).hexdigest() != expected:
    raise SystemExit("retail input does not match pinned SHA-256")
if built_sha != expected or elf.read_bytes() != retail.read_bytes():
    raise SystemExit("rebuilt ELF does not match retail SHA-256")
print("PASS: reconstructed boot ELF matches retail")
PY
echo "baseline build OK"
