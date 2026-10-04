#!/usr/bin/env bash
# One-command contributor setup for Lombyte.
#
#   ./setup.sh                 # install everything, then rebuild and verify the ELF
#   ./setup.sh --iso game.iso  # also take the boot ELF and level overlays from your own disc image
#   ./setup.sh --no-build      # install only
#
# Linux and WSL run natively. macOS (and any host with Docker) runs the same
# script inside an Ubuntu container: ./setup.sh --docker, then
# ./setup.sh --shell for a shell with the toolchain on PATH.
#
# Nothing proprietary is shipped with this repository. The script downloads
# the toolchain from public mirrors used by the PS2 decompilation community,
# checks every file against a pinned SHA-256, builds the game compiler from
# its GPL source and patch stack (patches/sce-991111b), and takes the retail
# boot ELF and level overlays only from a disc image or file you provide, into
# gitignored paths.
set -Eeuo pipefail

ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)
cd "$ROOT"

DOWNLOADS="$ROOT/build/downloads"
TOOLS="$ROOT/tools"
COMPILERS="$TOOLS/compilers"
STAGE="$ROOT/build/setup-stage"
ELF_TARGET="$ROOT/config/us/SCUS_971.99"
ELF_SHA=e050581032e4bb3f20341307da5b69b76f1574910519155380ea771e55c3c0c9

# Pinned public downloads (SHA-256 checked before anything is installed).
BINUTILS_URL=https://github.com/decompals/binutils-mips-ps2-decompals/releases/download/v0.10/binutils-mips-ps2-decompals-linux-x86-64.tar.gz
BINUTILS_SHA=9fe31ea3ee1a37536f9f0e2e12c668e7c3cb99e59f5154bd2f1fc4762473094c
OBJDIFF_URL=https://github.com/encounter/objdiff/releases/download/v3.8.0/objdiff-cli-linux-x86_64
OBJDIFF_SHA=bc1e047126f9c6914bd1695798175234642ab9eaf45e886f841b59a4231e1a81
SDK_CC_URL=https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.9-991111-01.tar.xz
SDK_CC_SHA=ed684fd98f89d36b0121caab311052089103e3b36241fcef4338cc9ea41c75b8
SN_URL=https://github.com/AngheloAlf/SN-Systems-ProDG_for_PS2_2.0/releases/download/1/eegcc_sn_v2.73a.tar.gz
SN_SHA=293903acfb0c8aee3b7766214119be933d80eca01fb7a3c2266287d91455962f
# The retail game code was assembled by the EE assembler of ProDG 3.01
# (ps2eeas 1.9.25.758); the 2.0 archive above ships an older one.
SN_AS_URL=https://raw.githubusercontent.com/AngheloAlf/SN-Systems-ProDG_for_PS2_3.01/d74f6fe08d24e7cf0df48cb570d85ad04db167c5/usr/local/sce/ee/gcc/ee/bin/Ps2EeAs.exe
SN_AS_SHA=cb5adda955e64626564212ef7e0c1434708c4e1ef423344a92ec8033306ed3aa
# Installed with its divbug padding turned off (scripts/patch-ps2eeas.py):
# retail has none of the NOPs it puts in front of a div.
SN_AS_PATCHED_SHA=457d75293b5aeb64b95ffb37ba21dbd1507e176b428ca6892b3069d1b0d1a27e
# GCC 2.95.2 headers for the SN compiler (stdarg.h and friends are not in the
# SN archive); sparse checkout of the Sony SDK mirror, pinned.
SDK_MIRROR_URL=https://github.com/AngheloAlf/sce_ps2_sdk_24.git
SDK_MIRROR_COMMIT=5c8bdf31f6bdd82ba4456413594198b5bd469f43
SDK_MIRROR_INCLUDE=local/sce/ee/gcc/lib/gcc-lib/ee/2.95.2/include
SDK_MIRROR_STDARG_SHA=84945e1a49c2c9aeee025705c53d94f1b91999bd30e2cafa639d800cad852966
SDK_MIRROR_VA_MIPS_SHA=af46841480f84b4620ca62c3f955cce20aac166928db5cdf17109ed71de41ce6

DOCKER_IMAGE=lombyte-dev
DOCKER_BASE=ubuntu:24.04

ISO=""
ELF=""
BUILD=1
MODE=native
WITH_PATCHED=0
IN_CONTAINER=${LOMBYTE_IN_CONTAINER:-0}

