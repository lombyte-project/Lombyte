#!/usr/bin/env bash
set -euo pipefail

# Rebuild the boot ELF byte-for-byte from the checked-in sources.
#
# The build runs in a staging directory inside the checkout (default
# build/baseline). Set BASELINE_ROOT to a native Linux directory (for example
# $HOME/rnc-baseline) when the checkout itself is on a Windows-mounted drive.
# `make iso` patches the verified ELF into a copy of your legally owned disc
# image.

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASELINE_ROOT="${BASELINE_ROOT:-$PROJECT_ROOT/build/baseline}"
VENV="${VENV:-$PROJECT_ROOT/.venv}"
# R5900 binutils: setup.sh installs them into the checkout; /opt is the
# older manual location.
if [[ -z "${BINUTILS_ROOT:-}" && -d "$PROJECT_ROOT/tools/binutils-mips-ps2-decompals" ]]; then
  BINUTILS_ROOT="$PROJECT_ROOT/tools/binutils-mips-ps2-decompals"
fi
BINUTILS_ROOT="${BINUTILS_ROOT:-/opt/binutils-mips-ps2-decompals}"
COMPILER_ROOT="${COMPILER_ROOT:-$PROJECT_ROOT/tools/compilers}"
# Per-unit textbin ee-gcc (Windows PE): lives in the repository so the driver
# can reach its native (Windows) path from the WSL mirror.
SN_TOOLCHAIN_ROOT="${SN_TOOLCHAIN_ROOT:-$PROJECT_ROOT/tools/compilers/ee-gcc-2.95.2}"
export SN_TOOLCHAIN_ROOT

die() { printf 'error: %s\n' "$1" >&2; exit 1; }
[[ "$(uname -s)" == Linux* ]] || die "run inside WSL or another Linux environment"

# Refuse destructive roots: never wipe the repository or any ancestor.
_RESOLVED_ROOT="$(realpath -m "$BASELINE_ROOT" 2>/dev/null || printf '%s' "$BASELINE_ROOT")"
case "$_RESOLVED_ROOT" in
  /*) ;;
  *) die "BASELINE_ROOT must be an absolute path: $BASELINE_ROOT" ;;
esac
case "$_RESOLVED_ROOT" in
  /) die "BASELINE_ROOT must not be the filesystem root" ;;
  "${HOME:-}") die "BASELINE_ROOT must not be the home directory (${HOME:-})" ;;
esac
_PROJECT_RESOLVED="$(realpath -m "$PROJECT_ROOT")"
case "$_PROJECT_RESOLVED" in
  "$_RESOLVED_ROOT" | "$_RESOLVED_ROOT"/*)
    die "BASELINE_ROOT must not be the checkout or a parent of it: $BASELINE_ROOT" ;;
esac

# Refuse to delete an existing directory that is not a baseline workspace.
# The marker below is created by this script; any other directory may hold
# unrelated user data.
if [[ -e "$BASELINE_ROOT" && ! -f "$BASELINE_ROOT/.rnc-baseline-root" ]]; then
  die "refusing to remove a directory that is not a baseline workspace: $BASELINE_ROOT (delete it manually if intended, or choose another BASELINE_ROOT)"
fi

[[ -x "$VENV/bin/python" ]] || die "missing virtual environment: $VENV (create it with: python3 -m venv .venv && .venv/bin/pip install -r requirements.txt)"
[[ -x "$COMPILER_ROOT/game-compiler/ee-gcc" ]] || die "missing game compiler: $COMPILER_ROOT/game-compiler"
[[ -x "$COMPILER_ROOT/sdk-compiler/bin/ee-gcc" ]] || die "missing SDK compiler: $COMPILER_ROOT/sdk-compiler"
[[ -x "$SN_TOOLCHAIN_ROOT/bin/ee-gcc.exe" ]] || die "missing textbin ee-gcc under $SN_TOOLCHAIN_ROOT"
if [[ -n "${EE_GCC_PATCHED_ROOT:-}" && -x "$EE_GCC_PATCHED_ROOT/xgcc" ]]; then
  printf 'note: patched EE-GCC profile: %s\n' "$EE_GCC_PATCHED_ROOT"
  "$VENV/bin/python" "$PROJECT_ROOT/scripts/check-patched-profile.py" \
    --profile "$EE_GCC_PATCHED_ROOT" || true
elif [[ -n "${EE_GCC_PATCHED_ROOT:-}" ]]; then
  printf 'warning: EE_GCC_PATCHED_ROOT has no xgcc; patched-profile units fall back to the retail oracle\n' >&2
else
  printf 'note: EE_GCC_PATCHED_ROOT is not set; patched-profile units fall back to the retail oracle (docs/patched-toolchain.md)\n'
fi

rm -rf "$BASELINE_ROOT"
mkdir -p "$BASELINE_ROOT"
touch "$BASELINE_ROOT/.rnc-baseline-root"
cp -a "$PROJECT_ROOT/configure.py" "$BASELINE_ROOT/configure.py"
mkdir -p "$BASELINE_ROOT/scripts"
mkdir -p "$BASELINE_ROOT/tools/objdiff"
cp "$PROJECT_ROOT/tools/objdiff/objdiff-cli" "$BASELINE_ROOT/tools/objdiff/objdiff-cli"
chmod +x "$BASELINE_ROOT/tools/objdiff/objdiff-cli"
cp -a "$COMPILER_ROOT" "$BASELINE_ROOT/tools/cc"
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
export PATH="$VENV/bin:$BINUTILS_ROOT:$PATH"
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
import hashlib, json, pathlib
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