usage() {
    sed -n '2,16p' "$ROOT/setup.sh" | sed 's/^# \{0,1\}//'
    cat <<'EOF'

Options:
  --iso PATH        extract SCUS_971.99 from this disc image (default: first dumps/*.iso)
  --elf PATH        copy this SCUS_971.99 instead
  --no-build        install the toolchain, skip `make elf`
  --with-patched    also build the optional patched EE-GCC profile (docs/patched-toolchain.md)
  --docker          run the setup inside an Ubuntu container (macOS, other distros)
  --shell           open a shell in that container with the toolchain on PATH
  --check           verify the installed toolchain and inputs, change nothing
  -h, --help        this text
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --iso) [[ $# -ge 2 ]] || { echo "error: --iso needs a path" >&2; exit 2; }; ISO=$2; shift 2 ;;
        --elf) [[ $# -ge 2 ]] || { echo "error: --elf needs a path" >&2; exit 2; }; ELF=$2; shift 2 ;;
        --no-build) BUILD=0; shift ;;
        --with-patched) WITH_PATCHED=1; shift ;;
        --docker) MODE=docker; shift ;;
        --shell) MODE=shell; shift ;;
        --check) MODE=check; shift ;;
        -h|--help) usage; exit 0 ;;
        *) echo "unknown option: $1" >&2; usage >&2; exit 2 ;;
    esac
done

say()  { printf '\033[1;36m==>\033[0m %s\n' "$*"; }
warn() { printf '\033[1;33mwarning:\033[0m %s\n' "$*" >&2; }
die()  { printf '\033[1;31merror:\033[0m %s\n' "$*" >&2; exit 1; }
report_error() {
    local status=$?
    printf '\033[1;31msetup failed\033[0m (exit %s) at line %s: %s\n' \
        "$status" "${BASH_LINENO[0]:-?}" "$BASH_COMMAND" >&2
    exit "$status"
}
trap report_error ERR

sha_ok() { printf '%s  %s\n' "$2" "$1" | sha256sum --check --status 2>/dev/null; }
assert_sha() { sha_ok "$1" "$2" || die "$1 does not match its pinned SHA-256 ($2)"; }
as_root() { if [[ "$(id -u)" == 0 ]]; then "$@"; else sudo "$@"; fi; }
is_wsl() { grep -qi microsoft /proc/version 2>/dev/null; }

# Replace an install directory under tools/ with a freshly staged one. An
# existing directory is kept as <name>.previous rather than deleted, so a
# hand-installed toolchain can be recovered.
replace_dir() { # target staged
    local target=$1 staged=$2
    case "$target" in "$TOOLS"/*) ;; *) die "internal: refusing to replace $target outside tools/" ;; esac
    if [[ -e "$target" ]]; then
        rm -rf -- "$target.previous"
        mv -- "$target" "$target.previous"
        warn "kept the previous $(basename -- "$target") as $(basename -- "$target").previous"
    fi
    mv -- "$staged" "$target"
}

download() { # url sha path
    local url=$1 sha=$2 path=$3
    mkdir -p "$(dirname -- "$path")"
    if [[ -f "$path" ]] && sha_ok "$path" "$sha"; then
        return
    fi
    say "downloading $(basename -- "$path")"
    rm -f -- "$path"
    curl --fail --location --retry 3 --retry-delay 2 --show-error --progress-bar \
        "$url" --output "$path.part"
    if ! sha_ok "$path.part" "$sha"; then
        rm -f -- "$path.part"
        die "$(basename -- "$path") failed its SHA-256 check; the download is corrupt or the mirror changed"
    fi
    mv -- "$path.part" "$path"
}

# ---------------------------------------------------------------- containers
docker_ready() {
    command -v docker >/dev/null 2>&1 || die "Docker is required for --docker/--shell on this host: https://docs.docker.com/get-docker/"
    docker info >/dev/null 2>&1 || die "the Docker daemon is not running (start Docker Desktop, or the docker service)"
}

docker_build_image() {
    docker_ready
    say "building the $DOCKER_IMAGE container image"
    docker build --platform linux/amd64 --tag "$DOCKER_IMAGE" - <<EOF
FROM --platform=linux/amd64 $DOCKER_BASE
ENV DEBIAN_FRONTEND=noninteractive LOMBYTE_IN_CONTAINER=1 WINEDEBUG=-all
RUN dpkg --add-architecture i386 && apt-get update && apt-get install -y --no-install-recommends \\
    ca-certificates curl file git make tar unzip xz-utils build-essential gcc-multilib \\
    libc6-i386 lib32gcc-s1 lib32stdc++6 python3 python3-venv python3-pip ninja-build flex \\
    sudo wine wine64 wine32:i386 && rm -rf /var/lib/apt/lists/*
WORKDIR /work
EOF
}

DOCKER_MOUNTS=()
docker_run() { # args...
    local tty=()
    [[ -t 0 && -t 1 ]] && tty=(-it)
    # The build writes into the checkout (tools/, build/, .venv), so the
    # container works on the bind-mounted checkout; a named volume keeps
    # Wine's prefix between runs. Nothing else on the host is mounted.
    docker run --rm ${tty[@]+"${tty[@]}"} --platform linux/amd64 \
        --volume "$ROOT:/work" --volume "$DOCKER_IMAGE-home:/root" \
        ${DOCKER_MOUNTS[@]+"${DOCKER_MOUNTS[@]}"} \
        --workdir /work --env LOMBYTE_IN_CONTAINER=1 \
        "$DOCKER_IMAGE" "$@"
}

if [[ "$MODE" == docker ]]; then
    docker_build_image
    args=(--no-build)
    [[ "$BUILD" == 1 ]] && args=()
    # The disc image and executable are mounted read-only, never copied.
    if [[ -n "$ISO" ]]; then
        [[ -f "$ISO" ]] || die "no such file: $ISO"
        DOCKER_MOUNTS+=(--volume "$(cd -- "$(dirname -- "$ISO")" && pwd -P)/$(basename -- "$ISO"):/input/game.iso:ro")
        args+=(--iso /input/game.iso)
    fi
    if [[ -n "$ELF" ]]; then
        [[ -f "$ELF" ]] || die "no such file: $ELF"
        DOCKER_MOUNTS+=(--volume "$(cd -- "$(dirname -- "$ELF")" && pwd -P)/$(basename -- "$ELF"):/input/SCUS_971.99:ro")
        args+=(--elf /input/SCUS_971.99)
    fi
    [[ "$WITH_PATCHED" == 1 ]] && args+=(--with-patched)
    docker_run bash setup.sh ${args[@]+"${args[@]}"}
    say "done. Open a shell with the toolchain: ./setup.sh --shell  (then: make elf, python3 scripts/check-unit.py ...)"
    exit 0
fi
if [[ "$MODE" == shell ]]; then
    docker_ready
    docker image inspect "$DOCKER_IMAGE" >/dev/null 2>&1 || docker_build_image
    exec docker run --rm -it --platform linux/amd64 \
        --volume "$ROOT:/work" --volume "$DOCKER_IMAGE-home:/root" \
        --workdir /work --env LOMBYTE_IN_CONTAINER=1 "$DOCKER_IMAGE" \
        bash -c 'export PATH="/work/.venv/bin:/work/tools/binutils-mips-ps2-decompals:$PATH"; exec bash'
fi

# ------------------------------------------------------------- host checks
case "$(uname -s)" in
    Linux) ;;
    Darwin)
        die "macOS cannot run the 32-bit Linux and Windows compiler binaries directly; run: ./setup.sh --docker" ;;
    *)
        die "unsupported host $(uname -s). On Windows, install WSL (wsl --install) and run this script inside it; elsewhere use ./setup.sh --docker" ;;
esac
[[ "$(uname -m)" == x86_64 ]] || die "an x86-64 host is required (found $(uname -m)); on other machines use ./setup.sh --docker"
case "$ROOT" in
    /mnt/[A-Za-z]/*)
        warn "the checkout is on a Windows drive ($ROOT); builds are much faster from the Linux filesystem, e.g. ~/Lombyte" ;;
esac

# ------------------------------------------------------------------ check
check_installed() {
    local ok=1
    check() { if "$@" >/dev/null 2>&1; then printf 'OK    %s\n' "$LABEL"; else printf 'MISS  %s\n' "$LABEL"; ok=0; fi; }
    LABEL="game compiler (tools/compilers/game-compiler)";      check test -x "$COMPILERS/game-compiler/cc1" -a -x "$COMPILERS/game-compiler/as" -a -f "$COMPILERS/game-compiler/include/stdarg.h"
    LABEL="SDK compiler (tools/compilers/sdk-compiler)";        check test -x "$COMPILERS/sdk-compiler/bin/ee-gcc"
    LABEL="SN compiler (tools/compilers/ee-gcc-2.95.2)";        check test -f "$COMPILERS/ee-gcc-2.95.2/bin/ee-gcc.exe"
    LABEL="SN assembler ps2eeas 1.9.25.758 (no divbug)";        check sha_ok "$COMPILERS/ee-gcc-2.95.2/ee/bin/Ps2EeAs.exe" "$SN_AS_PATCHED_SHA"
    LABEL="SN GCC 2.95.2 headers";                              check sha_ok "$COMPILERS/ee-gcc-2.95.2/lib/gcc-lib/ee/2.95.2/include/stdarg.h" "$SDK_MIRROR_STDARG_SHA"
    LABEL="R5900 binutils (tools/binutils-mips-ps2-decompals)"; check test -x "$TOOLS/binutils-mips-ps2-decompals/mips-ps2-decompals-ld"
    LABEL="objdiff-cli (tools/objdiff)";                        check test -x "$TOOLS/objdiff/objdiff-cli"
    LABEL="Python environment (.venv)";                         check "$ROOT/.venv/bin/python" -c "import splat, spimdisasm, ninja"
    if [[ "$ROOT" =~ ^/mnt/[A-Za-z]/ ]]; then
        LABEL="PE runner (WSL interop, checkout on a Windows drive)"; check true
    else
        LABEL="PE runner (wine)";                                   check command -v wine
    fi
    LABEL="retail boot ELF (config/us/SCUS_971.99)";            check sha_ok "$ELF_TARGET" "$ELF_SHA"
    [[ "$ok" == 1 ]] || return 1
    # optional: overlay work needs the disc image (scripts/overlay-extract.py)
    if [[ -d "$ROOT/config/us/overlays/asm" ]]; then
        printf 'OK    level overlays (config/us/overlays)\n'
    else
        printf 'SKIP  level overlays (config/us/overlays): needs --iso\n'
    fi
}
if [[ "$MODE" == check ]]; then
    check_installed && say "everything is in place; run: make elf"
    exit
fi

# ---------------------------------------------------------- system packages
install_packages() {
    local packages=(
        ca-certificates curl file git make tar unzip xz-utils build-essential gcc-multilib
        libc6-i386 lib32gcc-s1 lib32stdc++6 python3 python3-venv python3-pip ninja-build flex
    )
    local missing=()
    if command -v dpkg >/dev/null 2>&1 && command -v apt-get >/dev/null 2>&1; then
        local p
        for p in "${packages[@]}"; do
            dpkg -s "$p" >/dev/null 2>&1 || missing+=("$p")
        done
        # configure.py runs the PE tools under wine whenever the checkout is
        # not on /mnt/<drive>/ (_windows_exe): its Ps2EeAs exits 253 through
        # WSL interop on some inputs. Mirror that choice here rather than
        # keying on is_wsl, so a checkout on a native path gets wine on WSL too.
        if ! command -v wine >/dev/null 2>&1 && [[ ! "$ROOT" =~ ^/mnt/[A-Za-z]/ ]]; then
            missing+=(wine wine64 wine32:i386)
        fi
        if [[ ${#missing[@]} -gt 0 ]]; then
            say "installing packages: ${missing[*]} (may ask for your password)"
            export DEBIAN_FRONTEND=noninteractive
            if [[ " ${missing[*]} " == *" wine32:i386 "* ]]; then
                as_root dpkg --add-architecture i386
            fi
            as_root apt-get update
            as_root apt-get install -y --no-install-recommends "${missing[@]}"
        fi
    else
        local wine_note=""
        [[ "$ROOT" =~ ^/mnt/[A-Za-z]/ ]] || wine_note=" wine (with 32-bit support)"
        warn "not a Debian/Ubuntu system; make sure these are installed: ${packages[*]}${wine_note}"
    fi
    local need tool
    for tool in curl git make tar xz gcc python3 ninja flex; do
        command -v "$tool" >/dev/null 2>&1 || die "$tool is not installed"
    done
    echo 'int main(void){return 0;}' > "$STAGE/m32.c"
    gcc -m32 "$STAGE/m32.c" -o "$STAGE/m32" 2>/dev/null || die "gcc cannot build 32-bit programs (install gcc-multilib / lib32 packages)"
    need=$(getconf GNU_LIBC_VERSION 2>/dev/null | awk '{print $2}')
    if [[ "$need" =~ ^([0-9]+)\.([0-9]+) ]] && (( BASH_REMATCH[1] == 2 && BASH_REMATCH[2] < 38 )); then
        die "the 32-bit compilers need glibc 2.38 or newer (found $need); use Ubuntu 24.04+/Debian 13+, or ./setup.sh --docker"
    fi
    python3 - <<'EOF' || die "Python 3.10 or newer is required"
import sys; sys.exit(0 if sys.version_info >= (3, 10) else 1)
EOF
    if ! is_wsl; then
        command -v wine >/dev/null 2>&1 || die "wine is required on plain Linux to run the SN compiler and assembler"
    fi
}

# ------------------------------------------------------------- toolchains
install_binutils() {
    local target="$TOOLS/binutils-mips-ps2-decompals"
    [[ -x "$target/mips-ps2-decompals-ld" ]] && return
    local archive="$DOWNLOADS/binutils-mips-ps2-decompals-v0.10.tar.gz"
    download "$BINUTILS_URL" "$BINUTILS_SHA" "$archive"
    rm -rf -- "$STAGE/binutils"; mkdir -p "$STAGE/binutils"
    tar -xzf "$archive" -C "$STAGE/binutils"
    chmod 0755 "$STAGE/binutils"/mips-ps2-decompals-*
    mkdir -p "$TOOLS"
    replace_dir "$target" "$STAGE/binutils"
}

install_objdiff() {
    local target="$TOOLS/objdiff/objdiff-cli"
    [[ -x "$target" ]] && sha_ok "$target" "$OBJDIFF_SHA" && return
    local file="$DOWNLOADS/objdiff-cli-v3.8.0-linux-x86_64"
    download "$OBJDIFF_URL" "$OBJDIFF_SHA" "$file"
    mkdir -p "$(dirname -- "$target")"
    install -m 0755 "$file" "$target"
}

install_sdk_compiler() {
    local target="$COMPILERS/sdk-compiler"
    [[ -x "$target/bin/ee-gcc" ]] && return
    local archive="$DOWNLOADS/ee-gcc2.9-991111-01.tar.xz"
    download "$SDK_CC_URL" "$SDK_CC_SHA" "$archive"
    rm -rf -- "$STAGE/sdk"; mkdir -p "$STAGE/sdk" "$COMPILERS"
    tar -xJf "$archive" -C "$STAGE/sdk"
    assert_sha "$STAGE/sdk/bin/ee-gcc" 64d0a50fef499da0b98177eb5e79e41dfb066ad44246b137c78a266ef97ee265
    assert_sha "$STAGE/sdk/lib/gcc-lib/ee/2.9-ee-991111-01/cc1" b9aef69f93efb949f15ea58189e8eef4a002b9fe4de5d3fcf89c34e1244ec026
    replace_dir "$target" "$STAGE/sdk"
}

install_sn_compiler() {
    local target="$COMPILERS/ee-gcc-2.95.2"
    local include="$target/lib/gcc-lib/ee/2.95.2/include"
    if [[ -f "$target/bin/ee-gcc.exe" ]] && sha_ok "$target/ee/bin/Ps2EeAs.exe" "$SN_AS_PATCHED_SHA" \
        && sha_ok "$include/stdarg.h" "$SDK_MIRROR_STDARG_SHA"; then
        return
    fi
    local archive="$DOWNLOADS/eegcc-sn-v2.73a.tar.gz" assembler="$DOWNLOADS/Ps2EeAs-1.9.25.758.exe"
    download "$SN_URL" "$SN_SHA" "$archive"
    download "$SN_AS_URL" "$SN_AS_SHA" "$assembler"
    rm -rf -- "$STAGE/sn"; mkdir -p "$STAGE/sn" "$COMPILERS"
    tar -xzf "$archive" -C "$STAGE/sn"
    [[ -f "$STAGE/sn/bin/ee-gcc.exe" ]] || die "unexpected SN archive layout"
    # The retail game code was assembled by the ProDG 3.01 assembler; the
    # 2.0 archive's older one pads loops differently and the gate fails.
    # Its divbug padding is turned off: retail has none (scripts/patch-ps2eeas.py).
    python3 "$ROOT/scripts/patch-ps2eeas.py" "$assembler" "$STAGE/sn/ee/bin/Ps2EeAs.exe"
    # GCC 2.95.2 headers (stdarg.h, ...) from the Sony SDK mirror, pinned.
    local mirror="$DOWNLOADS/sce_ps2_sdk_24"
    if [[ ! -d "$mirror/.git" ]]; then
        say "fetching GCC 2.95.2 headers from the SDK mirror"
        rm -rf -- "$mirror"
        git clone --quiet --filter=blob:none --no-checkout "$SDK_MIRROR_URL" "$mirror"
    fi
    git -C "$mirror" sparse-checkout set --no-cone "$SDK_MIRROR_INCLUDE/*"
    git -C "$mirror" checkout --quiet --detach "$SDK_MIRROR_COMMIT"
    assert_sha "$mirror/$SDK_MIRROR_INCLUDE/stdarg.h" "$SDK_MIRROR_STDARG_SHA"
    assert_sha "$mirror/$SDK_MIRROR_INCLUDE/va-mips.h" "$SDK_MIRROR_VA_MIPS_SHA"
    mkdir -p "$(dirname -- "$STAGE/sn/lib/gcc-lib/ee/2.95.2/include")"
    rm -rf -- "$STAGE/sn/lib/gcc-lib/ee/2.95.2/include"
    cp -r "$mirror/$SDK_MIRROR_INCLUDE" "$STAGE/sn/lib/gcc-lib/ee/2.95.2/include"
    replace_dir "$target" "$STAGE/sn"
}

game_compiler_current() {
    local target="$COMPILERS/game-compiler"
    [[ -x "$target/cc1" && -x "$target/as" && -x "$target/ee-gcc" && -f "$target/provenance.json" ]] || return 1
    python3 - "$target/provenance.json" "$ROOT/patches/sce-991111b" <<'EOF'
import hashlib, json, pathlib, sys
prov = json.load(open(sys.argv[1]))
patches = pathlib.Path(sys.argv[2])
want = prov.get("patch_sha256", {})
for name, digest in want.items():
    path = patches / name
    if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != digest:
        sys.exit(1)
sys.exit(0 if want else 1)
EOF
}

install_game_compiler() {
    if game_compiler_current; then
        return
    fi
    say "building the game compiler from source (patches/sce-991111b, about a minute)"
    python3 scripts/build-game-compiler.py --output "$COMPILERS/game-compiler"
}

install_python() {
    if [[ -x .venv/bin/python ]] && .venv/bin/python -c "import splat, spimdisasm, ninja" 2>/dev/null; then
        return
    fi
    say "creating the Python environment (.venv)"
    python3 -m venv .venv
    .venv/bin/python -m pip install --quiet --disable-pip-version-check -r requirements.txt
}

install_patched_profile() {
    local root="$TOOLS/ee-gcc2.9-991111-01-patched"
    if [[ -x "$root/cc1" && -x "$root/xgcc" ]]; then
        return
    fi
    say "building the optional patched EE-GCC profile (docs/patched-toolchain.md)"
    python3 scripts/build-patched-toolchain.py
}

# ------------------------------------------------------------- game files
extract_elf_from_iso() { # iso
    say "extracting SCUS_971.99 from $(basename -- "$1")"
    python3 - "$1" "$ELF_TARGET" <<'EOF'
import mmap, pathlib, struct, sys
iso_path, out = pathlib.Path(sys.argv[1]), pathlib.Path(sys.argv[2])
with iso_path.open("rb") as handle, mmap.mmap(handle.fileno(), 0, access=mmap.ACCESS_READ) as iso:
    if iso[0x8000:0x8006] != b"\x01CD001":
        sys.exit("not an ISO9660 image (no CD001 volume descriptor); a raw .iso dump is needed")
    root = iso[0x8000 + 156:0x8000 + 190]
    lba, size = struct.unpack_from("<I", root, 2)[0], struct.unpack_from("<I", root, 10)[0]
    off, end = lba * 2048, lba * 2048 + size
    while off < end:
        rec = iso[off]
        if rec == 0:
            off += 1
            continue
        name_len = iso[off + 32]
        name = iso[off + 33:off + 33 + name_len].decode("latin1")
        if name == "SCUS_971.99;1":
            flba, fsize = struct.unpack_from("<I", iso, off + 2)[0], struct.unpack_from("<I", iso, off + 10)[0]
            out.parent.mkdir(parents=True, exist_ok=True)
            out.write_bytes(iso[flba * 2048:flba * 2048 + fsize])
            break
        off += rec
    else:
        sys.exit("SCUS_971.99 not found in the image's root directory; is this the USA (NTSC-U) disc?")
EOF
}

install_elf() {
    if sha_ok "$ELF_TARGET" "$ELF_SHA"; then
        return
    fi
    if [[ -f "$ELF_TARGET" && ( -n "$ELF" || -n "$ISO" || -n "$(ls dumps/*.iso 2>/dev/null)" ) ]]; then
        warn "config/us/SCUS_971.99 exists but is not the supported release; replacing it"
    fi
    if [[ -n "$ELF" ]]; then
        [[ -f "$ELF" ]] || die "no such file: $ELF"
        mkdir -p "$(dirname -- "$ELF_TARGET")"
        cp -- "$ELF" "$ELF_TARGET"
    else
        if [[ -z "$ISO" ]]; then
            ISO=$(ls dumps/*.iso 2>/dev/null | head -1 || true)
        fi
        if [[ -z "$ISO" ]]; then
            cat >&2 <<'EOF'

The retail boot executable is not part of this repository. Provide it from your
own USA / NTSC-U copy of Ratchet & Clank (2002), in one of these ways:

  ./setup.sh --iso /path/to/your-disc-image.iso      # extracted for you
  cp /path/to/your-disc-image.iso dumps/ && ./setup.sh
  ./setup.sh --elf /path/to/SCUS_971.99              # copied from the disc root

EOF
            die "no boot ELF (config/us/SCUS_971.99) and no disc image found"
        fi
        [[ -f "$ISO" ]] || die "no such file: $ISO"
        extract_elf_from_iso "$ISO"
    fi
    if ! sha_ok "$ELF_TARGET" "$ELF_SHA"; then
        printf 'got SHA-256 %s\n' "$(sha256sum "$ELF_TARGET" | cut -d' ' -f1)" >&2
        rm -f -- "$ELF_TARGET"
        die "the boot ELF is not the supported release (USA / NTSC-U SCUS_971.99, SHA-256 $ELF_SHA)"
    fi
}

install_overlays() {
    [[ -d config/us/overlays/asm ]] && return
    if [[ -z "$ISO" ]]; then
        ISO=$(ls dumps/*.iso 2>/dev/null | head -1 || true)
    fi
    if [[ -z "$ISO" ]]; then
        warn "no disc image: skipping the level overlays (run ./setup.sh --iso PATH to work on them)"
        return
    fi
    say "extracting the level overlays from $(basename -- "$ISO") (about 10 minutes)"
    .venv/bin/python scripts/overlay-extract.py --iso "$ISO"
}

# --------------------------------------------------------------------- run
mkdir -p "$DOWNLOADS" "$STAGE"
say "Lombyte setup in $ROOT"
install_packages
install_binutils
install_objdiff
install_sdk_compiler
install_sn_compiler
install_game_compiler
install_python
[[ "$WITH_PATCHED" == 1 ]] && install_patched_profile
install_elf
rm -rf -- "$STAGE"

say "toolchain check"
check_installed || die "some components are still missing (see above)"

if [[ "$BUILD" == 1 ]]; then
    say "rebuilding the boot ELF (make elf)"
    export WINEDEBUG=-all
    if [[ "$WITH_PATCHED" == 1 ]]; then
        export EE_GCC_PATCHED_ROOT="$TOOLS/ee-gcc2.9-991111-01-patched"
    fi
    make elf
    install_overlays
elif [[ -n "$ISO" && ! -d config/us/overlays/asm ]]; then
    warn "level overlays skipped: they are extracted after the first make elf; rerun without --no-build"
fi

cat <<EOF

Setup complete. Next steps (see CONTRIBUTING.md):
  make elf                                          rebuild and verify the boot ELF
  python3 scripts/list-functions.py --score          pick a function
  python3 scripts/check-unit.py <unit>               compare your C with retail
  make overlays                                     build and verify the level overlays
EOF
